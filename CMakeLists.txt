project(QGLViz)
cmake_minimum_required(VERSION 2.8.12)
set (CMAKE_CXX_STANDARD 11)

# Qt library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
# Eigen
find_package(Eigen3 REQUIRED)
# OpenGL & GLEW library
find_package(OpenGL)

include_directories(${EIGEN3_INCLUDE_DIR})

# executable
add_executable(QGLViz src/main.cpp)
qt5_use_modules(QGLViz Widgets OpenGL)
target_link_libraries(QGLViz ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
