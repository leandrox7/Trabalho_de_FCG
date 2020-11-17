#include "jogo/objeto_teste.h"

GameObjTst::GameObjTst(glm::vec3 position){

    //usados pela f�sica
    kinematicInfo.pos = position;
    kinematicInfo.vel = glm::vec3(0.0f,0.0f,0.0f);
    kinematicInfo.acc = glm::vec3(0.0f,0.0f,0.0f);

    //para moviemnto controlado pelo jogador
    maxSpeed = 75;
    initialSpeedFactor = 0.33f; //em propor��o ao m�ximo
    speedIncreaseFactor = 1.0f; //em propor��o ao m�ximo

    model = Matrix_Identity();

    //par�metros para "esticar e espremer" de acordo com o movimento
    maxStretch = 0.4; //pode aumentar em maxStretch*100% na dire��o do mivimento
    stretch = 1.0f; //1 -> normal. > 1 -> esticado; < 1 -> espremido
    movementAngle = 0;
    angleChangeFactor = 1; //0 -> �ngulo n�o mudou. 1 -> �ngulo mudou em 180�
    stretchEaseFactor = 0.75f; //fator de interpola��o (n�o linear) para esticamento
    angleEaseFactor = cbrt(stretchEaseFactor); //mesmo mas para a mudan�a de �ngulo no esticamento
    overSquish = 1.5f; //quando objeto muda de dire��o abruptamente, pode ENCONLHER por um fator overSquish - 1

    //controle da "anima��o" de explos�o (aumenta e fica transparente)
    exploding = false;
    timeAlreadySpentExploding = 0;
    alphaFactor = 1;
    scale = 1;
}

///esse m�todo � chamada a cada frame. O grosso da l�gica pr�pria dos objetos deve ser chamada daqui
void GameObjTst::step(){

    velocityFromInput();

    if(!exploding && g_actionKB[EXPLODE][STATE]) exploding = true;
    if(exploding) explode();

    stretchFromMovement();
}

void GameObjTst::updateTranslationMatrix(){ //colocar a f�sica para chamar isso?
    model = Matrix_Identity()*Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z);
}

void GameObjTst::explode(){
    //a raiz quadrada faz a exploz�o come�ar mais r�pido e "desacelerar" conforme cresce
    scale = SCALE_MIN + (SCALE_MAX-SCALE_MIN)*sqrt(timeAlreadySpentExploding/TIME_TO_EXPAND);
    //o fator alfa faz ficar mais transparente conforme cresce, sendo 100% opaco no in�cio da explos�o
    alphaFactor = 1 - (scale-SCALE_MIN)/(SCALE_MAX-SCALE_MIN);
    //a transpar�ncia deve acelerar com a expans�o (na minha cabe�a : p)
    alphaFactor *= alphaFactor;

    timeAlreadySpentExploding += clk.dt;

    //quando terminar de explodir, volta ao normal. Provavelmente n�o vai ser usado assim na pr�tica : p
    if (timeAlreadySpentExploding >= TIME_TO_EXPAND){
        timeAlreadySpentExploding = 0;
        exploding = false;
        alphaFactor = 1;
        scale = 1;
    }

    model *= Matrix_Scale(scale,scale,scale); //explos�o � igual em todas dire��es
}

//calcula as velocidades em x e y a partir do input e do tempo que passou
void GameObjTst::velocityFromInput(){

    speedFromDirectionsPressed();
    speedFromDirectionsRepeating();
    speedFromDirectionsJustReleased();

    float normSpeed = norm(glm::vec4(kinematicInfo.vel,0));

    if (normSpeed > maxSpeed) kinematicInfo.vel = (maxSpeed/normSpeed)* kinematicInfo.vel;
}

//assim que uma dire��o � apertada, d� uma velocidade "de arranque"
void GameObjTst::speedFromDirectionsPressed(){
    if (g_actionKB[LEFT][STATE] == JUST_PRESSED) kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(-1,0,0);
    if (g_actionKB[RIGHT][STATE] == JUST_PRESSED) kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(1,0,0);
    if (g_actionKB[UP][STATE] == JUST_PRESSED) kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(0,1,0);
    if (g_actionKB[DOWN][STATE] == JUST_PRESSED) kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(0,-1,0);
}

//se continua apertando uma dire��o, d� uma acelerada nela (isso deveria ser colocado como acelera��o mesmo?)
void GameObjTst::speedFromDirectionsRepeating(){
    if (g_actionKB[LEFT][STATE] == REPEATING) kinematicInfo.vel += clk.dt*speedIncreaseFactor*maxSpeed*glm::vec3(-1,0,0);
    if (g_actionKB[RIGHT][STATE] == REPEATING) kinematicInfo.vel += clk.dt*speedIncreaseFactor*maxSpeed*glm::vec3(1,0,0);
    if (g_actionKB[UP][STATE] == REPEATING) kinematicInfo.vel += clk.dt*speedIncreaseFactor*maxSpeed*glm::vec3(0,1,0);
    if (g_actionKB[DOWN][STATE] == REPEATING) kinematicInfo.vel += clk.dt*speedIncreaseFactor*maxSpeed*glm::vec3(0,-1,0);
}

//assim que solta uma dire��o, p�ra se tava indo pro lado dela, ou d� um "arranque" se tava indo pro sentido oposto
void GameObjTst::speedFromDirectionsJustReleased(){
    if (g_actionKB[LEFT][STATE] == JUST_RELEASED){
        kinematicInfo.vel.x = 0;
        if (g_actionKB[RIGHT][STATE] == REPEATING || g_actionKB[RIGHT][STATE] == JUST_PRESSED)
            kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(1,0,0);
    }
    if (g_actionKB[RIGHT][STATE] == JUST_RELEASED){
        kinematicInfo.vel.x = 0;
        if (g_actionKB[LEFT][STATE] == REPEATING || g_actionKB[LEFT][STATE] == JUST_PRESSED)
            kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(-1,0,0);
    }
    if (g_actionKB[UP][STATE] == JUST_RELEASED){
        kinematicInfo.vel.y = 0;
        if (g_actionKB[DOWN][STATE] == REPEATING || g_actionKB[DOWN][STATE] == JUST_PRESSED)
            kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(0,-1,0);
    }
    if (g_actionKB[DOWN][STATE] == JUST_RELEASED){
        kinematicInfo.vel.y = 0;
        if (g_actionKB[UP][STATE] == REPEATING || g_actionKB[UP][STATE] == JUST_PRESSED)
            kinematicInfo.vel += initialSpeedFactor*maxSpeed*glm::vec3(0,1,0);
    }
}

//faz a bolinha se esticar na dire��o do movimento e espremer na contr�ria, al�m de se contrair quando muda bruscamente dire��o
//eu fui viajando enquanto fazia, n�o sei qu�o entend�vel t�, vou ver melhor depois
void GameObjTst::stretchFromMovement(){

    //1) calcula angulo de movimento no frame atual (se n�o mudar, fica o anterior):
    float newAngle = movementAngle;

    if(kinematicInfo.vel.x == 0){
        if(kinematicInfo.vel.y > 0) newAngle = pi_f/2;
        if(kinematicInfo.vel.y < 0) newAngle = 3*pi_f/2;
    }
    else if (kinematicInfo.vel.y == 0) newAngle = (kinematicInfo.vel.x < 0)*(pi_f);
    else{
        float arcTan = atan(kinematicInfo.vel.y/kinematicInfo.vel.x);
        if(kinematicInfo.vel.y > 0 && kinematicInfo.vel.x > 0)
           newAngle =  arcTan;
        else if (kinematicInfo.vel.y > 0 && kinematicInfo.vel.x < 0)
           newAngle =  pi_f + arcTan;
        else if (kinematicInfo.vel.y < 0 && kinematicInfo.vel.x < 0)
           newAngle =  pi_f + arcTan;
        else if (kinematicInfo.vel.y < 0 && kinematicInfo.vel.x > 0)
           newAngle =  2*pi_f + arcTan;
    }

    //2) calcula altera��o de angulo
    float distanceAngles = abs(newAngle - movementAngle);
    movementAngle = newAngle;

    //3) Cria a fra��o de meia volta da mudan�a de �ngulo (indo de 0 a 1)
    if (distanceAngles > pi_f) distanceAngles = 2*pi_f - distanceAngles;
    distanceAngles = distanceAngles/pi_f;
    //a fra��o de meia volta � elevada ao cubo para o efeito ficar suficiente (0,1 -> 0,63; 0,5 -> 0,87; 0,9 -> 0,98)
    distanceAngles = 1-(1-distanceAngles)*(1-distanceAngles)*(1-distanceAngles)*(1-distanceAngles)*(1-distanceAngles);

    //4) calcula o fator de atenua��o nesse frame, levando em conta o quanto pode "espremer a mais" se mudan�a for abrupta:
    float newAngleChangeFactor = 1 - (overSquish)*distanceAngles; //valor < 0: vai espremer em vez de esticar

    //5) compara atenua��o nesse frame com a do anterior e escolhe a nova se for menor (parada) ou mistura elas
    if (newAngleChangeFactor < angleChangeFactor) angleChangeFactor = newAngleChangeFactor; //se "freiou", altera direto
    //interpola��o feita para que leve alguns frames at� se recuperar de uma mudan�a de dire��o:
    else angleChangeFactor = (angleEaseFactor)*angleChangeFactor + (1-angleEaseFactor)*newAngleChangeFactor;

    //6) calcula o quanto teria que esticar dada a velocidade, ignorando outros fatores
    float newStretch = norm(glm::vec4(kinematicInfo.vel,0))/maxSpeed;
    newStretch = sqrt(newStretch)*maxStretch; //raiz pra que comece mais r�pido o efeito, dando um "arranque"

    //7) calcula o esticamento final usando isso tudo:
    newStretch = 1 + newStretch*angleChangeFactor;
    if (newStretch < stretch) stretch = newStretch; //se "encolheu", altera direto
    //interpola��o feita para que leve alguns frames pra ir esticando se arrancar:
    else stretch = (stretchEaseFactor)*stretch + (1-stretchEaseFactor)*newStretch;

    //8) o que esticar numa dire��o, espreme na oposta. Ideia � manter "�rea" do objeto
    ///no momento sup�e movimento em s� um plano com c�mera ortogr�fica perpendicular!
    float squish = 1.0f/stretch;

    //9) faz uma transforma��o de semelhan�a para orientar o esticamento no sentido do movimento
    model *= Matrix_Rotate_Z(movementAngle);
    model *= Matrix_Scale(stretch,squish,1);
    model *= Matrix_Rotate_Z(-movementAngle);

    //printf("da: %f, st: %f, ang: %f, angFac: %f\n",distanceAngles,stretch,angle,angleChangeFactor);
}
