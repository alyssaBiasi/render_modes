#include "utils.h"

ShapeType torusObj;

/*****************************************************************************************
  VBO Functions
*****************************************************************************************/
void setTorusBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, torusObj.buffers[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*torusObj.numVerts, torusObj.vertArray, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, torusObj.buffers[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*torusObj.numVerts, torusObj.normArray, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusObj.buffers[INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*torusObj.numIndices, torusObj.indexArray, GL_DYNAMIC_DRAW);
	               
    int error = glGetError();
    if(error != 0) {
        printf("\nint:%d string:%s\n", error, gluErrorString(error));
    }
    
    /* Unbind buffers */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	torusObj.bufferUpdate = false;
}

void drawTorusBuffers() {
    /* If points have changed, update buffers */
	if(torusObj.bufferUpdate) {
		setTorusBuffers();
	}

	
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    /* Drawing buffers */
	glBindBuffer(GL_ARRAY_BUFFER, torusObj.buffers[NORMAL]);
	glNormalPointer(GL_FLOAT,0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, torusObj.buffers[VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusObj.buffers[INDEX]);
	glDrawElements(GL_TRIANGLE_STRIP, torusObj.numIndices, GL_UNSIGNED_INT, 0);

    /* Unbind buffers & disable states */
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


/*****************************************************************************************
  Vertex Array Functions
*****************************************************************************************/
void allocateTorusArrays(int tess) {
	/* Grid divisions */
	int divsU = tess * 2;
	int divsV = tess;
	
	/* Vertex dimensions */
	int rows = divsU + 1;
	int cols = divsV + 1;

	/* Calculate required size */
	torusObj.numVerts = rows * cols;
	torusObj.numIndices = divsU * divsV * 6;

	/* Update arrays & check for NULL */
	torusObj.vertArray = realloc(torusObj.vertArray, sizeof(float) * torusObj.numVerts * 3);

	if(!torusObj.vertArray) {
		printf("Error reallocating torus vertex array. \n");
		quit();
	}

	torusObj.normArray = realloc(torusObj.normArray, sizeof(float) * torusObj.numVerts * 3);

	if(!torusObj.normArray) {
		printf("Error reallocating torus normal array. \n");
		quit();
	}

	torusObj.indexArray = realloc(torusObj.indexArray, sizeof(float) * torusObj.numIndices);

	if(!torusObj.indexArray) {
		printf("Error reallocating torus index array.\n");
		quit();
	}
}


void generateTorusPoints(int tess) {
	int i, j;
	float u, v;

	/* Grid divisions */
	int divsU = tess*2;
	int divsV = tess;
	
	/* Vertex dimensions */
	int rows = divsU + 1;
	int cols = divsV + 1;

	torusObj.vCount = 0;
	torusObj.nCount = 0;
	torusObj.iCount = 0;

	/* Vertices  */
	for (j = 0; j < cols; ++j) {
		v = j / (float)(cols - 1);

		for (i = 0; i < rows; ++i) {
			u = i / (float)(rows - 1);

			storeTorusVertex(u, v);
		}
	}

	/* Indices */
	for (j = 0; j < cols - 1; ++j) {
		v = j + 1;

		for (i = 0; i < rows - 1; ++i) {
			u = i + 1;
			
			assert(j * rows + i < torusObj.numVerts);
			assert(v * rows + u < torusObj.numVerts);
			
		    torusObj.indexArray[torusObj.iCount++] = j*rows + i;
		    torusObj.indexArray[torusObj.iCount++] = v*rows + i;
		    torusObj.indexArray[torusObj.iCount++] = v*rows + u;
		    
		    torusObj.indexArray[torusObj.iCount++] = j*rows + i;
		    torusObj.indexArray[torusObj.iCount++] = v*rows + u;
		    torusObj.indexArray[torusObj.iCount++] = j*rows + u;
		}
	}

    assert(torusObj.iCount == torusObj.numIndices);
    
	torusObj.bufferUpdate = true;
}

void storeTorusVertex(float u, float v) {
	float theta = u * 2.0 * pi;
	float phi = v * 2.0 * pi;

    /* Calculate co-ordinates */
	float x = (torusObj.R + torusObj.r * sin(phi)) * cos(theta);
	float y = (torusObj.R + torusObj.r * sin(phi)) * sin(theta);
	float z = torusObj.r * cos(phi);
	
    /* Calculate & store normals */
	torusObj.normArray[torusObj.nCount++] = cos(theta) * sin(phi);
	torusObj.normArray[torusObj.nCount++] = sin(theta) * sin(phi);
	torusObj.normArray[torusObj.nCount++] = cos(phi);
	
	torusObj.vertArray[torusObj.vCount++] = x;
	torusObj.vertArray[torusObj.vCount++] = y;
	torusObj.vertArray[torusObj.vCount++] = z;
}

void drawTorusVertexArray() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    /* Drawing arrays */
    glNormalPointer(GL_FLOAT, 0, torusObj.normArray);
    glVertexPointer(3, GL_FLOAT, 0, torusObj.vertArray);
    glDrawElements(GL_TRIANGLES, torusObj.numIndices, GL_UNSIGNED_INT, torusObj.indexArray);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

/*****************************************************************************************
  Immediate Functions
*****************************************************************************************/
void drawTorusVertex(float u, float v)
{
	float theta = u * 2.0 * pi;
	float phi = v * 2.0 * pi;

	float x = (torusObj.R + torusObj.r * sin(phi)) * cos(theta);
	float y = (torusObj.R + torusObj.r * sin(phi)) * sin(theta);
	float z = torusObj.r * cos(phi);

	glNormal3f(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
	glVertex3f(x, y, z);
}


void drawTorusImmediate (int rows, int cols) {
	int i, j;
	float u, v, u1, v1;

	/* j outer loop over i inner loop for each circle */
	glBegin(GL_TRIANGLES);
	for (j = 0; j < cols - 1; ++j)
	{
		v = j / (float)(cols - 1);
		v1 = (j + 1) / (float)(cols - 1);
		for (i = 0; i < rows - 1; ++i)
		{
			u = i / (float)(rows - 1);
			u1 = (i + 1) / (float)(rows - 1);
			drawTorusVertex(u, v);
			drawTorusVertex(u, v1);
			drawTorusVertex(u1, v1);
			
			drawTorusVertex(u, v);
			drawTorusVertex(u1, v1);
			drawTorusVertex(u1, v);
		}
	}
	glEnd();
}


/*****************************************************************************************
  Entry point
*****************************************************************************************/
void drawTorus(int slices, enum RenderMode mode)
{
	/* Grid divisions */
	int divsU = slices*2;
	int divsV = slices;
	
	/* Vertex dimensions */
	int rows = divsU + 1;
	int cols = divsV + 1;

	/* Rendering mode */
	switch(mode) {
	    case immediate:
		    drawTorusImmediate(rows, cols);
	        break;
	     
	    case array:
		    drawTorusVertexArray();
	        break;
	        
	    case bufferObj:
	    	drawTorusBuffers();
	        break;
	    
	    default:
		    printf("Error - render mode outside of bounds\n");
		    quit();
	        break;
	}
}


void torusInit(float R, float r) {
	/* Initialising arrays */
	torusObj.vertArray = malloc(sizeof(Vec3f));

	if(!torusObj.vertArray) {
		printf("Error initialising torus vertex array. \n");
		quit();
	}

	torusObj.normArray = malloc(sizeof(Vec3f));

	if(!torusObj.normArray) {
		printf("Error initialising torus normal array.\n");
		quit();
	}

	torusObj.indexArray = malloc(sizeof(Vec3f));

	if(!torusObj.indexArray) {
		printf("Error initialising torus index array.\n");
		quit();
	}
	
    /* Initialising buffers */
    glGenBuffers(NUM_BUFFERS, torusObj.buffers);
 
    glBindBuffer(GL_ARRAY_BUFFER, torusObj.buffers[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * torusObj.numVerts, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusObj.buffers[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * torusObj.numIndices, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Initialising other struct members */
	torusObj.R = R;
	torusObj.r = r;
	torusObj.vCount = 0;
	torusObj.nCount = 0;
	torusObj.iCount = 0;
	torusObj.numVerts = 0;
	torusObj.numIndices = 0;
	torusObj.bufferUpdate = true;
}
