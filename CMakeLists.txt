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

#file(GLOB NBIND_SOURCE_FILES node_modules/nbind/src/*.cc node_modules/nbind/src/v8/*.cc)

#add_nodejs_module(MyEngine_node
#        ${NBIND_SOURCE_FILES}
#        src/nbind/Geometries/BoxGeometry.cpp
#        src/nbind/Geometries/SphereGeometry.cpp
#        src/nbind/BufferGeometry.cpp
#        src/nbind/Camera.cpp
#        src/nbind/Geometry.cpp
#        src/nbind/Material.cpp
#        src/nbind/Mesh.cpp
#        src/nbind/Object3D.cpp
#        src/nbind/PerspectiveCamera.cpp
#        src/nbind/Renderer.cpp
#        src/nbind/Scene.cpp
#        src/nbind/SoundObject.cpp
#        src/nbind/Vector2.cpp
#        src/nbind/Vector3.cpp
#        src/nbind/Vector4.cpp)

#target_include_directories(MyEngine_node BEFORE PUBLIC node_modules/nbind/include)
#set_property(TARGET MyEngine_node PROPERTY CXX_STANDARD 14)

#if (CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
#    message(" * C++ compiler: Emscripten")
#else ()
#    target_compile_definitions(MyEngine_node PRIVATE BUILDING_NODE_EXTENSION)
#endif ()

#target_link_libraries(MyEngine_node
#        MyEngine
#        ${CMAKE_JS_LIB})

#add_executable(BasicExample
#        test/core/BasicExample.cpp)

#target_link_libraries(BasicExample
#        MyEngine)

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
