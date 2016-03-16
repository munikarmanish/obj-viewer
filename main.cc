#include "Math.h"
#include <iostream>
#include <GL/glut.h>
#include "Scene.h"
#include "Graphics.h"
using namespace std;

// global variables
Vec3 CAMERA(0,0,10), TARGET(0,0,0);
float ANGLE;
Scene SCENE;
Window WINDOW;

void render();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

int main(int argc, char** argv)
{
    SCENE.load("karuwa.obj");
    WINDOW = Window(800, 600);

    // initialize GL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(WINDOW.width, WINDOW.height);
    glutCreateWindow("Manish Munikar");

    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW.width, 0, WINDOW.height);

    glutDisplayFunc(render);
    //glutIdleFunc(render);
    //glutDisplayFunc(wireframe);
    //glutIdleFunc(wireframe);
    //glutReshapeFunc(reshape);
    //glutKeyboardFunc(keyboard);
    //glutSpecialFunc(special);
    
    WINDOW.start();
}

void render()
{
    WINDOW.clear();
    WINDOW.gnLine({400,100,0,1}, {100,400,0,0}, {1,0,0});
    WINDOW.refresh();
}

//void reshape(int w, int h);
//void keyboard(unsigned char key, int x, int y);
//void special(int key, int x, int y);
