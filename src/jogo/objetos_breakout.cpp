#include "jogo/objetos_breakout.h"

Parede::Parede(glm::vec3 position, ObjModel* meshInfo, int comprimento, int largura):
    BaseCollidingObj(position, meshInfo, BB_CUBICA, false){
        boundingBox.maximum.x *= largura;
        boundingBox.minimum.x *= largura;
        boundingBox.maximum.y *= comprimento;
        boundingBox.minimum.y *= comprimento;
        physicsActive = false;

        model = Matrix_Identity()*Matrix_Translate(position.x,position.y,position.z)*Matrix_Scale(largura,comprimento,1);
    };

Bloco::Bloco(glm::vec3 position, ObjModel* meshInfo):
    BaseCollidingObj(position, meshInfo, BB_CUBICA, false),
    ExplosionAnimationBehaviour(EXPLOSION_TIME, EXPLOSION_INIT_SCALE, EXPLOSION_MAX_SCALE, &model){
        damage = 0;
        physicsActive = false;

        rotationX = (pi_f/2.f)*(rand()%4);
        rotationY = (pi_f/2.f)*(rand()%4);
        rotationZ = (pi_f/2.f)*(rand()%4);

        model = Matrix_Identity()*Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z)
                *Matrix_Rotate_Z(rotationZ)*Matrix_Rotate_X(rotationX)*Matrix_Rotate_Y(rotationY);
}

void Bloco::explode(){

    glm::vec3 nullPos(0,0,0);
    ExplosionAnimationBehaviour::explode(&nullPos);

    model = Matrix_Identity()*Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z)
            *Matrix_Rotate_Z(rotationZ)*Matrix_Rotate_X(rotationX)*Matrix_Rotate_Y(rotationY)*model;
}

void Bloco::onCollision(BaseCollidingObj* colisor){

    if( (damage >= 2) && (!exploding) ) {
        damage++;
        exploding = true;
    }
    else{
        if(damage == 0){
            meshInfo_p = &modeloCuboQuebrando1;
            shapeName = meshInfo_p->shapes[0].name.c_str();
            damage = 1;
        }
        else{
            meshInfo_p = &modeloCuboQuebrando2;
            shapeName = meshInfo_p->shapes[0].name.c_str();
            damage = 2;
        }
    }
    if(exploding){
        chkCollisionsFromThis = false;
        chkCollisionsToThis = false;
        explode();
    }
}

void Bloco::endOfExplosion(){
    physicsActive = false;
    active = false;
    chkCollisionsFromThis = false;
    chkCollisionsToThis = false;
    alphaFactor = 0;
}

void Bloco::step(){
    if(exploding) explode();
}

Paleta::Paleta(glm::vec3 position, int directionFront, ObjModel* meshInfo):
    BaseCollidingObj(position, meshInfo, BB_CUBICA, true),
    VelocityFromInputBehaviour(&maxSpeed,0.75f,0.75f,&kinematicInfo),
    StrechSquishFromMovementBehaviour(0.2f, &maxSpeed, &kinematicInfo, &model){


    maxSpeed = 80.f;
    faceDirection = (pi_f/2.f)*directionFront;
    rotateBBoxToFaceFront(true);

    scalingAlongEachAxis.x = 4/3.f;
    boundingBox.maximum.x *= scalingAlongEachAxis.x;
    boundingBox.minimum.x *= scalingAlongEachAxis.x;
}

void Paleta::rotateBBoxToFaceFront(bool inverse){
    int sign = 1;
    if(inverse) sign = -1;

    float m00 = Matrix_Rotate_Z(sign*faceDirection)[0][0];
    float m01 = Matrix_Rotate_Z(sign*faceDirection)[0][1];
    float m10 = Matrix_Rotate_Z(sign*faceDirection)[1][0];
    float m11 = Matrix_Rotate_Z(sign*faceDirection)[1][1];

    float b_max_x = m00*boundingBox.maximum.x + m01*boundingBox.maximum.y;
    boundingBox.maximum.y = m10*boundingBox.maximum.x + m11*boundingBox.maximum.y;
    boundingBox.maximum.x = b_max_x;

    float b_min_x = m00*boundingBox.minimum.x + m01*boundingBox.minimum.y;
    boundingBox.minimum.y = m10*boundingBox.minimum.x + m11*boundingBox.minimum.y;
    boundingBox.minimum.x = b_min_x;

    b_min_x = std::min(boundingBox.minimum.x,boundingBox.maximum.x);
    boundingBox.maximum.x = std::max(boundingBox.minimum.x,boundingBox.maximum.x);
    boundingBox.minimum.x = b_min_x;

    float b_min_y = std::min(boundingBox.minimum.y,boundingBox.maximum.y);
    boundingBox.maximum.y = std::max(boundingBox.minimum.y,boundingBox.maximum.y);
    boundingBox.minimum.y = b_min_y;

    boundingBox.center.x = (boundingBox.maximum.x+boundingBox.minimum.x)/2;
    boundingBox.center.y = (boundingBox.maximum.y+boundingBox.minimum.y)/2;

//    model = Matrix_Translate(-kinematicInfo.pos.x,-kinematicInfo.pos.y,-kinematicInfo.pos.z)*model;
//    model = Matrix_Rotate_Z(sign*faceDirection)*model;
//    model = Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z)*model;
}

void Paleta::step(){

    velocityFromInput();

    modelRotationAroundZAxis = faceDirection;
    model = Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z)*Matrix_Scale(scalingAlongEachAxis.x,1,1)
    *Matrix_Translate(-kinematicInfo.pos.x,-kinematicInfo.pos.y,-kinematicInfo.pos.z)*model;

    stretchFromMovement();
}

void Paleta::onCollision(BaseCollidingObj* colisor){

    if(colisor->chkCollisionsFromThis || !strcmp(colisor->meshInfo_p->shapes[0].name.c_str(),"cubo") ){
        if (g_actionKB[LEFT][STATE] == REPEATING) g_actionKB[LEFT][STATE] = JUST_PRESSED;
        if (g_actionKB[RIGHT][STATE] == REPEATING) g_actionKB[RIGHT][STATE] = JUST_PRESSED;
        if (g_actionKB[UP][STATE] == REPEATING) g_actionKB[UP][STATE] = JUST_PRESSED;
        if (g_actionKB[DOWN][STATE] == REPEATING) g_actionKB[DOWN][STATE] = JUST_PRESSED;
    }
    kinematicInfo.vel.x = kinematicInfo.vel.y = kinematicInfo.vel.z = 0;

}

Bolinha::Bolinha(glm::vec3 position, glm::vec3 initialVel, ObjModel* meshInfo, float randomness):
    BaseCollidingObj(position, meshInfo, BB_ESFERICA, true),
    ExplosionAnimationBehaviour(0.45f,0.66f,2.5f,&model),
    StrechSquishFromMovementBehaviour(0.4f, &maxSpeed, &kinematicInfo, &model){

        randomSpeedAmplitude = randomness;

        kinematicInfo.vel = initialVel;

        float velChange_x = ((2*rand()/((float)RAND_MAX))-1)*10*randomSpeedAmplitude;
        float velChange_y = 100*randomSpeedAmplitude*randomSpeedAmplitude - velChange_x*velChange_x;
        velChange_y = sqrt(velChange_y);
        velChange_y *= (2*(rand()%2) - 1);

        kinematicInfo.vel.x =+ velChange_x;
        kinematicInfo.vel.y =+ velChange_y;

        kinematicInfo.vel.y = abs(kinematicInfo.vel.y);

        mortes = 0;
        grip = GRIP_DEFAULT;
    }

void Bolinha::onCollision(BaseCollidingObj* colisor){

    //primeiro se calcula uma mudança aleatória na direção da velocidade, pra bola não ficar presa e pra dar uma graça
    //float speed = norm(kinematicInfo.vel); //pra se precisar renormalizar por problemas de arredondamentou ou sei lá
    float angleChange = 1 - (2*rand())/(float)RAND_MAX;
    angleChange = angleChange/sqrt(abs(angleChange));

    float maxRandomAngle = pi_f/18;

    if(kinematicInfo.vel.y != 0)
        if (kinematicInfo.vel.x/kinematicInfo.vel.y > 3)
            maxRandomAngle *= 2;

    angleChange *= maxRandomAngle;

    float m00 = Matrix_Rotate_Z(angleChange)[0][0];
    float m01 = Matrix_Rotate_Z(angleChange)[0][1];
    float m10 = Matrix_Rotate_Z(angleChange)[1][0];
    float m11 = Matrix_Rotate_Z(angleChange)[1][1];

    float newVelX = m00*kinematicInfo.vel.x + m01*kinematicInfo.vel.y;
    kinematicInfo.vel.y = m10*kinematicInfo.vel.x + m11*kinematicInfo.vel.y;
    kinematicInfo.vel.x = newVelX;

    //se precisar renormalizar por problemas de arredondamentou ou sei lá
    //    float renormFactor = sqrt(norm(kinematicInfo.vel)/speed);
    //    kinematicInfo.vel.y *= renormFactor;
    //    kinematicInfo.vel.x *= renormFactor;

    //depois testamos em que região em relação ao outro objeto a bolinha tava
    bool colRight = false;
    bool colLeft = false;
    bool colUp = false;
    bool colDown = false;
    bool colBack = false;
    bool colFront = false;

    //separado para levar velocidade da paleta em conta (e tb pra fugir de um bug que não entendi ainda D,: asuhasu)
    if(colisor->chkCollisionsFromThis){
        colDown = false;
        colUp = true;

        colRight = false;
        colLeft = false;
        if(colisor->kinematicInfo.vel.x > 0) colRight = true;
        else if(colisor->kinematicInfo.vel.x < 0) colLeft = true;

        //pra compensar o "abs()" que vem pelo colUp == true
        if(kinematicInfo.vel.y > 0) kinematicInfo.vel.y += (grip/2.f)*clk.dt*colisor->kinematicInfo.vel.y;
        if(kinematicInfo.vel.y < 0) kinematicInfo.vel.y -= (grip/2.f)*clk.dt*colisor->kinematicInfo.vel.y;
        kinematicInfo.vel.x += grip*clk.dt*colisor->kinematicInfo.vel.x;
    }

    else{
        glm::vec3 ourMaxPos;
        ourMaxPos.x = boundingBox.maximum.x + kinematicInfo.pos.x;
        ourMaxPos.y = boundingBox.maximum.y + kinematicInfo.pos.y;
        ourMaxPos.z = boundingBox.maximum.z + kinematicInfo.pos.z;

        glm::vec3 ourMinPos;
        ourMinPos.x = boundingBox.minimum.x + kinematicInfo.pos.x;
        ourMinPos.y = boundingBox.minimum.y + kinematicInfo.pos.y;
        ourMinPos.z = boundingBox.minimum.z + kinematicInfo.pos.z;

        glm::vec3 otherMaxPos;
        otherMaxPos.x = colisor->boundingBox.maximum.x + colisor->kinematicInfo.pos.x;
        otherMaxPos.y = colisor->boundingBox.maximum.y + colisor->kinematicInfo.pos.y;
        otherMaxPos.z = colisor->boundingBox.maximum.z + colisor->kinematicInfo.pos.z;

        glm::vec3 otherMinPos;
        otherMinPos.x = colisor->boundingBox.minimum.x + colisor->kinematicInfo.pos.x;
        otherMinPos.y = colisor->boundingBox.minimum.y + colisor->kinematicInfo.pos.y;
        otherMinPos.z = colisor->boundingBox.minimum.z + colisor->kinematicInfo.pos.z;

        if( ourMaxPos.x >= otherMaxPos.x ) colRight = true;
        if( ourMinPos.x <= otherMinPos.x ) colLeft = true;
        if( ourMaxPos.y >= otherMaxPos.y ) colUp = true;
        if( ourMinPos.y <= otherMinPos.y ) colDown = true;
        if( ourMaxPos.z >= otherMaxPos.z ) colBack = true;
        if( ourMinPos.z <= otherMinPos.z ) colFront = true;
    }

    //aí deixamos as velocidades em cada direção sempre "pra fora", pra direção de colisão, fazendo uma "reflexão"
    if(colUp) kinematicInfo.vel.y = abs(kinematicInfo.vel.y);
    if(colDown) kinematicInfo.vel.y = -abs(kinematicInfo.vel.y);
    if(colRight) kinematicInfo.vel.x = abs(kinematicInfo.vel.x);
    if(colLeft) kinematicInfo.vel.x = -abs(kinematicInfo.vel.x);
    if(colFront) kinematicInfo.vel.z = -abs(kinematicInfo.vel.z);
    if(colBack) kinematicInfo.vel.z = abs(kinematicInfo.vel.z);
}

void Bolinha::step(){

    if(!exploding && kinematicInfo.pos.y < DEATH_ZONE_Y) exploding = true;
    if(exploding) explode(&kinematicInfo.pos);

    stretchFromMovement();

    if( (kinematicInfo.pos.x < -GAME_AREA_DIST_FROM_CENTER)
       || (kinematicInfo.pos.y < -GAME_AREA_DIST_FROM_CENTER)
       || (kinematicInfo.pos.x > GAME_AREA_DIST_FROM_CENTER)
       || (kinematicInfo.pos.y > GAME_AREA_DIST_FROM_CENTER) ){

        kinematicInfo.vel = glm::vec3(0,0,0);

        float velChange_x = ((2*rand()/((float)RAND_MAX))-1)*10*randomSpeedAmplitude;
        float velChange_y = 100*randomSpeedAmplitude*randomSpeedAmplitude - velChange_x*velChange_x;
        velChange_y = sqrt(velChange_y);
        velChange_y *= (2*(rand()%2) - 1);

        kinematicInfo.vel.x =+ velChange_x;
        kinematicInfo.vel.y =+ velChange_y;

        kinematicInfo.vel.y = abs(kinematicInfo.vel.y);
    }

    float speed = norm(kinematicInfo.vel);
    if (speed > maxSpeed){
        //printf("speed: %f (max: %f)... ",speed, maxSpeed);
        float factor = 1 - clk.dt*(1-sqrt(maxSpeed/speed));
        kinematicInfo.vel.x  *= factor;
        kinematicInfo.vel.y  *= factor;
        //printf("fact: %f... new speed: %f\n",factor,norm(kinematicInfo.vel));
    }

}

void Bolinha::endOfExplosion(){
    kinematicInfo.pos = glm::vec3(0,1,0);
    kinematicInfo.vel = glm::vec3(0,0,0);

    float velChange_x = ((2*rand()/((float)RAND_MAX))-1)*10*randomSpeedAmplitude;
    float velChange_y = 100*randomSpeedAmplitude*randomSpeedAmplitude - velChange_x*velChange_x;
    velChange_y = sqrt(velChange_y);
    velChange_y *= (2*(rand()%2) - 1);

    kinematicInfo.vel.x =+ velChange_x;
    kinematicInfo.vel.y =+ velChange_y;

    kinematicInfo.vel.y = abs(kinematicInfo.vel.y);

    g_mortes++;
}

Fundo::Fundo(glm::vec3 position, float largura, float comprimento): BaseGameObj(position,&modelPlane){

    physicsActive = false;
    model = Matrix_Identity()*Matrix_Translate(position.x,position.y,position.z)*Matrix_Scale(largura,comprimento,1)*Matrix_Rotate_X(pi_f/2);
};

