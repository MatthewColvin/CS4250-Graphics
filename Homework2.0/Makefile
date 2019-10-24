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


homework: ./source/Homework1.cc 
	$(CC) ./source/Homework1.cc  $(InitHeaders) $(InitShader) $(OPTIONS) $(LDLIBS) -o homework


#texture.o: ./headers/texture.h ./source/texture.cc 
#	$(CC) ./source/texture.cc -c $(OPTIONS)

# clean up after yourself
clean: 
	rm -f animate homework square.o circle.o object.o
	rm *.o -r
