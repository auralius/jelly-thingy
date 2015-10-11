cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
   
project(demo_bunny)

add_executable(demo_bunny demo_bunny.cpp)

install(TARGETS demo_bunny RUNTIME DESTINATION bin)

add_library(jelly-thingy STATIC IMPORTED)
 
set_property(TARGET jelly-thingy PROPERTY IMPORTED_LOCATION ../lib/libjelly-thingy.a)

set_target_properties(demo_bunny PROPERTIES COMPILE_FLAGS " -frounding-math")

include_directories(/usr/include/GL/ ../inc/ ../ext/)

link_directories(/usr/local/lib)

target_link_libraries(demo_bunny jelly-thingy Irrlicht CGAL boost_thread boost_system glut GL GLU gmp gomp lapack armadillo)
