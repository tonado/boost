//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#include <boost/range/algorithm/max_element.hpp>

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/assign.hpp>
#include <boost/bind.hpp>
#include <boost/range/iterator.hpp>
#include "../test_driver/range_return_test_driver.hpp"
#include <algorithm>
#include <functional>
#include <list>
#include <numeric>
#include <deque>
#include <vector>

namespace boost
{
    namespace
    {
        class max_element_test_policy
        {
        public:
            template< class Container >
            BOOST_DEDUCED_TYPENAME range_iterator<Container>::type
            test_iter(Container& cont)
            {
                return boost::max_element(cont);
            }

            template<range_return_value return_type>
            struct test_range
            {
                template<class Container, class Policy>
                BOOST_DEDUCED_TYPENAME range_return<Container,return_type>::type
                operator()(Policy&, Container& cont)
                {
                    return boost::max_element<return_type>(cont);
                }
            };

            template< class Container >
            BOOST_DEDUCED_TYPENAME range_iterator<Container>::type
            reference(Container& cont)
            {
                return std::max_element(cont.begin(), cont.end());
            }
        };

        template<class Pred>
        class max_element_pred_test_policy
        {
        public:
            template< class Container >
            BOOST_DEDUCED_TYPENAME range_iterator<Container>::type
            test_iter(Container& cont)
            {
                return boost::max_element(cont, Pred());
            }

            Pred pred() const { return Pred(); }

            template< range_return_value return_type >
            struct test_range
            {
                template< class Container, class Policy >
                BOOST_DEDUCED_TYPENAME range_return<Container,return_type>::type
                operator()(Policy& policy, Container& cont)
                {
                    return boost::max_element<return_type>(cont, policy.pred());
                }
            };

            template< class Container >
            BOOST_DEDUCED_TYPENAME range_iterator<Container>::type
            reference(Container& cont)
            {
                return std::max_element(cont.begin(), cont.end(), Pred());
            }
        };

        template<class Container, class TestPolicy>
        void test_max_element_impl(TestPolicy policy)
        {
            using namespace boost::assign;

            typedef BOOST_DEDUCED_TYPENAME Container::value_type value_t;
            typedef BOOST_DEDUCED_TYPENAME remove_const<Container>::type container_t;

            range_test::range_return_test_driver test_driver;

            container_t cont;

            test_driver(cont, policy);

            cont.clear();
            cont += 1;

            test_driver(cont, policy);

            cont.clear();
            cont += 1,2,2,2,3,4,5,6,7,8,9;

            test_driver(cont, policy);
        }

        template<class Container>
        void test_max_element_impl()
        {
            test_max_element_impl<Container>(max_element_test_policy());
            
            test_max_element_impl<Container>(
                max_element_pred_test_policy<std::less<int> >());

            test_max_element_impl<Container>(
                max_element_pred_test_policy<std::greater<int> >());
        }

        void test_max_element()
        {
            test_max_element_impl< const std::vector<int> >();
            test_max_element_impl< const std::deque<int> >();
            test_max_element_impl< const std::list<int> >();

            test_max_element_impl< std::vector<int> >();
            test_max_element_impl< std::deque<int> >();
            test_max_element_impl< std::list<int> >();
        }
    }
}

boost::unit_test::test_suite*
init_unit_test_suite(int argc, char* argv[])
{
    boost::unit_test::test_suite* test
        = BOOST_TEST_SUITE( "RangeTestSuite.algorithm.max_element" );

    test->add( BOOST_TEST_CASE( &boost::test_max_element ) );

    return test;
}
