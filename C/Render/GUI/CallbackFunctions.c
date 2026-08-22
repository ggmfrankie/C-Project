//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

#include "../Drawing/Render.h"
#include "../../Utils/Os/Time.h"
#include "../../Utils/Misc/UtilityFun.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "CallbackFunctions.h"
#include "GuiElement.h"
#include "GLFW/glfw3.h"
#include <stdint.h>
#include "../../Utils/DataStructures/CArrayList.h"

bool onHover_changeColor(Element *element, Renderer *renderer) {
    element->visuals.color = *(Vec3f*) (element->elementData);
    return false;
}

void onRequestMove_SimpleDrag(Element *element, Vec2f pos) {
    element->dims.pos = pos;
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
    Element_setText_ptr(element, time);
#endif
}

void updateColorRainbow(Element *element) {
    static double hue = 0.0f;
    static unsigned long long lastTime = 0;

    if (lastTime == 0) lastTime = now_ns();

    const unsigned long long currentTime = now_ns();
    const unsigned long long timeNs = currentTime - lastTime;

    hue += 120.0 * (double)timeNs * 1e-9;
    if (hue >= 360.0) hue = 0;

    const Vec3f color = hsv_to_rgb((float)hue, .3f, 1.0f);
    element->visuals.color = color;
    lastTime = currentTime;
}
