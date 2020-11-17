#ifndef _OBJ_TST2_H
#define _OBJ_TST2_H

#include "engine/objetos_base.h"

class GameObjTst2: public BaseCollidingObj, public VelocityFromInputBehaviour, public ExplosionAnimationBehaviour, public StrechSquishFromMovementBehaviour{

    public:
        GameObjTst2(glm::vec3, ObjModel*, int);

        void onCollision(BaseCollidingObj* colisor);
        void step();
};

#endif  //_OBJ_TST2_H
