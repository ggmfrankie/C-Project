//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

#include "GuiElementData.h"
#include "../Drawing/Render.h"
#include "../../Utils/Os/Time.h"
#include "../../Utils/UtilityFun.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "GuiElement.h"
#include "GLFW/glfw3.h"
#include "../../Utils/DataStructures/CArrayList.h"
#include "CallbackFunctions.h"

bool isSelectedCharacter(Vec2f pos, float width, float height, const Vec2i mousePos) {
    if ((float)mousePos.x <= pos.x+width && (float)mousePos.x >= pos.x &&
        (float)mousePos.y <= pos.y+height && (float)mousePos.y >= pos.y) {
        return true;
        }
    return false;
}

bool defaultHoverFun(Element *element, Renderer *renderer) {

    return false;
}

bool changeColorOnHoverFun(Element *element, Renderer *renderer) {
    element->visuals.color = *(Vec3f*) (element->elementData);
    return false;
}

void onRequestMove_SimpleDrag(Element *element, Vec2i pos) {
}

bool runTaskFun(Element *element, Renderer *renderer) {
    if (element->task.func && !element->task.isBlocked) pushTask(element->task.func, element->task.userdata);
    return true;
}

void displayCurrentTime(Element *element) {
#ifdef _WIN32
    static u_int64 lastTime = 0;
    if (lastTime == 0) lastTime = now_ns();

    const u_int64 currentTime = now_ns();
    const u_int64 timeNs = currentTime - lastTime;

    if ((double)timeNs * 1e-9 < 1.0) return;

    SYSTEMTIME t;
    GetLocalTime(&t);

    char time[64];

    snprintf(
        time, 64,
        "%02d:%02d:%02d\n",
           t.wHour,
           t.wMinute,
           t.wSecond);
    Element_setText(element, time);
#endif
}

void updateColorRainbow(Element *element) {
    static double hue = 0.0f;
    static unsigned long long lastTime = 0;

    if (lastTime == 0) lastTime = now_ns();

    const unsigned long long currentTime = now_ns();
    const unsigned long long timeNs = currentTime - lastTime;

    hue += 120.0 * (double)timeNs * 1e-9;
    if (hue >= 360.0) hue -= 360.0;

    const Vec3f color = hsv_to_rgb((float)hue, .3f, 1.0f);
    element->visuals.color = color;
    lastTime = currentTime;
}
