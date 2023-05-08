add_library(Graphics
    render.cpp
    window.cpp
)

target_include_directories(Graphics PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
)

add_subdirectory(opengl)