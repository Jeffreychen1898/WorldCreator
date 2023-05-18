add_library(Generate
    perlinNoise.cpp
    worleyNoise.cpp
)

target_include_directories(Generate PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glm
)