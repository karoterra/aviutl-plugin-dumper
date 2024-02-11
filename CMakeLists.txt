cmake_minimum_required(VERSION 3.0)
project(aviutl_plugin_dumper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(aviutl_plugin_dumper
    src/aviutl_plugin_dumper.cpp
    src/Sha256Hasher.hpp
    src/Sha256Hasher.cpp
)
target_include_directories(aviutl_plugin_dumper PRIVATE
    vendor/aviutl_exedit_sdk
)
target_link_libraries(aviutl_plugin_dumper bcrypt)
