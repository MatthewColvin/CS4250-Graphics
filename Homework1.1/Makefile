# To use this Makefile, you type:
#
#        make 
#                  
# Libraries are assumed to be in the default search paths
# as are any required include files

# last-modified: last-modified: Fri Oct 25 15:20:14 2013

# Assumes the following file exists in the proper place.
include ../code-egs/Makefile.defs

# First target in the makefile will be used if you simply type make
# with no arguments. 
# animate is the executable to be created.
animate: animate.cc square.o circle.o object.o
	$(CC) animate.cc  $(InitShader) square.o circle.o object.o $(OPTIONS) $(LDLIBS) -o animate

# One pattern for each other file to be compiled.  These will be
# object files (i.e. .o files) Notice that only the options arguments
# are passed, not the LDLIBS as there will be no linking done for the
# .o files.
square.o: square.h square.cc object.h
	$(CC) square.cc -c $(OPTIONS)

circle.o: circle.h circle.cc object.h
	$(CC) circle.cc -c $(OPTIONS)

object.o: object.h object.cc
	$(CC) object.cc -c $(OPTIONS)

# clean up after yourself
clean: 
	rm -f animate square.o circle.o object.o
