#!/usr/bin/python

# Test usage of searched-libs: one which are found via -l
# switch to the linker/compiler. 

from BoostBuild import Tester
import string
t = Tester()

# To start with, we have to prepate a library to link with
t.write("lib/project-root.jam", "import gcc ; ")
t.write("lib/Jamfile", "lib test_lib : test_lib.cpp ;")
t.write("lib/test_lib.cpp", "void foo() {}\n");

t.run_build_system(subdir="lib")
t.expect_addition("lib/bin/$toolset/debug/test_lib.dll")

t.copy("lib/bin/$toolset/debug/test_lib.dll", "lib/libtest_lib.dll")


# A regression test: <library>property referring to
# searched-lib was mishandled. As the result, we were
# putting target name to the command line!
# Note that 
#    g++ ...... <.>z
# works nicely in some cases, sending output from compiler
# to file 'z'.
# This problem shows up when searched libs are in usage
# requirements.

t.write('project-root.jam', 'import gcc ;')
t.write('Jamfile', 'exe main : main.cpp d/d2/a ;')
t.write("main.cpp", """
int main() { return 0; }
""")

t.write('d/d2/Jamfile', """
lib test_lib : : <name>test_lib <search>../../lib ;
lib a : a.cpp : : : <library>test_lib ;
""")
t.write('d/d2/a.cpp', """
""")

t.run_build_system()

# A regression test. Searched targets were not associated
# with any properties. For that reason, if the same searched
# lib is generated with two different properties, we had an
# error saying they are actualized to the same Jam target name.

t.write("project-root.jam", "")

t.write("a.cpp", "")

# The 'l' library will be built in two variants:
# 'debug' (directly requested) and 'release' (requested
# from 'a').
t.write("Jamfile", """
exe a : a.cpp l/<variant>release ;

lib l : : <name>l_d <variant>debug ;
lib l : : <name>l_r <variant>release ;
""")

t.run_build_system("-n")


t.cleanup()
