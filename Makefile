################################################################################################
#
# Alyssa Biasi
#
################################################################################################
UNAME := $(shell uname)
DEBUG = -g
#OPTIMISE = -O3

CFLAGS = `sdl-config --cflags` $(DEBUG) $(OPTIMISE)
LDFLAGS = `sdl-config --libs` -lGL -lGLU -lglut -lm
MACSDL = 
EXE = render
OBJECTS = assignment1.o sdlbase.o sphere.o torus.o grid.o utils.o
 
# OS X
ifeq "$(UNAME)" "Darwin"
	CFLAGS =  
	LDFLAGS = -framework Carbon -framework GLUT \
				-framework SDL -framework OpenGL \
				-framework cocoa -lz 
	MACSDL = SDLMain.m
endif

#default target
all: $(EXE)

#executable
$(EXE) : $(OBJECTS)
	gcc -o $@ $(LDFLAGS) $(MACSDL) $(OBJECTS)

#general object file rule
%.o : %.c
	gcc -c -o $@ $(CFLAGS) $<

#additional dependencies
sdlbase.o : sdlbase.h
tute-vbo.o : sdlbase.h
utils.o : utils.h

#clean (-f stops error if files don't exist)
clean:
	rm -f $(EXE) \
	      $(OBJECTS)
