cmake_minimum_required(VERSION 3.18.4)

project(WorldCreator)

add_subdirectory(deps/glad)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME}
    GLAD
    GL
    dl
    glfw
)