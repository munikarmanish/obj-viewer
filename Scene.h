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
    Vertex(Vec3 position = {0,0,0}, Vec3 normal = {0,0,0}): p(position), n(normal) {}
};

// edge is used in scan-filling algorithm
class Edge {
public: // data
    Vec2 *u, *v;

public: // constructor
    Edge(Vec2 *v1=NULL, Vec2 *v2=NULL): u(v1), v(v2)
    {
        // make sure u->y <= v->y
        if (u->y > v->y)
            std::swap(u, v);
    }
};

// a scene is a collection of vertices, edges, normals, and faces
class Scene {
public: // data
    std::vector<Vec3> normals;
    std::vector<Vec2> textures;
    std::vector<Vec3> faces;
    std::vector<Vertex> vertices;

public: // file manipulators
    void load(std::string filename);
    void save(std::string filename) const;

public:
    void print() const;
    void calculateNormal();
};

#endif // SCENE_H
