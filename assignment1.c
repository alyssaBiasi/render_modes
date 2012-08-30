/* tute-vbo.c: Sat Jul 21 23:04:33 EST 2012 pknowles */

#include "utils.h"

//Miscelaneous globals
int tesselation = 4;
int numObj = 1;
int numLights = 0;
float sphere_r = 1.0f;
float torus_R = 1.0f;
float torus_r = 0.5f;

enum ObjectType objDraw = torus;
enum RenderMode mode = immediate;
char *modeStr[] = {" ", "Immediate", "Vertex Array", "Vertex Buffer"};

/* Scene globals */
Camera camera;
float currentFramerate, frameTime;
int windowWidth;
int windowHeight;
int lastMouseX = 0;
int lastMouseY = 0;
bool renderOptions[NUM_RENDER_OPTIONS] = { false, true, true, false, false };

/*****************************************************************************************
  Util Functions
  	- init, update, reshape, render options
*****************************************************************************************/
/* Update opengl state to match flags in renderOptions */
void setRenderOptions()
{
	if (renderOptions[RENDER_LIGHTING])
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
	if (renderOptions[RENDER_CULL_BACKFACE])
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	
	glShadeModel(renderOptions[RENDER_FLAT] ? GL_FLAT : GL_SMOOTH);

	glPolygonMode(GL_FRONT_AND_BACK, 
		renderOptions[RENDER_WIREFRAME] ? GL_LINE : GL_FILL);
}

/* Called once at program start */
void init()
{
	int argc = 0;  /* fake glutInit args */
	char *argv = "";
	glutInit(&argc, &argv);
	
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_NORMALIZE);	
	
	memset(&renderOptions, 0, sizeof(renderOptions));
	renderOptions[RENDER_LIGHTING] = true;

	memset(&camera, 0, sizeof(Camera));
	camera.sensitivity = 0.3f;
	camera.zoom = 3.0f;
    
    /* Initialising shape objects */
    sphereInit(sphere_r);
    torusInit(torus_R, torus_r);
    gridInit();

	reallocArrays(tesselation);
    
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

/* Called once at start and again on window resize */
void reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	
	/* Portion of viewport to render to */
	glViewport(0, 0, width, height);
	
	/* Calc aspect ratio */
	float aspect = width / (float)height;
	
	/* Begin editing projection matrix */
	glMatrixMode(GL_PROJECTION);
	
	/* Clear previous projection */
	glLoadIdentity();
	
	/* Generate perspective projection matrix */
	gluPerspective(75.0f, aspect, 0.01f, 100.0f);
	
	/* Restore modelview as current matrix */
	glMatrixMode(GL_MODELVIEW);
}

/* Called continuously. dt is time between frames in milliseconds */
void update(float dt)
{
	static float fpsTime = 0.0f;
	static int fpsFrames = 0;
	fpsTime += dt;
	fpsFrames += 1;
	if (fpsTime > 1.0f)
	{
		currentFramerate = fpsFrames / fpsTime;
		frameTime = 1000.00/currentFramerate;
		fpsTime = 0.0f;
		fpsFrames = 0;
	}
}

/*****************************************************************************************
  Scene drawing Functions
  	- Screen text, axes, display
*****************************************************************************************/
void drawOSD()
{
    char buffer[32];

	/* Backup previous "enable" state */
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/* Create a temporary orthographic projection, matching
	 * window dimensions, and push it onto the stack */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	
	/* draw framerate */
	glPushMatrix();
	    glLoadIdentity(); /* clear current modelview (ie. from display) */
	    
	    /* Writing fps */
	    snprintf(buffer, sizeof(buffer), "FR(fps): %d", (int)currentFramerate);
	    writeScreenText(buffer, newVec3f(10,10,0));
		
		/* Writing ft */
		snprintf(buffer, sizeof(buffer), "FT(ms): %3.3f", frameTime);
		writeScreenText(buffer, newVec3f(10,30,0));
		
		/* Writing tessellation */
		snprintf(buffer, sizeof(buffer), "Tess.: %d", (int)tesselation);
		writeScreenText(buffer, newVec3f(10,50,0));

		/* Writing rendering mode */
		snprintf(buffer, sizeof(buffer), "%11s", modeStr[mode]);
		writeScreenText(buffer, newVec3f(windowWidth - 125,10,0));
		
		/* Writing number of lights */
		snprintf(buffer, sizeof(buffer), "Num. lights: %d", numLights + 1);
		writeScreenText(buffer, newVec3f(windowWidth - 150,30,0));
		
		/* Writing number of objects */
		snprintf(buffer, sizeof(buffer), "Num. obj: %d x %d", numObj, numObj);
		writeScreenText(buffer, newVec3f(windowWidth - 150,50,0));
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();  /* pop projection */
	glMatrixMode(GL_MODELVIEW);

	/* Restore "enable" state */
	glPopAttrib();
}


/* Draws a 3d axis */
void drawAxes(float length) {
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
	    glDisable(GL_LIGHTING);
	
	    /* Draw the axes, red for x, green for y and blue for z */
	    glBegin(GL_LINES);
	        glColor3f(1, 0, 0);
	        glVertex3f(0, 0, 0); glVertex3f(0 + length, 0, 0);
	
	        glColor3f(0, 1, 0);
	        glVertex3f(0, 0, 0); glVertex3f(0, 0 + length, 0);
	
	        glColor3f(0, 0, 1);
	        glVertex3f(0, 0, 0); glVertex3f(0, 0, 0 + length);
	    glEnd();
	glPopAttrib();
}

void display()
{
	int i, j;
	float w, h;
	float spacing = torus_R + 3;
	float scale = 1/(float)numObj;
	int factor = numObj * 2 - 2;
	
	/* Clear the colour and depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Camera transformations */
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -camera.zoom);
	
	/* Draw scene */
	setRenderOptions();
	
	/* Lights */
	setLights(numLights);
	
	if(renderOptions[RENDER_AXES]) {
        drawAxes(3);
	}

    /* Initial co-ordinates */
	w = -factor;
	h = factor;
	
    /* Drawing objects */
    /* i = horizontal j = vertical */
	for(i=0; i<numObj; i++) {
	    h = factor;
	    for(j=0; j<numObj; j++) {
		    glPushMatrix();
		        glScalef(scale, scale, scale);
		        
		        glPushMatrix();
	                glRotatef(camera.rotX, 1.0f, 0.0f, 0.0f);
			        glTranslatef(w, h, 0);
	                glRotatef(camera.rotY, 0.0f, 1.0f, 0.0f);
	                
			        if(renderOptions[RENDER_AXES]) {
				        drawAxes(3);
			        }

			        //Flags to decide which object to draw - Torus is selected by default
			        switch(objDraw) {
				        case torus:
					        drawTorus(tesselation, mode);
					        break;
				        case sphere:
					        drawSphere(tesselation, mode);
					        break;
				        case gridWave:
					        drawGrid(tesselation, mode);
					        break;
			        }
			    glPopMatrix();
		    glPopMatrix();
		    
		    h -= spacing;
	    }
	    w += spacing;
	}

	/* OSD - framerate etc */
	drawOSD();
}

/*****************************************************************************************
  Event Functions
  	- Mouse, keyboard
*****************************************************************************************/
void mouseDown(int button, int state, int x, int y)
{
	if (button == SDL_BUTTON_LEFT)
		camera.rotating = (state == 1);
	if (button == SDL_BUTTON_RIGHT)
		camera.zooming = (state == 1);
}

void mouseMove(int x, int y)
{
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;
	if (camera.rotating)
	{
		camera.rotY += dx * camera.sensitivity;
		camera.rotX += dy * camera.sensitivity;
	}
	if (camera.zooming)
	{
		camera.zoom -= dy * camera.zoom * camera.sensitivity * 0.03f;
	}
	lastMouseX = x;
	lastMouseY = y;
}

void keyDown(int key)
{
	if (key == SDLK_ESCAPE) {
		quit();
	}
	/* Lighting */
	if (key == SDLK_s) {
		renderOptions[RENDER_LIGHTING] = !renderOptions[RENDER_LIGHTING]; 
	}
	/* Wireframe */
	if (key == SDLK_d) {
		renderOptions[RENDER_WIREFRAME] = !renderOptions[RENDER_WIREFRAME];
	}
	/* Axes */
	if(key == SDLK_a) {
		renderOptions[RENDER_AXES] = !renderOptions[RENDER_AXES];
	}
	/* Backface culling */
	if(key == SDLK_f) {
		renderOptions[RENDER_CULL_BACKFACE] = !renderOptions[RENDER_CULL_BACKFACE];
	}
	/* Flat or smooth rendering */
	if(key == SDLK_g) {
	    renderOptions[RENDER_FLAT] = !renderOptions[RENDER_FLAT];
	}
	
	/* Altering sphere tessellation */
   if(key == SDLK_EQUALS) {
      tesselation *= 2;
        
      if(tesselation >= 512) {
         tesselation = 512;
      }
	
	  reallocArrays(tesselation);
   }
   if(key == SDLK_MINUS) {
		tesselation /= 2;
		
      if(tesselation <= 1) {
         tesselation = 2;
      }
	
	  reallocArrays(tesselation);
   }

	/* Setting mode for which object to draw */
	if(key == SDLK_z) {
		objDraw = torus;
	}
	if(key == SDLK_x) {
		objDraw = sphere;
	}
	if(key == SDLK_c) {
		objDraw = gridWave;
	}
	
	/* Rotation of objects about their vertical axis */
	if(key == SDLK_LEFT) {
		camera.rotY -= 4;
	}
	if(key == SDLK_RIGHT) {
		camera.rotY += 4;
	}
	
	/*Increase/decrease number of lights*/
	if(key == SDLK_UP) {
		if(numLights < 7) {
			numLights++;
		}
	}
	if(key == SDLK_DOWN) {
		if(numLights > 0) {
			numLights--;
		}
	}

	/* Change rendering mode */
	if(key == SDLK_m) {
		mode++;
		if(mode >= NUM_RENDER_MODES) {
			mode = immediate;
		}
#ifdef DEBUG
		printf("mode: %d\n", mode);
#endif
	}

	/* Increases number of objects drawn */
	if(key == SDLK_w) {
		numObj /= 2;

		if(numObj < 1) {
			numObj = 1;
		}
	}
	if(key == SDLK_e) {
		numObj *= 2;
	}
}

void keyUp(int key)
{
}

void cleanup()
{
}

