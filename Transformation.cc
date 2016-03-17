#include "Transformation.h"
#include <cassert>
using namespace std;

Vec3 translate(Vec3 p, float tx, float ty, float tz)
{
    return Vec3(p.x + tx, p.y + ty, p.z + tz);
}

Vec3 rotate(Vec3 p, float angle, Vec3 axis)
{
    // calculate angles
    float theta = deg2rad(angle);
    float alpha = atan(axis.x / axis.y);
    float beta = atan(sqrt(pow(axis.x, 2) + pow(axis.y, 2)) / axis.z);

    // construct point matrix
    Mat P(4,1); P.set({p.x, p.y, p.z, 1});

    // construct composite transformation matrix
    Mat T1(4,4), T2(4,4), T3(4,4), T4(4,4), T5(4,4);
    T1.set({cos(alpha), -sin(alpha), 0, 0,
            sin(alpha), cos(alpha), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1});
    T2.set({1, 0, 0, 0,
            0, cos(beta), -sin(beta), 0,
            0, sin(beta), cos(beta), 0,
            0, 0, 0, 1});
    T3.set({cos(theta), -sin(theta), 0, 0,
            sin(theta), cos(theta), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1});
    T4.set({1, 0, 0, 0,
            0, cos(-beta), -sin(-beta), 0,
            0, sin(-beta), cos(-beta), 0,
            0, 0, 0, 1});
    T5.set({cos(-alpha), -sin(-alpha), 0, 0,
            sin(-alpha), cos(-alpha), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1});
    Mat T = T5*T4*T3*T2*T1;

    // get result matrix
    P = T*P;
    return Vec3(P(0), P(1), P(2));
}

Vec2 world_to_pixel(Vec3 p, Vec3 cam, Vec3 target,
        float win_width, float win_height, float angle_x)
{
    // calculate u, v, n vectors
    Vec3 n = (cam - target).normalize();
    Vec3 u = cross(Vec3(0,1,0), n).normalize();
    Vec3 v = cross(n, u).normalize();

    /*
    // DEBUG
    cout << "u = " << u << endl;
    cout << "v = " << v << endl;
    cout << "n = " << n << endl;
    cout << "u.v = " << dot(u,v) << endl;
    cout << "v.n = " << dot(v,n) << endl;
    cout << "n.u = " << dot(n,u) << endl;
    */

    // translate cam to origin
    p = translate(p, -cam.x, -cam.y, -cam.z);
    //cout << "Translated point = " << p << endl; // DEBUG

    // rotate to align the axes
    Mat R(4,4);
    R.set({u.x, u.y, u.z, 0,
           v.x, v.y, v.z, 0,
           n.x, n.y, n.z, 0,
           0, 0, 0, 1});
    //cout << "R = " << R << endl; // DEBUG
    Mat P(4,1); P.set({p.x, p.y, p.z, 1});
    //cout << "P = " << P << endl; // DEBUG;
    P = R * P;
    //cout << "RP = " << P << endl; // DEBUG
    p = {P(0), P(1), P(2)};

    // change to screen coordinates
    float aspect_ratio = static_cast<float>(win_width) / win_height;
    angle_x = deg2rad(angle_x);
    float angle_y = atan(tan(angle_x) / aspect_ratio);

    p.x = (1 + p.x/fabs(p.z*tan(angle_x/2))) * (win_width/2);
    p.y = (1 + p.y/fabs(p.z*tan(angle_y/2))) * (win_height/2);

    return Vec2(p.x, p.y, p.z);
}
