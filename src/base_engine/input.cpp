#include "engine/input.h"

#include <limits>

int g_actionKB[NUMBER_KB_ACTIONS][INFO_PER_ACTION]; //primeiro indice d� a��o, segundo se vai ver o c�digo GLFW associado, o estado autal, ou o anterior

double g_LastCursorPosX, g_LastCursorPosY, g_cursorPosX, g_cursorPosY, scrollYOffset;

void initInput(GLFWwindow* window){
    initInputCallbacks(window);
    initInputMap();
}

void initInputMap(){
    ///DEFINI��ES DE CONTROLE DE JOGO! TEM QUE SER ATUALIZADAS AQUI E NO INPUT.H! (melhorar isso?)

    g_actionKB[EXIT][GLFW_CODE] = GLFW_KEY_ESCAPE;
    g_actionKB[PERSPECTIVE_PROJ][GLFW_CODE] = GLFW_KEY_P;
    g_actionKB[ORTHO_PROJ][GLFW_CODE] = GLFW_KEY_O;
    g_actionKB[SHOW_HELP][GLFW_CODE] = GLFW_KEY_H;
    g_actionKB[RELOAD_SHADERS][GLFW_CODE] = GLFW_KEY_R;
    g_actionKB[EXPLODE][GLFW_CODE] = GLFW_KEY_X;
    g_actionKB[LEFT][GLFW_CODE] = GLFW_KEY_A;
    g_actionKB[RIGHT][GLFW_CODE] = GLFW_KEY_D;
    g_actionKB[UP][GLFW_CODE] = GLFW_KEY_W;
    g_actionKB[DOWN][GLFW_CODE] = GLFW_KEY_S;
    g_actionKB[RESET_CAMERA][GLFW_CODE] = GLFW_KEY_C;
    g_actionKB[TOGGLE_VSYNC][GLFW_CODE] = GLFW_KEY_V;


    ///FIM DAS DEFINI��ES DE CONTROLE

    for(int i = 0; i < NUMBER_KB_ACTIONS; i++){
        g_actionKB[i][STATE] = NOT_PRESSED;
        g_actionKB[i][LAST_STATE] = NOT_PRESSED;
    }
}

//o estado de cada tecla � alterado pela KeyCallback quando alguma tecla � pressionada ou solta
//mas isso por si s� n�o permite saber se uma tecla foi rec�m pressionada ou est� sendo segurada
//entre outras coisas. Esse processamento usa a info do estado anterior para ver isso:
void processInput(){
    for(int i = 0; i < NUMBER_KB_ACTIONS; i++){

        if( g_actionKB[i][STATE] == JUST_RELEASED ){
            if (g_actionKB[i][LAST_STATE] == JUST_RELEASED)
                g_actionKB[i][STATE] = NOT_PRESSED;
        }

        else if( g_actionKB[i][STATE] == JUST_PRESSED ){
            if (g_actionKB[i][LAST_STATE] == JUST_PRESSED)
                g_actionKB[i][STATE] = REPEATING;
        }
        g_actionKB[i][LAST_STATE] = g_actionKB[i][STATE];
    }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    for(int i = 0; i < NUMBER_KB_ACTIONS; i++){

        if (key == g_actionKB[i][GLFW_CODE] && action == GLFW_PRESS){
            g_actionKB[i][STATE] = JUST_PRESSED;
        }

        if (key == g_actionKB[i][GLFW_CODE] && action == GLFW_RELEASE){
            g_actionKB[i][STATE] = JUST_RELEASED;
        }

        if (key == g_actionKB[i][GLFW_CODE] && action == GLFW_REPEAT){
            g_actionKB[i][STATE] = REPEATING;
        }
    }
}

void initInputCallbacks(GLFWwindow* window){

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

///Os callbacks a seguir tem l�gica que podia estar "na c�mera":

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        g_LeftMouseButtonPressed = false;
    }
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (g_LeftMouseButtonPressed)
    {
        //usa coordenadas DE TELA (pois � em rela��o � janela)
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos par�metros da c�mera com os deslocamentos
        g_CameraTheta -= 0.01f*dx;
        g_CameraPhi   += 0.01f*dy;

        // Em coordenadas esf�ricas, o �ngulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f/2;
        float phimin = -phimax;

        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;

        // Atualizamos as vari�veis globais para armazenar a posi��o atual do
        // cursor como sendo a �ltima posi��o conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_CameraDistance -= 0.1f*yoffset;
    //camera lookat n�o deve ficar exatamento no ponto pro qual olha (divis�o por zero):
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (g_CameraDistance < verysmallnumber)
        g_CameraDistance = verysmallnumber;
}
