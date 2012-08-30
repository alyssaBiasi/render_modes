#include "utils.h"

ShapeType gridObj;

/*****************************************************************************************
  VBO Functions
*****************************************************************************************/
void setGridBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, gridObj.buffers[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*gridObj.numVerts, gridObj.vertArray, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, gridObj.buffers[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*gridObj.numVerts, gridObj.normArray, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridObj.buffers[INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*gridObj.numIndices, gridObj.indexArray, GL_DYNAMIC_DRAW);
	               
    int error = glGetError();
    if(error != 0) {
        printf("\nint:%d string:%s\n", error, gluErrorString(error));
    }
    
    /* Unbind buffers */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	gridObj.bufferUpdate = false;
}

void drawGridBuffers() {
    /* If points have changed, update buffers */
	if(gridObj.bufferUpdate) {
		setGridBuffers();
	}

	
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    /* Drawing buffers */
	glBindBuffer(GL_ARRAY_BUFFER, gridObj.buffers[NORMAL]);
	glNormalPointer(GL_FLOAT,0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, gridObj.buffers[VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridObj.buffers[INDEX]);
	glDrawElements(GL_TRIANGLES, gridObj.numIndices, GL_UNSIGNED_INT, 0);

    /* Unbind buffers & disable states */
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



/*****************************************************************************************
  Vertex Array Functions
*****************************************************************************************/
 void allocateGridArrays(int tess) {
	gridObj.numVerts = (tess) * (tess);
	gridObj.numIndices = (tess-1) * (tess-1) * 6;

	gridObj.vertArray = realloc(gridObj.vertArray, sizeof(float) * gridObj.numVerts * 3);

	if(!gridObj.vertArray) {
		printf("Error reallocating grid vertex array. \n");
		quit();
	}

	gridObj.normArray = realloc(gridObj.normArray, sizeof(float) * gridObj.numVerts * 3);

	if(!gridObj.normArray) {
		printf("Error reallocating grid normal array. \n");
		quit();
	}

	gridObj.indexArray = realloc(gridObj.indexArray, sizeof(float) * gridObj.numIndices * 3);

	if(!gridObj.indexArray) {
		printf("Error reallocating grid index array. \n");
		quit();
	}
 }

void generateGridPoints(int tess) {
	const float A = 0.082f;
	const float lambda = 0.2f;
	const float k = pi * 2.0f / lambda;

	int i, j, u, v;
	float x, y, z, mag;
	float size = 3;
	float stepSize = 1/(float)tess;

	gridObj.nCount = 0;
	gridObj.vCount = 0;
	gridObj.iCount = 0;

	/* Vertices */
	for(i=0; i<tess; i++) {
	//	x = (x-0.5) * size;
		
		for(j=0; j<tess; j++) {
		    x = j * stepSize;
			z = i * stepSize;
		//	z = (z-0.5) * size;

			y = A * sinf(k * x) + A * sinf(k * z);
			
			gridObj.vertArray[gridObj.vCount++] = x;
			gridObj.vertArray[gridObj.vCount++] = y;
			gridObj.vertArray[gridObj.vCount++] = z;

            float n[] = {-A * k * cosf(k * x), 1.0, - A * k * cosf(k * z)};
            mag = sqrt(pow(n[0],2) + 1 + pow(n[2],2));

            n[0] /= mag;
            n[1] /= mag;
            n[2] /= mag;
            
			gridObj.normArray[gridObj.nCount++] = n[0];
			gridObj.normArray[gridObj.nCount++] = n[1];
			gridObj.normArray[gridObj.nCount++] = n[2];
		}
	}

	/* Indices */
	for(i=0; i<(tess-1); i++) {
		u = i + 1;
		for(j=0; j<(tess-1); j++) {
			v = j + 1;

			gridObj.indexArray[gridObj.iCount++] = i * tess + j;
			gridObj.indexArray[gridObj.iCount++] = i * tess + v;
			gridObj.indexArray[gridObj.iCount++] = u * tess + j;

			gridObj.indexArray[gridObj.iCount++] = u * tess + j;
			gridObj.indexArray[gridObj.iCount++] = i * tess + v;
			gridObj.indexArray[gridObj.iCount++] = u * tess + v;
		}
	}
    
    gridObj.bufferUpdate = true;
}


void drawGridArray() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glNormalPointer(GL_FLOAT, 0, gridObj.normArray);
    glVertexPointer(3, GL_FLOAT, 0, gridObj.vertArray);
    glDrawElements(GL_TRIANGLES, gridObj.numIndices, GL_UNSIGNED_INT, gridObj.indexArray);
}


/*****************************************************************************************
  Immediate
*****************************************************************************************/
void drawGridImmediate(int rows) {
	const float A = 0.082f;
	const float lambda = 0.2f;
	const float k = pi * 2.0f / lambda;
	
    int i, j;
	float x, y, z, mag;
	float size = 3;
	float stepSize = 1/(float)rows;

    for(i=0; i<rows-1; i++) {
        glBegin(GL_TRIANGLE_STRIP);

            for(j=0; j<rows; j++) {
                x = j*stepSize;
                z = i*stepSize;
                
                y = A*sinf(k*x) + A*sinf(k*z);
                
                float n[] = {-A*k*cosf(k*x),1.0, -A*k*cosf(k*z)};
                mag = sqrt(pow(n[0],2) + 1 + pow(n[2],2));
                n[0] /= mag;
                n[1] /= mag;
                n[2] /= mag;

                glNormal3fv(n);
                glVertex3f(x,y,z);
                
                z += stepSize;
                y = A*sinf(k*x) + A*sinf(k*(z));
                
                n[2] = -A*k*cosf(k*z);
                mag = sqrt(pow(n[0],2) + 1 + pow(n[2],2));
                n[2] /= mag;
                
                glNormal3fv(n);
                glVertex3f(x,y,z);
            }
        glEnd();
    }
    

/*	glPushMatrix();
		glBegin(GL_TRIANGLE_STRIP);
			for (i = 0; i <= rows; ++i) {
			    x = i / (float)(rows-1);

			    y = A * sin(k * x);
			    z = 0.0;
			    
				nX = -A * k * cosf(k * x); 
				nY = 1.0;
				nZ = - A * k * cosf(k * z);

				glNormal3f(nX, nY, nZ);
			    glVertex3f(x, y, z);

			    z = 1.0;
				nZ = - A * k * cosf(k * z);

				glNormal3f(nX, nY, nZ);
			    glVertex3f(x, y, z);
		    }
		glEnd();
	glPopMatrix();
	*/
}


/*****************************************************************************************
  Entry point
*****************************************************************************************/
void drawGrid(int rows, enum RenderMode mode) {
	/* Rendering mode */
	switch(mode) {
		case immediate:
			drawGridImmediate(rows);
			break;

		case array:
			drawGridArray();
			break;

		case bufferObj:
			drawGridBuffers();
			break;

		default:
			printf("Error - render mode outside of bounds\n");
			quit();
			break;
	}
}


void gridInit() {
	/* Initialising arrays */
	gridObj.vertArray = malloc(sizeof(Vec3f));

	if(!gridObj.vertArray) {
		printf("Error initialising grid vertex array.\n");
		quit();
	}

	gridObj.normArray = malloc(sizeof(Vec3f));

	if(!gridObj.normArray) {
		printf("Error initialising grid normal array.\n");
		quit();
	}

	gridObj.indexArray = malloc(sizeof(Vec3f));

	if(!gridObj.indexArray) {
		printf("Error initialising grid index array.\n");
		quit();
	}

    /* Initialising buffers */
    glGenBuffers(NUM_BUFFERS, gridObj.buffers);
 
    glBindBuffer(GL_ARRAY_BUFFER, gridObj.buffers[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gridObj.numVerts, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridObj.buffers[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * gridObj.numIndices, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    /* Initialising other struct members */
	gridObj.nCount = 0;
	gridObj.vCount = 0;
	gridObj.iCount = 0;
	gridObj.numVerts = 0;
	gridObj.numIndices = 0;
	gridObj.bufferUpdate = true;
}
