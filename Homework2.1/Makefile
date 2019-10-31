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


srcpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.1/source
headerpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.1/headers
InitHeaders = -I$(headerpth)


homework: $(srcpth)/cube2.cc $(headerpth)/config.h
	$(CC) $(srcpth)/cube2.cc  $(InitHeaders) $(InitShader)  $(OPTIONS) $(LDLIBS) -o homework

run: homework
	./homework

# clean up 
clean: 
	rm -f homework 
	rm *.o -r
