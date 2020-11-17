///Objeto de teste com controle do jogador, esticamento e explosão. Herda das classes desses comportamentos e chama suas funções no step

#include "jogo/objeto_teste2.h"

GameObjTst2::GameObjTst2(glm::vec3 position, ObjModel* meshInfo, int bboxType):

    BaseCollidingObj(position,meshInfo,bboxType,true),
    VelocityFromInputBehaviour(&(this->maxSpeed), 0.33f, 1.0f, &(this->kinematicInfo)),
    ExplosionAnimationBehaviour(0.66f, 0.8f, 2.f, &model),
    StrechSquishFromMovementBehaviour(0.4f, &maxSpeed, &kinematicInfo, &model)
    {}

void GameObjTst2::step(){

    velocityFromInput();

    if(!exploding && g_actionKB[EXPLODE][STATE]) exploding = true;
    if(exploding) explode(&kinematicInfo.pos);

    stretchFromMovement();
}

void GameObjTst2::onCollision(BaseCollidingObj* colisor){

}
