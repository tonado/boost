
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <boost/plugin/virtual_constructors.hpp>
#include <string>

class Weapon {
public:
    virtual void fire() = 0;
    virtual ~Weapon() {}
};

namespace boost { namespace plugin {
template<>
struct virtual_constructors<Weapon> {
    typedef mpl::list<mpl::list<std::string>,
                      mpl::list<std::string, int>    
    > type;

};
}}

#endif
