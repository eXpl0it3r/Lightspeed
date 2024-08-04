cmake_minimum_required(VERSION 3.28)

project(Lightspeed LANGUAGES CXX)

# Set options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(SFML_USE_STATIC_STD_LIBS "Choose whether SFML is linked statically or not." ON)
option(LIGHTSPEED_STATIC_STD_LIBS "Use statically linked standard/runtime libraries? This option must match the one used for SFML." ON)

# Build SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(SFML)

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

# Make sure that the runtime library gets link statically
if(LIGHTSPEED_STATIC_STD_LIBS)
    if(NOT SFML_USE_STATIC_STD_LIBS)
        message("\n-> If you check LIGHTSPEED_STATIC_STD_LIBS, you also need to check SFML_USE_STATIC_STD_LIBS.")
        message("-> It would lead to multiple runtime environments which result in undefined behavior.\n")
    elseif(WIN32 AND MSVC)
        set_property(TARGET Lightspeed PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        # Note: Doesn't work for TDM compiler, since it's compiling the runtime libs statically by default
        target_compile_options(Lightspeed PRIVATE -static)
    endif()
endif()

# Lightspeed uses C++11 features
target_compile_features(Lightspeed PRIVATE cxx_std_17)

# Link SFML
target_link_libraries(Lightspeed PRIVATE sfml-graphics sfml-audio)

if(WIN32)
    set_target_properties(Lightspeed PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries(Lightspeed PRIVATE sfml-main)
endif()

# Install executable
install(TARGETS Lightspeed
        RUNTIME DESTINATION .)

# Install game data
install(DIRECTORY bin/assets
        DESTINATION .)

# Install OpenAL
install(DIRECTORY ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
        DESTINATION .)