#include "engine/gerenciamento_objetos.h"


std::vector<BaseObj*> instanceList;

int registerInstance(BaseObj* instance){
    instanceList.push_back(instance);
    return instanceList.size() - 1;
}

BaseObj* getAddress(int index){
    return instanceList.at(index);
}

void gameObjectsStep(){
    int listSize = instanceList.size();
    if (listSize > 0){
        for(int i = 0; i < listSize; i++) {
            BaseObj* obj = instanceList.at(i);
            obj->instanceID = i;
            if(obj->active) obj->step();
        }
    }
}
