#ifndef _MODELOS_H
#define _MODELOS_H

#define CAMINHO_BOLINHA "../../data/sphere.obj"
#define CAMINHO_PALETA "../../data/paleta.obj"
#define CAMINHO_BLOCO "../../data/cubo2.obj"
#define CAMINHO_BLOCO_QUEBRANDO "../../data/cubo_quebrando.obj"
#define CAMINHO_BLOCO_QUEBRANDO_MAIS "../../data/cubo_quebrando2.obj"
#define CAMINHO_PAREDE "../../data/cubo.obj"
#define CAMINHO_PLANO "../../data/plane.obj"

static ObjModel spheremodel(CAMINHO_BOLINHA);
static ObjModel modeloPaleta(CAMINHO_PALETA );
static ObjModel modeloCubo(CAMINHO_PAREDE );
static ObjModel modeloCubo2(CAMINHO_BLOCO);
static ObjModel modeloCuboQuebrando1(CAMINHO_BLOCO_QUEBRANDO);
static ObjModel modeloCuboQuebrando2(CAMINHO_BLOCO_QUEBRANDO_MAIS );
static ObjModel modelPlane(CAMINHO_PLANO );

#endif // _MODELOS_H
