
project(MyEngineX11)

set(X11_AVAILABLE FALSE CACHE INTERNAL "")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(X11_AVAILABLE TRUE CACHE INTERNAL "")
    option(ENABLE_X11 "Enables use of the X11 Window system" ON)
endif()

# TODO All the other platforms
if (X11_AVAILABLE AND ENABLE_X11)

    add_library(MyEngineX11 STATIC
            X11WindowPlugin.cpp
            )
    target_compile_features(MyEngineX11
            PUBLIC
            cxx_std_17
            )
    target_link_libraries(MyEngineX11 PRIVATE X11)
    target_include_directories(MyEngineX11
            PRIVATE
            ../../../include
            )
endif()
