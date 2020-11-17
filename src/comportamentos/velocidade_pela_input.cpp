#include "engine/comportamentos.h"

VelocityFromInputBehaviour::VelocityFromInputBehaviour
    (float* maximumSpeedPointer, float initialSpeedFraction, float speedIncreaseFraction, kinematicInfo_t *velInputKinematicInfo_pointer){

    velInputMaxSpeed_p = maximumSpeedPointer;
    initialSpeedFactor = initialSpeedFraction;
    speedIncreaseFactor = speedIncreaseFraction;

    velInputKinematicInfo_p = velInputKinematicInfo_pointer;

    acceptingInput = true;
}

//calcula as velocidades em x e y a partir do input e do tempo que passou
void VelocityFromInputBehaviour::velocityFromInput(){

    speedFromDirectionsPressed();
    speedFromDirectionsRepeating();
    speedFromDirectionsJustReleased();

    float normSpeed = norm(glm::vec4(velInputKinematicInfo_p->vel,0));

    if (normSpeed > (*velInputMaxSpeed_p)) velInputKinematicInfo_p->vel = ((*velInputMaxSpeed_p)/normSpeed)* velInputKinematicInfo_p->vel;
}

//assim que uma direção é apertada, dá uma velocidade "de arranque"
void VelocityFromInputBehaviour::speedFromDirectionsPressed(){
    if (g_actionKB[LEFT][STATE] == JUST_PRESSED) velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(-1,0,0);
    if (g_actionKB[RIGHT][STATE] == JUST_PRESSED) velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(1,0,0);
    if (g_actionKB[UP][STATE] == JUST_PRESSED) velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(0,1,0);
    if (g_actionKB[DOWN][STATE] == JUST_PRESSED) velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(0,-1,0);
}

//se continua apertando uma direção, dá uma acelerada nela (isso deveria ser colocado como aceleração mesmo?)
void VelocityFromInputBehaviour::speedFromDirectionsRepeating(){
    if (g_actionKB[LEFT][STATE] == REPEATING) velInputKinematicInfo_p->vel += clk.dt*speedIncreaseFactor*(*velInputMaxSpeed_p)*glm::vec3(-1,0,0);
    if (g_actionKB[RIGHT][STATE] == REPEATING) velInputKinematicInfo_p->vel += clk.dt*speedIncreaseFactor*(*velInputMaxSpeed_p)*glm::vec3(1,0,0);
    if (g_actionKB[UP][STATE] == REPEATING) velInputKinematicInfo_p->vel += clk.dt*speedIncreaseFactor*(*velInputMaxSpeed_p)*glm::vec3(0,1,0);
    if (g_actionKB[DOWN][STATE] == REPEATING) velInputKinematicInfo_p->vel += clk.dt*speedIncreaseFactor*(*velInputMaxSpeed_p)*glm::vec3(0,-1,0);
}

//assim que solta uma direção, pára se tava indo pro lado dela, ou dá um "arranque" se tava indo pro sentido oposto
void VelocityFromInputBehaviour::speedFromDirectionsJustReleased(){
    if (g_actionKB[LEFT][STATE] == JUST_RELEASED){
        velInputKinematicInfo_p->vel.x = 0;
        if (g_actionKB[RIGHT][STATE] == REPEATING || g_actionKB[RIGHT][STATE] == JUST_PRESSED)
            velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(1,0,0);
    }
    if (g_actionKB[RIGHT][STATE] == JUST_RELEASED){
        velInputKinematicInfo_p->vel.x = 0;
        if (g_actionKB[LEFT][STATE] == REPEATING || g_actionKB[LEFT][STATE] == JUST_PRESSED)
            velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(-1,0,0);
    }
    if (g_actionKB[UP][STATE] == JUST_RELEASED){
        velInputKinematicInfo_p->vel.y = 0;
        if (g_actionKB[DOWN][STATE] == REPEATING || g_actionKB[DOWN][STATE] == JUST_PRESSED)
            velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(0,-1,0);
    }
    if (g_actionKB[DOWN][STATE] == JUST_RELEASED){
        velInputKinematicInfo_p->vel.y = 0;
        if (g_actionKB[UP][STATE] == REPEATING || g_actionKB[UP][STATE] == JUST_PRESSED)
            velInputKinematicInfo_p->vel += initialSpeedFactor*(*velInputMaxSpeed_p)*glm::vec3(0,1,0);
    }
}

