#include <string>
#include <iostream>
#include <boost/string_algo/predicate.hpp>

using namespace std;
using namespace boost;

void predicate_example()
{
    cout << "* Predicate Example *" << endl << endl;

    string str1("123xxx321");
    string str2("abc");

    cout << "str1 starts with \"123\": " << 
        (starts_with( str1, string("123") )?"true":"false") << endl; 
    cout << "str1 ends with \"123\": " << 
        (ends_with( str1, string("123") )?"true":"false") << endl; 

    cout << "str1 contains \"xxx\": " << 
        (contains( str1, string("xxx") )?"true":"false") << endl; 

    // Note: equals have to be explicitly qualified, 
    // otherwise it conflicts with std::equals
    cout << "str2 equals \"abc\": " << 
        (boost::equals( str2, string("abc") )?"true":"false") << endl; 

    cout << endl;
}   
