'''
>>> from auto_ptr_ext import *
>>> x = X(42)
>>> x.value()
42
>>> look(x), look(x)
(42, 42)

>>> maybe_steal(x, 0)
42
>>> look(x)
42

>>> maybe_steal(x, 1)
42
>>> (not '--broken-auto-ptr' in sys.argv) and look(x) or -1
-1

>>> x = X(69)
>>> steal(x)
69
>>> (not '--broken-auto-ptr' in sys.argv) and look(x) or -1
-1

>>> if not '--broken-auto-ptr' in sys.argv:
...     try: x.value()
...     except TypeError: pass
...     else: print 'expected a TypeError exception'

>>> x = make()
>>> look(x)
77

>>> z = callback(lambda z: z)
>>> z.value()
77

>>> extract(x).value()
77

#
# Test derived to base conversions
#

>>> y = Y(42)
>>> y.value()
42

>>> maybe_steal(y, 0)
42

>>> try: maybe_steal(y, 0)
... except TypeError: pass
... else: print 'expected a TypeError exception'

>>> y.value()
42

>>> steal(y)
42

>>> if not '--broken-auto-ptr' in sys.argv:
...     try: y.value()
...     except TypeError: pass
...     else: print 'expected a TypeError exception'

'''

def run(args = None):
    import sys
    import doctest

    if args is not None:
        sys.argv = args
    return doctest.testmod(sys.modules.get(__name__))
    
if __name__ == '__main__':
    print "running..."
    import sys
    sys.exit(run()[0])
