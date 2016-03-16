#include "Transformation.h"
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
    Mat W2V(4,4), T(4,4), R(4,4), P(4,1);

    // calculate U,V,N vectors
    Vec3 N = (cam - target).normalize();
    Vec3 V(0,1,0);
    Vec3 U = cross(V,N).normalize();

    // construct transformation matrix
    T.set({1, 0, 0, -cam.x,
           0, 1, 0, -cam.y,
           0, 0, 1, -cam.z,
           0, 0, 0, 1});
    R.set({U.x, U.y, U.z, 0,
           V.x, V.y, V.z, 0,
           N.x, N.y, N.z, 0,
           0, 0, 0, 1});
    W2V = R*T;

    // construct point matrix
    P.set({p.x, p.y, p.z, 1});

    // perform transformation
    P = W2V * P;
    p = Vec3(P(0), P(1), P(2)); // now p is in viewing coordinates

    // change to screen coordinates
    float aspect_ratio = (float) win_width / win_height;
    angle_x = deg2rad(angle_x);
    float angle_y = atan(tan(angle_x) / aspect_ratio);

    p.x = (1 + p.x/fabs(p.z * tan(angle_x/2))) * (win_width/2);
    p.y = (1 + p.y/fabs(p.z * tan(angle_y/2))) * (win_height/2);

    return Vec2(p.x, p.y, p.z);
}
