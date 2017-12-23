cmake_minimum_required(VERSION 3.9)
project(MyEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps/spdlog)

set(MYENGINE_INCLUDE_DIR include)

include_directories(${MYENGINE_INCLUDE_DIR})

add_library(MyEngine
        src/DefaultRenderer.cpp
        src/Material.cpp
        src/Math.cpp
        src/SoundObject.cpp
        src/Texture.cpp
        src/Log.cpp)

add_dependencies(MyEngine spdlog)
target_link_libraries(MyEngine spdlog)

file(GLOB NBIND_SOURCE_FILES node_modules/nbind/src/*.cc node_modules/nbind/src/v8/*.cc)

add_library(MyEngine.node SHARED
        ${NBIND_SOURCE_FILES}
        src/nbind/Geometries/BoxGeometry.cpp
        src/nbind/Geometries/SphereGeometry.cpp
        src/nbind/Materials/GLES2NormalMaterial.cpp
        src/nbind/BufferGeometry.cpp
        src/nbind/Camera.cpp
        src/nbind/Geometry.cpp
        src/nbind/GeometryImporter.cpp
        src/nbind/GLES2MaterialLib.cpp
        src/nbind/GLES2OverlaySystem.cpp
        src/nbind/GLES2Renderer.cpp
        src/nbind/Material.cpp
        src/nbind/Mesh.cpp
        src/nbind/Object3D.cpp
        src/nbind/PerspectiveCamera.cpp
        src/nbind/Renderer.cpp
        src/nbind/Scene.cpp
        src/nbind/SoundObject.cpp
        src/nbind/Vector2.cpp
        src/nbind/Vector3.cpp
        src/nbind/Vector4.cpp)

set_target_properties(MyEngine.node PROPERTIES PREFIX "" SUFFIX "")
target_compile_definitions(MyEngine.node PUBLIC -DBUILDING_NODE_EXTENSION)
target_compile_definitions(MyEngine.node PUBLIC -DUSING_V8_SHARED)
target_compile_definitions(MyEngine.node PUBLIC -DUSING_UV_SHARED)
target_compile_definitions(MyEngine.node PUBLIC -DV8_DEPRECATION_WARNINGS)

target_include_directories(MyEngine.node PUBLIC ${CMAKE_JS_INC} node_modules/nbind/include)

target_link_libraries(MyEngine.node
        MyEngine
        ${CMAKE_JS_LIB})

add_executable(BasicExample
        test/core/BasicExample.cpp)

target_link_libraries(BasicExample
        MyEngine)

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
