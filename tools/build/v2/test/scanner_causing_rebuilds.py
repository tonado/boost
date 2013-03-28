#!/usr/bin/python

# Copyright 2012 Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#   Tests for a bug causing Boost Build's scanner targets to be rebuilt.
# unnecessarily in the following scenario:
#  * We want to build target X requiring target A.
#  * We have a multi-file action generating targets A & B.
#  * Out action generates target B with a more recent timestamp than target A.
#  * Target A includes target B.
#  * Target A has a registered include scanner.
# Now even if our targets A & B have already been built and are up-to-date
# (e.g. in a state left by a previous successful build run), our scanner target
# tasked with scanning target A will be marked for updating, thus causing any
# targets depending on it to be updated/rebuilt as well.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("foo.jam", r"""
import common ;
import generators ;
import modules ;
import type ;
import types/cpp ;

type.register FOO : foo ;
type.register BAR : bar ;
generators.register-standard foo.foo : FOO : CPP BAR ;

local rule sleep-cmd ( delay )
{
    if [ modules.peek : NT ]
    {
        return ping 127.0.0.1 -n $(delay) -w 1000 >NUL ;
    }
    else
    {
        return sleep $(delay) ;
    }
}

.touch = [ common.file-creation-command ] ;
.sleep = [ sleep-cmd 2 ] ;

rule foo ( cpp bar : foo : properties * )
{
    # We add the INCLUDE relationship between our generated CPP & BAR targets
    # explicitly instead of relying on Boost Jam's internal implementation
    # detail - automatically adding such relationships between all files
    # generated by the same action. This way our test will continue to function
    # correctly even if the related Boost Jam implementation detail changes.
    # Note that adding this relationship by adding an #include directive in our
    # generated CPP file is not good enough as such a relationship would get
    # added only after the scanner target's relationships have already been
    # established and they (as affected by our initial INCLUDE relationship) are
    # the original reason for this test failing.
    INCLUDES $(cpp) : $(bar) ;
}

actions foo
{
    $(.touch) "$(<[1])"
    $(.sleep)
    $(.touch) "$(<[2])"
}
""")
t.write("x.foo", "")
t.write("jamroot.jam", """\
import foo ;
lib x : x.foo : <link>static ;
""")


# Get everything built once.
t.run_build_system()

# Simply rerunning the build without touching any of its source target files
# should not cause any files to be affected.
t.run_build_system()
t.expect_nothing_more()
