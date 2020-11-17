#ifndef _GLOBALS_H
#define _GLOBALS_H

#define CAMERA_THETA_ZERO (0.f)
#define CAMERA_PHI_ZERO (0.f)
#define CAMERA_DIST_ZERO (48.f)

extern const float pi_f;

// Raz�o de propor��o da janela (largura/altura). Veja fun��o FramebufferSizeCallback().
extern float g_ScreenRatio;

// �ngulos de Euler que controlam a rota��o de um dos cubos da cena virtual
extern float g_AngleX;
extern float g_AngleY;
extern float g_AngleZ;

// "g_LeftMouseButtonPressed = true" se o usu�rio est� com o bot�o esquerdo do mouse
// pressionado no momento atual. Veja fun��o MouseButtonCallback().
extern bool g_LeftMouseButtonPressed;

// Vari�veis que definem a c�mera em coordenadas esf�ricas, controladas pelo
// usu�rio atrav�s do mouse (veja fun��o CursorPosCallback()). A posi��o
// efetiva da c�mera � calculada dentro da fun��o main(), dentro do loop de
// renderiza��o.
extern float g_CameraTheta; // �ngulo no plano ZX em rela��o ao eixo Z
extern float g_CameraPhi;   // �ngulo em rela��o ao eixo Y
extern float g_CameraDistance; // Dist�ncia da c�mera para a origem

// Vari�vel que controla o tipo de proje��o utilizada: perspectiva ou ortogr�fica.
extern bool g_UsePerspectiveProjection;

// Vari�vel que controla se o texto informativo ser� mostrado na tela.
extern bool g_ShowInfoText;

#endif  //_GLOBALS_H
