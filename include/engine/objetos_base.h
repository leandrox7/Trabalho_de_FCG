#ifndef _OBJ_H
#define _OBJ_H

#include "engine/input.h"
#include "engine/fisica.h"
#include "util/matrices.h"
#include "engine/gerenciamento_objetos.h"
#include <cstring>
#include "engine/comportamentos.h"
#include <string>
#include <util/tiny_obj_loader.h>

///Pro sistema de renderização:

struct SceneObject
{
    std::string  name;        // Nome do objeto
    size_t       first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
    ///
    glm::vec3    bbox_min; // Axis-Aligned Bounding Box do objeto
    glm::vec3    bbox_max;
    ///
};

// Estrutura que representa um modelo geométrico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .
struct ObjModel
{
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
    ///
    glm::vec3    bbox_min; // Axis-Aligned Bounding Box do objeto
    glm::vec3    bbox_max;
    ///

    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true);
};

///Da "engine":

class BaseObj{

    friend void gameObjectsStep();

    public:
        BaseObj();
        ///esse método é chamada a cada frame. O grosso da lógica própria dos objetos deve ser chamada "daqui"
        virtual void step();

        bool active;

    private:
        int instanceID;
};

class BaseGameObj: public BaseObj{

    friend void physicsStep();

    public:
        BaseGameObj(glm::vec3 pos, ObjModel* meshInformation);

        void changeMesh(ObjModel* newMeshsInfo);
        virtual void updateTranslationMatrix();

        void step();

        glm::mat4 model;
        glm::vec3 rotationAroundEachAxis;
        glm::vec3 scalingAlongEachAxis;

        kinematicInfo_t kinematicInfo;
        kinematicInfo_t lastKinematicInfo;

        float maxSpeed;

        bool physicsActive;

        ObjModel* meshInfo_p;
        const char* shapeName;

    private:
        int physicsID;
};

class BaseCollidingObj: public BaseGameObj{

    friend void collisionStep();

    public:
        BaseCollidingObj(glm::vec3 pos, ObjModel* meshInfo, int BoundingBoxType, bool checkCollisionsFromThis);
//        using BaseGameObj::BaseGameObj(glm::vec3 pos, char* meshName);

        virtual void onCollision(BaseCollidingObj* colisor);
        void updateTranslationMatrix();

        boundingBox_t boundingBox;
        bool chkCollisionsFromThis;
        bool chkCollisionsToThis;

    private:
        boundingBox_t getBoundsFromMesh(ObjModel* meshInfo, int bbType);
        int collisionID;
};

//adicionar classe com colisão. Tem bounding box e registro no vetor de colidíveis

#endif  //_OBJ_H

