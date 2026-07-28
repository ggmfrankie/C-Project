//
// Created by ertls on 28.07.2026.
//

#pragma once
#include <string>
#include <vector>

namespace ggm {
    class Parsing {
        using string = std::string;
    public:
        struct Function {
            struct Parameters {
                string type;
                string name;
                bool isConst;
            };
            string returnType;
            string name;
            std::vector<Parameters>;
        };

        Function parseFunction(const std::string_view funcBody);
    private:

    };
} // ggm
