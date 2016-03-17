#include "Graphics.h"
#include <GL/glut.h>
using namespace std;

Window::Window(int w, int h, float nearz, float farz):
    width(w), height(h), nearz(nearz), farz(farz)
{
    // initialize the zbuffer
    zbuffer = vector<float>(w*h, farz-1);
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
    zbuffer = vector<float>(width*height, farz-1);
}

void Window::refresh()
{
    glutSwapBuffers();
}

void Window::start()
{
    glutMainLoop();
}

void Window::setPixel(Vec2 p, Vec3 c, float i)
{
    int x = ROUND(p.x);
    int y = ROUND(p.y);

    // check boundary
    if (x < 0 or x > width or y < 0 or y > height)
        return;

    // check z buffer
    if (p.z <= zbuffer[height*x + y] or p.z < farz or p.z > nearz)
        return;
    zbuffer[height*x + y] = p.z;

    glColor3f(c.x*i, c.y*i, c.z*i);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void Window::setPixel(int x, int y, float d, Vec3 c, float i)
{
    Vec2 p(x,y,d);
    setPixel(p, c, i);
}

void Window::stLine(Vec2 p1, Vec2 p2, Vec3 c)
{
    if (p1.x > p2.x) std::swap(p1, p2);

    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x; //y2 = p2.y;

    float del_x = x2 - x1;
    //float del_y = y2 - y1;
    float del_d = p2.z - p1.z;
    float del_i = p2.i - p1.i;

    float d = p1.z;
    float i = p1.i;

    for (int x = ROUND(x1), y = y1; x <= ROUND(x2); x++) {
        setPixel(x, ROUND(y), d, c, i);
        d += del_d/del_x;
        i += del_i/del_x;
    }
}

void Window::gnLine(Vec2 p1, Vec2 p2, Vec3 c)
{
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;

    float d = p1.z, del_d = p2.z - p1.z;
    float i = p1.i, del_i = p2.i - p1.i;

    float del_x = x2 - x1; float del_y = y2 - y1;
    int step = (abs(del_x) > abs(del_y))? abs(del_x) : abs(del_y);

    float x = x1, y = y1;
    for (int k = 0; k <= step; k++) {
        setPixel(ROUND(x), ROUND(y), d, c, i);

        x += del_x / step;
        y += del_y / step;
        d += del_d / step;
        i += del_i / step;
    }
}

void Window::wireframe(Scene scene, Vec3 camera, Vec3 target, float angle_x, float angle)
{
    // debug
    //cout << camera << " ==> " << target << endl;
    
    // project points to 2d
    vector<Vec2> vertices2d;
    for (unsigned long i = 0; i < scene.vertices.size(); i++) {
        // get the coordinate of vertex
        Vec3 point3d = scene.vertices[i].p;
        // rotate the vertex about world y-axis
        point3d = rotate(point3d, angle, {0,1,0});
        // project
        Vec2 point2d = world_to_pixel(point3d, camera, target, width, height, angle_x);
        //cout << point3d << " ==> " << point2d << endl; // DEBUG
        vertices2d.push_back(point2d);
    }

    // draw edges
    for (unsigned long i = 0; i < scene.faces.size(); i += 3) {
        // get the 3 vertex's index
        int index1 = scene.faces[i].x - 1;
        int index2 = scene.faces[i+1].x - 1;
        int index3 = scene.faces[i+2].x - 1;

        // get the vertices
        Vec2 p1 = vertices2d[index1];
        Vec2 p2 = vertices2d[index2];
        Vec2 p3 = vertices2d[index3];

        // draw
        gnLine(p1, p2);
        gnLine(p2, p3);
        gnLine(p3, p1);
    }
}
