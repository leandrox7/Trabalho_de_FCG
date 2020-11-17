//relógio, atualização de velocidade e posição, talvez teste de colisão e controle de tamanho de passo por velocidade integrados a isso
//além disso controle de alguns efeitos ligados ao movimento (como esticar e espremer)

#include "engine/fisica.h"
#include "engine/objetos_base.h"

//relogio com dt (tempo do último "frame"), dt anterior, "hora" da última atualização do relógio e "média" de tempo dos últimos frames
gameClock_t clk;

std::vector<BaseGameObj*> physicsInstanceList;
std::vector<BaseCollidingObj*> collidingInstanceList;

int registerForPhysics(BaseGameObj* instance){
    physicsInstanceList.push_back(instance);
    return physicsInstanceList.size() - 1;
}

int registerForCollisions(BaseCollidingObj* instance){
    collidingInstanceList.push_back(instance);
    return collidingInstanceList.size() - 1;

}

float initClock(){

    clk.dt = 0;
    clk.lastDt = 0;
    clk.time = glfwGetTime();
    clk.runningAvarageDt = 0;

    return clk.time;
}

float tickClock(){

    clk.lastDt = clk.dt;
    float newTime = glfwGetTime();
    clk.dt = newTime - clk.time;
    clk.time = newTime;
    clk.runningAvarageDt = ((CLK_FRAMES_TO_AVARAGE-1)*clk.runningAvarageDt + clk.dt)/CLK_FRAMES_TO_AVARAGE;

    return clk.dt;
}

float getFPS(){
    return 1.0/clk.runningAvarageDt;
}

void physicsStep(){
    int listSize = physicsInstanceList.size();
    if (listSize > 0){
        for(int i = 0; i < listSize; i++) {
            //BaseGameObj* obj = (BaseGameObj*)instanceList.at(i);
            BaseGameObj* obj = physicsInstanceList.at(i);
            obj->physicsID = i;

            if(obj->physicsActive){

                obj->lastKinematicInfo = obj->kinematicInfo;

                if (norm(obj->kinematicInfo.acc))
                    stepMRUV(&obj->kinematicInfo);
                else
                    stepMRU(&obj->kinematicInfo);
                obj->updateTranslationMatrix();
            }
        }
    }
    collisionStep();
}

void collisionStep(){
    int listSize = collidingInstanceList.size();
    if (listSize > 0){
        for(int i = 0; i < listSize; i++) {
            BaseCollidingObj* obj = collidingInstanceList.at(i);
            obj->collisionID = i;
            //printf("collison stp! %i\n",(int)obj->chkCollisionsFromThis);
            if(obj->chkCollisionsFromThis) checkAndSolveCollisions(obj,i);
        }
    }
}

void checkAndSolveCollisions(BaseCollidingObj* obj, int index){
    int listSize = collidingInstanceList.size();
    for(int i = 0; i < listSize; i++) {
        if(i != index){ //não testa colisão consigo mesmo
            BaseCollidingObj* secondObj = collidingInstanceList.at(i);
            if( ((secondObj->chkCollisionsFromThis) && (i > index)) ||
                ((!secondObj->chkCollisionsFromThis) && (secondObj->chkCollisionsToThis)) ){
               chkAndSolveCollision(obj, secondObj);
            }
        }
    }
}

void chkAndSolveCollision(BaseCollidingObj* obj, BaseCollidingObj* obj2){

    glm::vec3 obj_min = glm::vec3(
                          obj->kinematicInfo.pos.x + obj->boundingBox.minimum.x,
                          obj->kinematicInfo.pos.y + obj->boundingBox.minimum.y,
                          obj->kinematicInfo.pos.z + obj->boundingBox.minimum.z
                        );

    glm::vec3 obj2_min = glm::vec3(
                          obj2->kinematicInfo.pos.x + obj2->boundingBox.minimum.x,
                          obj2->kinematicInfo.pos.y + obj2->boundingBox.minimum.y,
                          obj2->kinematicInfo.pos.z + obj2->boundingBox.minimum.z
                        );

    glm::vec3 obj_max = glm::vec3(
                          obj->kinematicInfo.pos.x + obj->boundingBox.maximum.x,
                          obj->kinematicInfo.pos.y + obj->boundingBox.maximum.y,
                          obj->kinematicInfo.pos.z + obj->boundingBox.maximum.z
                        );

    glm::vec3 obj2_max = glm::vec3(
                          obj2->kinematicInfo.pos.x + obj2->boundingBox.maximum.x,
                          obj2->kinematicInfo.pos.y + obj2->boundingBox.maximum.y,
                          obj2->kinematicInfo.pos.z + obj2->boundingBox.maximum.z
                        );

    glm::vec3 min_ef = glm::vec3(std::max(obj_min.x,obj2_min.x),std::max(obj_min.y,obj2_min.y),std::max(obj_min.z,obj2_min.z));
    glm::vec3 max_ef = glm::vec3(std::min(obj_max.x,obj2_max.x),std::min(obj_max.y,obj2_max.y),std::min(obj_max.z,obj2_max.z));

    if( (min_ef.x <= max_ef.x) && (min_ef.y <= max_ef.y) && (min_ef.z <= max_ef.z) ){
        obj->kinematicInfo.pos = obj->lastKinematicInfo.pos;
        //obj->kinematicInfo.vel = glm::vec3(0,0,0);
        //obj->kinematicInfo.acc = glm::vec3(0,0,0);
//        obj2->kinematicInfo.vel = glm::vec3(obj->kinematicInfo.vel.x,obj->kinematicInfo.vel.y,obj->kinematicInfo.vel.z);
//        obj2->kinematicInfo.acc = glm::vec3(0,0,0);

        obj->onCollision(obj2);
        obj2->onCollision(obj2);
    }
}

//pra mover objeto levando em conta aceleração. Usa método de velocity verlet, deve ser bom o suficiente
void stepMRUV(kinematicInfo_t* kinInfo){

    kinInfo->vel = kinInfo->vel + (clk.dt/2.0f)*kinInfo->acc;
    kinInfo->pos = kinInfo->pos + clk.dt*kinInfo->vel;
	//ATUALIZA ACELERACAO SE FOR O CASO (acho que não vai precisar pra gente)
	kinInfo->vel = kinInfo->vel + (clk.dt/2.0f)*kinInfo->acc;
}

//pra mover objeto de acordo com velocidade ignorando aceleração
void stepMRU(kinematicInfo_t* kinInfo){
    kinInfo->pos += clk.dt*kinInfo->vel;
}

float getMovementAngle(glm::vec3 vel, float oldAngleIfKnowZeroOtherwise){

    float newAngle = oldAngleIfKnowZeroOtherwise;

    if(vel.x == 0){
        if(vel.y > 0) newAngle = pi_f/2;
        if(vel.y < 0) newAngle = 3*pi_f/2;
    }
    else if (vel.y == 0) newAngle = (vel.x < 0)*(pi_f);
    else{
        float arcTan = atan(vel.y/vel.x);

        if(vel.y > 0 && vel.x > 0)
           newAngle =  arcTan;
        else if (vel.y > 0 && vel.x < 0)
           newAngle =  pi_f + arcTan;
        else if (vel.y < 0 && vel.x < 0)
           newAngle =  pi_f + arcTan;
        else if (vel.y < 0 && vel.x > 0)
           newAngle =  2*pi_f + arcTan;
    }

    return newAngle;
}


