cmake_minimum_required(VERSION 3.9)
project(MyEngineGLFWPlugin)

find_package(spdlog REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_library(MyEngineGLFWPlugin STATIC
        GLFWWindowPlugin.cpp
        )
target_compile_features(MyEngineGLFWPlugin
        PUBLIC
        cxx_std_17
        )
target_link_libraries(MyEngineGLFWPlugin
        PRIVATE
        glfw
        spdlog
        )
target_include_directories(MyEngineGLFWPlugin
        PRIVATE
        ../../include
        )
