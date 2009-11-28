//  (C) Copyright Andy Tompkins 2009. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  libs/uuid/test/test_tagging.cpp  -------------------------------//

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>

class object
{
public:
    object()
        : tag(boost::uuids::random_generator()())
        , state(0)
    {}
    
    explicit object(int state)
        : tag(boost::uuids::random_generator()())
        , state(state)
    {}
    
    object(object const& rhs)
        : tag(rhs.tag)
        , state(rhs.state)
    {}
    
    bool operator==(object const& rhs) const {
        return tag == rhs.tag;
    }
    
    object& operator=(object const& rhs) {
        tag = rhs.tag;
        state = rhs.state;
    }
    
    int get_state() const { return state; }
    void set_state(int new_state) { state = new_state; }
    
private:
    boost::uuids::uuid tag;
    
    int state;
};

template <typename elem, typename traits>
std::basic_ostream<elem, traits>& operator<<(std::basic_ostream<elem, traits>& os, object const& o)
{
    os << o.get_state();
    return os;
}

int test_main(int, char*[])
{
    //using boost::test_tools::output_test_stream;
    
    object o1(1);

    object o2 = o1;
    BOOST_CHECK_EQUAL(o1, o2);

    o2.set_state(2);
    BOOST_CHECK_EQUAL(o1, o2);
    
    object o3;
    o3.set_state(3);

    BOOST_CHECK_NE(o1, o3);
    BOOST_CHECK_NE(o2, o3);
    
    return 0;
}
