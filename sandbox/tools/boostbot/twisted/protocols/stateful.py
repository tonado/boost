# -*- test-case-name: twisted.test.test_stateful -*-

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

from twisted.internet import protocol

try:
    from cStringIO import StringIO
except ImportError:
    from StringIO import StringIO

from struct import pack, unpack

class StatefulProtocol(protocol.Protocol):
    """A Protocol that stores state for you.

    state is a pair (function, num_bytes). When num_bytes bytes of data arrives
    from the network, function is called. It is expected to return the next
    state or None to keep same state. Initial state is returned by
    getInitialState (override it).
    """
    _sful_data = None, None, 0

    def makeConnection(self, transport):
        protocol.Protocol.makeConnection(self, transport)
        self._sful_data = self.getInitialState(), StringIO(), 0

    def getInitialState(self):
        raise NotImplementedError

    def dataReceived(self, data):
        state, buffer, offset = self._sful_data
        buffer.seek(0, 2)
        buffer.write(data)
        blen = buffer.tell() # how many bytes total is in the buffer
        buffer.seek(offset)
        while blen - offset >= state[1]:
            d = buffer.read(state[1])
            offset += state[1]
            next = state[0](d)
            if self.transport.disconnecting: # XXX: argh stupid hack borrowed right from LineReceiver
                return # dataReceived won't be called again, so who cares about consistent state
            if next:
                state = next
        if offset != 0:
            b = buffer.read()
            buffer.reset()
            buffer.truncate()
            buffer.write(b)
            offset = 0
        self._sful_data = state, buffer, offset

