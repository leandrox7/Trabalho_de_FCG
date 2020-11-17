#ifndef _GERENCIA_OBJETOS_JOGO_H
#define _GERENCIA_OBJETOS_JOGO_H

#include <GLFW/glfw3.h>
#include <vector>
#include "engine/objetos_base.h"

class BaseObj;

int registerInstance(BaseObj*);
void renewID(int index, BaseObj* instance);
BaseObj* getAddress(int index);
void gameObjectsStep();

#endif //GERENCIA_OBJETOS_JOGO_H
