//
// Created by ertls on 24.08.2026.
//

#pragma once
#include "../Dependencies/json/json.hpp"

namespace mcp::ReturnTypes {
    using Json = nlohmann::json;

    template<typename T>
    requires requires (T a) { std::to_string((a)); }
    Json asText(const T& value) {
        return {
            {"type", "text"},
            {"text", std::to_string((value))}
        };
    }

    inline Json asText(const char* value) {
        return {
            {"type", "text"},
            {"text", std::string((value))}
        };
    }

    inline Json asText(const std::string& value) {
        return {
            {"type", "text"},
            {"text", value}
        };
    }

    inline Json asText(const bool value) {
        return {
            {"type", "text"},
            {"text", value ? "true" : "false"}
        };
    }
}
