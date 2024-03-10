#include <Windows.h>

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include <aviutl.hpp>
#include <CLI/CLI.hpp>

#include "Sha256Hasher.hpp"
#include "plugin_info.hpp"
#include "version.hpp"

namespace fs = std::filesystem;

void printDllInfos(const std::vector<PluginDllInfo>& dllInfos) {
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

int main(int argc, char* argv[]) {
    CLI::App app{ "Dump AviUtl plugin info." };

    std::vector<std::string> inputs;
    app.add_option("input", inputs, "plugin file path")
        ->check(CLI::ExistingFile);

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

    printDllInfos(dllInfos);

    return 0;
}
