cmake_minimum_required(VERSION 3.18.4)
project(eval VERSION 0.1.0)

add_executable(${PROJECT_NAME} ${QUESTION}.cpp)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_dependencies(${PROJECT_NAME} OpenGL::OpenGL)

include_directories(${GLUT_INCLUDE_DIRS} )
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
