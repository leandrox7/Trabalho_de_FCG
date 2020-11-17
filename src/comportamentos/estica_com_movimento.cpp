 #include "engine/comportamentos.h"

StrechSquishFromMovementBehaviour::StrechSquishFromMovementBehaviour
    (float maximumStretch, float* maxSpeedPointer, kinematicInfo_t* kinematicInfoPointer, glm::mat4* modelPointer){

    //parâmetros para "esticar e espremer" de acordo com o movimento
    maxStretch = maximumStretch; //pode aumentar em maxStretch*100% na direção do mivimento
    ssMaxSpeed_p = maxSpeedPointer;

    overSquish = 1.5f; //quando objeto muda de direção abruptamente, pode ENCONLHER por um fator overSquish - 1

    movementAngle = 0;
    angleChangeFactor = 1; //0 -> ângulo não mudou. 1 -> ângulo mudou em 180º

    stretchEaseFactor = 0.75f; //fator de interpolação (não linear) para esticamento
    angleEaseFactor = cbrt(stretchEaseFactor); //mesmo mas para a mudança de ângulo no esticamento

    stretch = 1.0f; //1 -> normal. > 1 -> esticado; < 1 -> espremido

    ssModel_p = modelPointer;
    ssKinematicInfo_p = kinematicInfoPointer;
}

//faz a bolinha se esticar na direção do movimento e espremer na contrária, além de se contrair quando muda bruscamente direção
//eu fui viajando enquanto fazia, não sei quão entendível tá, vou ver melhor depois
void   StrechSquishFromMovementBehaviour::stretchFromMovement(){

    //1) calcula angulo de movimento no frame atual (se não mudar, fica o anterior):
    float newAngle = getMovementAngle(ssKinematicInfo_p->vel, movementAngle);

    //2) calcula alteração de angulo
    float distanceAngles = abs(newAngle - movementAngle);
    movementAngle = newAngle;

    //3) Cria a fração de meia volta da mudança de ângulo (indo de 0 a 1)
    if (distanceAngles > pi_f) distanceAngles = 2*pi_f - distanceAngles;
    distanceAngles = distanceAngles/pi_f;
    //a fração de meia volta é elevada ao cubo para o efeito ficar suficiente (0,1 -> 0,63; 0,5 -> 0,87; 0,9 -> 0,98)
    distanceAngles = 1-(1-distanceAngles)*(1-distanceAngles)*(1-distanceAngles)*(1-distanceAngles)*(1-distanceAngles);

    //4) calcula o fator de atenuação nesse frame, levando em conta o quanto pode "espremer a mais" se mudança for abrupta:
    float newAngleChangeFactor = 1 - (overSquish)*distanceAngles; //valor < 0: vai espremer em vez de esticar

    //5) compara atenuação nesse frame com a do anterior e escolhe a nova se for menor (parada) ou mistura elas
    if (newAngleChangeFactor < angleChangeFactor) angleChangeFactor = newAngleChangeFactor; //se "freiou", altera direto
    //interpolação feita para que leve alguns frames até se recuperar de uma mudança de direção:
    else angleChangeFactor = (angleEaseFactor)*angleChangeFactor + (1-angleEaseFactor)*newAngleChangeFactor;

    //6) calcula o quanto teria que esticar dada a velocidade, ignorando outros fatores
    float newStretch = norm(glm::vec4(ssKinematicInfo_p->vel,0))/(*ssMaxSpeed_p);
    newStretch = sqrt(newStretch)*maxStretch; //raiz pra que comece mais rápido o efeito, dando um "arranque"

    //7) calcula o esticamento final usando isso tudo:
    newStretch = 1 + newStretch*angleChangeFactor;
    if (newStretch < stretch) stretch = newStretch; //se "encolheu", altera direto
    //interpolação feita para que leve alguns frames pra ir esticando se arrancar:
    else stretch = (stretchEaseFactor)*stretch + (1-stretchEaseFactor)*newStretch;

    //8) o que esticar numa direção, espreme na oposta. Ideia é manter "área" do objeto
    ///no momento supõe movimento em só um plano com câmera ortográfica perpendicular!
    float squish = 1.0f/stretch;

    //9) faz uma transformação de semelhança para orientar o esticamento no sentido do movimento
    glm::mat4 transformation =  Matrix_Identity()
                                *Matrix_Rotate_Z(movementAngle)*Matrix_Scale(stretch,squish,1)*Matrix_Rotate_Z(-movementAngle)
                                *Matrix_Rotate_Z(modelRotationAroundZAxis);

    *ssModel_p *= transformation;
}
