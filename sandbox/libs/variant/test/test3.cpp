
/***************************************************************************
                          test3.cpp  -  description
                             -------------------
    begin                : Fri Jun 28 08:46:58 IDT 2002
    copyright            : (C) 2002 by Itay Maman
    email                : itay_maman@yahoo.com
 ***************************************************************************/

//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
//



/////////////////////////////////////////////////////////////////////

//
// Boost.Test
//
#include <boost/test/minimal.hpp>



//
// Test program code starts here...
//
#include <boost/variant.hpp>
#include <boost/incomplete.hpp>
#include <iostream>

#include <sstream>
#include <string>


/////////////////////////////////////////////////////////////////////


using boost::variant;
using boost::incomplete;
using std::cout;
using std::endl;


/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////


struct Add;
struct Sub;

typedef variant<int, incomplete<Add>, incomplete<Sub> > Expr;



struct Sub
{
   Sub();
   Sub(const Expr& l, const Expr& r);
   Sub(const Sub& other);

   Expr lhs_;
   Expr rhs_;
};

struct Add
{
   Add() { }
   Add(const Expr& l, const Expr& r) : lhs_(l), rhs_(r) { }
   Add(const Add& other) : lhs_(other.lhs_), rhs_(other.rhs_) { }

   Expr lhs_;
   Expr rhs_;
};

Sub::Sub() { }
Sub::Sub(const Expr& l, const Expr& r) : lhs_(l), rhs_(r) { }
Sub::Sub(const Sub& other) : lhs_(other.lhs_), rhs_(other.rhs_) { }



//
// insert-to operators
//
std::ostream& operator<<(std::ostream& out, const Expr& a);

std::ostream& operator<<(std::ostream& out, const Add& a)
{
   out << '(' << a.lhs_ << '+' << a.rhs_ << ')';
   return out;
}

std::ostream& operator<<(std::ostream& out, const Sub& a)
{
   out << '(' << a.lhs_ << '-' << a.rhs_ << ')';
   return out;
}


struct raw_text_maker : boost::static_visitor<std::string>
{
   template<typename T>
   std::string operator()(const T& t) const
   {
      std::ostringstream ost;
      ost << t;

      return ost.str();
   }   
};

std::ostream& operator<<(std::ostream& out, const Expr& a)
{
   std::string temp = boost::apply_visitor(raw_text_maker(), a);
   out << temp;

   return out;
}

//
// Expression evaluation visitor
//
struct Calculator : boost::static_visitor<int>
{
   Calculator()  { }

   int operator()(Add& x) const
   {
      Calculator calc;
      int n1 = boost::apply_visitor(calc, x.lhs_);
      int n2 = boost::apply_visitor(calc, x.rhs_);
      
      return n1 + n2;
   }

   int operator()(Sub& x) const
   {
      return boost::apply_visitor(Calculator(), x.lhs_) 
         - boost::apply_visitor(Calculator(), x.rhs_);
   }

   int operator()(Expr& x) const
   {
      Calculator calc;
      return boost::apply_visitor(calc, x);
   }

   int operator()(int x) const
   {
      return x;
   }

}; // Calculator


/////////////////////////////////////////////////////////////////////


int test_main(int, char* [])
{

   int n = 13;
   Expr e1( Add(n, Sub(Add(40,2),Add(10,4))) ); //n + (40+2)-(10+14) = n+28

   BOOST_CHECK(e1.type() == typeid(Add));
   BOOST_CHECK(boost::apply_visitor(raw_text_maker(), e1) == 
      "(13+((40+2)-(10+4)))");

   //Evaluate expression
   int res = boost::apply_visitor(Calculator(), e1);
   BOOST_CHECK(res == n + 28);

   return 0;
}

