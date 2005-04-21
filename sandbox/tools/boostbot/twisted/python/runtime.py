
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

# System imports
import os
import sys
import time

def shortPythonVersion():
    hv = sys.hexversion
    major = (hv & 0xff000000L) >> 24
    minor = (hv & 0x00ff0000L) >> 16
    teeny = (hv & 0x0000ff00L) >> 8
    return "%s.%s.%s" % (major,minor,teeny)

knownPlatforms = {
    'nt': 'win32',
    'posix': 'posix',
    'java': 'java',
    'org.python.modules.os': 'java',
    }

_timeFunctions = {
    #'win32': time.clock,
    'win32': time.time,
    }

class Platform:
    """Gives us information about the platform we're running on"""

    type = knownPlatforms.get(os.name)
    seconds = staticmethod(_timeFunctions.get(type, time.time))

    def __init__(self, name=None):
        if name is not None:
            self.type = knownPlatforms.get(name)
            self.seconds = _timeFunctions.get(self.type, time.time)
    
    def isKnown(self):
        """Do we know about this platform?"""
        return self.type != None
    
    def getType(self):
        """Return 'posix', 'win32' or 'java'"""
        return self.type

    def isMacOSX(self):
        """Return if we are runnng on Mac OS X."""
        return sys.platform == "darwin"
    
    def isWinNT(self):
        """Are we running in Windows NT?"""
        if self.getType() == 'win32':
            import _winreg
            try:
                k=_winreg.OpenKeyEx(_winreg.HKEY_LOCAL_MACHINE,
                                    r'Software\Microsoft\Windows NT\CurrentVersion')
                _winreg.QueryValueEx(k, 'SystemRoot')
                return 1
            except WindowsError:
                return 0
        # not windows at all...
        return 0

platform = Platform()
platformType = platform.getType()
seconds = platform.seconds
