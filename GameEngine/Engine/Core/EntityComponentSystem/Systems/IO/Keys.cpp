//
// Created by Stefan on 27.11.2026.
//

#include "Keys.hpp"

namespace ggm {
    StringView toString(Key e) {
        switch (e) {
            case Key::$0: return "0";
            case Key::$1: return "1";
            case Key::$2: return "2";
            case Key::$3: return "3";
            case Key::$4: return "4";
            case Key::$5: return "5";
            case Key::$6: return "6";
            case Key::$7: return "7";
            case Key::$8: return "8";
            case Key::$9: return "9";

            case Key::SPACE: return "Space";
            case Key::APOSTROPHE: return "^";
            case Key::COMMA: return ",";
            case Key::MINUS: return "-";
            case Key::PERIOD: return ".";
            case Key::SLASH: return "/";
            case Key::SEMICOLON: return ";";
            case Key::EQUAL: return "=";
            case Key::A: return "A";
            case Key::B: return "B";
            case Key::C: return "C";
            case Key::D: return "D";
            case Key::E: return "E";
            case Key::F:
                break;
            case Key::G:
                break;
            case Key::H:
                break;
            case Key::I:
                break;
            case Key::J:
                break;
            case Key::K:
                break;
            case Key::L:
                break;
            case Key::M:
                break;
            case Key::N:
                break;
            case Key::O:
                break;
            case Key::P:
                break;
            case Key::Q:
                break;
            case Key::R:
                break;
            case Key::S: return "S";
            case Key::T:
                break;
            case Key::U:
                break;
            case Key::V:
                break;
            case Key::W: return "W";
            case Key::X:
                break;
            case Key::Y:
                break;
            case Key::Z:
                break;
            case Key::ESCAPE:
                break;
            case Key::TAB:
                break;
            case Key::LEFT_CONTROL:
                break;
            case Key::LEFT:
                break;
            case Key::RIGHT:
                break;
            case Key::UP:
                break;
            case Key::DOWN:
                break;
        }
        return "Unknown";
    }
}
