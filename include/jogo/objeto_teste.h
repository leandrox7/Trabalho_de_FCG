#ifndef _OBJ_TST_H
#define _OBJ_TST_H

#include "engine/input.h"
#include "engine/fisica.h"
#include <glm/mat4x4.hpp>
#include "util/matrices.h"

//defini��es para "anima��o" de explos�o:
#define TIME_TO_EXPAND 0.66f //tempo de dura��o em segundos
#define SCALE_MAX 2 //propor��o m�xima da expans�o
#define SCALE_MIN 0.8f //propor��o inicial da expans�o (< 1 significa que encolhe primeiro)

class GameObjTst{

    public:
        GameObjTst(glm::vec3);

        void step();
        void updateTranslationMatrix();

        kinematicInfo_t kinematicInfo; //f�sica usa para mover
        //fatores de escala e de alpha usados para "anima��o" de explodir
        float scale;
        float alphaFactor;
        glm::mat4 model; //transla��o, rota��o e escalamento do objeto combinados para gpu

    private:

        void velocityFromInput();
        void speedFromDirectionsPressed();
        void speedFromDirectionsRepeating();
        void speedFromDirectionsJustReleased();

        void explode();

        void stretchFromMovement();

        bool exploding;
        float timeAlreadySpentExploding;

        //para movimenta��o controlada pelo jogador
        float maxSpeed;
        float initialSpeedFactor; //em propor��o ao m�ximo
        float speedIncreaseFactor; //em propor��o ao m�ximo

        //para efeito de esticar e espremer devido ao movimento
        float maxStretch; //pode esticar em at� maxStretch*100% na dire��o do mivimento
        float stretch; //1 -> normal. > 1 -> esticado; < 1 -> espremido
        float movementAngle; //0 -> �ngulo n�o mudou. 1 -> �ngulo mudou em 180�
        float stretchEaseFactor; //fator de interpola��o (n�o linear) para esticamento
        float angleChangeFactor; //controla quanto o objeto "desestica" se h� mudan�a de angulo de movimento
        float angleEaseFactor; //mesmo mas para a mudan�a de �ngulo no esticamento
        float overSquish; //quando objeto muda de dire��o abruptamente, pode ENCONLHER por um fator overSquish - 1
};

#endif  //_OBJ_TST_H
