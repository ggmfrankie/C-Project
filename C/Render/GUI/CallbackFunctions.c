//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

#include "GuiElementData.h"
#include "../Drawing/Render.h"
#include "../../Utils/TimeMeasurenments.h"
#include "../../Utils/UtilityFun.h"
#include <windows.h>

#include "../Engine.h"
#include "Utils/DataStructures.h"

bool dragFun(Element *element, const Renderer *renderer) {
    static Vec2i offset;
    static bool dragging = false;
    const bool isMouseDown = isMousePressed(renderer->window, GLFW_MOUSE_BUTTON_LEFT);

    if (!isMouseDown) {
        dragging = false;
        return false;
    }

    if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        const Vec2i parentWorldPos = element->parentElement ? element->parentElement->dims.worldPos : (Vec2i){0, 0};
        if (!dragging) {
            offset.x = renderer->mousePos.x - element->dims.worldPos.x;
            offset.y = renderer->mousePos.y - element->dims.worldPos.y;
            dragging = true;
        } else {
            element->positionMode = POS_ABSOLUTE;
            element->dims.pos.x = (renderer->mousePos.x - parentWorldPos.x) - offset.x - ((element->parentElement)?element->parentElement->padding.left:0);
            element->dims.pos.y = (renderer->mousePos.y - parentWorldPos.y) - offset.y - ((element->parentElement)?element->parentElement->padding.up:0);
        }
        return true;
    }
    return false;
}

bool isSelectedCharacter(Vec2f pos, float width, float height, const Vec2i mousePos) {
    if ((float)mousePos.x <= pos.x+width && (float)mousePos.x >= pos.x &&
        (float)mousePos.y <= pos.y+height && (float)mousePos.y >= pos.y) {
        return true;
        }
    return false;
}

bool hoverAndDragFun(Element *element, Renderer *renderer) {
    return dragFun(element, renderer);
}

bool hoverAndDragFunctionInvis(Element *element, Renderer *renderer) {
    return dragFun(element, renderer);
}

bool defaultHoverFun(Element *element, Renderer *renderer) {
    element->visuals.brightness = 0.8f;
    return false;
}

bool changeColorOnHoverFun(Element *element, Renderer *renderer) {
    element->visuals.color = *(Vec4f*) (element->elementData);
    return false;
}

bool runTaskFun(Element *element, Renderer *renderer) {
    if (element->task.func && !element->task.isBlocked) pushTask(element->task.func, element->task.userdata);
    return true;
}

bool click(GLFWwindow *window, const int mouseButton) {
    static bool wasClicked = false;
    const bool isMouseDown = isMousePressed(window, mouseButton);

    if (!isMouseDown) {
        wasClicked = false;
        return false;
    }
    if (!wasClicked) {
        wasClicked = true;
        return true;
    }
    return false;
}

bool sliderCallbackFun(Element *element, Renderer *renderer) {
    SliderData* sliderDate = (SliderData*)element->elementData;

    return false;
}

bool textField_onClick(Element *element, Renderer *renderer) {
    if(element->type != t_textField) return false;

    TextFieldData* data = element->elementData;
    if (Strings.isEmpty(&data->text)) return false;

    List_Character* charQuads = &element->textElement.charQuads;
    Vec2i mousePos = getMousePos();

    Vec2f relMousePos = {(float)(mousePos.x - element->dims.pos.x), (float)(mousePos.y - element->dims.pos.y)};
    mousePos.x -= element->dims.pos.x;
    mousePos.y -= element->dims.pos.y;
    int i = 0;
    for (; i < charQuads->size; i++) {
        Character* currentChar = &charQuads->content[i];
        if (relMousePos.x < currentChar->pos.x + currentChar->width/2) {
            data->cursor.byteIndex = i;
            return true;
        }
    }
    data->cursor.byteIndex = i;
    return true;
}

bool textField_runTask(Element *element, Renderer *renderer) {
    if(element->type != t_textField) return false;
    TextFieldData* data = element->elementData;
    if (data->text.length == 0) return false;

    char* newBuffer = malloc(data->text.length + 1);
    memcpy(newBuffer, data->text.content, data->text.length);
    newBuffer[data->text.length] = '\0';

    str_clear(&data->text);
    data->cursor.byteIndex = 0;
    setText_noLock(element,"");

    if (element->task.func && !element->task.isBlocked) {
        pushTask(element->task.func, newBuffer);
    }
    return true;
}

void displayCurrentTime(Element *element) {
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
    setText(element, time);
}

void syncWithScreen(Element *element) {
    const Vec2i window = getWindowSize();
    element->dims.width = window.x;
    element->dims.height = window.y;
}

void updateColorRainbow(Element *element) {
    static double hue = 0.0f;
    static u_int64 lastTime = 0;

    if (lastTime == 0) lastTime = now_ns();

    const u_int64 currentTime = now_ns();
    const u_int64 timeNs = currentTime - lastTime;

    hue += 120.0 * (double)timeNs * 1e-9;
    if (hue >= 360.0) hue -= 360.0;

    Vec3f color = hsv_to_rgb((float)hue, .3f, 1.0f);
    element->visuals.color = (Vec4f){color.x,color.y, color.z, 1.0f};
    lastTime = currentTime;
}

void incrementWidth(Element *element) {
    static int calls = 0;
    static int direction = 1;
    calls++;
    if (calls < 1) return;
    calls = 0;
    element->dims.width += direction * 1;
    if (element->dims.width <= 0) direction = -direction;
    if (element->dims.width >= 800) direction = -direction;
}

void incrementHeight(Element *element) {
    static int calls = 0;
    static int direction = 1;
    calls++;
    if (calls < 1) return;
    calls = 0;
    element->dims.height += direction * 1;
    if (element->dims.height <= 0) direction = -direction;
    if (element->dims.height >= 800) direction = -direction;
}

void shiftPosition(Element *element) {
    static int calls = 0;
    calls++;
    if (calls < 100) return;
    calls = 0;
    element->dims.pos.x += 20;
    element->dims.pos.y += 20;
}

void changeTextSize(Element *element) {
    static int calls = 0;
    calls++;
    if (calls < 10) return;
    calls = 0;
    element->textElement.textScale *= 1.1f;
    if (element->textElement.textScale > 5.0f) element->textElement.textScale = 1.0f;
    reloadTextQuads(getFont(), element);
}