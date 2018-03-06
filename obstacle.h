#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <GL/freeglut.h>
#include <string>
#include "circle.h"

using namespace std;

class Obstacle : public Circle
{
    private:
        GLint height;

    public:
        Obstacle(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id) : Circle(cx, cy, radius, fill, id) { }
        void set(GLint obstacleHeight);
};

#endif
