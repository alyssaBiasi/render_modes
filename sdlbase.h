
#ifndef SDL_BASE_H
#define SDL_BASE_H

#ifdef _WIN32
#	include <windows.h>
#endif

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#	include "SDL/SDL_main.h"
#	include "SDL/SDL.h"
#else
#	include <GL/gl.h>
#	include <GL/glut.h>
#	include <GL/glu.h>
#	include <SDL/SDL.h>
#endif


/* to be implemented in SDL application */
void reshape(int w, int h);
void init();
void display();
void update(float dt);
void mouseDown(int button, int state, int x, int y);
void mouseMove(int x, int y);
void keyDown(int key);
void keyUp(int key);
void cleanup();

/* utility functions */
void quit();

#endif
