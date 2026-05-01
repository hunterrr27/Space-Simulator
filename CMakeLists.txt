cmake_minimum_required(VERSION 3.14)
project(space LANGUAGES CXX)

# 1. Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Fetch raylib from GitHub
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        master # Or a specific version like 5.0
)

# 3. Make raylib available
FetchContent_MakeAvailable(raylib)

# find raygui.h (also in Homebrew include on your machine)
find_path(RAYGUI_INCLUDE_DIR raygui.h
    PATHS /opt/homebrew/include /usr/local/include
    REQUIRED
)

# 4. Define your executable
add_executable(${PROJECT_NAME} main.cpp )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${RAYGUI_INCLUDE_DIR})

# 5. Link raylib to your executable
# 'raylib' is the name of the target provided by the fetched library
target_link_libraries(${PROJECT_NAME} raylib)

# 6. Platform-specific settings (Optional but recommended)
if(REPOS_PLATFORM STREQUAL "Web")
    # Custom flags for Emscripten if you ever target Web
else()
    # Desktop-specific flags (e.g., hide console on Windows)
    if(WIN32)
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
endif()