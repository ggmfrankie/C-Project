//
// Created by ertls on 28.07.2026.
//

#include "Parsing.hpp"

#include <iostream>

#include "Utils.hpp"

namespace ggm {
    using std::string_view, std::string, std::vector;

    Parsing::Function Parsing::parseFunction(const std::string_view funcBody) {
        int start = 0, end = 0;
        bool foundParameters = false;
        Function out;
        out.parameters.reserve(4);

        for (int i = 0; i < funcBody.length(); ++i) {
            if (const char c = funcBody[i];
                c == ' ')
            {
                out.returnType = {funcBody.begin(), funcBody.begin()+i};
                break;
            }
        }

        bool hasFoundName = false;
        int i = 0;
        for (; i < funcBody.length(); ++i) {
            const char c = funcBody[i];
            if (!hasFoundName) {
                start++;
                if (c == ' ') hasFoundName = true;
            } else {
                if (c == '(') break;
            }
        }
        out.name = {funcBody.begin()+start, funcBody.begin()+i};
        start = i+1;
        for (; i < funcBody.length(); ++i) {
            if (funcBody[i] == ')') break;
        }

        const string_view parameters {funcBody.begin()+start, funcBody.begin()+i};

        for (const vector<string_view> parameterList = split(parameters, ',');
            const string_view parameter: parameterList)
        {
            auto tokens = split(parameter, ' ');
            const int len = static_cast<int>(tokens.size());

            const int typeIndex = (len == 2) ? 0 : 1;
            const int nameIndex = typeIndex + ((len > 3) ? 2 : 1);

            out.parameters.emplace_back(
                string(tokens[typeIndex]),
                string(tokens[nameIndex])
            );
        }
        return out;
    }
} // ggm