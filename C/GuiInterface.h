//
// Created by Stefan on 27.02.2026.
//

#ifndef MIXEDPROJECT_GUIINTERFACE_H
#define MIXEDPROJECT_GUIINTERFACE_H
#include "GLFW/glfw3.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    GLFWkeyfun keyCallback;
    GLFWcharfun charCallback;
    GLFWcursorposfun cursorPositionCallback;
    GLFWframebuffersizefun resizeCallback;

} GUI_callbackFunctions;

struct Element;

    void gui_init(GLFWwindow* window, int width, int height, void (*generateGUI)(Element* guiRoot));

    void gui_update();
    void gui_render();


    void gui_charCallback(GLFWwindow*, unsigned int codepoint);
    void gui_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void gui_cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
    void gui_resizeCallback(GLFWwindow *window, int width, int height);

    void gui_setVisible(const char* name, bool b);
    void gui_toggleVisible(const char* name);

#ifdef __cplusplus
}
#endif

#endif //MIXEDPROJECT_GUIINTERFACE_H