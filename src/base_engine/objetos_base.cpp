#include "engine/objetos_base.h"

BaseObj::BaseObj(){

    instanceID = registerInstance(this);
    active = true;
}

void BaseObj::step() {}

BaseGameObj::BaseGameObj(glm::vec3 pos, ObjModel* meshInformation):BaseObj(){

    kinematicInfo.pos = pos;
    kinematicInfo.vel = glm::vec3(0.0f,0.0f,0.0f);
    kinematicInfo.acc = glm::vec3(0.0f,0.0f,0.0f);

    lastKinematicInfo = kinematicInfo;

    physicsID = registerForPhysics(this);
    physicsActive = true;

    maxSpeed = 75.f;

    rotationAroundEachAxis = glm::vec3(0,0,0);
    scalingAlongEachAxis = glm::vec3(1,1,1);

    model = Matrix_Identity();
    meshInfo_p = meshInformation;

    shapeName = meshInfo_p->shapes[0].name.c_str();
}

void BaseGameObj::step() {}

void BaseGameObj::updateTranslationMatrix(){ //colocar a física para chamar isso?
    model = Matrix_Identity()*Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z)
    *Matrix_Scale(scalingAlongEachAxis.x,scalingAlongEachAxis.y,scalingAlongEachAxis.z)
    *Matrix_Translate(-kinematicInfo.pos.x,-kinematicInfo.pos.y,-kinematicInfo.pos.z);

//    model = Matrix_Identity()*Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z)
//    *Matrix_Scale(scalingAlongEachAxis.x,scalingAlongEachAxis.y,scalingAlongEachAxis.z)
//    *Matrix_Rotate_Z(rotationAroundEachAxis.x)*Matrix_Rotate_Y(rotationAroundEachAxis.x)*Matrix_Rotate_X(rotationAroundEachAxis.x)
//    *Matrix_Translate(-kinematicInfo.pos.x,-kinematicInfo.pos.y,-kinematicInfo.pos.z);
}

void BaseGameObj::changeMesh(ObjModel* newMeshsInfo){
    meshInfo_p = newMeshsInfo;
    shapeName = meshInfo_p->shapes[0].name.c_str();
}

BaseCollidingObj::BaseCollidingObj(glm::vec3 pos, ObjModel* meshInfo, int boundingBoxType, bool checkCollisionsFromThis):BaseGameObj(pos, meshInfo){

    boundingBox = getBoundsFromMesh(meshInfo, boundingBoxType);

    collisionID = registerForCollisions(this);

    chkCollisionsToThis = true;
    chkCollisionsFromThis = checkCollisionsFromThis;
}

void BaseCollidingObj::onCollision(BaseCollidingObj* colisor){

}

boundingBox_t BaseCollidingObj::getBoundsFromMesh(ObjModel* meshInfo, int boundingBoxType){

    boundingBox_t bbox;

    bbox.minimum = glm::vec4(meshInfo->bbox_min,1);
    bbox.maximum = glm::vec4(meshInfo->bbox_max,1);
    bbox.center = glm::vec4((meshInfo->bbox_min + meshInfo->bbox_max)/2.0f,1);
    bbox.radius = (bbox.maximum.x - bbox.minimum.x)/2.0;
    bbox.type = boundingBoxType;

    return bbox;
}

void BaseCollidingObj::updateTranslationMatrix(){

    model = Matrix_Identity()*Matrix_Translate(kinematicInfo.pos.x,kinematicInfo.pos.y,kinematicInfo.pos.z);
}

///Para renderização:
// Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
// Veja: https://github.com/syoyo/tinyobjloader
ObjModel::ObjModel(const char* filename, const char* basepath, bool triangulate)
{
    printf("Carregando modelo \"%s\"... ", filename);

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

    if (!err.empty())
        fprintf(stderr, "\n%s\n", err.c_str());

    if (!ret)
        throw std::runtime_error("Erro ao carregar modelo.");

    printf("OK.\n");
}
