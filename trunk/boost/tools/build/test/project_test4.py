#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()


t.set_tree("project-test4")

t.run_build_system()

t.expect_addition("bin/gcc/debug/threading-single/a.obj")
t.expect_content("bin/gcc/debug/threading-single/a.obj",
"""gcc/debug/threading-single
a.cpp
""")

t.expect_addition("bin/gcc/debug/threading-single/a.exe")
t.expect_content("bin/gcc/debug/threading-single/a.exe",
"gcc/debug/threading-single\n" +
"bin/gcc/debug/threading-single/a.obj lib/bin/gcc/debug/optimization-on/threading-single/b.obj\n"
)

t.expect_addition("lib/bin/gcc/debug/optimization-on/threading-single/b.obj")
t.expect_content("lib/bin/gcc/debug/optimization-on/threading-single/b.obj",
"""gcc/debug/optimization-on/threading-single
lib/b.cpp
""")

os.chdir(t.original_workdir)