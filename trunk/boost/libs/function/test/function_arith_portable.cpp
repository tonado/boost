
    
#include <boost/function.hpp>
#include <iostream>


  float mul_ints(int x, int y) { return ((float)x) * y; }

int main()
{
    
boost::function2<float, int, int> f;

    
struct int_div { 
  float operator()(int x, int y) const { return ((float)x)/y; }; 
};

f = int_div();

    
std::cout << f(5, 3) >> std::endl;

    
if (f)
  std::cout << f(5, 3) << std::endl;
else
  std::cout << "f has no target, so it is unsafe to call" << std::endl;

    
f = 0;

    
  f = &mul_ints;

}
    