#ifndef _OBJ_TST_H
#define _OBJ_TST_H

#include "engine/input.h"
#include "engine/fisica.h"
#include <glm/mat4x4.hpp>
#include "util/matrices.h"

//definições para "animação" de explosão:
#define TIME_TO_EXPAND 0.66f //tempo de duração em segundos
#define SCALE_MAX 2 //proporção máxima da expansão
#define SCALE_MIN 0.8f //proporção inicial da expansão (< 1 significa que encolhe primeiro)

class GameObjTst{

    public:
        GameObjTst(glm::vec3);

        void step();
        void updateTranslationMatrix();

        kinematicInfo_t kinematicInfo; //física usa para mover
        //fatores de escala e de alpha usados para "animação" de explodir
        float scale;
        float alphaFactor;
        glm::mat4 model; //translação, rotação e escalamento do objeto combinados para gpu

    private:

        void velocityFromInput();
        void speedFromDirectionsPressed();
        void speedFromDirectionsRepeating();
        void speedFromDirectionsJustReleased();

        void explode();

        void stretchFromMovement();

        bool exploding;
        float timeAlreadySpentExploding;

        //para movimentação controlada pelo jogador
        float maxSpeed;
        float initialSpeedFactor; //em proporção ao máximo
        float speedIncreaseFactor; //em proporção ao máximo

        //para efeito de esticar e espremer devido ao movimento
        float maxStretch; //pode esticar em até maxStretch*100% na direção do mivimento
        float stretch; //1 -> normal. > 1 -> esticado; < 1 -> espremido
        float movementAngle; //0 -> ângulo não mudou. 1 -> ângulo mudou em 180º
        float stretchEaseFactor; //fator de interpolação (não linear) para esticamento
        float angleChangeFactor; //controla quanto o objeto "desestica" se há mudança de angulo de movimento
        float angleEaseFactor; //mesmo mas para a mudança de ângulo no esticamento
        float overSquish; //quando objeto muda de direção abruptamente, pode ENCONLHER por um fator overSquish - 1
};

#endif  //_OBJ_TST_H
