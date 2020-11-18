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

directionalLight_t light;

void initLight(){

    light.direction = glm::vec3(-0.7071,0,-0.7071);
    light.duracaoDia = 60;
    light.duracaoAno = 3.23606*light.duracaoDia;
    light.theta = 5*pi_f/4;
    light.latitude = 0; //0 sul, 1 norte
}

float getFPS(){
    return 1.0/clk.runningAvarageDt;
}

void physicsStep(){
    if(clk.dt <= MAX_FRAMETIME){
        int listSize = physicsInstanceList.size();
        if (listSize > 0){
            for(int i = 0; i < listSize; i++) {
                //BaseGameObj* obj = (BaseGameObj*)instanceList.at(i);
                BaseGameObj* obj = physicsInstanceList.at(i);
                obj->physicsID = i;

                if(obj->physicsActive){
                    obj->lastKinematicInfo = obj->kinematicInfo;
                    int steps = ceil(std::max(abs(obj->kinematicInfo.vel.x),abs(obj->kinematicInfo.vel.y))*clk.dt);
                    if (steps < 1) steps = 1;
                    float dt = clk.dt/steps;
                    for(int i = 0; i < steps; i++){

                        if (norm(obj->kinematicInfo.acc) > 0)
                            stepMRUV(&obj->kinematicInfo,dt);
                        else
                            stepMRU(&obj->kinematicInfo,dt);
                    }
                    obj->updateTranslationMatrix();
                }
            }
        }
        collisionStep();
        stepLight();
    }
}

void stepLight(){

    light.theta += (2*pi_f*clk.dt/light.duracaoDia)*(3-2*sin(light.theta))/5;
    if (light.theta > 2*pi_f) light.theta -= 2*pi_f;
    light.latitude += clk.dt/light.duracaoAno;
    if( light.latitude > 1) light.latitude -= 1;

    light.direction.x = cos(light.theta);
    light.direction.y = 2*(light.latitude-0.5);
    light.direction.z = sin(light.theta)*sin(pi_f*(light.latitude));

    float norma = norm(light.direction);
    light.direction.x /= norma;
    light.direction.y /= norma;
    light.direction.z /= norma;

    float xFactor = abs(cos(light.theta));
    float zFactor = (sin(light.theta)+1)/2;
    float yFactor = 2*(light.latitude - 0.5);



    //quero que tudo fique alto pra quando z alto, x baixo e y pequeno, mas azulado se não for bem no meio

    float composto = (1-xFactor)*zFactor*(1-abs(yFactor));

    light.RGB.r = light.RGB.g = composto*composto;
    light.RGB.b = sqrt(composto);

    //agora se x é grande, quero vermelho
    light.RGB.r += sqrt(xFactor)/(1+2*(1-xFactor));
    //já quando y tá grande esverdeia
    light.RGB.g += abs(yFactor)/(1+2*(1-abs(yFactor)));
    //já se y é negativo, mais azul, mas se sol n mto baixo
    light.RGB.b += (yFactor<0)*abs(yFactor)*(zFactor>0)*sqrt(zFactor);
    if(light.RGB.b>1) light.RGB.b = 1;

    //por fim, z baixo baixa tudo
    composto = (zFactor<0)*(1-abs(zFactor)) + (zFactor>0)*zFactor;
    light.RGB.r *= composto;

    //printf("theta: %f, lat: %f, r: %f, g: %f, b: %f\n",light.theta,light.latitude,light.RGB.r,light.RGB.g,light.RGB.b);
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

        obj2->onCollision(obj);
        obj->onCollision(obj2);
    }
}

//pra mover objeto levando em conta aceleração. Usa método de velocity verlet, deve ser bom o suficiente
void stepMRUV(kinematicInfo_t* kinInfo, float dt){

    kinInfo->vel = kinInfo->vel + (dt/2.0f)*kinInfo->acc;
    kinInfo->pos = kinInfo->pos + dt*kinInfo->vel;
	//ATUALIZA ACELERACAO SE FOR O CASO (acho que não vai precisar pra gente)
	kinInfo->vel = kinInfo->vel + (dt/2.0f)*kinInfo->acc;
}

//pra mover objeto de acordo com velocidade ignorando aceleração
void stepMRU(kinematicInfo_t* kinInfo,float dt){
    kinInfo->pos += dt*kinInfo->vel;
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


