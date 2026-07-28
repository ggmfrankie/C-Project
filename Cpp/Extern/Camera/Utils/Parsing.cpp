//
// Created by ertls on 28.07.2026.
//

#include "Parsing.hpp"

namespace ggm {
    Parsing::Function Parsing::parseFunction(const std::string_view funcBody) {
        enum class State {
            RETURN_TYPE,
            NAME,
            PARAMETERS
        };
        State state;

        for (char c: funcBody) {
            switch (state) {
                case State::RETURN_TYPE: {

                } break;
                case State::NAME: {

                } break;
                case State::PARAMETERS: {

                } break;
            }
        }
    }
} // ggm