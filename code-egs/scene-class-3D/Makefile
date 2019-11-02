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
include ../Makefile.defs

# Next line says what options to use to compile programs
OPTIONS+=-Wall -std=c++14

OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc))

all: scene

run: all
	./scene

sphere.o: sphere.h sphere.cc common.o
	$(CC) sphere.cc -c $(OPTIONS)
cube.o: cube.h cube.cc common.o
	$(CC) cube.cc -c $(OPTIONS)
cube_door.o: cube_door.h cube_door.cc common.o
	$(CC) cube_door.cc -c $(OPTIONS)
common.o: common.cc common.h
	$(CC) common.cc -c $(OPTIONS)

scene: common.o common.h cube.o cube.h scene.cc sphere.h sphere.o cube_door.h cube_door.o
	$(CC)  scene.cc common.o cube.o cube_door.o sphere.o $(InitShader) $(OPTIONS) $(LDLIBS) -o scene
clean:
	rm -f $(OBJECTS) *.o *~
