#include "globals.h"

const float pi_f = 3.141592653589793238463f;

int g_vsync = 0;

int g_timesWon = 0;
float g_startTime = 0;
int g_mortes = 0;

// Raz�o de propor��o da janela (largura/altura). Veja fun��o FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// �ngulos de Euler que controlam a rota��o de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usu�rio est� com o bot�o esquerdo do mouse
// pressionado no momento atual. Veja fun��o MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;

// Vari�veis que definem a c�mera em coordenadas esf�ricas, controladas pelo
// usu�rio atrav�s do mouse (veja fun��o CursorPosCallback()). A posi��o
// efetiva da c�mera � calculada dentro da fun��o main(), dentro do loop de
// renderiza��o.
float g_CameraTheta = CAMERA_THETA_ZERO; // �ngulo no plano ZX em rela��o ao eixo Z
float g_CameraPhi = CAMERA_PHI_ZERO;   // �ngulo em rela��o ao eixo Y
float g_CameraDistance = CAMERA_DIST_ZERO; // Dist�ncia da c�mera para a origem

// Vari�vel que controla o tipo de proje��o utilizada: perspectiva ou ortogr�fica.
bool g_UsePerspectiveProjection = true;

// Vari�vel que controla se o texto informativo ser� mostrado na tela.
bool g_ShowInfoText = true;
