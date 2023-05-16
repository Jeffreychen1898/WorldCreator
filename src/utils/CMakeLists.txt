add_library(Utils
    verticesArray.cpp
    helperFunctions.cpp
)

target_include_directories(Utils PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
)