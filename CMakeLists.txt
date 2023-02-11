cmake_minimum_required(VERSION 3.16)

project(Lightspeed)

# Set options
set(SFML_STATIC_LIBS FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")
set(LIGHTSPEED_STATIC_STD_LIBS FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML.")

# Make sure that the runtime library gets link statically
if(LIGHTSPEED_STATIC_STD_LIBS)
    if(NOT SFML_STATIC_LIBS)
        message("\n-> If you check LIGHTSPEED_STATIC_STD_LIBS, you also need to check SFML_STATIC_LIBRARIES.")
        message("-> It would lead to multiple runtime environments which result in undefined behavior.\n")
    elseif(WIN32 AND MSVC)
        set_property(TARGET SmallGameEngine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        # Note: Doesn't work for TDM compiler, since it's compiling the runtime libs statically by default
        target_compile_options(SmallGameEngine PRIVATE -static)
    endif()
endif()

# Make sure that FindSFML.cmake searches for the static libraries
if(SFML_STATIC_LIBS)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

# Find SFML
find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)

set(
    SOURCES
    src/Application.hpp
    src/Block.hpp
    src/MenuState.hpp
    src/Player.hpp
    src/PlayState.hpp
    src/State.hpp
    src/Strip.hpp
    src/Utility.hpp
    src/Application.cpp
    src/Block.cpp
    src/MenuState.cpp
    src/Player.cpp
    src/PlayState.cpp
    src/Strip.cpp
    src/main.cpp
)

# Tell CMake to build a executable
add_executable(Lightspeed ${SOURCES})

# Lightspeed uses C++11 features
target_compile_features(Lightspeed PRIVATE cxx_std_17)

# Link SFML
target_link_libraries(Lightspeed sfml-graphics sfml-audio)

# Install executable
install(TARGETS Lightspeed
        RUNTIME DESTINATION .)

# Install game data
install(DIRECTORY bin/assets
        DESTINATION .)
