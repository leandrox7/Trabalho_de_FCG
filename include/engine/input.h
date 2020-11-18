#ifndef _INPUT_H
#define _INPUT_H

#include <GLFW/glfw3.h>
#include "globals.h"

///DEFINIÇÕES DE CONTROLE DO JOGO! TEM QUE SER ATUALIZADAS AQUI E NO INPUT.CPP! (em  initInputMap())

//existe um array g_actionKB[NUMBER_KB_ACTIONS][INFO_PER_ACTION]
//o primeiro índice determina qual a ação e o segundo qual a informação sobre ela.
//as ações são:
#define NUMBER_KB_ACTIONS 12

#define EXIT 0
#define PERSPECTIVE_PROJ 1
#define ORTHO_PROJ 2
#define SHOW_HELP 3
#define RELOAD_SHADERS 4
#define EXPLODE 5
#define LEFT 6
#define RIGHT 7
#define UP 8
#define DOWN 9
#define RESET_CAMERA 10
#define TOGGLE_VSYNC 11

//para cada ação tem-se informação sobre o código glfw associado (tecla), estado atual e estado anterior
#define INFO_PER_ACTION 3
#define KEY_STATE_MEMORY 2 //lembra do estado atual e do anterior para cada tecla
//indices para cada informação (usado no segundo índice):
#define GLFW_CODE 0
#define STATE 1
#define LAST_STATE 2

//cada tecla pode estar em um dos seguinte estados:
#define JUST_RELEASED -1
#define NOT_PRESSED 0
#define JUST_PRESSED 1
#define REPEATING 2

///FIM DAS DEFINIÇÕES DE CONTROLE

extern int g_actionKB[NUMBER_KB_ACTIONS][INFO_PER_ACTION]; //primeiro indice associa ação a um código de teclado do glfw. Segundo diz estado da tecla

extern double g_LastCursorPosX, g_LastCursorPosY, g_cursorPosX, g_cursorPosY, scrollYOffset;

void initInput(GLFWwindow* window);
void initInputCallbacks(GLFWwindow* window);
void initInputMap();
void processInput();

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


#endif  //_INPUT_H
