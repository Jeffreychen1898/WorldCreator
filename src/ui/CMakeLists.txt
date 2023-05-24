add_library(UI
    userInterface.cpp
)

target_include_directories(UI PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
    ${PROJECT_SOURCE_DIR}/deps/imgui
)