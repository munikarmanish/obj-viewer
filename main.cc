#include "Math.h"
#include <iostream>
#include <GL/glut.h>
#include "Scene.h"
#include "Graphics.h"
using namespace std;

// global variables
Vec3 CAMERA(0,0,5);
Vec3 TARGET(0,0,0);
Scene SCENE;
float FIELD_OF_VIEW = 45;
float ANGLE = 0;
Window WINDOW;

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

int main(int argc, char** argv)
{
    SCENE.load(argv[1]);
    SCENE.print();
    WINDOW = Window(600, 600);

    // initialize GL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(WINDOW.width, WINDOW.height);
    glutCreateWindow("Manish Munikar");

    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW.width, 0, WINDOW.height);

    glutDisplayFunc(display);
    //glutIdleFunc(display);
    //glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    
    WINDOW.start();
}

void display()
{
    WINDOW.clear();
    //WINDOW.gnLine({400,100,0,1}, {100,400,0,0}, {1,0,0});
    WINDOW.wireframe(SCENE, CAMERA, TARGET, FIELD_OF_VIEW, ANGLE);
    WINDOW.refresh();
}

//void reshape(int w, int h);

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) exit(EXIT_SUCCESS);

    else if (key == 'w') CAMERA.z--;
    else if (key == 's') CAMERA.z++;
    else if (key == 'a') CAMERA.x--;
    else if (key == 'd') CAMERA.x++;
    else if (key == 'q') CAMERA.y--;
    else if (key == 'e') CAMERA.y++;

    else if (key == 'i') TARGET.z--;
    else if (key == 'k') TARGET.z++;
    else if (key == 'j') TARGET.x--;
    else if (key == 'l') TARGET.x++;
    else if (key == 'u') TARGET.y--;
    else if (key == 'o') TARGET.y++;
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)   ANGLE--;
    else if (key == GLUT_KEY_RIGHT)  ANGLE++;

    glutPostRedisplay();
}
