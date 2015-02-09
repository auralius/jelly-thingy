cmake_minimum_required(VERSION 2.8)

project(jelly-thingy)
set(CMAKE_BUILD_TYPE Degug)

add_library(jelly-thingy SHARED src/DeformableObject.cpp src/GLWindow.cpp src/Integrator.cpp src/Node.cpp src/RigidSphere.cpp ext/zpr.c ext/glm.cpp)

include_directories(/usr/include/GL/ inc ext)

link_directories(/usr/local/lib)

install(TARGETS jelly-thingy LIBRARY DESTINATION bin)

target_link_libraries(jelly-thingy Irrlicht)