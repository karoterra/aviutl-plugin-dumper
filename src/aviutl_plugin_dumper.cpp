#include <Windows.h>

#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>

#include <aviutl.hpp>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include "Sha256Hasher.hpp"
#include "encoding.hpp"
#include "plugin_info.hpp"
#include "version.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

enum class OutputFormat {
    Friendly,
    Json,
    JsonLines,
};

void output_friendly(const std::vector<PluginDllInfo>& dllInfos) {
    std::ranges::for_each(dllInfos, [](const PluginDllInfo& info) {
        std::cout << "Path: " << info.path.string() << "\n"
            << "Filename: " << info.path.filename().string() << "\n"
            << "SHA256: " << info.sha256 << "\n"
            << "Type: " << toString(info.type) << "\n";

        std::ranges::for_each(info.table, [](const PluginTableInfo& t) {
            std::cout << "  - Name: " << t.name << "\n"
                << "    Info: " << t.info << "\n";
        });
        std::cout << "\n";
    });
}

void output_json(const std::vector<PluginDllInfo>& dllInfos, int indent) {
    json j = dllInfos;
    std::cout << j.dump(indent) << std::endl;
}

void output_json_lines(const std::vector<PluginDllInfo>& dllInfos) {
    std::ranges::for_each(dllInfos, [](const PluginDllInfo& info) {
        json j = info;
        std::cout << j << std::endl;
    });
}

int main(int argc, char* argv[]) {
    chcp cp_utf8{CP_UTF8};
    CLI::App app{ "Dump AviUtl plugin info." };

    std::vector<std::string> inputs;
    app.add_option("input", inputs, "plugin file path")
        ->check(CLI::ExistingFile);

    OutputFormat outputFormat{OutputFormat::Friendly};
    std::map<std::string, OutputFormat> outputFormatMap{
        {"friendly", OutputFormat::Friendly},
        {"json", OutputFormat::Json},
        {"jsonl", OutputFormat::JsonLines},
    };
    app.add_option("-f,--format", outputFormat, "Output format")
        ->transform(CLI::CheckedTransformer(outputFormatMap, CLI::ignore_case), "hoge");

    int indent = 4;
    app.add_option("--indent", indent, "Indent size (minify with negative number)");

    app.set_version_flag("-v,--version", APP_VERSION, "Print version");

    CLI11_PARSE(app, argc, argv);

    Sha256Hasher hasher;
    PluginDllInfoFactory factory{ hasher };
    std::vector<PluginDllInfo> dllInfos;
    std::ranges::transform(
        inputs,
        std::back_inserter(dllInfos),
        [&factory](const std::string& input) {
            return factory.create(input);
        }
    );

    switch (outputFormat) {
    case OutputFormat::Friendly:
        output_friendly(dllInfos);
        break;
    case OutputFormat::Json:
        output_json(dllInfos, indent);
        break;
    case OutputFormat::JsonLines:
        output_json_lines(dllInfos);
        break;
    }

    return 0;
}
