# Usage:
#
#   Create a new empty directory anywhere (preferably not in the boost tree).
#   Copy this Makefile to that new directory and rename it to "Makefile"
#   Set the BOOST pathname below.
#
#   make softlinks     Create softlinks to source code and tests
#   make               Compile all sources
#   make test          Run doctest tests
#   make clean         Remove all object files
#   make unlink        Remove softlinks

BOOST= /net/cci/rwgk/boost

PYEXE= /usr/local/Python-1.5.2/bin/python
PYINC= -I/usr/local/Python-1.5.2/include/python1.5
#PYEXE= /usr/local/Python-2.0/bin/python
#PYINC= -I/usr/local/Python-2.0/include/python2.0
#STLPORTINC= -I/usr/local/STLport-4.1b3/stlport
#STLPORTOPTS= \
# -D__USE_STD_IOSTREAM \
# -D__STL_NO_SGI_IOSTREAMS \
# -D__STL_USE_NATIVE_STRING \
# -D__STL_NO_NEW_C_HEADERS \
# -D_RWSTD_COMPILE_INSTANTIATE=1
#STLPORTINC= -I/usr/local/STLport-4.1b4/stlport
#STLPORTOPTS= -D__NO_USE_STD_IOSTREAM -D__STL_NO_SGI_IOSTREAMS
STLPORTINC= -I/net/cci/xp/C++_C_headers

STDOPTS= -std strict_ansi
WARNOPTS= -msg_disable 186,450,1115
# use -msg_display_number to obtain integer tags for -msg_disable

CPP= cxx
CPPOPTS= $(STLPORTINC) $(STLPORTOPTS) -I$(BOOST) $(PYINC) \
         $(STDOPTS) $(WARNOPTS) -g
MAKEDEP= -Em

LD= cxx
LDOPTS= -shared -expect_unresolved 'Py*' -expect_unresolved '_Py*'

#HIDDEN= -hidden

BPL_SRC = $(BOOST)/libs/python/src
BPL_TST = $(BOOST)/libs/python/test
BPL_EXA = $(BOOST)/libs/python/example
SOFTLINKS = \
$(BPL_SRC)/classes.cpp \
$(BPL_SRC)/conversions.cpp \
$(BPL_SRC)/extension_class.cpp \
$(BPL_SRC)/functions.cpp \
$(BPL_SRC)/init_function.cpp \
$(BPL_SRC)/module_builder.cpp \
$(BPL_SRC)/objects.cpp \
$(BPL_SRC)/types.cpp \
$(BPL_TST)/comprehensive.cpp \
$(BPL_TST)/comprehensive.hpp \
$(BPL_TST)/comprehensive.py \
$(BPL_TST)/doctest.py \
$(BPL_EXA)/abstract.cpp \
$(BPL_EXA)/getting_started1.cpp \
$(BPL_EXA)/getting_started2.cpp \
$(BPL_EXA)/getting_started3.cpp \
$(BPL_EXA)/getting_started4.cpp \
$(BPL_EXA)/getting_started5.cpp \
$(BPL_EXA)/test_abstract.py \
$(BPL_EXA)/test_getting_started1.py \
$(BPL_EXA)/test_getting_started2.py \
$(BPL_EXA)/test_getting_started3.py \
$(BPL_EXA)/test_getting_started4.py \
$(BPL_EXA)/test_getting_started5.py

OBJ = classes.o conversions.o extension_class.o functions.o \
      init_function.o module_builder.o \
      objects.o types.o
DEPOBJ= $(OBJ) comprehensive.o abstract.o \
        getting_started1.o getting_started2.o getting_started3.o \
        getting_started4.o getting_started5.o

.SUFFIXES: .o .cpp

all: libboost_python.a boost_python_test.so abstract.so \
     getting_started1.so getting_started2.so getting_started3.so \
     getting_started4.so getting_started5.so

softlinks:
	@ for pn in $(SOFTLINKS); \
	  do \
            bn=`basename "$$pn"`; \
	    if [ ! -e "$$bn" ]; then \
              echo "ln -s $$pn ."; \
	      ln -s "$$pn" .; \
            else \
              echo "info: no softlink created (file exists): $$bn"; \
	    fi; \
	  done

unlink:
	@ for pn in $(SOFTLINKS); \
	  do \
            bn=`basename "$$pn"`; \
	    if [ -L "$$bn" ]; then \
              echo "rm $$bn"; \
              rm "$$bn"; \
            elif [ -e "$$bn" ]; then \
              echo "info: not a softlink: $$bn"; \
	    fi; \
	  done

libboost_python.a: $(OBJ)
	rm -f libboost_python.a
	cd cxx_repository; \
          ls -1 > ../libboost_python.a.input; \
          ar r ../libboost_python.a -input ../libboost_python.a.input
	rm -f libboost_python.a.input
	ar r libboost_python.a $(OBJ)

boost_python_test.so: $(OBJ) comprehensive.o
	$(LD) $(LDOPTS) $(OBJ) comprehensive.o -o boost_python_test.so -lm

abstract.so: $(OBJ) abstract.o
	$(LD) $(LDOPTS) $(OBJ) abstract.o -o abstract.so

getting_started1.so: $(OBJ) getting_started1.o
	$(LD) $(LDOPTS) $(OBJ) getting_started1.o -o getting_started1.so

getting_started2.so: $(OBJ) getting_started2.o
	$(LD) $(LDOPTS) $(OBJ) getting_started2.o -o getting_started2.so

getting_started3.so: $(OBJ) getting_started3.o
	$(LD) $(LDOPTS) $(OBJ) getting_started3.o -o getting_started3.so

getting_started4.so: $(OBJ) getting_started4.o
	$(LD) $(LDOPTS) $(OBJ) getting_started4.o -o getting_started4.so

getting_started5.so: $(OBJ) getting_started5.o
	$(LD) $(LDOPTS) $(OBJ) getting_started5.o -o getting_started5.so

.cpp.o:
	$(CPP) $(CPPOPTS) -c $*.cpp

test:
	$(PYEXE) comprehensive.py
	$(PYEXE) test_abstract.py
	$(PYEXE) test_getting_started1.py
	$(PYEXE) test_getting_started2.py
	$(PYEXE) test_getting_started3.py
	$(PYEXE) test_getting_started4.py
	$(PYEXE) test_getting_started5.py

clean:
	rm -f $(OBJ) libboost_python.a libboost_python.a.input
	rm -f comprehensive.o boost_python_test.so
	rm -f abstract.o abstract.so
	rm -f getting_started1.o getting_started1.so
	rm -f getting_started2.o getting_started2.so
	rm -f getting_started3.o getting_started3.so
	rm -f getting_started4.o getting_started4.so
	rm -f getting_started5.o getting_started5.so
	rm -f so_locations *.pyc
	rm -rf cxx_repository

depend:
	@ cat Makefile.nodepend; \
          for obj in $(DEPOBJ); \
          do \
            bn=`echo "$$obj" | cut -d. -f1`; \
            $(CPP) $(CPPOPTS) $(MAKEDEP) "$$bn".cpp; \
          done

