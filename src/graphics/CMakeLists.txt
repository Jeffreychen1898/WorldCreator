add_library(Graphics
    render.cpp
    window.cpp
    camera.cpp
)

target_include_directories(Graphics PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
    ${PROJECT_SOURCE_DIR}/deps/glm
)

add_subdirectory(opengl)