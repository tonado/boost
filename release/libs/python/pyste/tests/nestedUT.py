# Copyright Bruno da Silva de Oliveira 2003. Use, modification and 
# distribution is subject to the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
import unittest
from _nested import *

class NestedTest(unittest.TestCase):

    def testIt(self):              
        self.assertEqual(Root.staticXValue, 10)
        self.assertEqual(Root.Y.staticYValue, 20)
        z = Root.Y.Z()
        z.valueZ = 3
        self.assertEqual(z.valueZ, 3)
        

if __name__ == '__main__':
    unittest.main()
