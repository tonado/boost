# Twisted, the Framework of Your Internet
# Copyright (C) 2001 Matthew W. Lefkowitz
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser General Public
# License as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# $Id$

#""" Implementation module for the `conch` command.
#"""
from twisted.conch.client import agent, connect, default, options
from twisted.conch.error import ConchError
from twisted.conch.ssh import userauth, connection, common
from twisted.conch.ssh import session, forwarding, channel
from twisted.internet import reactor, stdio, defer, protocol
from twisted.internet.error import CannotListenError
from twisted.python import log, usage

import os, sys, getpass, struct, tty, fcntl, base64, signal, stat, errno

try:
    import cPickle as pickle
except ImportError:
    import pickle

class ClientOptions(options.ConchOptions):
    
    synopsis = """Usage:   conch [options] host [command]
"""
    
    optParameters = options.ConchOptions.optParameters + \
                    [['escape', 'e', '~'],
                      ['localforward', 'L', None, 'listen-port:host:port   Forward local port to remote address'],
                      ['remoteforward', 'R', None, 'listen-port:host:port   Forward remote port to local address'],
                     ]

    optFlags = options.ConchOptions.optFlags + \
                [['null', 'n', 'Redirect input from /dev/null.'],
                 ['fork', 'f', 'Fork to background after authentication.'],
                 ['tty', 't', 'Tty; allocate a tty even if command is given.'],
                 ['notty', 'T', 'Do not allocate a tty.'],
                 ['noshell', 'N', 'Do not execute a shell or command.'],
                 ['subsystem', 's', 'Invoke command (mandatory) as SSH2 subsystem.'],
                ]

    localForwards = []
    remoteForwards = []

    def opt_escape(self, esc):
        "Set escape character; ``none'' = disable"
        if esc == 'none':
            self['escape'] = None
        elif esc[0] == '^' and len(esc) == 2:
            self['escape'] = chr(ord(esc[1])-64)
        elif len(esc) == 1:
            self['escape'] = esc
        else:
            sys.exit("Bad escape character '%s'." % esc)

    def opt_localforward(self, f):
        "Forward local port to remote address (lport:host:port)"
        localPort, remoteHost, remotePort = f.split(':') # doesn't do v6 yet
        localPort = int(localPort)
        remotePort = int(remotePort)
        self.localForwards.append((localPort, (remoteHost, remotePort)))

    def opt_remoteforward(self, f):
        """Forward remote port to local address (rport:host:port)"""
        remotePort, connHost, connPort = f.split(':') # doesn't do v6 yet
        remotePort = int(remotePort)
        connPort = int(connPort)
        self.remoteForwards.append((remotePort, (connHost, connPort)))

    def parseArgs(self, host, *command):
        self['host'] = host
        self['command'] = ' '.join(command)

# Rest of code in "run"
options = None
conn = None
exitStatus = 0
keyAgent = None
old = None
_inRawMode = 0
_savedRawMode = None

def run():
    global options, old
    args = sys.argv[1:]
    if '-l' in args: # cvs is an idiot
        i = args.index('-l')
        args = args[i:i+2]+args
        del args[i+2:i+4]
    for arg in args[:]:
        try:
            i = args.index(arg)
            if arg[:2] == '-o' and args[i+1][0]!='-':
                args[i:i+2] = [] # suck on it scp
        except ValueError:
            pass
    options = ClientOptions()
    try:
        options.parseOptions(args)
    except usage.UsageError, u:
        options.opt_help()
        print 'ERROR: %s' % u
        sys.exit(1)
    if options['log']:
        realout = sys.stdout
        log.startLogging(sys.stderr)
        sys.stdout = realout
    else:
        log.discardLogs()
    doConnect()
    fd = sys.stdin.fileno()
    try:
        old = tty.tcgetattr(fd)
    except:
        old = None
    try:
        reactor.run()
    finally:
        if old:
            tty.tcsetattr(fd, tty.TCSANOW, old)
        if (options['command'] and options['tty']) or not options['notty']:
            signal.signal(signal.SIGWINCH, signal.SIG_DFL)
    if sys.stdout.isatty() and not options['command']:
        print 'Connection to %s closed.' % options['host']
    sys.exit(exitStatus)

def handleError():
    from twisted.python import failure
    global exitStatus
    exitStatus = 2
    reactor.stop()
    log.err(failure.Failure())
    raise

def doConnect():
    log.deferr = handleError # HACK
    if '@' in options['host']:
        options['user'], options['host'] = options['host'].split('@',1)
    if not options.identitys:
        options.identitys = ['~/.ssh/id_rsa', '~/.ssh/id_dsa']
    host = options['host']
    if not options['user']:
        options['user'] = getpass.getuser() 
    if not options['port']:
        options['port'] = 22
    else:
        options['port'] = int(options['port'])
    host = options['host']
    port = options['port']
    vhk = default.verifyHostKey
    uao = default.SSHUserAuthClient(options['user'], options, SSHConnection())
    connect.connect(host, port, options, vhk, uao).addErrback(_ebExit)

def _ebExit(f):
    global exitStatus
    if hasattr(f.value, 'value'):
        s = f.value.value
    else:
        s = str(f)
    exitStatus = "conch: exiting with error %s" % f
    reactor.stop()

def onConnect():
#    if keyAgent and options['agent']:
#        cc = protocol.ClientCreator(reactor, SSHAgentForwardingLocal, conn)
#        cc.connectUNIX(os.environ['SSH_AUTH_SOCK'])
    if hasattr(conn.transport, 'sendIgnore'):
        keepAlive()
    if options.localForwards:
        for localPort, hostport in options.localForwards:
            reactor.listenTCP(localPort,
                        forwarding.SSHListenForwardingFactory(conn,
                            hostport,
                            SSHListenClientForwardingChannel))
    if options.remoteForwards:
        for remotePort, hostport in options.remoteForwards:
            log.msg('asking for remote forwarding for %s:%s' %
                    (remotePort, hostport))
            conn.requestRemoteForwarding(remotePort, hostport)
    if not options['noshell'] or options['agent']:
        conn.openChannel(SSHSession())
    if options['fork']:
        if os.fork():
            os._exit(0)
        os.setsid()
        for i in range(3):
            try:
                os.close(i)
            except OSError, e:
                import errno
                if e.errno != errno.EBADF:
                    raise
def keepAlive():
    conn.transport.sendIgnore('')
    reactor.callLater(300, keepAlive)

def stopConnection():
    if options.remoteForwards:
        for remotePort, hostport in options.remoteForwards:
            log.msg('cancelling %s:%s' % (remotePort, hostport))
            conn.cancelRemoteForwarding(remotePort)
    reactor.stop()


class SSHConnection(connection.SSHConnection):
    def serviceStarted(self):
        global conn, keyAgent
        conn = self
        self.remoteForwards = {}
        if keyAgent:
            keyAgent.transport.loseConnection()
        onConnect()

    def serviceStopped(self):
        stopConnection()

    def requestRemoteForwarding(self, remotePort, hostport):
        data = forwarding.packGlobal_tcpip_forward(('0.0.0.0', remotePort))
        self.sendGlobalRequest('tcpip-forward', data)
        self.remoteForwards[remotePort] = hostport
        log.msg('requesting remote forwarding %s:%s' %(remotePort, hostport))
        log.msg(repr(self.remoteForwards))

    def cancelRemoteForwarding(self, remotePort):
        data = forwarding.packGlobal_tcpip_forward(('0.0.0.0', remotePort))
        self.sendGlobalRequest('cancel-tcpip-forward', data)
        log.msg('cancelling remote forwarding %s' % remotePort)
        del self.remoteForwards[remotePort]
        log.msg(repr(self.remoteForwards))

    def channel_forwarded_tcpip(self, windowSize, maxPacket, data):
        remoteHP, origHP = forwarding.unpackOpen_forwarded_tcpip(data)
        log.msg(self.remoteForwards)
        if self.remoteForwards.has_key(remoteHP[1]):
            connectHP = self.remoteForwards[remoteHP[1]]
            log.msg('connect forwarding %s' % (connectHP,))
            return SSHConnectForwardingChannel(connectHP,
                                            remoteWindow = windowSize,
                                            remoteMaxPacket = maxPacket,
                                            conn = self)
        else:
            return connection.OPEN_CONNECT_FAILED, "don't know about that port"

    def channel_auth_agent_openssh_com(self, windowSize, maxPacket, data):
        if options['agent'] and keyAgent:
            return agent.SSHAgentForwardingChannel(remoteWindow = windowSize,
                                             remoteMaxPacket = maxPacket,
                                             conn = self)
        else:
            return connection.OPEN_CONNECT_FAILED, "don't have an agent"

class SSHSession(channel.SSHChannel):

    name = 'session'

    def channelOpen(self, foo):
        #global globalSession
        #globalSession = self
        # turn off local echo
        log.msg('session %s open' % self.id)
        if options['agent']:
            d = self.conn.sendRequest(self, 'auth-agent-req@openssh.com', '', wantReply=1)
            d.addBoth(lambda x:log.msg(x))
        if options['noshell']: return
        if (options['command'] and options['tty']) or not options['notty']:
            _enterRawMode()
        c = session.SSHSessionClient()
        if options['escape'] and not options['notty']:
            self.escapeMode = 1
            c.dataReceived = self.handleInput
        else:
            c.dataReceived = self.write
        c.connectionLost = lambda x=None,s=self:s.sendEOF()
        self.stdio = stdio.StandardIO(c)
        fd = 0
        if options['subsystem']:
            self.conn.sendRequest(self, 'subsystem', \
                common.NS(options['command']))
        elif options['command']:
            if options['tty']:
                term = os.environ['TERM']
                winsz = fcntl.ioctl(fd, tty.TIOCGWINSZ, '12345678')
                winSize = struct.unpack('4H', winsz)
                ptyReqData = session.packRequest_pty_req(term, winSize, '')
                self.conn.sendRequest(self, 'pty-req', ptyReqData)
                signal.signal(signal.SIGWINCH, self._windowResized)
            self.conn.sendRequest(self, 'exec', \
                common.NS(options['command']))
        else:
            if not options['notty']:
                term = os.environ['TERM']
                winsz = fcntl.ioctl(fd, tty.TIOCGWINSZ, '12345678')
                winSize = struct.unpack('4H', winsz)
                ptyReqData = session.packRequest_pty_req(term, winSize, '')
                self.conn.sendRequest(self, 'pty-req', ptyReqData)
                signal.signal(signal.SIGWINCH, self._windowResized)
            self.conn.sendRequest(self, 'shell', '')
            #if hasattr(conn.transport, 'transport'):
            #    conn.transport.transport.setTcpNoDelay(1)

    def handleInput(self, char):
        #log.msg('handling %s' % repr(char))
        if char in ('\n', '\r'):
            self.escapeMode = 1
            self.write(char)
        elif self.escapeMode == 1 and char == options['escape']:
            self.escapeMode = 2
        elif self.escapeMode == 2:
            self.escapeMode = 1 # so we can chain escapes together
            if char == '.': # disconnect
                log.msg('disconnecting from escape')
                stopConnection()
                return
            elif char == '\x1a': # ^Z, suspend
                def _():
                    _leaveRawMode()
                    sys.stdout.flush()
                    sys.stdin.flush()
                    os.kill(os.getpid(), signal.SIGTSTP)
                    _enterRawMode()
                reactor.callLater(0, _)
                return
            elif char == 'R': # rekey connection
                log.msg('rekeying connection')
                self.conn.transport.sendKexInit()
                return
            elif char == '#': # display connections
                self.stdio.write('\r\nThe following connections are open:\r\n')
                channels = self.conn.channels.keys()
                channels.sort()
                for channelId in channels:
                    self.stdio.write('  #%i %s\r\n' % (channelId, str(self.conn.channels[channelId])))
                return
            self.write('~' + char)
        else:
            self.escapeMode = 0
            self.write(char)

    def dataReceived(self, data):
        self.stdio.write(data)

    def extReceived(self, t, data):
        if t==connection.EXTENDED_DATA_STDERR:
            log.msg('got %s stderr data' % len(data))
            sys.stderr.write(data)
            sys.stderr.flush()

    def eofReceived(self):
        log.msg('got eof')
        self.stdio.closeStdin()

    def closed(self):
        global old
        log.msg('closed %s' % self)
        if len(self.conn.channels) == 1 and not (options['noshell'] and not options['nocache']): # just us left
            stopConnection()
        elif not options['nocache']: # fork into the background
            if os.fork():
                if old:
                    fd = sys.stdin.fileno()
                    tty.tcsetattr(fd, tty.TCSANOW, old)
                if (options['command'] and options['tty']) or \
                    not options['notty']:
                    signal.signal(signal.SIGWINCH, signal.SIG_DFL)
                os._exit(0)
            os.setsid()
            for i in range(3):
                try:
                    os.close(i)
                except OSError, e:
                    import errno
                    if e.errno != errno.EBADF:
                        raise

    def request_exit_status(self, data):
        global exitStatus
        exitStatus = int(struct.unpack('>L', data)[0])
        log.msg('exit status: %s' % exitStatus)

    def sendEOF(self):
        self.conn.sendEOF(self)

    def stopWriting(self):
        self.stdio.stopReading()

    def startWriting(self):
        self.stdio.startReading()

    def _windowResized(self, *args):
        winsz = fcntl.ioctl(0, tty.TIOCGWINSZ, '12345678')
        winSize = struct.unpack('4H', winsz)
        newSize = winSize[1], winSize[0], winSize[2], winSize[3]
        self.conn.sendRequest(self, 'window-change', struct.pack('!4L', *newSize))
           

class SSHListenClientForwardingChannel(forwarding.SSHListenClientForwardingChannel):

    def closed(self):
        forwarding.SSHListenClientForwardingChannel.closed(self)
        if len(self.conn.channels) == 1 and not (options['noshell'] and not options['nocache']): # just us left
            stopConnection()

class SSHConnectForwardingChannel(forwarding.SSHConnectForwardingChannel):

    def closed(self):
        forwarding.SSHConnectForwardingChannel.closed(self)
        if len(self.conn.channels) == 1 and not (options['noshell'] and not options['nocache']): # just us left
            stopConnection()

def _leaveRawMode():
    global _inRawMode
    if not _inRawMode:
        return
    fd = sys.stdin.fileno()
    tty.tcsetattr(fd, tty.TCSANOW, _savedMode)
    _inRawMode = 0

def _enterRawMode():
    global _inRawMode, _savedMode
    if _inRawMode:
        return
    fd = sys.stdin.fileno()
    try:
        old = tty.tcgetattr(fd)
        new = old[:]
    except:
        log.msg('not a typewriter!')
    else:
        # iflage
        new[0] = new[0] | tty.IGNPAR
        new[0] = new[0] & ~(tty.ISTRIP | tty.INLCR | tty.IGNCR | tty.ICRNL |
                            tty.IXON | tty.IXANY | tty.IXOFF)
        if hasattr(tty, 'IUCLC'):
            new[0] = new[0] & ~tty.IUCLC

        # lflag
        new[3] = new[3] & ~(tty.ISIG | tty.ICANON | tty.ECHO | tty.ECHO | 
                            tty.ECHOE | tty.ECHOK | tty.ECHONL)
        if hasattr(tty, 'IEXTEN'):
            new[3] = new[3] & ~tty.IEXTEN

        #oflag
        new[1] = new[1] & ~tty.OPOST

        new[6][tty.VMIN] = 1
        new[6][tty.VTIME] = 0

        _savedMode = old
        tty.tcsetattr(fd, tty.TCSANOW, new)
        #tty.setraw(fd)
        _inRawMode = 1

if __name__ == '__main__':
    run()

