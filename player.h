#ifndef PLAYER_H
#define PLAYER_H

#include "circle.h"
#include "bullet.h"
#include <iostream>
#include <GL/freeglut.h>
#include <string>
#include <list>

using namespace std;

class Player : public Circle
{
    protected:
        GLfloat gX;
        GLfloat gY;
        GLfloat gR;
        GLfloat ratio;
        GLfloat gThetaBody;
        GLfloat gThetaGun;
        GLfloat vel;
        GLfloat bodyRadius;

        GLfloat velTiro;
        GLfloat bulletRadius;
        GLfloat gThetaBullet;

        GLint legWidth;
        GLint legHeight;
        GLint legPosX;
        GLfloat deltaLeg;

        GLint gunPosX;
        GLint gunPosY;
        GLint gunWidth;
        GLint gunHeight;

        bool jumping = false;
        bool touching = false;
        bool touchingObstacle = false;
        bool onCircle = false;
        
        GLint windowRatio;
        GLint windowSize;

        list<Bullet> bulletList;

        GLint obstacleHeight;

    public:
        Circle *circle; // Black circle which the player is on
        Player(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id) : Circle(cx, cy, radius, fill, id)
        { 
            gX = 0; gY = 0; gR = 0; ratio = 1; gThetaBody = 0; gThetaGun = 0; gThetaBullet = 0;
            circle = NULL; 
            Player::set(cx, cy, radius, fill, id);
        };

        Player()
        { 
            gX = 0; gY = 0; gR = 0; ratio = 1; gThetaBody = 0; gThetaGun = 0; gThetaBullet = 0; RGB[0] = 0; RGB[1] = 0.5; RGB[2] = 0;
            circle = NULL; 
        }

        void set(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id);
        void set(GLfloat velTiro, GLfloat vel);

        void handleCollision(bool w, bool a, bool s, bool d);

        void draw();
        void drawBody();
        void drawGun(GLfloat x, GLfloat y);
        void drawArms(GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B);
        void drawLegs(GLfloat width, GLfloat height);
        void drawChar(GLfloat x, GLfloat y);
        void drawRect(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawBullets();

        void rotateGun(GLint mouseX, GLint mouseY);
        void rotate(GLfloat deltaX);
        void moveY(GLfloat deltaY);
        void shoot();
        void jump(GLfloat deltaR);

        void setTouching(bool isTouching);
        void setTouchingObstacle(bool isTouchingObstacle);
        bool isTouchingObstacle();
        void setOnCircle(bool isOn);
        bool isOnCircle();

        void handleCollision(bool state);

        void addToBulletList(Bullet bullet);
        void updateBullets();

        GLfloat getgX();
        GLfloat getgY();
        GLfloat getgR();
        GLfloat getGunHeight();

        void setwindowRatio(GLint windowRatio);
        void setWindowSize(GLint windowSize);

        void setgr(GLfloat r);

        bool isJumping();
        void setJumping(bool state);

        GLint getBulletListSize();
        GLfloat getBulletgX(GLint i);
        GLfloat getBulletgY(GLint i);
        GLfloat getBulletRadius(GLint i);
        void destroyBullet(GLint i);

        void setObstacleHeight(GLint height);
};

#endif
