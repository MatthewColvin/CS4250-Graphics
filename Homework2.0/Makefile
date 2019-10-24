# To use this Makefile, you type:
#
#        make 
#                  
# Libraries are assumed to be in the default search paths
# as are any required include files

# last-modified: last-modified: Fri Oct 25 15:20:14 2013

# Assumes the following file exists in the proper place.
#include /home/cs425/code-egs/Makefile.defs
include /home/mcolvin/Documents/CS4250-Grapics/code-egs/Makefile.defs
# First target in the makefile will be used if you simply type make
# with no arguments. 
# animate is the executable to be created.

InitHeaders = "-I/home/mcolvin/Documents/CS4250-Grapics/Homework1/headers"


homework: ./source/Homework1.cc character.o  vertex.o
	$(CC) ./source/Homework1.cc  $(InitHeaders) $(InitShader) character.o  vertex.o $(OPTIONS) $(LDLIBS) -o homework

animate: animate.cc square.o circle.o object.o
	$(CC) animate.cc  $(InitHeaders) $(InitShader) square.o circle.o object.o $(OPTIONS) $(LDLIBS) -o animate

# One pattern for each other file to be compiled.  These will be
# object files (i.e. .o files) Notice that only the options arguments
# are passed, not the LDLIBS as there will be no linking done for the
# .o files.

vertex.o: ./headers/vertex.h ./source/vertex.cc 
	$(CC) $(InitHeaders) ./source/vertex.cc -c $(OPTIONS)
#texture.o: ./headers/texture.h ./source/texture.cc 
#	$(CC) ./source/texture.cc -c $(OPTIONS)
character.o: ./headers/character.h ./source/character.cc 
	$(CC)  $(InitHeaders) ./source/character.cc -c $(OPTIONS)


square.o: square.h square.cc object.h
	$(CC) $(InitHeaders) square.cc -c $(OPTIONS)

circle.o: circle.h circle.cc object.h
	$(CC) $(InitHeaders) circle.cc -c $(OPTIONS)

object.o: object.h object.cc
	$(CC)  $(InitHeaders) object.cc -c $(OPTIONS)

# clean up after yourself
clean: 
	rm -f animate homework square.o circle.o object.o
	rm *.o -r
