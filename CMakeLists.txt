cmake_minimum_required(VERSION 3.20)
project(StickFigureGame VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vcpkg packages
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/World.cpp
    src/Renderer.cpp
    src/InputManager.cpp
    src/player/BodyPart.cpp
    src/player/StickFigureBody.cpp
)

set(HEADERS
    include/Game.h
    include/Player.h
    include/World.h
    include/Renderer.h
    include/InputManager.h
    include/Vector2.h
    include/player/BodyPart.h
    include/player/StickFigureBody.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
    SDL2_image::SDL2_image
    glm::glm
)

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})