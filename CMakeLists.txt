cmake_minimum_required(VERSION 3.9)
project(MyEngine)

add_subdirectory(deps/spdlog)

option(ENABLE_GLFW_WINDOW "Enables use of GLFW for window creation" ON)
#option(ENABLE_X11_WINDOW "Enables X11 as the window system" OFF)

if(ENABLE_GLFW_WINDOW)
    add_subdirectory(plugins/glfw)
endif()

add_library(MyEngine
        src/DefaultRenderer.cpp
        src/Material.cpp
        src/Math.cpp
        src/SoundObject.cpp
        src/Texture.cpp
        src/PluginLoader.cpp
        src/BasicWindowEventHandler.cpp
        )

set(MYENGINE_INCLUDES
        include
        )

target_include_directories(MyEngine PUBLIC ${MYENGINE_INCLUDES})
target_compile_features(MyEngine
        PUBLIC
        cxx_std_17
        )

add_dependencies(MyEngine
        spdlog
        )
target_link_libraries(MyEngine
        PUBLIC
        spdlog
        # TODO add diligent libs (platform dependent)
        linux
        )

add_executable(unitTests
        test/unit/Box2.cc
        test/unit/constants.cc
        test/unit/constants.hpp
        test/unit/Log.cc
        test/unit/Matrix4.cc
        test/unit/Vector2.cc
        test/unit/Vector3.cc)

target_link_libraries(unitTests
        MyEngine
        )
