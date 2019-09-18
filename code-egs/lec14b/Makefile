# This sample Makefile allows you to make an OpenGL application
#   whose source is exactly one .c file or one .cc file under Solaris.
#   check the location of the X libraries on your implementation.
#
# To use this Makefile, you type:
#
#        make xxxx
#                  
# where
#       xxxx.cc is the name of the c++-file you wish to compile 
#  or 
#       xxxx.c is the name of the c-file you wish to compile 
#       
# A binary named xxxx will be produced
# Libraries are assumed to be in the default search paths
# as are any required include files

#
# Modified by Prof. Chelberg to also allow you to make from a single
# c++ source file.  So if xxxx.cc is the sourcefile, you type make
# xxxx to produce the binary file.

# last-modified: Fri Oct 16 12:58:38 2015

# Assumes the following file exists in the proper place.
include /home/cs425/code-egs/Makefile.defs

# Next line says what options to use to compile programs
OPTIONS+=-Wall

OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc))

all: $(OBJECTS)

# patterns to allow compilation of many c++ or c programs
.c:
	$(CC)  $@.c   $(InitShader) $(OPTIONS) $(LDLIBS) -o $@

.cc:
	$(CC)  $@.cc  $(InitShader) $(OPTIONS) $(LDLIBS) -o $@

.cpp:
	$(CC)  $@.cpp  $(InitShader) $(OPTIONS) $(LDLIBS) -o $@

clean:
	rm -f $(OBJECTS) *~
