# Simple makefile to make it compile on macs.
#
# Written by David M. Chelberg
# Updated to try to fix issue with Mac OS 10.12
# last-modified: Sun Sep 24 14:03:10 2017
#
#GCC_OPTIONS=-Wall -pedantic -I ../include -I/Users/chelberg/class/cs425/code/ -I/opt/X11/include
GCC_OPTIONS=-Wall -pedantic

GL_OPTIONS=-framework OpenGL -framework GLUT -framework Foundation -framework ApplicationServices
#LDLIBS = -L/opt/X11/lib

OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS) 

InitShader.o: InitShader.cpp
	g++   $(GCC_OPTIONS) -c -o InitShader.o InitShader.cpp 
