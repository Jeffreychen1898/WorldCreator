add_library(UI
    userInterface.cpp
    uiTools.cpp
    popupUI.cpp
)

target_include_directories(UI PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
    ${PROJECT_SOURCE_DIR}/deps/imgui
    ${PROJECT_SOURCE_DIR}/deps/glm
)