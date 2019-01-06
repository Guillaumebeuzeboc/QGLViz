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

# Install
install(TARGETS QGLViz
  DESTINATION bin)

set(CMAKE_PROJECT_NAME "QGLViz")
include(${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)
if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
include(InstallRequiredSystemLibraries)
set(CPACK_SET_DESTDIR "on")
set(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION "QGLViz my own viz")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It's a POC of what a visaualization tool could be")
set(CPACK_PACKAGE_VENDOR "beuzecorp")
set(CPACK_PACKAGE_CONTACT "guillaume.beuzeboc@gmail.com")
set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")

set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # auto detect dependencies

set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION "beuz")
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
include(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
