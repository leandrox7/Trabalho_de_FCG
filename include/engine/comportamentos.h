#ifndef _BEHAVE_H
#define _BEHAVE_H

#include "engine/input.h"
#include "engine/fisica.h"
#include <glm/mat4x4.hpp>
#include "util/matrices.h"

class VelocityFromInputBehaviour{

    public:
        VelocityFromInputBehaviour
            (float* maximumSpeed, float initialSpeedFraction, float speedIncreaseFraction, kinematicInfo_t *kinematicInfo_pointer);

    bool acceptingInput;

    protected:
        //para movimentação controlada pelo jogador
        float initialSpeedFactor; //em proporção ao máximo
        float speedIncreaseFactor; //em proporção ao máximo, por segundo!

        void velocityFromInput();
        void speedFromDirectionsPressed();
        void speedFromDirectionsRepeating();
        void speedFromDirectionsJustReleased();

    private:
        kinematicInfo_t* velInputKinematicInfo_p;
        float* velInputMaxSpeed_p;
};

class ExplosionAnimationBehaviour{

    public:
        ExplosionAnimationBehaviour
            (float timeExpansion, float minimumScale, float maximunScale, glm::mat4* modelPointer);

        //fatores de escala e de alpha usados para "animação" de explodir
        float scale;
        float alphaFactor;

    protected:
        void explode(glm::vec3* objPositionInfo);
        virtual void endOfExplosion();

        bool exploding; //se tá explodingo agora
        float timeAlreadySpentExploding;
        //parâmetros para "animação" de explosão:
        float timeToExpand;
        float maxScale; //proporção máxima da expansão
        float minScale; //proporção inicial da expansão (< 1 significa que encolhe primeiro)

    private:
         glm::mat4* expModel_p; //translação, rotação e escalamento do objeto combinados para gpu
};

class StrechSquishFromMovementBehaviour{

    public:
        StrechSquishFromMovementBehaviour
            (float maximumStretch, float* maxSpeedPointer, kinematicInfo_t* kinematicInfoPointer, glm::mat4* modelPointer);

    protected:
        void stretchFromMovement();

        //para efeito de esticar e espremer devido ao movimento
        float maxStretch; //pode esticar em até maxStretch*100% na direção do mivimento
        float overSquish; //quando objeto muda de direção abruptamente, pode ENCONLHER por um fator overSquish - 1

        float stretchEaseFactor; //fator de interpolação (não linear) para esticamento
        float angleChangeFactor; //controla quanto o objeto "desestica" se há mudança de angulo de movimento

        float modelRotationAroundZAxis = 0;
        float movementAngle; //0 -> ângulo não mudou. 1 -> ângulo mudou em 180º
        float angleEaseFactor; //mesmo mas para a mudança de ângulo no esticamento

        float stretch; //1 -> normal. > 1 -> esticado; < 1 -> espremido

    private:
        kinematicInfo_t* ssKinematicInfo_p;
        glm::mat4* ssModel_p; //translação, rotação e escalamento do objeto combinados para gpu
        float* ssMaxSpeed_p; //efeito é dependente da velocidade em relação a uma máxima
};

//adicionar ricochetear

#endif //_BEHAVE_H
