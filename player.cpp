#include <GL/freeglut.h>
#include <cmath>
#include <list>
#include <stdlib.h>
#include "player.h"

void Player::set(GLfloat cx, GLfloat cy, GLfloat radius, string fill, GLint id)
{
    this->gX = cx;
    this->gY = cy;
    this->radius = radius;
    this->fill = fill;
    this->id = id;

    this->bodyRadius = this->radius/2;

    this->legWidth = this->radius/4;
    this->legHeight = this->radius;
    this->legPosX = this->radius/4;
    this->deltaLeg = 1;

    this->gunWidth = this->radius/4;
    this->gunHeight = this->radius;

    this->gunPosX = this->radius/1.25;
    this->gunPosY = 0;

    this->bulletRadius = this->radius/7;
}

void Player::set(GLfloat velTiro, GLfloat vel)
{
    this->velTiro = velTiro*10;
    this->vel = vel*10;
}

void Player::drawBody()
{
    glColor3f(RGB[0], RGB[1], RGB[2]);
    glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < 360; i++)
        {
            float rad = i*(M_PI/180.0);
            glVertex2f(cos(rad)*(bodyRadius), sin(rad)*(bodyRadius));
        }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float rad = i*(M_PI/180.0);
            glVertex2f(cos(rad)*(bodyRadius), sin(rad)*(bodyRadius));
        }
    glEnd();
}

void Player::drawArms(GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
        GLfloat radiusX = this->radius;
        GLfloat radiusY = 0.20*this->radius;

        for (int i = 0; i < 360; i++)
        {
            float rad = i*(M_PI/180.0);
            glVertex2f(cos(rad)*radiusX + x, sin(rad)*radiusY + y);
        }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float rad = i*(M_PI/180.0);
            glVertex2f(cos(rad)*radiusX + x, sin(rad)*radiusY + y);
        }
    glEnd();
}

void Player::drawRect(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_QUADS);
        glVertex3f(-width/2, 0.0, 0.0);
        glVertex3f(-width/2, height, 0.0);
        glVertex3f(width/2, height, 0.0);
        glVertex3f(width/2, 0.0, 0.0);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
        glVertex3f(-width/2, 0.0, 0.0);
        glVertex3f(-width/2, height, 0.0);
        glVertex3f(width/2, height, 0.0);
        glVertex3f(width/2, 0.0, 0.0);
    glEnd();
}

void Player::drawLegs(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(-x, y, 0);
        drawRect(legWidth, -legHeight, 0, 0, 0);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(x, y, 0);
        drawRect(legWidth, legHeight, 0, 0, 0);
    glPopMatrix();
}

void Player::drawGun(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(gThetaGun, 0, 0, 1);
        drawRect(gunWidth, -gunHeight, RGB[0], RGB[1], RGB[2]);
    glPopMatrix();
}

void Player::drawBullets()
{
    if (bulletList.empty())
    {
        return;
    }

    for (Bullet &it : bulletList)
    {
        it.drawBullet();
        it.updateBullet();
    }
}

void Player::draw()
{
    drawChar(this->gX, this->gY);
}

void Player::drawChar(GLfloat x, GLfloat y)
{
    drawBullets();

    glPushMatrix();
        glTranslatef(x, y, 0.0);
        glRotatef(gThetaBody, 0, 0, 1);
        glScalef(ratio, ratio, 0);
        drawGun(gunPosX, 0);        
        drawArms(0, 0, RGB[0], RGB[1], RGB[2]);
        drawLegs(legPosX, 0);
        drawBody();
    glPopMatrix();
}

void Player::rotateGun(GLint mouseX, GLint mouseY)
{
    gThetaGun = atan2(mouseX + windowRatio - gX, mouseY + windowRatio - gY)*(180.0/M_PI) + 180 + gThetaBody;
    gThetaGun *= -1;

    if (gThetaGun <= -180)
    {
        gThetaGun += 360;
    }

    if (gThetaGun <= -45)
    {
        gThetaGun = -45;
    }
    else if (gThetaGun >= 45)
    {
        gThetaGun = 45;
    }
}

void Player::jump(GLfloat deltaR)
{   
    
}

void Player::setObstacleHeight(GLint height)
{
    this->obstacleHeight = height;
}

void Player::shoot()
{
    if (this->jumping || this->onCircle)
    {
        return;
    }

    Bullet newBullet(bulletRadius, gX, gY, gThetaBody + gThetaGun, gunPosX - gunWidth, gunPosY - gunHeight, velTiro);
    this->addToBulletList(newBullet);

    return;
}

void Player::addToBulletList(Bullet bullet)
{
    bulletList.push_back(bullet);
    
    return;
}

void Player::rotate(GLfloat deltaX)
{
    if (touching)
    {
        return;
    }

    gThetaBody += deltaX*360/(4*M_PI*this->radius);

    // When player angle is bigger than 360
    // Loops back to 0
    if (abs(gThetaBody) >= 360)
    {
        gThetaBody = 0;
    }

    return;
}

void Player::moveY(GLfloat deltaY)
{
    if (touching)
    {
        gX -= vel*-2*deltaY*sin(gThetaBody*(M_PI/180.0));
        gY += vel*-2*deltaY*cos(gThetaBody*(M_PI/180.0));
    }
    else
    {
        gX -= vel*deltaY*sin(gThetaBody*(M_PI/180.0));
        gY += vel*deltaY*cos(gThetaBody*(M_PI/180.0));
    }

    if (legHeight == radius || legHeight == -radius)
    {
        deltaLeg *= -1;
    }

    legHeight += deltaLeg;

    return;
}

void Player::handleCollision(bool state)
{
    setTouching(state);
}

GLfloat Player::getgX()
{
    return this->gX;
}

GLfloat Player::getgY()
{
    return this->gY;
}

void Player::setTouching(bool isTouching)
{
    this->touching = isTouching;
}

void Player::setTouchingObstacle(bool state)
{
    this->touchingObstacle = state;
}

bool Player::isTouchingObstacle()
{
    return this->touchingObstacle;
}

void Player::setwindowRatio(GLint windowRatio)
{
    this->windowRatio = windowRatio;
}

void Player::setWindowSize(GLint windowSize)
{
    this->windowSize = windowSize;
}

GLfloat Player::getGunHeight()
{
    return this->gunHeight;
}

GLfloat Player::getgR()
{
    return this->gR;
}

void Player::setgr(GLfloat r)
{
    this->gR = r;
    this->ratio = (this->getRadius() + gR) / this->getRadius();
}

bool Player::isJumping()
{
    return this->jumping;
}

void Player::setOnCircle(bool isOn)
{
    this->onCircle = isOn;
}

bool Player::isOnCircle()
{
    return this->onCircle;
}

void Player::setJumping(bool state)
{
    this->jumping = state;
}

GLint Player::getBulletListSize()
{
    return this->bulletList.size();
}

GLfloat Player::getBulletgX(GLint i)
{
    auto bullet = bulletList.begin();
    std::advance(bullet, i);

    return bullet->getgX();
}

GLfloat Player::getBulletgY(GLint i)
{
    auto bullet = bulletList.begin();
    std::advance(bullet, i);
    
    return bullet->getgY();
}

GLfloat Player::getBulletRadius(GLint i)
{
    auto bullet = bulletList.begin();
    std::advance(bullet, i);
    
    return bullet->getRadius();
}

void Player::destroyBullet(GLint i)
{
    auto bullet = bulletList.begin();
    std::advance(bullet, i);

    bulletList.erase(bullet);
}
