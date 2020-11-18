#ifndef _BREAKOUT_H
#define _BREAKOUT_H

#include "engine/objetos_base.h"
#include "engine/comportamentos.h"
#include "jogo/modelos.h"
#include <cstdlib>
#include <time.h>

#define PARA_CIMA 0
#define PARA_ESQUERDA 1
#define PARA_BAIXO 2
#define PARA_DIREITA 3
#define EXPLOSION_TIME (0.66f)
#define EXPLOSION_INIT_SCALE (0.8f)
#define EXPLOSION_MAX_SCALE (2.f)
#define DEATH_ZONE_Y (-22.5f)
#define GRIP_DEFAULT 5

static bool stage[13][7][5]{//marca que blocos aparecem para cada dois níveis (primeiro índice)
    {
        {true, false, true, false, true},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {false, true, false, true, false},
        {false, false, true, false, false},
        {true, false, true, false, true},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, false, true, false, true},
        {false, true, false, true, false},
        {true, false, true, false, true},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {false, true, true, true, false},
        {true, false, false, false, true},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {false, true, false, true, false},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {false, true, false, true, false},
        {true, false, false, false, true},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, false, true, false, true},
        {false, true, false, true, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {false, true, true, true, false},
        {true, false, false, false, true},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, false, true, true},
        {true, true, true, true, true},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
    },
    {
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, false, false, false},
        {false, false, true, false, false},
        {false, false, false, false, false},
    },
    {
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
    },
};

class Parede: public BaseCollidingObj{

    public:
        Parede(glm::vec3 position, ObjModel* meshInfo, int comprimento, int largura);
};

class Bloco: public BaseCollidingObj, public ExplosionAnimationBehaviour{

    public:
        Bloco(glm::vec3 position, ObjModel* meshInfo);
        void onCollision(BaseCollidingObj* colisor);
        void endOfExplosion();
        void step();
        void explode();
        int damage;

    private:
        float rotationX;
        float rotationY;
        float rotationZ;
};


class Paleta: public BaseCollidingObj, public VelocityFromInputBehaviour, public StrechSquishFromMovementBehaviour{

    public:
        Paleta(glm::vec3 position, int directionFront, ObjModel* meshInfo);
        void step();
        void onCollision(BaseCollidingObj* colisor);

        float faceDirection;

    private:
        void rotateBBoxToFaceFront(bool inverse);
};

class Bolinha: public BaseCollidingObj, public ExplosionAnimationBehaviour, public StrechSquishFromMovementBehaviour{

    public:
        Bolinha(glm::vec3 position, glm::vec3 initialVelocity, ObjModel* meshInfo, float randomness);
        void step();
        void onCollision(BaseCollidingObj* colisor);

        float randomSpeedAmplitude;
        int mortes;

    private:
        void endOfExplosion();
        float grip;
};

class Fundo: public BaseGameObj{
    public:
        Fundo(glm::vec3 position, float largura, float comprimento);
};

#endif //_BREAKOUT_H
