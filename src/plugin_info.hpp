#pragma once

#include <Windows.h>

#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include <format>
#include <concepts>

#include <aviutl.hpp>
#include <nlohmann/json.hpp>

#include "Sha256Hasher.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

template<typename T>
concept AviUtlTablePlugin = requires(T t) {
    { t.name } -> std::convertible_to<const char*>;
    { t.information } -> std::convertible_to<const char*>;
};

enum class PluginType {
    Unknown,
    Filter,
    Input,
    Output,
    Color,
    Language,
};

std::string toString(PluginType type) {
    switch (type) {
        case PluginType::Filter:
            return "filter";
        case PluginType::Input:
            return "input";
        case PluginType::Output:
            return "output";
        case PluginType::Color:
            return "color";
        case PluginType::Language:
            return "language";
        default:
            return "unknown";
    }
}

struct PluginTableInfo {
    std::string name;
    std::string info;
};

struct PluginDllInfo {
    fs::path path;
    std::string sha256;
    PluginType type;
    std::vector<PluginTableInfo> table;
};

void to_json(json& j, const PluginTableInfo& p) {
    j = json{
        {"name", p.name},
        {"info", p.info},
    };
}

void to_json(json& j, const PluginDllInfo& p) {
    j = json{
        {"path", p.path.string()},
        {"filename", p.path.filename().string()},
        {"sha256", p.sha256},
        {"type", toString(p.type)},
        {"table", p.table},
    };
}

class PluginDllInfoFactory {
public:
    PluginDllInfoFactory(Sha256Hasher& hasher) : hasher(hasher) {}

    PluginDllInfo create(const fs::path& path) {
        std::string sha256 = hasher.getFileHash(path);
        PluginType type = PluginType::Unknown;
        std::vector<PluginTableInfo> table{};

        if (path.extension() == ".auf") {
            type = PluginType::Filter;
            createTableList<AviUtl::FilterPluginDLL>(path, type, "GetFilterTable", "GetFilterTableList", table);
        }
        else if (path.extension() == ".aui") {
            type = PluginType::Input;
            createTableList<AviUtl::InputPluginDLL>(path, type, "GetInputPluginTable", "GetInputPluginTableList", table);
        }
        else if (path.extension() == ".auo") {
            type = PluginType::Output;
            createTableList<AviUtl::OutputPluginDLL>(path, type, "GetOutputPluginTable", "GetOutputPluginTableList", table);
        }
        else if (path.extension() == ".auc") {
            type = PluginType::Color;
            createTableList<AviUtl::ColorPluginDLL>(path, type, "GetColorPluginTable", "GetColorPluginTableList", table);
        }
        else if (path.extension() == ".aul") {
            type = PluginType::Language;
            auto hmod = LoadLibrary(path.string().c_str());
            if (!hmod) {
                throw std::runtime_error{ std::format("failed to LoadLibrary \"{}\"", path.string()) };
            }
            char buf[1024];
            if (LoadStringA(hmod, 0, buf, 1024)) {
                std::string name{ buf };
                LoadStringA(hmod, 1, buf, 1024);
                std::string info{ buf };
                table.emplace_back(name, info);
            }
            FreeLibrary(hmod);
        }
        else {
            type = PluginType::Unknown;
        }

        return PluginDllInfo{
            .path = path,
            .sha256 = sha256,
            .type = type,
            .table = table,
        };
    }

private:
    Sha256Hasher& hasher;

    template<AviUtlTablePlugin T>
    void createTable(const T* tp, std::vector<PluginTableInfo>& table) {
        if (!tp) {
            return;
        }

        std::string name{ (tp->name) ? (tp->name) : "" };
        std::string info{ (tp->information) ? (tp->information) : "" };
        auto name_utf8 = convert_encoding(name, 932, CP_UTF8);
        auto info_utf8 = convert_encoding(info, 932, CP_UTF8);
        table.emplace_back(name_utf8, info_utf8);
    }

    template<AviUtlTablePlugin T>
    void createTableList(const fs::path& path, PluginType type, const char* getTableName, const char* getTableListName, std::vector<PluginTableInfo>& tableList) {
        auto hmod = LoadLibrary(path.string().c_str());
        if (!hmod) {
            throw std::runtime_error{ std::format("failed to LoadLibrary \"{}\"", path.string()) };
        }

        auto getTable = (T * (__stdcall*)(void))GetProcAddress(hmod, getTableName);
        auto getTableList = (T * *(__stdcall*)(void))GetProcAddress(hmod, getTableListName);
        if (getTable) {
            createTable<T>(getTable(), tableList);
        }
        else if (getTableList) {
            for (auto list = getTableList(); *list; list++) {
                createTable<T>(*list, tableList);
            }
        }

        FreeLibrary(hmod);
    }
};
