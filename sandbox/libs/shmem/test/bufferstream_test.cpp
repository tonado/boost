#include <boost/shmem/streams/bufferstream.hpp>
#include <sstream>
#include <string.h>

using namespace boost::shmem;

//Force instantiations to catch compile-time errors
template class basic_bufferbuf<char>;
template class basic_bufferbuf<wchar_t>;
template class basic_bufferstream<char>;
template class basic_bufferstream<wchar_t>;
template class basic_ibufferstream<char>;
template class basic_ibufferstream<wchar_t>;
template class basic_obufferstream<char>;
template class basic_obufferstream<wchar_t>;

static int bufferstream_test()
{
   //Static big enough buffer
   {
      const int BufSize = 10001;
      //This will be zero-initialized
      static char buffer [BufSize];
      bufferstream bufstream;;
      std::stringstream std_stringstream;
      std::string str1, str2, str3("testline:");
      int number1, number2;

      //Make sure we have null in the last byte
      bufstream.buffer(buffer, BufSize-1);
      for(int i = 0; i < 100; ++i){
         bufstream         << "testline: " << i << std::endl;
         std_stringstream  << "testline: " << i << std::endl;
      }

      if(strcmp(buffer, std_stringstream.str().c_str()) != 0){
         return -1;
      }

      //We shouldn't have reached the end of the buffer writing
      if(bufstream.bad()){
         assert(0);
         return -1;
      }

      bufstream.buffer(buffer, BufSize-1);
      for(int i = 0; i < 100; ++i){
         bufstream         >> str1 >> number1;
         std_stringstream  >> str2 >> number2;
         if((str1 != str2) || (str1 != str3)){
            assert(0); return -1;  
         }
         if((number1 != number2) || (number1 != i)){
            assert(0); return -1;   
         }
      }
      //We shouldn't have reached the end of the buffer reading
      if(bufstream.eof()){
         assert(0);
         return -1;
      }
   }

   //Static small buffer. Check if buffer
   //overflow protection works.
   {
      const int BufSize = 101;
      //This will be zero-initialized
      static char buffer [BufSize];
      bufferstream bufstream;
      std::stringstream std_stringstream;
      std::string str1;
      int number1;

      //Make sure we have null in the last byte
      bufstream.buffer(buffer, BufSize-1);
      for(int i = 0; i < 100; ++i){
         bufstream         << "testline: " << i << std::endl;
         std_stringstream  << "testline: " << i << std::endl;
      }
     
      //Contents should be different
      if(strcmp(buffer, std_stringstream.str().c_str()) == 0){
         return -1;
      }
      //The stream shouldn't be in good health
      if(bufstream.good()){
         assert(0);
         return -1;
      }
         //The bad flag should be active. This indicates overflow attempt
      if(!bufstream.bad()){
         assert(0);
         return -1;
      }

      //Now let's test read overflow
      bufstream.clear();
      bufstream.buffer(buffer, BufSize-1);
      for(int i = 0; i < 100; ++i){
         bufstream         >> str1 >> number1;
      }
      //The stream shouldn't be in good health
      if(bufstream.good()){
         assert(0);
         return -1;
      }
      //The eof flag indicates we have reached the end of the 
      //buffer while reading
      if(!bufstream.eof()){
         assert(0);
         return -1;
      }
   }
   return 0;
}

int main ()
{
   if(bufferstream_test()==-1){
      return -1;
   }
   return 0;
}


