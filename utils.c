#include "utils.h"

void disableLights() {
	glDisable(GL_LIGHT0);
}
void setLights(int numLights) {
    static float lightPosition[] = {0.0f, 0.0f, 5.0f, 1.0f};
    static float lightPosition1[] = {0.0f, 0.0f, 1.0f, 0.0f};
    static float lightPosition2[] = {0.5f, 0.5f, 1.0f, 0.0f};
    static float lightPosition3[] = {0.5f, 1.0f, 1.0f, 0.0f};
    static float lightPosition4[] = {-0.5f, 0.5f, 1.0f, 0.0f};
    static float lightPosition5[] = {-0.5, 1.0f, 1.0f, 0.0f};
    static float lightPosition6[] = {1.0f, 0.5f, 1.0f, 0.0f};
    static float lightPosition7[] = {0.0f, 0.5f, 1.0f, 0.0f};

    static float lightAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f}; /* Constant, add to everything */
//    static float materialDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f}; /* Brighter as surface faces light */
    static float materialDiffuse[] = {0.26f, 0.43f, 0.93f, 1.0f}; /* Blue */
    static float materialDiffuse1[] = {1.00f, 0.00f, 0.10f, 0.0f}; /* Red */
    static float materialSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f}; /* Highlight, direct reflection from light */
    static float materialShininess = 64.0f; /* 1 to 128, higher gives sharper highlight */

    /* Yay, lights! */
    switch(numLights) {
        case 7:
            glEnable(GL_LIGHT7);
            glLightfv(GL_LIGHT7, GL_POSITION, lightPosition7);
            glLightfv(GL_LIGHT7, GL_AMBIENT, lightAmbient);
            break;

        case 6:
            glEnable(GL_LIGHT6);
            glDisable(GL_LIGHT7);
            glLightfv(GL_LIGHT6, GL_POSITION, lightPosition6);
            glLightfv(GL_LIGHT6, GL_AMBIENT, lightAmbient);
            break;

        case 5:
            glEnable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
            glLightfv(GL_LIGHT5, GL_POSITION, lightPosition5);
            glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmbient);
            break;

        case 4:
            glEnable(GL_LIGHT4);
            glDisable(GL_LIGHT5);
            glLightfv(GL_LIGHT4, GL_POSITION, lightPosition4);
            glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient);
            break;

        case 3:
            glEnable(GL_LIGHT3);
            glDisable(GL_LIGHT4);
            glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);
            glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient);
            break;

        case 2:
            glEnable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
            glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
            glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
            break;

        case 1:
            glDisable(GL_LIGHT2);
            glEnable(GL_LIGHT1);
            glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
            //glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse1);
            break;

        case 0:
            glEnable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
            break;

        default:
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
            break;
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
  		  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
    }
}


void writeScreenText(char *buffer, Vec3f pos) {
    char *bufp;

    glRasterPos2i(pos.x, pos.y);
    for (bufp = buffer; *bufp; bufp++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    }
}

void generateArrays(int tess) {
    generateSpherePoints(tess);
    generateTorusPoints(tess);
    generateGridPoints(tess);
}


void reallocArrays(int tess) {
    allocateSphereArrays(tess);
    allocateTorusArrays(tess);
    allocateGridArrays(tess);

    generateArrays(tess);
}


/* Vec3f functions */
Vec3f newVec3f(float x, float y, float z) {
    Vec3f v;
    v.x = x;
    v.y = y;
    v.z = z;
        
    return v;
}

Vec3f addVec3f(Vec3f v1, Vec3f v2) {
    Vec3f v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    v.y = v1.x + v2.y;
    
    return v;
}
