#include "InputSystem.h"
#include "../utils/Const.h"

InputSystem::InputSystem(EventSystem *eventSystem)
    : eventSystem(eventSystem) {}

InputSystem::~InputSystem() {}

void InputSystem::actualizarInputMap(Entity &player)
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");
    GLFWwindow *window = motorRaylib.getMotorDeJuego().getWindow();

    unsigned int id = player.getId();
    auto &inputMap = inputMaps[id]; 

 
    inputMap["moveRight"] = false;
    inputMap["moveLeft"] = false;
    inputMap["moveUp"] = false;
    inputMap["moveDown"] = false;
    inputMap["enter"] = false;
    inputMap["placeBomb"] = false;
    inputMap["pause"] = false;

   
    if (playerToJoystick.find(id) == playerToJoystick.end())
    {
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid)
        {
            if (glfwJoystickPresent(jid) && joystickAssigned.find(jid) == joystickAssigned.end())
            {
                playerToJoystick[id] = jid;
                joystickAssigned.insert(jid);
                break;
            }
        }
    }

    
    if (playerToJoystick.find(id) == playerToJoystick.end())
    {
        inputMap["moveRight"] = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
        inputMap["moveLeft"] = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
        inputMap["moveUp"] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
        inputMap["moveDown"] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            if (!teclaEnter_presionada)
            {
                teclaEnter_presionada = true;
                inputMap["enter"] = true;
            }
        }
        else
            teclaEnter_presionada = false;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if (!teclaEspacio_presionada)
            {
                teclaEspacio_presionada = true;
                inputMap["placeBomb"] = true;
            }
        }
        else
            teclaEspacio_presionada = false;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            if (!teclaPause_presionada)
            {
                teclaPause_presionada = true;
                inputMap["pause"] = true;
            }
        }
        else
            teclaPause_presionada = false;
    }
    else  
    {
        int jid = playerToJoystick[id];

        if (glfwJoystickPresent(jid))
        {
            int axisCount, buttonCount;
            const float *axes = glfwGetJoystickAxes(jid, &axisCount);
            const unsigned char *buttons = glfwGetJoystickButtons(jid, &buttonCount);
            const float deadzone = 0.2f;

            if (axisCount >= 2)
            {
                if (axes[0] > deadzone) inputMap["moveRight"] = true;
                else if (axes[0] < -deadzone) inputMap["moveLeft"] = true;

                if (axes[1] < -deadzone) inputMap["moveUp"] = true;
                else if (axes[1] > deadzone) inputMap["moveDown"] = true;
            }

            if (buttonCount >= 2)
            {
                // Botón B
                if (buttons[1] == GLFW_PRESS)
                {
                    if (!botonB_presionado[id])
                    {
                        botonB_presionado[id] = true;
                        inputMap["enter"] = true;
                    }
                }
                else
                    botonB_presionado[id] = false;

                // Botón A
                if (buttons[0] == GLFW_PRESS)
                {
                    if (!botonA_presionado[id])
                    {
                        botonA_presionado[id] = true;
                        inputMap["placeBomb"] = true;
                    }
                }
                else
                    botonA_presionado[id] = false;

                // Botón Start
                if (buttonCount > 7 && buttons[7] == GLFW_PRESS)
                {
                    if (!botonStart_presionado[id])
                    {
                        botonStart_presionado[id] = true;
                        inputMap["pause"] = true;
                    }
                }
                else
                    botonStart_presionado[id] = false;
            }
        }
    }

  
    if (eventSystem)
    {
        if (inputMap["moveRight"]) eventSystem->triggerEvent(EventType::MOVE_RIGHT, player);
        if (inputMap["moveLeft"]) eventSystem->triggerEvent(EventType::MOVE_LEFT, player);
        if (inputMap["moveUp"]) eventSystem->triggerEvent(EventType::MOVE_UP, player);
        if (inputMap["moveDown"]) eventSystem->triggerEvent(EventType::MOVE_DOWN, player);
        if (inputMap["placeBomb"]) eventSystem->triggerEvent(EventType::PLACE_BOMB, player);
        if (inputMap["enter"]) eventSystem->triggerEvent(EventType::ENTER, player);
        if (inputMap["pause"]) eventSystem->triggerEvent(EventType::PAUSE, player);
    }
}

const std::unordered_map<std::string, bool> &InputSystem::getInputMap(Entity &player) const
{
    return inputMaps.at(player.getId());
}



int InputSystem::contarMandosConectados() const
{
    int count = 0;
    for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid)
    {
        if (glfwJoystickPresent(jid))
        {
            count++;
        }
    }
    return count;
}