
import os
import string
import sys

class failure_exception:
    def __init__( self, rc ):
        self.rc_ = rc

def system( commands ):
    if sys.platform == 'win32':
        f = open( 'tmp.cmd', 'w' )
        f.write( string.join( commands, '\n' ) )
        f.close()
        rc = os.system( 'tmp.cmd' )
        return rc
    else:
        rc = os.system( '&&'.join( commands ) )
        return rc

    
def checked_system( commands, valid_return_codes = [ 0 ] ):
    rc = system( commands ) 
    if rc not in [ 0 ] + valid_return_codes: raise failure_exception( rc )
    return rc
