#include <GL/freeglut.h>
#include <list>
#include <cmath>
#include <string.h>
#include "arena.h"

using namespace std;

// Add to lists
void Arena::addToCircleList(Circle circle)
{
    circleList.push_back(circle);
    
    return;
}

void Arena::addToEnemyList(Enemy enemy)
{
    enemy.set(enemyFreqTiro, enemyVelTiro, enemyVel, obstacleHeight);
    enemyList.push_back(enemy);

    return;
}

void Arena::addToObstacleList(Obstacle obstacle)
{
    obstacle.set(obstacleHeight);
    obstacleList.push_back(obstacle);

    return;
}

// Add outer and inner circle
void Arena::addOuterCircle(Circle circle)
{
    this->outerCircle = circle;
}

void Arena::addInnerCircle(Circle circle)
{
    this->innerCircle = circle;
}

void Arena::setObstacleHeight(GLint height)
{
    this->obstacleHeight = height;
}

// Add player
void Arena::addPlayer(Player player)
{
    this->player = player;
}

// Enemy
void Arena::enemyShots()
{
    for (Enemy &it : enemyList)
    {
        it.shoot();
    }
}

void Arena::enemyMovement()
{
    for (Enemy &it : enemyList)
    {
        it.patrol();
    }
}

void Arena::setEnemyAtts(GLfloat freqTiro, GLfloat velTiro, GLfloat vel)
{
    this->enemyFreqTiro = freqTiro;
    this->enemyVelTiro = velTiro;
    this->enemyVel = vel;
}

// Draw
void Arena::drawArena()
{
    outerCircle.draw();
    innerCircle.draw();

    for (Obstacle &it : obstacleList)
    {
        it.draw();
    }

    for (Enemy &it : enemyList)
    {
        it.draw();
    }
}

void Arena::drawText()
{
    char *string = (char*)"Enemies: ";
    glColor3d(0.0, 1.0, 0.0);
    glRasterPos2d(600, 300);
    int i;
	int len = strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
    glRasterPos2d(600, 320);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, enemyList.size());
}

// Circle landing
void Arena::checkOnCircle(Player* player)
{
    if (player->circle != NULL)
	{
		float dx = player->getgX() - player->circle->getcx();
		float dy = player->getgY() - player->circle->getcy();

		float d = sqrt(dx*dx + dy*dy);

		if (d >= player->getRadius() + player->circle->getRadius())
		{
            player->circle = NULL;
            player->setOnCircle(false);
        }
        else
        {
            player->setOnCircle(true);
        }
    }
    else if (player->circle == NULL && !player->isJumping())
    {
        player->setgr(0);
    }
}

void Arena::checkIfLandedOnCircle(Player *player)
{
    GLfloat leastDistance = 1000;
    
    for (Obstacle &it : obstacleList)
    {
        float distanceX = player->getgX() - it.getcx();
        float distanceY = player->getgY() - it.getcy();

        float distance = sqrt(distanceX*distanceX + distanceY*distanceY);

        if (distance < leastDistance)
        {
            leastDistance = distance;
        }

        if ( ( distance < ( player->getRadius() + it.getRadius() ) ) && distance == leastDistance )
        {
            player->circle = &it;
        }
    }
}

// Collision
void Arena::bulletCollision(Player *unit)
{
    GLint blsize = unit->getBulletListSize();
    GLint elsize = getEnemyListSize();

    GLfloat distanceX;
    GLfloat distanceY;
    GLfloat distance;

    bool touching;
    
    for (GLint i = 0; i < blsize; i++)
    {
        GLfloat bulletgX = unit->getBulletgX(i);
        GLfloat bulletgY = unit->getBulletgY(i);

        GLfloat bulletRadius = unit->getBulletRadius(i);

        if (unit->getFill() == "green") // player
        {
            // check if shot down an enemy
            for (GLint e = 0; e < elsize; e++)
            {
                GLfloat ex = getEnemygX(e);
                GLfloat ey = getEnemygY(e);

                distanceX = bulletgX - ex;
                distanceY = bulletgY - ey;
        
                distance = sqrt(distanceX*distanceX + distanceY*distanceY);
        
                touching = distance < (bulletRadius + getEnemyRadius(e));

                if (touching)
                {
                    unit->destroyBullet(i);
                    destroyEnemy(e);
                    if (enemyList.size() == 0)
                    {
                        playerWins = true;
                    }
                    return;
                }
            }
        }
        else // check if shot down player
        {
            GLfloat playerX = player.getgX();
            GLfloat playerY = player.getgY();

            distanceX = bulletgX - playerX;
            distanceY = bulletgY - playerY;

            distance = sqrt(distanceX*distanceX + distanceY*distanceY);
            
            touching = distance < (bulletRadius + player.getRadius());

            if (touching)
            {
                unit->destroyBullet(i);
                destroyPlayer();
                return;
            }
        }

        for (Circle &it : circleList)
        {
            GLfloat circlecx = it.getcx();
            GLfloat circlecy = it.getcy();
    
            distanceX = bulletgX - circlecx;
            distanceY = bulletgY - circlecy;
    
            distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    
            touching = distance < (bulletRadius + it.getRadius());
    
            if (touching && it.getFill() != "blue")
            {
                unit->destroyBullet(i);
                return;
            }
            else if (it.getFill() == "blue")
            {
                // Distance from bullet to circle border
                distanceX = bulletgX - circlecx;
                distanceY = bulletgY - circlecy;
    
                distance = sqrt(distanceX*distanceX + distanceY*distanceY);
                distance += bulletRadius;
    
                if (distance > it.getRadius())
                {
                    unit->destroyBullet(i);
                    return;
                }
            }
        }

        for (Obstacle &it : obstacleList)
        {
            GLfloat circlecx = it.getcx();
            GLfloat circlecy = it.getcy();
    
            distanceX = bulletgX - circlecx;
            distanceY = bulletgY - circlecy;
    
            distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    
            touching = distance < (bulletRadius + it.getRadius());
    
            if (touching)
            {
                unit->destroyBullet(i);
                return;
            }
        }
    }
}

void Arena::checkEnemyBulletCollision()
{
    for (Enemy &enemy : enemyList)
    {
        bulletCollision(&enemy);
    }
}

void Arena::checkBulletCollision(Player* player)
{
    bulletCollision(player);
}

void Arena::checkEnemyCollision()
{
    for (Enemy &enemy : enemyList)
    {
        checkCollision(&enemy);
    }
}

void Arena::checkCollision(Player *unit)
{  
    // Unit coordinates
    GLfloat unitgX = unit->getgX();
    GLfloat unitgY = unit->getgY();

    // Circle coordinates
    GLfloat circlecx;
    GLfloat circlecy;

    // Distances
    GLfloat distanceX;
    GLfloat distanceY;
    GLfloat distance;

    // Checks if unit is touching some circle
    bool touching;

    // Check obstacle collision
    for (Obstacle &it : obstacleList)
    {
        circlecx = it.getcx();
        circlecy = it.getcy();

        distanceX = unitgX - circlecx;
        distanceY = unitgY - circlecy;

        distance = sqrt(distanceX*distanceX + distanceY*distanceY);

        touching = distance < (unit->getRadius() + it.getRadius());

        if ( touching && unit->getgR() < obstacleHeight/10 )
        {
            unit->setTouchingObstacle(true);
            unit->handleCollision(touching);
            return;
        }
        else if ( touching && unit->getgR() >= obstacleHeight/10 )
        {
            unit->setTouchingObstacle(true);
            unit->handleCollision(false);
        }
        else
        {
            unit->setTouchingObstacle(false);
        }
    }

    // Check collision with enemy
    if (unit->getFill() == "green")
    {
        GLfloat playergX = unit->getgX();
        GLfloat playergY = unit->getgY();

        for (Enemy &it : enemyList)
        {
            GLfloat enemygX = it.getgX();
            GLfloat enemygY = it.getgY();
    
            distanceX = playergX - enemygX;
            distanceY = playergY - enemygY;
    
            distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    
            bool touching = distance < (unit->getRadius() + it.getRadius());
    
            if (touching)
            {
                unit->handleCollision(touching);
                return;
            }
        }
    }
    else
    {
        GLfloat playergX = player.getgX();
        GLfloat playergY = player.getgY();

        GLfloat enemygX = unit->getgX();
        GLfloat enemygY = unit->getgY();

        distanceX = playergX - enemygX;
        distanceY = playergY - enemygY;

        distance = sqrt(distanceX*distanceX + distanceY*distanceY);

        bool touching = distance < (unit->getRadius() + player.getRadius());

        if (touching)
        {
            unit->handleCollision(touching);
            return;
        }
    }

    // Check collision with outer circle
    circlecx = outerCircle.getcx();
    circlecy = outerCircle.getcy();

    distanceX = unitgX - circlecx;
    distanceY = unitgY - circlecy;

    distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    distance += unit->getRadius();

    if (distance >= outerCircle.getRadius())
    {
        unit->handleCollision(true);
        return;
    }

    // Check collision with inner circle
    circlecx = innerCircle.getcx();
    circlecy = innerCircle.getcy();

    distanceX = unitgX - circlecx;
    distanceY = unitgY - circlecy;

    distance = sqrt(distanceX*distanceX + distanceY*distanceY);

    touching = distance < (unit->getRadius() + innerCircle.getRadius());

    if (touching)
    {
        unit->handleCollision(touching);
        return;
    }

    // If it did not collide
    else
    {
        unit->handleCollision(false);
    }
}

// Getters
GLfloat Arena::getEnemyFreqTiro()
{
    return this->enemyFreqTiro;
}

GLfloat Arena::getRadius(string fill)
{
    for (Circle &it : circleList)
    {
        if (it.getFill() == fill)
        {
            return it.getRadius();
        }
    }
}

GLint Arena::getcx(string fill)
{
    for (Circle &it : circleList)
    {
        if (it.getFill() == fill)
        {
            return it.getcx();
        }
    }
}

GLint Arena::getcy(string fill)
{
    for (Circle &it : circleList)
    {
        if (it.getFill() == fill)
        {
            return it.getcy();
        }
    }
}

GLint Arena::getObstacleHeight()
{
    return this->obstacleHeight;
}

GLint Arena::getEnemyListSize()
{
    return this->enemyList.size();
}

GLfloat Arena::getEnemygX(GLint index)
{
    auto enemy = enemyList.begin();
    std::advance(enemy, index);

    return enemy->getgX();
}

GLfloat Arena::getEnemygY(GLint index)
{
    auto enemy = enemyList.begin();
    std::advance(enemy, index);

    return enemy->getgY();
}

GLfloat Arena::getEnemyRadius(GLint index)
{
    auto enemy = enemyList.begin();
    std::advance(enemy, index);

    return enemy->getRadius();
}

void Arena::destroyEnemy(GLint index)
{
    auto enemy = enemyList.begin();
    std::advance(enemy, index);

    enemyList.erase(enemy);
}

void Arena::destroyPlayer()
{
    this->drawPlayer = false;
    gameOver = true;
}

bool Arena::isGameOver()
{
    return this->gameOver;
}

bool Arena::isPlayerWins()
{
    return this->playerWins;
}

bool Arena::isDrawPlayer()
{
    return this->drawPlayer;
}
