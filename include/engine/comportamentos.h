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
        //para movimenta��o controlada pelo jogador
        float initialSpeedFactor; //em propor��o ao m�ximo
        float speedIncreaseFactor; //em propor��o ao m�ximo, por segundo!

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

        //fatores de escala e de alpha usados para "anima��o" de explodir
        float scale;
        float alphaFactor;

    protected:
        void explode(glm::vec3* objPositionInfo);
        virtual void endOfExplosion();

        bool exploding; //se t� explodingo agora
        float timeAlreadySpentExploding;
        //par�metros para "anima��o" de explos�o:
        float timeToExpand;
        float maxScale; //propor��o m�xima da expans�o
        float minScale; //propor��o inicial da expans�o (< 1 significa que encolhe primeiro)

    private:
         glm::mat4* expModel_p; //transla��o, rota��o e escalamento do objeto combinados para gpu
};

class StrechSquishFromMovementBehaviour{

    public:
        StrechSquishFromMovementBehaviour
            (float maximumStretch, float* maxSpeedPointer, kinematicInfo_t* kinematicInfoPointer, glm::mat4* modelPointer);

    protected:
        void stretchFromMovement();

        //para efeito de esticar e espremer devido ao movimento
        float maxStretch; //pode esticar em at� maxStretch*100% na dire��o do mivimento
        float overSquish; //quando objeto muda de dire��o abruptamente, pode ENCONLHER por um fator overSquish - 1

        float stretchEaseFactor; //fator de interpola��o (n�o linear) para esticamento
        float angleChangeFactor; //controla quanto o objeto "desestica" se h� mudan�a de angulo de movimento

        float modelRotationAroundZAxis = 0;
        float movementAngle; //0 -> �ngulo n�o mudou. 1 -> �ngulo mudou em 180�
        float angleEaseFactor; //mesmo mas para a mudan�a de �ngulo no esticamento

        float stretch; //1 -> normal. > 1 -> esticado; < 1 -> espremido

    private:
        kinematicInfo_t* ssKinematicInfo_p;
        glm::mat4* ssModel_p; //transla��o, rota��o e escalamento do objeto combinados para gpu
        float* ssMaxSpeed_p; //efeito � dependente da velocidade em rela��o a uma m�xima
};

//adicionar ricochetear

#endif //_BEHAVE_H
