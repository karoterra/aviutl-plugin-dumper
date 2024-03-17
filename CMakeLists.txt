cmake_minimum_required(VERSION 3.21)

project(aviutl_plugin_dumper
    VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CLI11 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(aviutl_plugin_dumper
    src/aviutl_plugin_dumper.cpp
    src/version.hpp
    src/encoding.hpp
    src/plugin_info.hpp
    src/Sha256Hasher.hpp
    src/Sha256Hasher.cpp
    $<$<CXX_COMPILER_ID:MSVC>:src/manifest.manifest>
)
target_include_directories(aviutl_plugin_dumper PRIVATE
    vendor/aviutl_exedit_sdk
)
target_link_libraries(aviutl_plugin_dumper
    CLI11::CLI11
    nlohmann_json::nlohmann_json
    bcrypt
)
target_compile_definitions(aviutl_plugin_dumper
    PRIVATE
        APP_VERSION="${PROJECT_VERSION}"
)
target_compile_options(aviutl_plugin_dumper
    PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_custom_target(package
    COMMAND 7z a publish/${PROJECT_NAME}_v${PROJECT_VERSION}.zip
        $<TARGET_FILE:aviutl_plugin_dumper>
        README.md
        LICENSE
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS aviutl_plugin_dumper
)
