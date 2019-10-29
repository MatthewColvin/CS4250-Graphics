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


srcpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.0/source
headerpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.0/headers
InitHeaders = -I$(headerpth)



homework: $(srcpth)/Homework1.cc room.o
	$(CC) $(srcpth)/Homework1.cc  $(InitHeaders) $(InitShader) room.o $(OPTIONS) $(LDLIBS) -o homework

run: homework
	./homework


room.o: $(headerpth)/room.h $(srcpth)/room.cc
	$(CC) $(srcpth)/room.cc  $(InitHeaders) -c $(OPTIONS)


#texture.o: ./headers/texture.h ./source/texture.cc 
#	$(CC) ./source/texture.cc -c $(OPTIONS)

# clean up after yourself
clean: 
	rm -f homework 
	rm *.o -r
