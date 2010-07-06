#include <boost/functional/hash.hpp>

namespace test
{
    struct base {};
    std::size_t hash_value(base const&) { return 0; }
    
    struct converts { operator base() const { return base(); } };
}

int main() {
    boost::hash<test::converts> hash;
    test::converts x;
    
    hash(x);
}