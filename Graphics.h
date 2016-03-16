#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Math.h"
#include <vector>

class Window {
public:
    int width, height;
    float nearz, farz;
    std::vector<float> zbuffer;

public:
    Window(int w=640, int h=480, float nearz=-1, float farz=-100);

public:
    void clear();
    void refresh();
    void start();

    void setPixel(Vec2 p, Vec3 c={1,1,1}, float i=1);
    void setPixel(int x, int y, float d, Vec3 c={1,1,1}, float i=1);

    void stLine(Vec2 p1, Vec2 p2, Vec3 c={1,1,1});
    void gnLine(Vec2 p1, Vec2 p2, Vec3 c={1,1,1});
};

#endif // GRAPHICS