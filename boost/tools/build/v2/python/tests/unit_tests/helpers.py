# Copyright Pedro Ferreira 2005. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import sys
import os.path

# Setup the environment
script_dir = os.path.dirname (__file__)
src_dir = os.path.split (script_dir) [0]
if not os.path.isabs (src_dir):
    src_dir = os.path.join (os.getcwd (), src_dir)
src_dir = os.path.normpath (src_dir)
src_dir = os.path.normpath (os.path.join (src_dir, '..'))
sys.path.append (src_dir)

def reset ():
    """ Cleans up all modules' state.
    """
    from boost.build.build import feature, property, property_set, toolset, scanner, type, action
    from boost.build.tools import builtin
    feature.reset ()
    property.reset ()
    property_set.reset ()
    toolset.reset ()
    scanner.reset ()
    type.reset ()
    action.reset ()
    builtin.reset ()
    