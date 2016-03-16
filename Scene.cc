#include "Scene.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include "Math.h"
using namespace std;

void Scene::load(string filename)
{
    normals.clear();
    vertices.clear();
    textures.clear();
    faces.clear();

    ifstream objfile(filename);
    if (!objfile) throw runtime_error("unable to open the object file");

    string line, keyword;
    while (getline(objfile, line)) {
        if (line.size() < 8) continue;
        istringstream linestream(line);
        linestream >> keyword;

        if (keyword == "v") {
            Vec3 v;
            linestream >> v.x >> v.y >> v.z;
            vertices.push_back(Vertex(v));
        }

        else if (keyword == "vt") {
            Vec2 t;
            linestream >> t.x >> t.y;
            textures.push_back(t);
        }

        else if (keyword == "vn") {
            Vec3 n;
            linestream >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }

        else if (keyword == "f") {
            Vec3 v[3];
            // determine type (v / v+n / v+t+n )
            if (line.find("/") == string::npos) { // v only
                for (int i = 0; i < 3; i++) {
                    linestream >> v[i].x;
                    faces.push_back(v[i]);
                }
            }

            else if (line.find("//") != string::npos) { // v+n
                for (int i = 0; i < 3; i++) {
                    linestream >> v[i].x;
                    linestream.get(); linestream.get(); // eat up the "//" string
                    linestream >> v[i].z;
                    faces.push_back(v[i]);
                }
            }

            else { // v+t+n
                for (int i = 0; i < 3; i++) {
                    linestream >> v[i].x; linestream.get();
                    linestream >> v[i].y; linestream.get();
                    linestream >> v[i].z;
                    faces.push_back(v[i]);
                }
            }
        }
    }

    objfile.close();
}

void Scene::print() const
{
    // print vertices
    for (auto v : vertices) {
        cout << "v " << v.p.x << " " << v.p.y << " " << v.p.z << endl;
    }

    // print textures
    for (auto t : textures) {
        cout << "vt " << t.x << " " << t.y << endl;
    }

    // print normals
    for (auto n : normals) {
        cout << "vn " << n.x << " " << n.y << " " << n.z << endl;
    }

    // print faces
    for (int i = 0; i < faces.size(); i += 3) {
        cout << "f ";
        cout << faces[i].x << "/" << faces[i].y << "/" << faces[i].z << " ";
        cout << faces[i+1].x << "/" << faces[i+1].y << "/" << faces[i+1].z << " ";
        cout << faces[i+2].x << "/" << faces[i+2].y << "/" << faces[i+2].z << " ";
        cout << endl;
    }
}

void Scene::calculateNormal()
{
    vector<int> vcount(vertices.size(), 0);
    for (int i = 0; i < faces.size(); i += 3) {
        // get the 3 indices
        int k1 = faces[i].x - 1;
        int k2 = faces[i+1].x - 1;
        int k3 = faces[i+2].x - 1;

        // get the 3 vertices of the of face
        Vec3 v1 = vertices[k1].p;
        Vec3 v2 = vertices[k2].p;
        Vec3 v3 = vertices[k3].p;

        // calculate the normal of the face
        Vec3 A = (v2 - v1).normalize();
        Vec3 B = (v3 - v2).normalize();
        Vec3 N = cross(A,B).normalize();

        vertices[k1].n = (N + vertices[k1].n).normalize();
        vertices[k2].n = (N + vertices[k2].n).normalize();
        vertices[k3].n = (N + vertices[k3].n).normalize();

        vcount[k1]++; vcount[k2]++; vcount[k3]++;

        for (int i = 0; i < vertices.size(); i++) {
            vertices[i].n = (vertices[i].n / vcount[i]).normalize();
        }
    }
}
