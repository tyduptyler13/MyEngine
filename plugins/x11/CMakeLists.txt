project(MyEnginePlugin_X11)

set(X11_AVAILABLE FALSE CACHE INTERNAL "")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(X11_AVAILABLE TRUE CACHE INTERNAL "")
    option(ENABLE_X11 "Enables use of the X11 Window system" ON)
endif()

# TODO All the other platforms
if (X11_AVAILABLE AND ENABLE_X11)

    find_library(SPDLOG spdlog)

    add_library(MyEnginePlugin_X11 STATIC
            X11WindowPlugin.cpp
            )
    add_dependencies(MyEngineX11 spdlog)
    target_compile_features(MyEngineX11
            PUBLIC
            cxx_std_17
            )
    target_link_libraries(MyEngineX11
            PRIVATE
            X11
            ${SPDLOG}
            )
    target_include_directories(MyEngineX11
            PRIVATE
            ../include
            )
endif()
