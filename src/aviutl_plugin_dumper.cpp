#include <Windows.h>

#include <iostream>
#include <filesystem>
#include <format>

#include <aviutl.hpp>

#include "Sha256Hasher.hpp"

namespace fs = std::filesystem;

typedef AviUtl::FilterPluginDLL* (__stdcall *GetFilterTable)(void);
typedef AviUtl::FilterPluginDLL** (__stdcall *GetFilterTableList)(void);
typedef AviUtl::InputPluginDLL* (__stdcall* GetInputPluginTable)(void);
typedef AviUtl::InputPluginDLL** (__stdcall* GetInputPluginTableList)(void);
typedef AviUtl::OutputPluginDLL* (__stdcall* GetOutputPluginTable)(void);
typedef AviUtl::OutputPluginDLL** (__stdcall* GetOutputPluginTableList)(void);
typedef AviUtl::ColorPluginDLL* (__stdcall* GetColorPluginTable)(void);
typedef AviUtl::ColorPluginDLL** (__stdcall* GetColorPluginTableList)(void);

const char* Filter = "Filter";
const char* InputPlugin = "InputPlugin";
const char* OutputPlugin = "OutputPlugin";
const char* ColorPlugin = "ColorPlugin";

template<typename T>
void dump_table(const T* tp) {
    if (!tp) return;

    std::cout << std::format("  - Name: {}\n", (tp->name != nullptr ? tp->name : ""));
    std::cout << std::format("    Info: {}\n", (tp->information != nullptr ? tp->information : ""));
}

template<typename T, const char*& TYPE>
void dump_plugin(const char* path) {
    auto hmod = LoadLibrary(path);
    if (!hmod) {
        std::cerr << "Error: LoadLibrary\n";
        return;
    }

    auto get_table = (T * (__stdcall*)(void))GetProcAddress(hmod, std::format("Get{}Table", TYPE).c_str());
    auto get_table_list = (T * *(__stdcall*)(void))GetProcAddress(hmod, std::format("Get{}TableList", TYPE).c_str());
    if (get_table) {
        dump_table<T>(get_table());
    }
    else if (get_table_list) {
        auto list = get_table_list();
        while (*list) {
            dump_table<T>(*list);
            list++;
        }
    }

    FreeLibrary(hmod);
}

void dump(const char* path) {
    fs::path p = path;
    if (fs::is_directory(p)) return;

    std::cout << path << std::endl;
    std::cout << std::format("  Filename: {}\n", p.filename().string());

    Sha256Hasher sha256;
    std::cout << std::format("  SHA256: {}\n", sha256.getFileHash(p));

    auto ext = p.extension().string();
    if (ext == ".auf") {
        std::cout << "  Type: filter plugin\n";

        dump_plugin<AviUtl::FilterPluginDLL, Filter>(path);
    }
    else if (ext == ".aui") {
        std::cout << "  Type: input plugin\n";

        dump_plugin<AviUtl::InputPluginDLL, InputPlugin>(path);
    }
    else if (ext == ".auo") {
        std::cout << "  Type: output plugin\n";
        dump_plugin<AviUtl::OutputPluginDLL, OutputPlugin>(path);
    }
    else if (ext == ".auc") {
        std::cout << "  Type: color plugin\n";
        dump_plugin<AviUtl::ColorPluginDLL, ColorPlugin>(path);
    }
    else if (ext == ".aul") {
        auto hmod = LoadLibrary(path);
        if (!hmod) {
            std::cerr << "Error: LoadLibrary\n";
            return;
        }
        char buf[1024];
        if (LoadString(hmod, 0, buf, 1024)) {
            std::cout << "  Type: language plugin\n";
            std::cout << std::format("  - Name: {}\n", buf);
            LoadString(hmod, 1, buf, 1024);
            std::cout << std::format("    Info: {}\n", buf);
        }

        FreeLibrary(hmod);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        dump(argv[i]);
    }

    return 0;
}
