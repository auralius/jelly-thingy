cmake_minimum_required(VERSION 2.8)

project(jelly-thingy)

set(CMAKE_BUILD_TYPE Debug)

add_library(jelly-thingy STATIC src/DeformableObject.cpp src/GLWindow.cpp src/Integrator.cpp src/Node.cpp src/RigidSphere.cpp ext/zpr.c)

include_directories(/usr/include/GL/ inc ext)

link_directories(/usr/local/lib)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

INSTALL(
  TARGETS jelly-thingy
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT library
)

target_link_libraries(jelly-thingy)

set_target_properties(jelly-thingy PROPERTIES COMPILE_FLAGS " -frounding-math")