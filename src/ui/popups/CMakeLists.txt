add_library(Popups
    popups.cpp
)

target_include_directories(Popups PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
    ${PROJECT_SOURCE_DIR}/deps/imgui
    ${PROJECT_SOURCE_DIR}/deps/glm
)