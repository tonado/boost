
# Declare a couple of functions called from Boost.Build
#
# Each function will receive as many arguments as there ":"-separated
# arguments in bjam call. Each argument is a list of strings.
# As a special exception (aka bug), if no arguments are passed in bjam,
# Python function will be passed a single empty list.
#
# All Python functions must return a list of strings, which may be empty.

def test1(l):
    return ["foo", "bar"]
    
def test2(l, l2):
    return [l[0], l2[0]]