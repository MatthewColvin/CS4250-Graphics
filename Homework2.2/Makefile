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
include ../code-egs/Makefile.defs

# Next line says what options to use to compile programs
OPTIONS+=-Wall -std=c++14


srcpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.2/source/
headerpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.2/headers/
InitHeaders = -I$(headerpth)


OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc)) 

all: homework2

run: all
	./homework2

sphere.o: $(headerpth)sphere.h $(srcpth)sphere.cc common.o
	$(CC) $(srcpth)sphere.cc -c  $(InitHeaders) $(OPTIONS)
cube.o: $(headerpth)cube.h $(srcpth)cube.cc common.o
	$(CC) $(srcpth)cube.cc -c $(InitHeaders) $(OPTIONS)
cube_door.o: $(headerpth)cube_door.h $(srcpth)cube_door.cc common.o
	$(CC) $(srcpth)cube_door.cc -c $(InitHeaders) $(OPTIONS)
common.o: $(srcpth)common.cc $(headerpth)common.h
	$(CC) $(srcpth)common.cc -c $(InitHeaders) $(OPTIONS)

homework2: common.o $(headerpth)common.h cube.o $(headerpth)cube.h $(srcpth)scene.cc $(headerpth)sphere.h sphere.o $(headerpth)cube_door.h cube_door.o
	$(CC)  $(srcpth)scene.cc common.o cube.o cube_door.o sphere.o $(InitShader) $(InitHeaders) $(OPTIONS) $(LDLIBS) -o homework2
clean:
	rm -f $(OBJECTS) *.o *~
allclean: clean
	rm -f homework2 