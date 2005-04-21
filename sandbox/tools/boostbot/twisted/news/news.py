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

"""
Maintainer: U{Jp Calderone<mailto:exarkun@twistedmatrix.com>}

Stability: semi-stable
"""

from twisted.protocols import nntp
from twisted.internet import protocol, reactor

import time

class NNTPFactory(protocol.ServerFactory):
    """A factory for NNTP server protocols."""
    
    protocol = nntp.NNTPServer
    
    def __init__(self, backend):
        self.backend = backend
    
    def buildProtocol(self, connection):
        p = self.protocol()
        p.factory = self
        return p


class UsenetClientFactory(protocol.ClientFactory):
    def __init__(self, groups, storage):
        self.lastChecks = {}
        self.groups = groups
        self.storage = storage


    def clientConnectionLost(self, connector, reason):
        pass


    def clientConnectionFailed(self, connector, reason):
        print 'Connection failed: ', reason
    
    
    def updateChecks(self, addr):
        self.lastChecks[addr] = time.mktime(time.gmtime())


    def buildProtocol(self, addr):
        last = self.lastChecks.setdefault(addr, time.mktime(time.gmtime()) - (60 * 60 * 24 * 7))
        p = nntp.UsenetClientProtocol(self.groups, last, self.storage)
        p.factory = self
        return p


# XXX - Maybe this inheritence doesn't make so much sense?
class UsenetServerFactory(NNTPFactory):
    """A factory for NNTP Usenet server protocols."""

    protocol = nntp.NNTPServer

    def __init__(self, backend, remoteHosts = None, updatePeriod = 60):
        NNTPFactory.__init__(self, backend)
        self.updatePeriod = updatePeriod
        self.remoteHosts = remoteHosts or []
        self.clientFactory = UsenetClientFactory(self.remoteHosts, self.backend)


    def startFactory(self):
        self._updateCall = reactor.callLater(0, self.syncWithRemotes)


    def stopFactory(self):
        if self._updateCall:
            self._updateCall.cancel()
            self._updateCall = None


    def buildProtocol(self, connection):
        p = self.protocol()
        p.factory = self
        return p


    def syncWithRemotes(self):
        for remote in self.remoteHosts:
            reactor.connectTCP(remote, 119, self.clientFactory)
        self._updateCall = reactor.callLater(self.updatePeriod, self.syncWithRemotes)


# backwards compatability
Factory = UsenetServerFactory
