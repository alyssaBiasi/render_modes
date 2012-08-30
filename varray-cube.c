/*
 *  varray-cube.c 
 *  This program demonstrates vertex arrays for a cube.
 *  Originally based on the redbook varray.c example.
 *  $Id: varray-cube.c,v 1.3 2010/08/04 04:19:56 gl Exp gl $
 */

#define DEBUG
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DRAWARRAY 1
#define MULTIDRAWARRAY 2
#define ARRAYELEMENT  3
#define DRAWELEMENTS 4
#define DRAWELEMENTSALL 5
#define MULTIDRAWELEMENTS 6

int renderMethod = 1;
const int numRenderMethods = 6;

bool useBufferObjects = false;

GLfloat vertices[] = {
  0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, // 1
  1.0, 1.0, 0.0, // 2
  0.0, 1.0, 0.0, // 3
  0.0, 0.0, 1.0, // 4
  1.0, 0.0, 1.0, // 5
  1.0, 1.0, 1.0, // 6
  0.0, 1.0, 1.0  // 7
};

GLfloat colors[] = {
  0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, // 1
  0.0, 1.0, 0.0, // 2
  0.0, 0.0, 1.0, // 3
  1.0, 1.0, 0.0, // 4
  1.0, 0.0, 1.0, // 5
  0.0, 1.0, 1.0, // 6
  1.0, 1.0, 1.0, // 7
};

GLfloat colorsAndVertices[] = {
  // Colors
  0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, // 1
  0.0, 1.0, 0.0, // 2
  0.0, 0.0, 1.0, // 3
  1.0, 1.0, 0.0, // 4
  1.0, 0.0, 1.0, // 5
  0.0, 1.0, 1.0, // 6
  1.0, 1.0, 1.0, // 7
  // Vertices
  0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, // 1
  1.0, 1.0, 0.0, // 2
  0.0, 1.0, 0.0, // 3
  0.0, 0.0, 1.0, // 4
  1.0, 0.0, 1.0, // 5
  1.0, 1.0, 1.0, // 6
  0.0, 1.0, 1.0, // 7
};

GLfloat colorsAndVerticesInterleaved[] = {
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, 1.0, 0.0, 0.0, // 1
  0.0, 1.0, 0.0, 1.0, 1.0, 0.0, // 2
  0.0, 0.0, 1.0, 0.0, 1.0, 0.0, // 3
  1.0, 1.0, 0.0, 0.0, 0.0, 1.0, // 4
  1.0, 0.0, 1.0, 1.0, 0.0, 1.0, // 5
  0.0, 1.0, 1.0, 1.0, 1.0, 1.0, // 6
  1.0, 1.0, 1.0, 0.0, 1.0, 1.0, // 7
};

GLfloat verticesQuads[] = {
  // Front
  0.0, 0.0, 1.0, // 4
  1.0, 0.0, 1.0, // 5
  1.0, 1.0, 1.0, // 6
  0.0, 1.0, 1.0, // 7

  // Back
  0.0, 0.0, 0.0, // 0
  0.0, 1.0, 0.0, // 3
  1.0, 1.0, 0.0, // 2
  1.0, 0.0, 0.0, // 1

  // Left
  0.0, 0.0, 0.0, // 0
  0.0, 0.0, 1.0, // 4
  0.0, 1.0, 1.0, // 7
  0.0, 1.0, 0.0, // 3

  // Right
  1.0, 0.0, 0.0, // 1
  1.0, 1.0, 0.0, // 2
  1.0, 1.0, 1.0, // 6
  1.0, 0.0, 1.0, // 5

  // Bottom
  0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, // 1
  1.0, 0.0, 1.0, // 5
  0.0, 0.0, 1.0, // 4

  // Top
  1.0, 1.0, 0.0, // 2
  0.0, 1.0, 0.0, // 3
  0.0, 1.0, 1.0, // 7
  1.0, 1.0, 1.0, // 6
};

GLfloat colorsQuads[] = {
  // Front
  1.0, 1.0, 0.0, // 4
  1.0, 0.0, 1.0, // 5
  0.0, 1.0, 1.0, // 6
  1.0, 1.0, 1.0, // 7

  // Back
  0.0, 0.0, 0.0, // 0
  0.0, 0.0, 1.0, // 3
  0.0, 1.0, 0.0, // 2
  1.0, 0.0, 0.0, // 1

  // Left
  0.0, 0.0, 0.0, // 0
  1.0, 1.0, 0.0, // 4
  1.0, 1.0, 1.0, // 7
  0.0, 0.0, 1.0, // 3

  // Right
  1.0, 0.0, 0.0, // 1
  0.0, 1.0, 0.0, // 2
  0.0, 1.0, 1.0, // 6
  1.0, 0.0, 1.0, // 5

  // Bottom
  0.0, 0.0, 0.0, // 0
  1.0, 0.0, 0.0, // 1
  1.0, 0.0, 1.0, // 5
  1.0, 1.0, 0.0, // 4

  // Top
  0.0, 1.0, 0.0, // 2
  0.0, 0.0, 1.0, // 3
  1.0, 1.0, 1.0, // 7
  0.0, 1.0, 1.0, // 6
};

GLuint indices1DArray[] = { 
  4, 5, 6, 7,   // Front
  0, 3, 2, 1,   // Back
  0, 4, 7, 3,   // Left
  1, 2, 6, 5,   // Right 
  0, 1, 5, 4,   // Bottom 
  2, 3, 7, 6    // Top
};

GLuint indices2DArray[][4] = { 
  { 4, 5, 6, 7 },  // Front
  { 0, 3, 2, 1 },  // Back
  { 0, 4, 7, 3 },  // Left 
  { 1, 2, 6, 5 },  // Right
  { 0, 1, 5, 4 },  // Bottom 
  { 2, 3, 7, 6 }   // Top
};


GLuint frontIndices[] = { 4, 5, 6, 7 };
GLuint backIndices[] = { 0, 3, 2, 1 };
GLuint leftIndices[] = { 0, 4, 7, 3 };
GLuint rightIndices[] = { 1, 2, 6, 5 };
GLuint bottomIndices[] = { 0, 1, 5, 4 };
GLuint topIndices[] = { 2, 3, 7, 6 };

const GLvoid* indices1DArrayOfArray[] = { 
  frontIndices,
  backIndices,
  leftIndices,
  rightIndices,
  bottomIndices,
  topIndices
};

GLint indicesFirsts[] = { 0, 4, 8, 12, 16, 20 };

GLsizei indicesCounts[] = { 4, 4, 4, 4, 4, 4 };

const GLvoid* indicesOffsets[] = { 
  (GLvoid*)(0),
  (GLvoid*)(1 * sizeof(indices2DArray[0])),
  (GLvoid*)(2 * sizeof(indices2DArray[0])),
  (GLvoid*)(3 * sizeof(indices2DArray[0])),
  (GLvoid*)(4 * sizeof(indices2DArray[0])),
  (GLvoid*)(5 * sizeof(indices2DArray[0]))
};

GLsizei numQuads = sizeof(indices2DArray) / sizeof(indices2DArray[0]);

#define NUM_BUFFERS 3
#define VERTICES 0
#define COLORS 1
#define INDICES 2

GLuint buffers[NUM_BUFFERS];

void enableVertexArrays(void)
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
}

void generateBuffers(void)
{
  glGenBuffers(NUM_BUFFERS, buffers);
}

void bufferData()
{
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[COLORS]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2DArray), 
	       indices2DArray, GL_STATIC_DRAW);
}

void unBindBuffers()
{
  int buffer;

  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffer);
  if (buffer != 0)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffer);
  if (buffer != 0)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init(void) 
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  generateBuffers();
  enableVertexArrays();
  bufferData();
  unBindBuffers();
}

void display(void)
{
  int i, j;
  GLenum error;
  static bool bufferObjectsBound = false;

#ifdef DEBUG
  printf("rendering method: %d\n", renderMethod);
  printf("useBufferObjects %d\n", useBufferObjects);
#endif

  glLoadIdentity();
  gluLookAt(4.0, 4.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  /* Only leave buffer objects bound for right cases */
  if (bufferObjectsBound && 
      (!useBufferObjects ||!(renderMethod == DRAWELEMENTS || 
	     renderMethod == DRAWELEMENTSALL || 
	     renderMethod == MULTIDRAWELEMENTS)))
  {
    unBindBuffers();
    bufferObjectsBound = false;
  } 

  /* Render using chosen technique. Factor out common code for
   * different techniques. Nested switches seems neater than ifs.
   */
  switch (renderMethod) {
  case DRAWARRAY:
  case MULTIDRAWARRAY:
    glVertexPointer(3, GL_FLOAT, 0, verticesQuads);
    glColorPointer(3, GL_FLOAT, 0, colorsQuads);
    switch (renderMethod) {
    case DRAWARRAY:
      glDrawArrays(GL_QUADS, 0, sizeof(verticesQuads)/(3*sizeof(GLfloat)));
      break;
    case MULTIDRAWARRAY:
      /* Just an example to demonstrate usage - no advantage to use
       * glMultiDrawArrays for cube as all primitives are quads so
       * glDrawArrays does the job.
       */
      glMultiDrawArrays(GL_QUADS, indicesFirsts, indicesCounts, numQuads);
      break;
    }
  case ARRAYELEMENT:
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glBegin(GL_QUADS);
    for (i = 0; i < numQuads; i++) {
      for (j = 0; j < 4; j++)
	glArrayElement(((GLuint*)indices1DArrayOfArray[i])[j]);
    }
    glEnd();
    break;
  case DRAWELEMENTS:
  case DRAWELEMENTSALL:
  case MULTIDRAWELEMENTS:
    /* All three techniques can use vertex arrays or buffer objects.
     * Factor out common code for setting up both.
     */
    if (!useBufferObjects) {
      glVertexPointer(3, GL_FLOAT, 0, vertices);
      glColorPointer(3, GL_FLOAT, 0, colors);
    } else if (useBufferObjects && !bufferObjectsBound) {
      glBindBuffer(GL_ARRAY_BUFFER, buffers[COLORS]);
      glColorPointer(3, GL_FLOAT, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES]);
      bufferObjectsBound = true;
    }
    switch (renderMethod) {
      case DRAWELEMENTS:
        for (i = 0; i < numQuads; i++)
        	if (useBufferObjects)
        	  glDrawElements(GL_QUADS, 
  			 sizeof(indices2DArray) / sizeof(indices2DArray[0]),
  			 GL_UNSIGNED_INT, 
  			 (const GLvoid*)(i*sizeof(indices2DArray[0])));
  	else
  	  glDrawElements(GL_QUADS, 
  			 sizeof(indices2DArray) / sizeof(indices2DArray[0]),
  			 GL_UNSIGNED_INT, indices2DArray[i]);
        break;
      case DRAWELEMENTSALL:
        if (useBufferObjects)
  	glDrawElements(GL_QUADS, sizeof(indices2DArray) / sizeof(GLint),
  		       GL_UNSIGNED_INT, 0);
        else
  	glDrawElements(GL_QUADS, sizeof(indices1DArray) / sizeof(GLuint),
  		       GL_UNSIGNED_INT, indices1DArray);
        break;
      case MULTIDRAWELEMENTS:
        /* Another example just to demonstrate usage, like
         * glMultiDrawArrays.  Again no advantage using the 'multi'
         * version for cube consisting of quads.
         */
        if (useBufferObjects)
  	glMultiDrawElements(GL_QUADS, indicesCounts, GL_UNSIGNED_INT, 
  			    indicesOffsets, numQuads);
        else
  	glMultiDrawElements(GL_QUADS, indicesCounts, GL_UNSIGNED_INT, 
  			    indices1DArrayOfArray, numQuads);
        break;
    }
  }

  if ((error = glGetError()) != GL_NO_ERROR) {
    printf("%s\n", gluErrorString(error));
  }

  glFlush();
}

void reshape (int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 2.0, -1.0, 2.0, 0.1, 10.0);
  glMatrixMode(GL_MODELVIEW);
} 


void mouse (int button, int state, int x, int y)
{
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) {
        useBufferObjects = !useBufferObjects;
        glutPostRedisplay();
      }
      break;

    case GLUT_RIGHT_BUTTON:
      if (state == GLUT_DOWN) {
        renderMethod++;
        if (renderMethod > numRenderMethods)
  	       renderMethod = 1;
        glutPostRedisplay();
      }
      break;
      
    default:
      break;
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(0);
      break;
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(350, 350); 
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
