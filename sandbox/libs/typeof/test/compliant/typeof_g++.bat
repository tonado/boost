time /T

g++ -IC:\boost\boost_1_32_0 -I..\..\..\.. -D BOOST_TYPEOF_COMPLIANT -D BOOST_TYPEOF_LIMIT_SIZE=50 -D BOOST_MPL_LIMIT_VECTOR_SIZE=50 ..\main.cpp test_compliant.cpp odr1.cpp odr2.cpp 

time /T

g++ -IC:\boost\boost_1_32_0 -I..\..\..\.. ..\main.cpp test_compliant.cpp odr1.cpp odr2.cpp

time /T
