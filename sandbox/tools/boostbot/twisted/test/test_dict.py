
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

from twisted.trial import unittest
from twisted.protocols import dict

paramString = "\"This is a dqstring \\w\\i\\t\\h boring stuff like: \\\"\" and t\\hes\\\"e are a\\to\\ms"
goodparams = ["This is a dqstring with boring stuff like: \"", "and", "thes\"e", "are", "atoms"]

class ParamTest(unittest.TestCase):
    def testParseParam(self):
        """Testing command response handling"""
        params = []
        rest = paramString
        while 1:
            (param, rest) = dict.parseParam(rest)
            if param == None:
                break
            params.append(param)
        self.failUnlessEqual(params, goodparams)#, "DictClient.parseParam returns unexpected results")
