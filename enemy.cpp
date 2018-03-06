#include <GL/freeglut.h>
#include <cmath>
#include <list>
#include <stdlib.h>
#include <time.h>
#include "enemy.h"

void Enemy::set(GLfloat freqTiro, GLfloat velTiro, GLfloat vel, GLint altura)
{
    Player::set(velTiro, vel);
    this->freqTiro = freqTiro;
    this->obstacleHeight = obstacleHeight;
}

void Enemy::set(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id)
{
    Player::set(cx, cy, radius, fill, id);
}

void Enemy::patrol()
{
    if (touchingObstacle && !isJumping())
    {
        setJumping(true);
        return;
    }
    else if (!isJumping() && !touchingObstacle)
    {
        setgr(0);
        setJumping(false);
    }

    if (isJumping())
    {
        moveY(-1);
        return;
    }

    GLfloat rotation;

    srand(time(NULL));

    rotation = rand() % 2;

    if (rotation <= 0.5)
    {
        rotate(1);
        moveY(-1);
    }
    else
    {
        rotate(-1);
        moveY(-1);
    }
}
