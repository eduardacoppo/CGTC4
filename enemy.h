#ifndef ENEMY_H_
#define ENEMY_H_

#include "player.h"
#include "bullet.h"
#include <iostream>
#include <GL/freeglut.h>
#include <string>
#include <list>

using namespace std;

class Enemy : public Player
{
    private:
        GLfloat freqTiro;
    
    public:
        Enemy(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id) : Player(cx, cy, radius, fill, id) { };
        void set(GLfloat freqTiro, GLfloat velTiro, GLfloat vel, GLint altura);
        void set(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id);
        void patrol();
};

#endif