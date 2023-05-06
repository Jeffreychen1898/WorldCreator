cmake_minimum_required(VERSION 3.18.4)

project(WorldCreator)

add_subdirectory(src)
add_subdirectory(deps/glad)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
)

target_link_libraries(${PROJECT_NAME}
    GLAD
    GL
    dl
    glfw
)