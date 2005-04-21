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

"""Creation of  Windows shortcuts.

Requires win32all.
"""

from win32com.shell import shell
import pythoncom
import os


def open(filename):
    """Open an existing shortcut for reading.

    @return: The shortcut object
    @rtype: Shortcut
    """
    sc=Shortcut()
    sc.load(filename)
    return sc


class Shortcut:
    """A shortcut on Win32.
    >>> sc=Shortcut(path, arguments, description, workingdir, iconpath, iconidx)
    @param path: Location of the target
    @param arguments: If path points to an executable, optional arguments to
                      pass
    @param description: Human-readable decription of target
    @param workingdir: Directory from which target is launched
    @param iconpath: Filename that contains an icon for the shortcut
    @param iconidx: If iconpath is set, optional index of the icon desired
    """

    def __init__(self, 
                 path=None,
                 arguments=None, 
                 description=None,
                 workingdir=None,
                 iconpath=None,
                 iconidx=0):
        self._base = pythoncom.CoCreateInstance(
            shell.CLSID_ShellLink, None,
            pythoncom.CLSCTX_INPROC_SERVER, shell.IID_IShellLink
        )
        data = map(None, 
                   ['"%s"' % os.path.abspath(path), arguments, description,
                    os.path.abspath(workingdir), os.path.abspath(iconpath)], 
                   ("SetPath", "SetArguments", "SetDescription",
                   "SetWorkingDirectory") )
        for value, function in data:
            if value and function:
                # call function on each non-null value
                getattr(self, function)(value)
        if iconpath:
            self.SetIconLocation(iconpath, iconidx)

    def load( self, filename ):
        """Read a shortcut file from disk."""
        self._base.QueryInterface(pythoncom.IID_IPersistFile).Load(filename)
    
    def save( self, filename ):
        """Write the shortcut to disk.

        The file should be named something.lnk.
        """
        self._base.QueryInterface(pythoncom.IID_IPersistFile).Save(filename, 0)
    
    def __getattr__( self, name ):
        if name != "_base":
            return getattr(self._base, name)
        raise AttributeError, "%s instance has no attribute %s" % \
                (self.__class__.__name__, name)
