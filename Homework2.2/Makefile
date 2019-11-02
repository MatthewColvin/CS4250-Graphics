# This sample Makefile allows you to make an OpenGL application
#
# To use this Makefile, you type:
#
#        make          -to compile all of the code for Homework 2
#        make run      -to compile and then run the program         
#      	 make release  -to compile just the binary for running the program
#		 make debug    -to keep all of the object files
#
# Assumes the following file exists in the proper place.
include ../code-egs/Makefile.defs

# Next line says what options to use to compile programs
OPTIONS+=-Wall -std=c++14

srcpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.2/source/
headerpth = /home/mcolvin/Documents/CS4250-Grapics/Homework2.2/headers/
InitHeaders = -I$(headerpth)

OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc)) 

all: homework2

release: all clean

run: release
	./homework2

camera.o: $(srcpth)camera.cc $(headerpth)camera.h
	$(CC) $(srcpth)camera.cc -c $(InitHeaders) $(OPTIONS)
common.o: $(srcpth)common.cc $(headerpth)common.h
	$(CC) $(srcpth)common.cc -c $(InitHeaders) $(OPTIONS)
cube.o: $(headerpth)cube.h $(srcpth)cube.cc common.o
	$(CC) $(srcpth)cube.cc -c $(InitHeaders) $(OPTIONS)
cube_door.o: $(headerpth)cube_door.h $(srcpth)cube_door.cc common.o
	$(CC) $(srcpth)cube_door.cc -c $(InitHeaders) $(OPTIONS)
scene.o :$(srcpth)scene.cc $(headerpth)scene.h
	$(CC) $(srcpth)scene.cc -c $(InitHeaders) $(OPTIONS)
sphere.o: $(headerpth)sphere.h $(srcpth)sphere.cc common.o
	$(CC) $(srcpth)sphere.cc -c  $(InitHeaders) $(OPTIONS)






homework2: common.o    $(headerpth)common.h    \
		   cube.o      $(headerpth)cube.h      \
		   scene.o     $(headerpth)scene.h     \
		   sphere.o    $(headerpth)sphere.h    \
		   cube_door.o $(headerpth)cube_door.h \
		   camera.o    $(headerpth)camera.h
	$(CC)  Homework2.cc common.o scene.o cube.o cube_door.o camera.o sphere.o $(InitShader) $(InitHeaders) $(OPTIONS) $(LDLIBS) -o homework2
clean:
	rm -f $(OBJECTS) *.o *~
squeakyclean: clean
	rm -f homework2 