#!/usr/bin/python

# Copyright (C) Vladimir Prus 2003.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#         http://www.boost.org/LICENSE_1_0.txt)

# Test the 'glob' rule in Jamfile context.

import BoostBuild

# Create a temporary working directory.
t = BoostBuild.Tester()

t.write("jamroot.jam", """
""")

t.write("d1/a.cpp", """
int main() {}
""")

t.write("d1/jamfile.jam", """
exe a : [ glob *.cpp ] ../d2/d//l ;
""")

t.write("d2/d/l.cpp", """
#if defined(_WIN32)
__declspec(dllexport)
void force_import_lib_creation() {}
#endif
""")

t.write("d2/d/jamfile.jam", """
lib l : [ glob *.cpp ] ;
""")

t.write("d3/d/jamfile.jam", """
exe a : [ glob ../*.cpp ] ;
""")

t.write("d3/a.cpp", """
int main() {}
""")

t.run_build_system(subdir="d1")
t.expect_addition("d1/bin/$toolset/debug/a.exe")

t.run_build_system(subdir="d3/d")
t.expect_addition("d3/d/bin/$toolset/debug/a.exe")

t.rm("d2/d/bin")

t.run_build_system(subdir="d2/d")
t.expect_addition("d2/d/bin/$toolset/debug/l.dll")


# Test that when 'source-location' is explicitly-specified glob works relatively
# to the source location.
t.rm("d1")

t.write("d1/src/a.cpp", """
int main() {}
""")

t.write("d1/jamfile.jam", """
project : source-location src ;
exe a : [ glob *.cpp ] ../d2/d//l ;
""")

t.run_build_system(subdir="d1")
t.expect_addition("d1/bin/$toolset/debug/a.exe")

# Test that wildcards can include directories. Also test exclusion patterns.
t.rm("d1")

t.write("d1/src/foo/a.cpp", """
void bar();
int main() { bar(); }
""")

t.write("d1/src/bar/b.cpp", """
void bar() {}
""")

t.write("d1/src/bar/bad.cpp", """
very bad non-compilable file
""")

t.write("d1/jamfile.jam", """
project : source-location src ;
exe a : [ glob foo/*.cpp bar/*.cpp : bar/bad* ] ../d2/d//l ;
""")

t.run_build_system(subdir="d1")
t.expect_addition("d1/bin/$toolset/debug/a.exe")


# Test that 'glob-tree' works.
t.rm("d1/bin/$toolset/debug/a.exe")

t.write("d1/jamfile.jam", """
project : source-location src ;
exe a : [ glob-tree *.cpp : bad* ] ../d2/d//l ;
""")

t.run_build_system(subdir="d1")
t.expect_addition("d1/bin/$toolset/debug/a.exe")


# Test that directory names in patterns for 'glob-tree' are rejected.
t.write("d1/jamfile.jam", """
project : source-location src ;
exe a : [ glob-tree foo/*.cpp bar/*.cpp : bad* ] ../d2/d//l ;
""")

t.run_build_system(subdir="d1", status=1)
t.expect_output_line("error: The patterns * may not include directory")


t.rm("d1/src/bar/bad.cpp")

# Test that 'glob' works with absolute names.
t.rm("d1/bin")

# Note that to get current dir, we use bjam's PWD, not Python's os.getcwd(),
# because the former will always return long path while the latter might return
# a short path, and that will confuse path.glob.
t.write("d1/jamfile.jam", """
project : source-location src ;
local pwd = [ PWD ] ;  # Always absolute
exe a : [ glob $(pwd)/src/foo/*.cpp $(pwd)/src/bar/*.cpp ] ../d2/d//l ;
""")

t.run_build_system(subdir="d1")
t.expect_addition("d1/bin/$toolset/debug/a.exe")


# Regression test: glob excludes used to be broken when building from a
# subdirectory.
t.rm(".")

t.write("jamroot.jam", """
build-project p ;
""")

t.write("p/p.c", """
int main() {}
""")

t.write("p/p_x.c", """
int main() {}
""")

t.write("p/jamfile.jam", """
exe p : [ glob *.c : p_x.c ] ;
""")

t.run_build_system(subdir="p")
t.expect_addition("p/bin/$toolset/debug/p.exe")

t.cleanup()
