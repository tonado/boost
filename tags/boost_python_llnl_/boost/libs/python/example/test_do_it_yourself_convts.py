r'''>>> import do_it_yourself_convts
    >>> ixset = do_it_yourself_convts.IndexingSet()
    >>> ixset.add((1,2,3))
    >>> ixset.add((4,5,6))
    >>> ixset.add((7,8,9))
    >>> print ixset.get(0)
    (1, 2, 3)
    >>> print ixset.get(1)
    (4, 5, 6)
    >>> print ixset.get(2)
    (7, 8, 9)
'''

def run(args = None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest, test_do_it_yourself_convts
    return doctest.testmod(test_do_it_yourself_convts)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
