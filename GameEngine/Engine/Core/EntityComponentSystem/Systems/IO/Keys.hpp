//
// Created by Stefan on 27.11.2026.
//

#pragma once
#include "DataStorage/StringView.hpp"
#include <glad/gl.h>
#include "GLFW/glfw3.h"

namespace ggm {
    enum class Key {
        SPACE = GLFW_KEY_SPACE,
        APOSTROPHE = GLFW_KEY_APOSTROPHE,

        COMMA = GLFW_KEY_COMMA,
        MINUS = GLFW_KEY_MINUS,
        PERIOD = GLFW_KEY_PERIOD,
        SLASH = GLFW_KEY_SLASH,

        $0 = GLFW_KEY_0,
        $1 = GLFW_KEY_1,
        $2 = GLFW_KEY_2,
        $3 = GLFW_KEY_3,
        $4 = GLFW_KEY_4,
        $5 = GLFW_KEY_5,
        $6 = GLFW_KEY_6,
        $7 = GLFW_KEY_7,
        $8 = GLFW_KEY_8,
        $9 = GLFW_KEY_9,

        SEMICOLON = GLFW_KEY_SEMICOLON,
        EQUAL = GLFW_KEY_EQUAL,

        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,

        ESCAPE = GLFW_KEY_ESCAPE,
        TAB = GLFW_KEY_TAB,

        LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,

        LEFT = GLFW_KEY_LEFT,
        RIGHT = GLFW_KEY_RIGHT,
        UP = GLFW_KEY_UP,
        DOWN = GLFW_KEY_DOWN,
        //TODO: finish
    };

    enum class MouseButton {
        LEFT = GLFW_MOUSE_BUTTON_LEFT,
        MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
        RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    };

    enum class Modifier {
        NONE = 0,
        ANY = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT | GLFW_MOD_SUPER | GLFW_MOD_CAPS_LOCK |
              GLFW_MOD_NUM_LOCK,

        SHIFT = GLFW_MOD_SHIFT,
        CONTROL = GLFW_MOD_CONTROL,
        ALT = GLFW_MOD_ALT,
        SUPER = GLFW_MOD_SUPER,
        CAPSLOCK = GLFW_MOD_CAPS_LOCK,
        NUMLOCK = GLFW_MOD_NUM_LOCK
    };

    StringView toString(Key e);
}
