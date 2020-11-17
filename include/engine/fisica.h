#ifndef _FISICA_H
#define _FISICA_H

#include <GLFW/glfw3.h>
#include "util/matrices.h"
#include <vector>
#include  <algorithm>

#define CLK_FRAMES_TO_AVARAGE 10.0f

class BaseGameObj;
class BaseCollidingObj;

typedef struct {

    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;

} kinematicInfo_t;

#define BB_ESFERICA 0
#define BB_CUBICA 1
#define BB_PLANA 2
typedef struct {

    glm::vec4 minimum;
    glm::vec4 maximum;
    glm::vec4 center;
    float radius;
    int type;

} boundingBox_t;

typedef struct {

    float dt;
    float lastDt;
    float time;
    float runningAvarageDt;

} gameClock_t;

extern gameClock_t clk;

float initClock(); //retorna tempo atual
float tickClock(); //retorna dt, tempo que passou desde o último "tick"
float getFPS();
int registerForPhysics(BaseGameObj* instance);
int registerForCollisions(BaseCollidingObj* instance);
void physicsStep();
void collisionStep();
void checkAndSolveCollisions(BaseCollidingObj* obj, int index);
void chkAndSolveCollision(BaseCollidingObj* obj, BaseCollidingObj* secondObj);
float getMovementAngle(glm::vec3 vel, float oldAngleIfKnowZeroOtherwise);

void stepMRUV(kinematicInfo_t* kinInfo); //pra mover objeto levando em conta aceleração. Usa método de velocity verlet, deve ser bom o suficiente
void stepMRU(kinematicInfo_t* kinInfo); //pra mover objeto de acordo com velocidade ignorando aceleração

#endif  //_FISICA_H
