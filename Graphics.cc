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
    setPixel(ROUND(p.x), ROUND(p.y), p.z, c, i);
}

void Window::setPixel(int x, int y, float z, Vec3 c, float i)
{
    if (x < 0 or x > width or y < 0 or y > height)
        return;

    // check z buffer
    if (z <= zbuffer[x*height + y] or z < farz or z > nearz)
        return;
    zbuffer[x*height + y] = z;

    glColor3f(c.x*i, c.y*i, c.z*i);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void Window::gnLine(Vec2 p1, Vec2 p2, Vec3 c)
{
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;

    float d = p1.z, del_d = p2.z - p1.z;
    float i = p1.i, del_i = p2.i - p1.i;

    float del_x = x2 - x1;
    float del_y = y2 - y1;
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

void Window::fillTriangle(Vec2 v1, Vec2 v2, Vec2 v3)
{
    // vec2 comparer
    struct Vec2Comparer{ bool operator()(Vec2 a, Vec2 b) { return a.y < b.y; } };
    vector<Vec2> v = {v1, v2, v3};
    sort(v.begin(), v.end(), Vec2Comparer());

    float y, x1, z1, i1, x2, z2, i2;
    for (y = v[0].y; y < v[1].y; y++) {
        if (v[1].y == v[0].y) break;
        x1 = v[0].x + (y - v[0].y) * (v[1].x - v[0].x) / (v[1].y - v[0].y);
        z1 = v[0].z + (y - v[0].y) * (v[1].z - v[0].z) / (v[1].y - v[0].y);
        i1 = v[0].i + (y - v[0].y) * (v[1].i - v[0].i) / (v[1].y - v[0].y);
        x2 = v[0].x + (y - v[0].y) * (v[2].x - v[0].x) / (v[2].y - v[0].y);
        z2 = v[0].z + (y - v[0].y) * (v[2].z - v[0].z) / (v[2].y - v[0].y);
        i2 = v[0].i + (y - v[0].y) * (v[2].i - v[0].i) / (v[2].y - v[0].y);
        gnLine(Vec2(x1,y,z1,i1), Vec2(x2,y,z2,i2), {1,1,0});
    }

    for (y = v[1].y; y <= v[2].y; y++) {
        if (v[2].y == v[1].y) break;
        x1 = v[1].x + (y - v[1].y) * (v[2].x - v[1].x) / (v[2].y - v[1].y);
        z1 = v[1].z + (y - v[1].y) * (v[2].z - v[1].z) / (v[2].y - v[1].y);
        i1 = v[1].i + (y - v[1].y) * (v[2].i - v[1].i) / (v[2].y - v[1].y);
        x2 = v[0].x + (y - v[0].y) * (v[2].x - v[0].x) / (v[2].y - v[0].y);
        z2 = v[0].z + (y - v[0].y) * (v[2].z - v[0].z) / (v[2].y - v[0].y);
        i2 = v[0].i + (y - v[0].y) * (v[2].i - v[0].i) / (v[2].y - v[0].y);
        gnLine(Vec2(x1,y,z1,i1), Vec2(x2,y,z2,i2), {1,1,0});
    }

    /*
    float delx, dely, delz, deli, step, x, y, z, i, dx, dy, dz, di;
    delx = v[2].x - v[1].x;
    dely = v[2].y - v[1].y;
    delz = v[2].z - v[1].z;
    deli = v[2].i - v[1].i;
    step = std::max(fabs(delx), fabs(dely));
    dx = delx / step;
    dy = dely / step;
    dz = delz / step;
    di = deli / step;
    x = v[1].x;
    y = v[1].y;
    z = v[1].z;
    i = v[1].i;
    for (int k = 0; k <= step; k++) {
        gnLine(v[0], Vec2(x,y,z,i));
        x += dx;
        y += dy;
        z += dz;
        i += di;
    }
    */
}

void Window::fillTriangleFlat(Vec2 v1, Vec2 v2, Vec2 v3, float i)
{
    // vec2 comparer
    struct Vec2Comparer{ bool operator()(Vec2 a, Vec2 b) { return a.y < b.y; } };
    vector<Vec2> v = {v1, v2, v3};
    sort(v.begin(), v.end(), Vec2Comparer());

    float y, x1, z1, x2, z2;
    for (y = v[0].y; y < v[1].y; y++) {
        if (v[1].y == v[0].y) break;
        x1 = v[0].x + (y - v[0].y) * (v[1].x - v[0].x) / (v[1].y - v[0].y);
        z1 = v[0].z + (y - v[0].y) * (v[1].z - v[0].z) / (v[1].y - v[0].y);
        x2 = v[0].x + (y - v[0].y) * (v[2].x - v[0].x) / (v[2].y - v[0].y);
        z2 = v[0].z + (y - v[0].y) * (v[2].z - v[0].z) / (v[2].y - v[0].y);
        gnLine(Vec2(x1,y,z1,i), Vec2(x2,y,z2,i), {1,1,0});
    }

    for (y = v[1].y; y <= v[2].y; y++) {
        if (v[2].y == v[1].y) break;
        x1 = v[1].x + (y - v[1].y) * (v[2].x - v[1].x) / (v[2].y - v[1].y);
        z1 = v[1].z + (y - v[1].y) * (v[2].z - v[1].z) / (v[2].y - v[1].y);
        x2 = v[0].x + (y - v[0].y) * (v[2].x - v[0].x) / (v[2].y - v[0].y);
        z2 = v[0].z + (y - v[0].y) * (v[2].z - v[0].z) / (v[2].y - v[0].y);
        gnLine(Vec2(x1,y,z1,i), Vec2(x2,y,z2,i), {1,1,0});
    }
}

void Window::wireframe(const Scene& scene, Vec3 camera, Vec3 target, float angle_x, float angle)
{
    // debug
    //cout << camera << " ==> " << target << endl;

    // project points to 2d
    vector<Vec2> vertices2d;
    for (unsigned long i = 0; i < scene.vertices.size(); i++) {
        // get the coordinate of vertex
        Vec3 point3d = scene.vertices[i];
        // rotate the vertex about world y-axis
        point3d = rotate_y(point3d, angle);
        // project
        Vec2 point2d = world_to_pixel(point3d, camera, target, width, height, angle_x);
        vertices2d.push_back(point2d);
    }

    // draw edges
    for (unsigned long i = 0; i < scene.faces.size(); i += 3) {
        // get the 3 vertex's index
        int index1 = scene.faces[i];
        int index2 = scene.faces[i+1];
        int index3 = scene.faces[i+2];

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

void Window::render(const Scene& scene, Vec3 camera, Vec3 target, Vec3 light,
        float angle_x, float angle)
{
    vector<Vec2> vertices2d(scene.vertices.size());
    vector<Vec3> vertices3d(scene.vertices.size());
    
    // project to 2d as well as find the intensities
    Vec3 point3d, u, v, n, L, N, R, H, V; Mat M(4,4), P(4,1); float d;
    for (unsigned long i = 0; i < scene.vertices.size(); i++) {
        point3d = scene.vertices[i];

        // rotate the point in world axis
        point3d = rotate_y(point3d, angle);

        // translate camera to origin
        point3d = translate(point3d, -camera.x, -camera.y, -camera.z);

        // calculate u,v,n vectors
        n = (camera - target).normalize();
        u = cross({0,1,0}, n).normalize();
        v = cross(n, u).normalize();

        // align camera axes to world axes
        M.set({u.x, u.y, u.z, 0,
               v.x, v.y, v.z, 0,
               n.x, n.y, n.z, 0,
               0, 0, 0, 1});
        P.set({point3d.x, point3d.y, point3d.z, 1});
        P = M*P;
        point3d = {P(0), P(1), P(2)}; // this is in camera coordinates

        // project to screen coordinates
        vertices2d[i] = project(point3d, width, height, angle_x);
        vertices3d[i] = point3d;
    }

    // calculate intensities
    for (unsigned long i = 0; i < vertices3d.size(); i++) {
        // calculate surface intensity
        N = scene.normals[i].normalize();
        L = (light - vertices3d[i]);
        d = L.mag();
        L = L.normalize();
        R = 2*dot(N,L)*N;
        V = (Vec3(0,0,0) - vertices3d[i]).normalize();
        H = (L + V).normalize();
        float intensity = 0.1 + 200*dot(N,L)/(d*d) + powf(dot(N,H), 256);
        if (intensity > 1) intensity = 1;
        vertices2d[i].i = intensity;
    }

    // now fill every triangle
    Vec2 v1, v2, v3;
    for (unsigned long i = 0; i < scene.faces.size(); i += 3) {
        // get vertices
        v1 = vertices2d[scene.faces[i]];
        v2 = vertices2d[scene.faces[i+1]];
        v3 = vertices2d[scene.faces[i+2]];

        // fill triangle
        fillTriangle(v1, v2, v3);
    }
}

void Window::renderFlat(const Scene& scene, Vec3 camera, Vec3 target, Vec3 light,
        float angle_x, float angle)
{
    vector<Vec2> vertices2d(scene.vertices.size());
    vector<Vec3> vertices3d(scene.vertices.size());
    
    // project to 2d as well as find the intensities
    Vec3 point3d, u, v, n, L, N; Mat M(4,4), P(4,1);
    for (unsigned long i = 0; i < scene.vertices.size(); i++) {
        point3d = scene.vertices[i];

        // rotate the point in world axis
        point3d = rotate_y(point3d, angle);

        // translate camera to origin
        point3d = translate(point3d, -camera.x, -camera.y, -camera.z);

        // calculate u,v,n vectors
        n = (camera - target).normalize();
        u = cross({0,1,0}, n).normalize();
        v = cross(n, u).normalize();

        // align camera axes to world axes
        M.set({u.x, u.y, u.z, 0,
               v.x, v.y, v.z, 0,
               n.x, n.y, n.z, 0,
               0, 0, 0, 1});
        P.set({point3d.x, point3d.y, point3d.z, 1});
        P = M*P;
        point3d = {P(0), P(1), P(2)}; // this is in camera coordinates

        // project to screen coordinates
        vertices2d[i] = project(point3d, width, height, angle_x);
        vertices3d[i] = point3d;
    }

    Vec3 R, V, H; float d;
    for (unsigned long i = 0; i < scene.faces.size(); i += 3) {
        // calculate surface normal
        Vec3 v1 = vertices3d[scene.faces[i]];
        Vec3 v2 = vertices3d[scene.faces[i+1]];
        Vec3 v3 = scene.vertices[scene.faces[i+2]];
        Vec3 A = (v2 - v1).normalize();
        Vec3 B = (v3 - v2).normalize();
        Vec3 N = cross(A,B).normalize();

        // calculate surface intensity
        Vec3 centroid = (v1 + v2 + v3)/3;
        Vec3 L = (light - centroid);
        d = L.mag();
        L = L.normalize();
        R = 2*dot(N,L)*N;
        V = (Vec3(0,0,0) - centroid).normalize();
        H = (L + V).normalize();
        float intensity = 0.1 + 200*dot(N,L)/(d*d) + powf(dot(N,H), 256);
        if (intensity > 1) intensity = 1;

        // get 2d vertices
        Vec2 p1 = vertices2d[scene.faces[i]];
        Vec2 p2 = vertices2d[scene.faces[i+1]];
        Vec2 p3 = vertices2d[scene.faces[i+2]];

        fillTriangleFlat(p1, p2, p3, intensity);
    }
}
