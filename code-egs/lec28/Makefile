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

# last-modified: Mon Sep 17 15:17:00 2012

# Assumes the following file exists in the proper place.
include /home/cs425/code-egs/Makefile.defs

# patterns to allow compilation of many c++ or c programs
.c:
	$(CC)  $@.c   $(InitShader) $(OPTIONS) $(LDLIBS) -o $@

.cc:
	$(CC)  $@.cc  $(InitShader) $(OPTIONS) $(LDLIBS) -o $@

all: texture0 texture1 texture2 texture3 texture4 texture4a texture5

# bitmap.o should be recompiled whenever bitmap.cc or bitmap.h are
# modified.
bitmap.o: bitmap.cc bitmap.h
	$(CC) $(INCS) bitmap.cc -c

# Explicit make rule for texture3 to be sure dependecies are
# explicitly stated.
texture3: texture3.cc bitmap.o 
	$(CC)  texture3.cc bitmap.o $(InitShader) $(OPTIONS) $(LDLIBS) -o texture3

texture4: texture4.cc bitmap.o 
	$(CC)  texture4.cc bitmap.o $(InitShader) $(OPTIONS) $(LDLIBS) -o texture4

texture4a: texture4a.cc bitmap.o 
	$(CC)  texture4a.cc bitmap.o $(InitShader) $(OPTIONS) $(LDLIBS) -o texture4a

texture5: texture5.cc bitmap.o 
	$(CC)  texture5.cc bitmap.o $(InitShader) $(OPTIONS) $(LDLIBS) -o texture5

sphere3: sphere3.cc bitmap.o
	$(CC)  sphere3.cc bitmap.o $(InitShader) $(OPTIONS) $(LDLIBS) -o sphere3

clean: 
	rm -f sphere3 texture0 texture1 texture2 texture3 texture4 texture4a texture5 bitmap.o
