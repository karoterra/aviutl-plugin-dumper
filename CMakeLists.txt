cmake_minimum_required(VERSION 3.21)

project(aviutl_plugin_dumper
    VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CLI11 CONFIG REQUIRED)

add_executable(aviutl_plugin_dumper
    src/aviutl_plugin_dumper.cpp
    src/version.hpp
    src/Sha256Hasher.hpp
    src/Sha256Hasher.cpp
)
target_include_directories(aviutl_plugin_dumper PRIVATE
    vendor/aviutl_exedit_sdk
)
target_link_libraries(aviutl_plugin_dumper
    CLI11::CLI11
    bcrypt
)
target_compile_definitions(aviutl_plugin_dumper
    PRIVATE
        APP_VERSION="${PROJECT_VERSION}"
)
