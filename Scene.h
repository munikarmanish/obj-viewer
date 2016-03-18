#ifndef SCENE_H
#define SCENE_H
#include "Math.h"
#include "Transformation.h"
#include <algorithm>
#include <string>

// 3D vertex
class Vertex {
public: // data
    Vec3 p, n; // position and normal

public: // constructor
    Vertex(Vec3 position = {0,0,0}): p(position), n(0,0,0) {}
};

// edge is used in scan-filling algorithm
class Edge {
public: // data
    Vec2 *u, *v;

public: // constructor
    Edge() = default;
    Edge(Vec2 *v1, Vec2 *v2): u(v1), v(v2)
    {
        // make sure u->y <= v->y
        if (u->y > v->y)
            std::swap(u, v);
    }

    float length() const { return (Vec3(u->x, u->y, u->z) - Vec3(v->x, v->y, v->z)).mag(); }
    float dely() const { return v->y - u->y; }
    float delx() const { return v->x - u->x; }
    float delz() const { return v->z - u->z; }
    float m() const { return dely()/delx(); }
    float interpolate_x(float y) const;
    float interpolate_z(float y) const;
    float interpolate_i(float y) const;
};

// a scene is a collection of vertices, edges, normals, and faces
class Scene {
public: // data
    std::vector<Vec3> normals;
    //std::vector<Vec2> textures;
    std::vector<int> faces;
    std::vector<Vec3> vertices;

public: // file manipulators
    void load(std::string filename);
    void save(std::string filename) const;

public:
    void print() const;
    void calculateNormal();
};

#endif // SCENE_H
