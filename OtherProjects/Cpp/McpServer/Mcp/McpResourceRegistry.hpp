//
// Created by ertls on 25.08.2026.
//

#pragma once
#include <string>
#include <concepts>
#include "../Dependencies/json/json.hpp"

namespace mcp {
class McpResourceRegistry {
public:
    enum class MimeType {
        JSON,
        TEXT,
        MARKDOWN,
        OCTETSTREAM
    };
private:
    using string = std::string;
    using Json = nlohmann::json;

    struct ResourceMeta {
        string uri;
        string name;
        string description;
        MimeType mimeType;
    };

    class IResource {
    public:
        virtual const ResourceMeta& getMeta() = 0;
        virtual Json read(const Json& args) = 0;
        virtual ~IResource() = default;
    };

    template<typename R>
    requires std::invocable<R, const Json&>
    class ResourceWrapper: public IResource {
        ResourceMeta mMeta;
        R mReader;

    public:
        ResourceWrapper(const ResourceMeta& meta, R reader):
            mMeta(meta),
            mReader(reader)
        {}

        const ResourceMeta& getMeta() override {
            return mMeta;
        }

        Json read(const Json& args) override {
            return std::invoke(mReader, args);
        }
    };

    static std::string_view toMimeString(MimeType mime) {
        constexpr std::array<std::string_view, 4> map {
            "application/json", 
            "text/plain",
            "text/markdown",
            "application/octet-stream"
        };

        return map[static_cast<int>(mime)];
    }

    std::unordered_map<string, std::unique_ptr<IResource>> mResources;

public:
    static McpResourceRegistry& Get() {
        static McpResourceRegistry localINTERFACE{};
        return localINTERFACE;
    }

    Json listResources() {
        Json out = Json::array();

        for (const auto& resource: mResources | std::views::values) {
            const auto&[uri, name, description, mimeType] = resource->getMeta();
            Json res = {
                {"uri", uri},
                {"name", name},
                {"description", description},
                {"mimeType", mimeType}
            };
            out.push_back(res);
        }

        return out;
    }

    Json readResource(const Json& params) {
        Json out = Json::array();

        const std::unique_ptr<IResource>& resource = mResources[params["uri"]];
        const ResourceMeta& meta = resource->getMeta();

        Json contents = {
            {"uri", meta.uri},
            {"mimeType", toMimeString(meta.mimeType)},
            resource->read(params)
        };

        return out;
    }

    template<typename R>
    requires std::invocable<R, const Json&>
    void addResource(const string& name, const string& uri, MimeType mime, R reader, const string& description = "") {
        mResources[uri] = std::make_unique<ResourceWrapper<R>>(ResourceMeta{.uri = uri, .name = name, .description = description, .mimeType = mime}, reader);
    }
};
}