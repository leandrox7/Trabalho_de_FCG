#ifndef _GLOBALS_H
#define _GLOBALS_H

#define CAMERA_THETA_ZERO (0.f)
#define CAMERA_PHI_ZERO (0.f)
#define CAMERA_DIST_ZERO (48.f)
#define MAX_FRAMETIME (0.1f)
#define GAME_AREA_DIST_FROM_CENTER 35

extern const float pi_f;

extern int g_timesWon;

extern float g_startTime;

extern int g_mortes;

extern int g_vsync;

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
extern float g_ScreenRatio;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
extern float g_AngleX;
extern float g_AngleY;
extern float g_AngleZ;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
extern bool g_LeftMouseButtonPressed;

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
extern float g_CameraTheta; // Ângulo no plano ZX em relação ao eixo Z
extern float g_CameraPhi;   // Ângulo em relação ao eixo Y
extern float g_CameraDistance; // Distância da câmera para a origem

// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
extern bool g_UsePerspectiveProjection;

// Variável que controla se o texto informativo será mostrado na tela.
extern bool g_ShowInfoText;

#endif  //_GLOBALS_H
