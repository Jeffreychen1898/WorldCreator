
add_executable(${PROJECT_NAME} main.cpp ../)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#add_subdirectory(render)

#target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/render/)
#target_link_libraries(${PROJECT_NAME} Render)

