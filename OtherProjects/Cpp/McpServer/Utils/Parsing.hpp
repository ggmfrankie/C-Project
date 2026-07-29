//
// Created by ertls on 28.07.2026.
//

#pragma once
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ggm {
    class Parsing {
        using string = std::string;
    public:
        struct Parameter {
            string type;
            string name;
        };

        struct Function {
            string name;
            string returnType;
            std::vector<Parameter> parameters;
        };

        static Function extractParameters(std::string_view funcBody);
    private:
    };
} // ggm
