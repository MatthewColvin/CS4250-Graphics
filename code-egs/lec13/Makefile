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
OPTIONS+=-Wall

OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc))

all: cube0 cube1 cube2 honolulu shadow

common.o: common.cc common.h
	$(CC) common.cc -c $(OPTIONS)

cube0: common.o common.h cube0.cc
	$(CC)  cube0.cc common.o  $(InitShader) $(OPTIONS) $(LDLIBS) -o cube0
cube1: common.o common.h cube1.cc
	$(CC)  cube1.cc common.o  $(InitShader) $(OPTIONS) $(LDLIBS) -o cube1
cube2: common.o common.h cube2.cc
	$(CC)  cube2.cc common.o  $(InitShader) $(OPTIONS) $(LDLIBS) -o cube2

honolulu: honolulu.cc
	$(CC)  honolulu.cc  $(InitShader) $(OPTIONS) $(LDLIBS) -o honolulu
shadow: shadow.cc
	$(CC)  shadow.cc  $(InitShader) $(OPTIONS) $(LDLIBS) -o shadow

clean:
	rm -f $(OBJECTS) *~
