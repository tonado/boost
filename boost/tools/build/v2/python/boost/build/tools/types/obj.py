# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

from boost.build.build import type

def register ():
    type.register_type ('OBJ', ['obj'], None, ['NT', 'CYGWIN'])
    type.register_type ('OBJ', ['o'])

register ()
