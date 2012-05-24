//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/intermodule_singleton.hpp>
#include <iostream>

using namespace boost::interprocess;

class MyClass
{
   public:
   MyClass()
   {
      std::cout << "Constructor\n";
   }

   void shout() const
   {
      std::cout << "Shout\n";
   }

   ~MyClass()
   {
      std::cout << "Destructor\n";
   }
};

class MyDerivedClass
   : public MyClass
{};

class MyThrowingClass
{
   public:
   MyThrowingClass()
   {
      throw int(0);
   }
};


template < template<class T, bool LazyInit = false, bool Phoenix = true> class IntermoduleType >
int intermodule_singleton_test()
{
   bool exception_thrown = false;
   bool exception_2_thrown = false;

   try{
      IntermoduleType<MyThrowingClass, true>::get();
   }
   catch(int &){
      exception_thrown = true;
      //Second try
      try{
         IntermoduleType<MyThrowingClass, true>::get();
      }
      catch(interprocess_exception &){
         exception_2_thrown = true;
      }
   }

   if(!exception_thrown || !exception_2_thrown){
      return 1;
   }

   MyClass & mc = IntermoduleType<MyClass>::get();
   mc.shout();
   IntermoduleType<MyClass>::get().shout();
   IntermoduleType<MyDerivedClass>::get().shout();

   //Second try
   exception_2_thrown = false;
   try{
      IntermoduleType<MyThrowingClass, true>::get();
   }
   catch(interprocess_exception &){
      exception_2_thrown = true;
   }
   if(!exception_2_thrown){
      return 1;
   }

   return 0;  
}

//A class simulating a logger
//We'll register constructor/destructor counts
//to test the singleton was correctly resurrected
//by LogUser singleton.
template<class Tag>
class Logger
{
   public:
   Logger()
   {
      ++constructed_times;
   }

   void log_it()
   {}

   ~Logger()
   {
      ++destroyed_times;
   }

   static unsigned int constructed_times;
   static unsigned int destroyed_times;
};

template<class Tag>
unsigned int Logger<Tag>::constructed_times;

template<class Tag>
unsigned int Logger<Tag>::destroyed_times;

//A class simulating a logger user.
//The destructor uses the logger so that
//the logger is resurrected if it was
//already destroyed
template<class LogSingleton>
class LogUser
{
   public:
   LogUser()
   {}

   void function_using_log()
   {  LogSingleton::get().log_it();  }

   ~LogUser()
   {  LogSingleton::get().log_it();  }
};

//A class that tests the correct
//phoenix singleton behaviour.
//Logger should be resurrected by LogUser
template<class Tag>
class LogPhoenixTester
{
   public:
   LogPhoenixTester()
   {}

   void dummy()
   {}

   ~LogPhoenixTester()
   {
      //Test Phoenix singleton was correctly executed:
      //created and destroyed two times
      //This test will be executed after main ends
      if(Logger<Tag>::constructed_times != Logger<Tag>::destroyed_times ||
         Logger<Tag>::constructed_times != 2)
      {
         std::string s("LogPhoenixTester failed for tag ");
         s += typeid(Tag).name();
         throw std::runtime_error(s.c_str());
      }
   }
};

//A class simulating a logger user.
//The destructor uses the logger so that
//the logger is resurrected if it was
//already destroyed
template<class LogSingleton>
class LogDeadReferenceUser
{
   public:
   LogDeadReferenceUser()
   {}

   void function_using_log()
   {  LogSingleton::get().log_it();  }

   ~LogDeadReferenceUser()
   {
      //Make sure the exception is thrown as we are
      //try to use a dead non-phoenix singleton
      try{
         LogSingleton::get().log_it();
         std::string s("LogDeadReferenceUser failed for LogSingleton ");
         s += typeid(LogSingleton).name();
         throw std::runtime_error(s.c_str());
      }
      catch(interprocess_exception &){
         //Correct behaviour
      }
   }
};

template < template<class T, bool LazyInit = false, bool Phoenix = true> class IntermoduleType >
int phoenix_singleton_test()
{
   typedef int DummyType;
   typedef IntermoduleType<DummyType, true, true>              Tag;
   typedef Logger<Tag>                                         LoggerType;
   typedef IntermoduleType<LoggerType, true, true>             LoggerSingleton;
   typedef LogUser<LoggerSingleton>                            LogUserType;
   typedef IntermoduleType<LogUserType, true, true>            LogUserSingleton;
   typedef IntermoduleType<LogPhoenixTester<Tag>, true, true>  LogPhoenixTesterSingleton;

   //Instantiate Phoenix tester singleton so that it will be destroyed the last
   LogPhoenixTesterSingleton::get().dummy();

   //Now instantitate a log user singleton
   LogUserType &log_user = LogUserSingleton::get();

   //Then force LoggerSingleton instantiation
   //calling a function that will use it.
   //After main ends, LoggerSingleton will be destroyed
   //before LogUserSingleton due to LIFO
   //singleton semantics
   log_user.function_using_log();

   //Next, LogUserSingleton destructor will resurrect
   //LoggerSingleton.
   //After that LoggerSingleton will be destroyed and
   //lastly LogPhoenixTester will be destroyed checking
   //LoggerSingleton was correctly destroyed.  
   return 0;
}

template < template<class T, bool LazyInit = false, bool Phoenix = true> class IntermoduleType >
int dead_reference_singleton_test()
{
   typedef int DummyType;
   typedef IntermoduleType<DummyType, true, false>                Tag;
   typedef Logger<Tag>                                            LoggerType;
   typedef IntermoduleType<LoggerType, true, false>               LoggerSingleton;
   typedef LogDeadReferenceUser<LoggerSingleton>                  LogDeadReferenceUserType;
   typedef IntermoduleType<LogDeadReferenceUserType, true, false> LogDeadReferenceUserSingleton;

   //Now instantitate a log user singleton
   LogDeadReferenceUserType &log_user = LogDeadReferenceUserSingleton::get();

   //Then force LoggerSingleton instantiation
   //calling a function that will use it.
   //After main ends, LoggerSingleton will be destroyed
   //before LogDeadReferenceUserType due to LIFO
   //singleton semantics
   log_user.function_using_log();

   //Next, LogDeadReferenceUserType destructor will try to use
   //LoggerSingleton and an exception will be raised an catched.
   return 0;
}

int main ()
{
   if(0 != intermodule_singleton_test<ipcdetail::portable_intermodule_singleton>()){
      return 1;
   }

   #ifdef BOOST_INTERPROCESS_WINDOWS
   if(0 != intermodule_singleton_test<ipcdetail::windows_intermodule_singleton>()){
      return 1;
   }
   #endif

   //Phoenix singletons are tested after main ends,
   //LogPhoenixTester does the work
   phoenix_singleton_test<ipcdetail::portable_intermodule_singleton>();
   #ifdef BOOST_INTERPROCESS_WINDOWS
   phoenix_singleton_test<ipcdetail::windows_intermodule_singleton>();
   #endif

   //Dead reference singletons are tested after main ends,
   //LogDeadReferenceUser does the work
   dead_reference_singleton_test<ipcdetail::portable_intermodule_singleton>();
   #ifdef BOOST_INTERPROCESS_WINDOWS
   dead_reference_singleton_test<ipcdetail::windows_intermodule_singleton>();
   #endif

   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>

