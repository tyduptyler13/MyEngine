cmake_minimum_required(VERSION 3.9)
project(MyEngine)

add_subdirectory(deps/spdlog)
add_subdirectory(deps/DiligentEngine/DiligentCore)

add_library(MyEngine
        src/DefaultRenderer.cpp
        src/Material.cpp
        src/Math.cpp
        src/SoundObject.cpp
        src/Texture.cpp
        src/PlatformUtil-SDL.cpp)

set(MYENGINE_INCLUDES "include" "DiligentCore")

target_include_directories(MyEngine PUBLIC "include" PRIVATE "DiligentCore")
set_target_properties(MyEngine PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)

add_dependencies(MyEngine spdlog)
target_link_libraries(MyEngine
        PRIVATE
        spdlog
        # TODO add diligent libs (platform dependent)
        )

set(X11_AVAILABLE FALSE CACHE INTERNAL "")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(X11_AVAILABLE TRUE CACHE INTERNAL "")
    option(ENABLE_X11 "Enables use of the X11 Window system" ON)
endif()

# TODO All the other platforms
if (X11_AVAILABLE AND ENABLE_X11)
    target_link_libraries(MyEngine PRIVATE X11)
    target_compile_definitions(MyEngine PRIVATE ENABLE_X11)
endif()

add_executable(unitTests
        test/unit/Box2.cc
        test/unit/constants.cc
        test/unit/constants.hpp
        test/unit/Log.cc
        test/unit/Matrix4.cc
        test/unit/Vector2.cc
        test/unit/Vector3.cc)

target_link_libraries(unitTests
        MyEngine)
