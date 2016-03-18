#include "Math.h"
#include <iostream>
#include <GL/glut.h>
#include "Scene.h"
#include "Graphics.h"
using namespace std;

// global variables
Vec3 CAMERA(0,3,7), TARGET(0,0,0), LIGHT(10,10,10);
Scene SCENE;
float FIELD_OF_VIEW = 45, ANGLE = 0;
Window WINDOW;

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

int main(int argc, char** argv)
{
    SCENE.load(string(argv[1]));
    //SCENE.print();
    
    WINDOW = Window(800, 600, -1, -200);

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
    //cout << "Camera = " << CAMERA << ", Target = " << TARGET << ", Light = " << LIGHT << endl;
    WINDOW.clear();
    //WINDOW.wireframe(SCENE, CAMERA, TARGET, FIELD_OF_VIEW, ANGLE);
    WINDOW.render(SCENE, CAMERA, TARGET, LIGHT, FIELD_OF_VIEW, ANGLE);
    //WINDOW.renderFlat(SCENE, CAMERA, TARGET, LIGHT, FIELD_OF_VIEW, ANGLE);
    //WINDOW.fillTriangle(Vec2(200,200,-5), Vec2(500,100,-20, 0.5), Vec2(600,400,-5, 0.1));
    WINDOW.refresh();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) exit(EXIT_SUCCESS);

    else if (key == 'w') CAMERA.z--;
    else if (key == 's') CAMERA.z++;
    else if (key == 'a') CAMERA.x--;
    else if (key == 'd') CAMERA.x++;
    else if (key == 'q') CAMERA.y--;
    else if (key == 'e') CAMERA.y++;

    else if (key == 'i') LIGHT.z--;
    else if (key == 'k') LIGHT.z++;
    else if (key == 'j') LIGHT.x--;
    else if (key == 'l') LIGHT.x++;
    else if (key == 'u') LIGHT.y--;
    else if (key == 'o') LIGHT.y++;

    else if (key == 't') TARGET.z--;
    else if (key == 'g') TARGET.z++;
    else if (key == 'f') TARGET.x--;
    else if (key == 'h') TARGET.x++;
    else if (key == 'r') TARGET.y--;
    else if (key == 'y') TARGET.y++;

    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        ANGLE -= 5;
    else if (key == GLUT_KEY_RIGHT) 
        ANGLE += 5;

    glutPostRedisplay();
}

void reshape(int w, int h)
{
    WINDOW.width = w;
    WINDOW.height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW.width, 0, WINDOW.height);
}
