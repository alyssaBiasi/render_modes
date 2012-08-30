#ifndef UTILS_H
#define UTILS_H

#include "sdlbase.h"
#include "assert.h"

#include <math.h>
#include <string.h>
#include <stdbool.h>

#define NUM_BUFFERS 3
#define VERTEX 0
#define INDEX 1
#define NORMAL 2

static const float pi = 3.14159265f;

/* Vector struct */
/*  utility add/sub/dot/normalize functions. */
typedef struct {
	float x, y, z;
} Vec3f;

/* Basic camera struct */
typedef struct {
	int rotating;
	int zooming;
	float rotX, rotY;
	float zoom;
	float sensitivity;
} Camera;

/* Basic struct for shapes */
typedef struct {
	float *vertArray;
	float *normArray;
	unsigned int *indexArray;
	float R;
	float r;
	int vCount; 
	int nCount;
	int iCount;
	int numVerts;
	int numIndices;
	GLuint buffers[NUM_BUFFERS];
	bool bufferUpdate;
} ShapeType;

/* Render state enums */
enum RenderOptions {
	RENDER_LIGHTING,
	RENDER_WIREFRAME,
	RENDER_FLAT,
	RENDER_CULL_BACKFACE,
	RENDER_AXES,
	NUM_RENDER_OPTIONS
};

enum RenderMode {
	immediate = 1,
	array = 2,
	bufferObj = 3,
	NUM_RENDER_MODES
};

enum ObjectType {
	torus=1, 
	sphere=2, 
	gridWave=3
};

void drawAxes(float length);

/* Torus functions */
void drawTorusVertex(float u, float v);
void drawTorusImmediate (int rows, int cols);
void drawTorus(int slices, enum RenderMode mode);
void allocateTorusArrays(int tess);
void generateTorusPoints(int tess);
void storeTorusVertex(float u, float v);
void drawTorusVertexArray();
void torusInit(float R, float r);
void drawTorusBuffers();
void setTorusBuffers();


/* Sphere functions */
void drawSphere(int slices, enum RenderMode mode);
void drawSphereImmediate(int rows, int cols);
void drawSphereVertexImmediate(float u, float v);
void generateSpherePoints(int slices);
void storeSphereVertex(float u, float v);
void drawSphereArray();
void allocateSphereArrays(int slices);
void sphereInit(float radius);
void setSphereBuffers();
void drawSphereBuffers();


/* Grid functions */
void drawGrid(int rows, enum RenderMode mode);
void drawGridImmediate(int rows);
void allocateTorusArrays(int tess);
void gridInit();
void generateGridPoints(int tess);
void drawGridArray();
void setGridBuffers();
void drawGridBuffers();

/* Vertex array functions */
void generateArrays(int tess);
void reallocArrays(int tess);

/*Light functions*/
void setLights(int numLights);
void disableLights();

/*GUI function*/
void writeScreenText(char *buffer, Vec3f pos);


/* Vec3f functions */
Vec3f newVec3f(float x, float y, float z);
Vec3f addVec3f(Vec3f v1, Vec3f v2);
#endif
