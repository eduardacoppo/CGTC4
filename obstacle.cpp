#include <GL/freeglut.h>
#include <cmath>
#include "obstacle.h"

using namespace std;

void Obstacle::set(GLint obstacleHeight)
{
    this->height = obstacleHeight;
}

void Obstacle::draw()
{
    glColor3dv(0.0, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(cx, cy, 0);
        for(i = 0; i <= this->50; ++i)
        {
            glVertex3d(
                cx + (this->radius * cos(i * M_PI / 50)), 
                cy + (this->radius * sin(i * M_PI / 50)),
                0
            );
        }
    glEnd();
}
