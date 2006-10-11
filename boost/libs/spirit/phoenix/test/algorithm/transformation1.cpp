/*=============================================================================
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2005-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/phoenix/stl/algorithm/transformation.hpp>
#include <boost/spirit/phoenix/core.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <functional>
#include <list>

namespace
{
    struct even
    {
        bool operator()(const int i) const
        {
            return i % 2 == 0;
        }
    };

    struct mod_2_comparison
    {
        bool operator()(
            const int lhs,
            const int rhs)
        {
            return lhs % 2 == rhs % 2;
        };
    };

    void copy_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        int output[4];
        BOOST_TEST(
            copy(arg1, arg2)(array, output) == output + 3);
        BOOST_TEST(output[0] == 1);
        BOOST_TEST(output[1] == 2);
        BOOST_TEST(output[2] == 3);
        return;
    }

    void copy_backward_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        int output[4];
        int* output_end = output + 3;
        BOOST_TEST(
            copy_backward(arg1, arg2)(array, output_end) == output);
        BOOST_TEST(output[0] == 1);
        BOOST_TEST(output[1] == 2);
        BOOST_TEST(output[2] == 3);
        return;
    }

    struct increment
    {
        int operator()(
            int i) const
        {
            return i+1;
        }
    };

    void transform_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        BOOST_TEST(
            transform(arg1, arg2, increment())(array, array) == 
            array + 3);
        BOOST_TEST(array[0] == 2);
        BOOST_TEST(array[1] == 3);
        BOOST_TEST(array[2] == 4);

        int array2[] = {1,2,3};
        BOOST_TEST(
            boost::phoenix::transform(arg1, arg2, arg3, std::plus<int>())(array, array2, array) == 
            array +3);
        BOOST_TEST(array[0] == 2 + 1);
        BOOST_TEST(array[1] == 3 + 2);
        BOOST_TEST(array[2] == 4 + 3);
        return;
    }

    void replace_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        replace(arg1,2,4)(array);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 4);
        BOOST_TEST(array[2] == 3);
        return;
    }

    void replace_if_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        replace_if(arg1, even(), 4)(array);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 4);
        BOOST_TEST(array[2] == 3);
        return;
    }

    void replace_copy_test()
    {
        using namespace boost::phoenix;
        int input[] = {1,2,3};
        int output[3];
        replace_copy(arg1, arg2, 2, 4)(input, output);
        BOOST_TEST(output[0] == 1);
        BOOST_TEST(output[1] == 4);
        BOOST_TEST(output[2] == 3);
        return;
    }

    void replace_copy_if_test()
    {
        using namespace boost::phoenix;
        int input[] = {1,2,3};
        int output[3];
        replace_copy_if(arg1, arg2, even(), 4)(input, output);
        BOOST_TEST(output[0] == 1);
        BOOST_TEST(output[1] == 4);
        BOOST_TEST(output[2] == 3);
        return;
    }

    void fill_test()
    {
        using namespace boost::phoenix;
        int array[] = {0,0,0};
        fill(arg1, 1)(array);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 1);
        BOOST_TEST(array[2] == 1);
        return;
    }

    void fill_n_test()
    {
        using namespace boost::phoenix;
        int array[] = {0,0,0};
        fill_n(arg1, 2, 1)(array);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 1);
        BOOST_TEST(array[2] == 0);
        return;
    }

    class int_seq
    {
    public:
        int_seq() : val_(0) { }

        int operator()()
        {
            return val_++;
        }
    private:
        int val_;
    };

    void generate_test()
    {
        using namespace boost::phoenix;
        int array[3];
        generate(arg1, int_seq())(array);
        BOOST_TEST(array[0] == 0);
        BOOST_TEST(array[1] == 1);
        BOOST_TEST(array[2] == 2);
        return;
    }

    void generate_n_test()
    {
        using namespace boost::phoenix;
        int array[] = {0,0,1};
        generate_n(arg1, 2, int_seq())(array);
        BOOST_TEST(array[0] == 0);
        BOOST_TEST(array[1] == 1);
        BOOST_TEST(array[2] == 1);
        return;
    }


    void remove_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        std::list<int> test_list(array, array + 3);
        BOOST_TEST(boost::phoenix::remove(arg1, 2)(array) == array + 2);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 3);
        BOOST_TEST(boost::phoenix::remove(arg1, 2)(test_list) == test_list.end());
        std::list<int>::const_iterator it(test_list.begin());
        BOOST_TEST(*it++ == 1);
        BOOST_TEST(*it++ == 3);
        return;
    }

    void remove_if_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        std::list<int> test_list(array, array + 3);
        BOOST_TEST(boost::phoenix::remove_if(arg1, even())(array) == array + 2);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 3);
        BOOST_TEST(boost::phoenix::remove_if(arg1, even())(test_list) == test_list.end());
        std::list<int>::const_iterator it(test_list.begin());
        BOOST_TEST(*it++ == 1);
        BOOST_TEST(*it++ == 3);
        return;
    }

    void remove_copy_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        int array2[2];
        BOOST_TEST(boost::phoenix::remove_copy(arg1, arg2, 2)(array, array2) == array2 + 2);
        BOOST_TEST(array2[0] == 1);
        BOOST_TEST(array2[1] == 3);
        return;
    }

    void remove_copy_if_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        int array2[2];
        BOOST_TEST(boost::phoenix::remove_copy_if(arg1, arg2, even())(array, array2) == array2 + 2);
        BOOST_TEST(array2[0] == 1);
        BOOST_TEST(array2[1] == 3);
        return;
    }

    void unique_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,2,3};
        std::list<int> test_list(array, array + 4);
        BOOST_TEST(unique(arg1)(array) == array + 3);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 2);
        BOOST_TEST(array[2] == 3);

        BOOST_TEST(unique(arg1)(test_list) == test_list.end());
        std::list<int>::const_iterator it(test_list.begin());
        BOOST_TEST(*it++ == 1);
        BOOST_TEST(*it++ == 2);
        BOOST_TEST(*it++ == 3);

        int array2[] = {1,3,2};
        std::list<int> test_list2(array2, array2 + 3);
        BOOST_TEST(unique(arg1, mod_2_comparison())(array2) == array2 + 2);
        BOOST_TEST(array2[0] == 1);
        BOOST_TEST(array2[1] == 2);
        
        BOOST_TEST(unique(arg1, mod_2_comparison())(test_list2) == test_list2.end());
        std::list<int>::const_iterator jt(test_list2.begin());
        BOOST_TEST(*jt++ == 1);
        BOOST_TEST(*jt++ == 2);
        
        return;
    }

    void unique_copy_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,2,3};
        int out[3];
        BOOST_TEST(unique_copy(arg1, arg2)(array, out) == out + 3);
        BOOST_TEST(out[0] == 1);
        BOOST_TEST(out[1] == 2);
        BOOST_TEST(out[2] == 3);

        int array2[] = {1,3,2};
        int out2[2];
        BOOST_TEST(unique_copy(arg1, arg2, mod_2_comparison())(array2, out2) == out2 + 2);
        BOOST_TEST(out2[0] == 1);
        BOOST_TEST(out2[1] == 2);
        
        return;
    }

    void reverse_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        std::list<int> test_list(array, array + 3);
        reverse(arg1)(array);
        BOOST_TEST(array[0] == 3);
        BOOST_TEST(array[1] == 2);
        BOOST_TEST(array[2] == 1);

        reverse(arg1)(test_list);
        std::list<int>::iterator it(test_list.begin());
        BOOST_TEST(*it++ == 3);
        BOOST_TEST(*it++ == 2);
        BOOST_TEST(*it++ == 1);
        return;
    }

    void reverse_copy_test()
    {
        using namespace boost::phoenix;
        int array[] = {1,2,3};
        int array2[3];
        reverse_copy(arg1, arg2)(array, array2);
        BOOST_TEST(array[0] == 1);
        BOOST_TEST(array[1] == 2);
        BOOST_TEST(array[2] == 3);

        BOOST_TEST(array2[0] == 3);
        BOOST_TEST(array2[1] == 2);
        BOOST_TEST(array2[2] == 1);

        return;
    }
}

int main()
{
    copy_test();
    copy_backward_test();
    transform_test();
    replace_test();
    replace_if_test();
    replace_copy_test();
    replace_copy_if_test();
    fill_test();
    fill_n_test();
    generate_test();
    generate_n_test();
    remove_test();
    remove_if_test();
    remove_copy_test();
    remove_copy_if_test();
    unique_test();
    unique_copy_test();
    reverse_test();
    reverse_copy_test();
    boost::report_errors();
}
