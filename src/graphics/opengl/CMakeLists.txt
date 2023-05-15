add_library(Opengl
    opengl.cpp
    shader.cpp
    texture.cpp
)

target_include_directories(Opengl PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
)