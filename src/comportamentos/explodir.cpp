 #include "engine/comportamentos.h"

 ExplosionAnimationBehaviour::ExplosionAnimationBehaviour
    (float timeExpansion, float minimumScale, float maximumScale, glm::mat4* modelPointer){

    timeToExpand = timeExpansion;
    minScale = minimumScale;
    maxScale = maximumScale;

    exploding = false;
    alphaFactor = 1;
    timeAlreadySpentExploding = 0;

    expModel_p = modelPointer;
}

void  ExplosionAnimationBehaviour::explode(glm::vec3* objPositionInfo){
    //a raiz quadrada faz a explozão começar mais rápido e "desacelerar" conforme cresce
    scale = minScale + (maxScale-minScale)*sqrt(timeAlreadySpentExploding/timeToExpand);
    //o fator alfa faz ficar mais transparente conforme cresce, sendo 100% opaco no início da explosão
    alphaFactor = 1 - (scale-minScale)/(maxScale-minScale);
    //a transparência deve acelerar com a expansão (na minha cabeça : p)
    alphaFactor *= alphaFactor;

    timeAlreadySpentExploding += clk.dt;

    //quando terminar de explodir, volta ao normal. Provavelmente não vai ser usado assim na prática : p
    if (timeAlreadySpentExploding >= timeToExpand){
        timeAlreadySpentExploding = 0;
        exploding = false;
        alphaFactor = 1;
        scale = 1;
        endOfExplosion();
    }

    *expModel_p = Matrix_Identity()*Matrix_Translate(objPositionInfo->x,objPositionInfo->y,objPositionInfo->z)*Matrix_Scale(scale,scale,scale);
}

void ExplosionAnimationBehaviour::endOfExplosion(){}
