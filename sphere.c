#include "utils.h"

ShapeType sphereObj;

/*****************************************************************************************
  VBO Functions
*****************************************************************************************/
void setSphereBuffers() {
    /* Passing data to buffers */
	glBindBuffer(GL_ARRAY_BUFFER, sphereObj.buffers[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*sphereObj.numVerts, sphereObj.vertArray, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, sphereObj.buffers[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*sphereObj.numVerts, sphereObj.normArray, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereObj.buffers[INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*sphereObj.numIndices, sphereObj.indexArray, GL_DYNAMIC_DRAW);
	               
    int error = glGetError();
    if(error != 0) {
        printf("\nint:%d string:%s\n", error, gluErrorString(error));
    }
    
    /* Unbind buffers */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	sphereObj.bufferUpdate = false;
}

void drawSphereBuffers() {
    /* If points have changed, update buffers */
	if(sphereObj.bufferUpdate) {
		setSphereBuffers();
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    /* Drawing buffers */
	glBindBuffer(GL_ARRAY_BUFFER, sphereObj.buffers[NORMAL]);
	glNormalPointer(GL_FLOAT,0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, sphereObj.buffers[VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereObj.buffers[INDEX]);
	glDrawElements(GL_TRIANGLE_STRIP, sphereObj.numIndices, GL_UNSIGNED_INT, 0);

    /* Unbind buffers & disable states */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*****************************************************************************************
  Vertex Array Functions
*****************************************************************************************/
void allocateSphereArrays(int tess) {
	/* Grid divisions */
	int divsU = tess*2;
	int divsV = tess;
	
	/* Vertex dimensions */
	int rows = divsU + 1;
	int cols = divsV + 1;
	
	/* Calculate required size */
    sphereObj.numVerts = rows * cols;
    sphereObj.numIndices = 6 * divsU * divsV;
    
	/* Update arrays & check for NULL */
    sphereObj.vertArray = realloc(sphereObj.vertArray, sizeof(float) * sphereObj.numVerts * 3);
    
    if(!sphereObj.vertArray) {
        printf("Error initialising sphere vertex array. \n");
        quit();
    }
        
    sphereObj.normArray = realloc(sphereObj.normArray, sizeof(float) * sphereObj.numVerts * 3);
    
    if(!sphereObj.normArray) {
        printf("Error initialising sphere normal vertex array. \n");
        quit();
    }
    
    sphereObj.indexArray = realloc(sphereObj.indexArray, sizeof(int) * sphereObj.numIndices);
        
    if(!sphereObj.indexArray) {
        printf("Error initialising sphere normal vertex array. \n");
        quit();
    }
}

void generateSpherePoints(int tess) {    
	int i, j;
	float u, v;
	
	/* Grid divisions */
	int divsU = tess*2;
	int divsV = tess;
	
	/* Vertex dimensions */
	int rows = divsU + 1;
	int cols = divsV + 1;
	
    sphereObj.vCount = 0;
    sphereObj.nCount = 0;
    sphereObj.iCount = 0;

    /* Vertices */
	for (j = 0; j < cols; ++j) {
		v = j / (float)(cols - 1); 

		for (i = 0; i < rows; ++i) {
			u = i / (float)(rows - 1);

			storeSphereVertex(u, v);
		}
	}
	
	/* Indices */
	for (j = 0; j < cols-1; ++j) {
		v = (j + 1);

		for (i = 0; i < rows-1; ++i) {
			u = (i + 1);
			
			assert(j * rows + i < sphereObj.numVerts);
			assert(v * rows + u < sphereObj.numVerts);
			
		    sphereObj.indexArray[sphereObj.iCount++] = j * rows + i;
		    sphereObj.indexArray[sphereObj.iCount++] = v * rows + i;
		    sphereObj.indexArray[sphereObj.iCount++] = v * rows + u;
		    
		    sphereObj.indexArray[sphereObj.iCount++] = j * rows + i;
		    sphereObj.indexArray[sphereObj.iCount++] = v * rows + u;
		    sphereObj.indexArray[sphereObj.iCount++] = j * rows + u;
		}
	}

    assert(sphereObj.iCount == sphereObj.numIndices);

    sphereObj.bufferUpdate = true;
}


void storeSphereVertex(float u, float v)
{
	float theta = u * 2.0 * pi;
	float phi = v * pi;

    /* Calculate co-ordinates */
	float x = sphereObj.R * cos(theta) * sin(phi);
	float y = sphereObj.R * sin(theta) * sin(phi);
	float z = sphereObj.R * cos(phi);
	
    /* Calculate & store normals */
	sphereObj.normArray[sphereObj.nCount++] = x/sphereObj.R;
	sphereObj.normArray[sphereObj.nCount++] = y/sphereObj.R;
	sphereObj.normArray[sphereObj.nCount++] = z/sphereObj.R;
	
	sphereObj.vertArray[sphereObj.vCount++] = x;
	sphereObj.vertArray[sphereObj.vCount++] = y;
	sphereObj.vertArray[sphereObj.vCount++] = z;
}


void drawSphereArray() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glNormalPointer(GL_FLOAT, 0, sphereObj.normArray);
    glVertexPointer(3, GL_FLOAT, 0, sphereObj.vertArray);
    glDrawElements(GL_TRIANGLES,sphereObj.numIndices,GL_UNSIGNED_INT, sphereObj.indexArray);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}


/*****************************************************************************************
  Immediate Functions
*****************************************************************************************/
void drawSphereVertexImmediate(float u, float v)
{
	/* Use maths rather than physics spherical coordinate convention */
	float theta = u * 2.0 * pi;
	float phi = v * pi;

	float x = sphereObj.R * cos(theta) * sin(phi);
	float y = sphereObj.R * sin(theta) * sin(phi);
	float z = sphereObj.R * cos(phi);
	
	glNormal3f(x/sphereObj.R, y/sphereObj.R, z/sphereObj.R);
	glVertex3f(x, y, z);
}


void drawSphereImmediate(int rows, int cols) {
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
			drawSphereVertexImmediate(u, v);
			drawSphereVertexImmediate(u, v1);
			drawSphereVertexImmediate(u1, v1);
			
			drawSphereVertexImmediate(u, v);
			drawSphereVertexImmediate(u1, v1);
			drawSphereVertexImmediate(u1, v);
		}
	}
	glEnd();
}


/*****************************************************************************************
  Entry point
*****************************************************************************************/
void drawSphere(int slices, enum RenderMode mode) {	
	/* Grid divisions */
	int divsU = slices*2;
	int divsV = slices;
	
	/* Vertex dimensions */
	int rows = divsU + 1;
	int cols = divsV + 1;
	
	/* Rendering mode */
	switch(mode) {
	    case immediate:
		    drawSphereImmediate(rows, cols);
	        break;
	     
	    case array:
		    drawSphereArray();
	        break;
	        
	    case bufferObj:
	    	drawSphereBuffers();
	        break;
	    
	    default:
		    printf("Error - render mode outside of bounds\n");
		    quit();
	        break;
	}
}


void sphereInit(float radius) {
	/* Initialising arrays */
    sphereObj.vertArray = malloc(sizeof(Vec3f));
    
    if(!sphereObj.vertArray) {
        printf("Error initialising sphere vertex array. \n");
        quit();
    }
    
    sphereObj.normArray = malloc(sizeof(Vec3f));
    
    if(!sphereObj.normArray) {
        printf("Error initialising sphere normal vertex array. \n");
        quit();
    }
    
    sphereObj.indexArray = malloc(sizeof(Vec3f));
    
    if(!sphereObj.indexArray) {
        printf("Error initialising sphere normal vertex array. \n");
        quit();
    }

    /* Initialising buffers */
    glGenBuffers(NUM_BUFFERS, sphereObj.buffers);
 
    glBindBuffer(GL_ARRAY_BUFFER, sphereObj.buffers[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphereObj.numVerts, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereObj.buffers[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * sphereObj.numIndices, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Initialising other struct members */
    sphereObj.R = radius;
    sphereObj.vCount = 0;
    sphereObj.nCount = 0;
    sphereObj.iCount = 0;
    sphereObj.numVerts = 0;
    sphereObj.numIndices = 0;
    sphereObj.bufferUpdate = true;
}
