
add_executable(${PROJECT_NAME} main.cpp ../)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_definitions(DEBUG_MODE)

add_subdirectory(graphics)
add_subdirectory(utils)
add_subdirectory(generate)

target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/graphics)
target_link_libraries(${PROJECT_NAME} Graphics)

target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/graphics/opengl)
target_link_libraries(${PROJECT_NAME} Opengl)

target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/utils)
target_link_libraries(${PROJECT_NAME} Utils)

target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/generate)
target_link_libraries(${PROJECT_NAME} Generate)