add_library(GeometryCache
    geometryCache.cpp
    shape.cpp
)

target_include_directories(GeometryCache PUBLIC
    ${PROJECT_SOURCE_DIR}/deps/glad
    ${PROJECT_SOURCE_DIR}/deps/glm
)