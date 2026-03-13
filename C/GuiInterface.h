//
// Created by Stefan on 27.02.2026.
//

#ifndef MIXEDPROJECT_GUIINTERFACE_H
#define MIXEDPROJECT_GUIINTERFACE_H

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

    void f_gui_loadTextures(char* first, ...);
    #define gui_loadTextures(first, ...) f_gui_loadTextures(first, __VA_ARGS__, nullptr)
    void gui_setTexture(Element* e, const char* name);

    void gui_charCallback(GLFWwindow*, unsigned int codepoint);
    void gui_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void gui_cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
    void gui_resizeCallback(GLFWwindow *window, int width, int height);

    void gui_setActive(const char* name, bool b);
    void gui_toggleVisible(const char* name);
    void gui_setText(const char* name, const char* text);
    void gui_setColor(const char* name, float r, float g, float b);
    void gui_resetColor(const char* name);
    void gui_setCornerRadius(const char* name, int radius);

#ifdef __cplusplus
}
#endif

#endif //MIXEDPROJECT_GUIINTERFACE_H