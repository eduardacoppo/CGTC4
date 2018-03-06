#ifndef ARENA_H
#define ARENA_H

#include <iostream>
#include <GL/freeglut.h>
#include <string>
#include <list>
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "obstacle.h"

using namespace std;

class Circle;
class Arena
{
    private:
        list<Circle> circleList; // Only has blue circle and white circle in it
        list<Obstacle> obstacleList; // Black circles
        
        // Blue and white circle respectively (I plan on eliminating the circleList)
        Circle outerCircle;
        Circle innerCircle;

        // Player
        Player player;

        // Enemy properties
        GLfloat enemyFreqTiro;
        GLfloat enemyVelTiro;
        GLfloat enemyVel;

        // Obstacle property
        GLint obstacleHeight;

        bool gameOver = false;
        bool playerWins = false;
        bool drawPlayer = true;

    public:
        list<Enemy> enemyList; // Red circles
        Arena() { }
        
        // add to list
        void addToCircleList(Circle circle);
        void addToEnemyList(Enemy enemy);
        void addToObstacleList(Obstacle obstacle);

        // add outer and inner circle
        void addOuterCircle(Circle circle);
        void addInnerCircle(Circle circle);

        // add player
        void addPlayer(Player player);

        // draw
        void drawArena();
        void drawText();

        // Collision functions (too many!)
        void checkCollision(Player *player);
        void checkBulletCollision(Player *player);
        void checkEnemyBulletCollision();
        void bulletCollision(Player *unit);
        void checkEnemyCollision();
        void checkCollisionWithObstacle(Player *unit);
        void checkCollisionWithOuterCircle(Player* unit);
        void checkCollisionWithInnerCircle(Player *unit);

        // Get circle properties per fill
        GLfloat getRadius(string fill);
        GLint getcx(string fill);
        GLint getcy(string fill);

        void checkOnCircle(Player *player);
        void checkIfLandedOnCircle(Player* player);

        void setEnemyAtts(GLfloat freqTiro, GLfloat velTiro, GLfloat vel);
        void setObstacleHeight(GLint height);

        void enemyShots();
        void enemyMovement();

        GLint getObstacleHeight();
        GLfloat getEnemyFreqTiro();

        GLint getEnemyListSize();
        GLfloat getEnemygX(GLint index);
        GLfloat getEnemygY(GLint index);
        GLfloat getEnemyRadius(GLint index);
        void destroyEnemy(GLint index);
        void destroyPlayer();

        bool isGameOver();
        bool isPlayerWins();
        bool isDrawPlayer();
};

#endif
