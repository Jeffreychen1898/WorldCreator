add_library(Generate
    perlinNoise.cpp
)

target_include_directories(Generate PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glm
)