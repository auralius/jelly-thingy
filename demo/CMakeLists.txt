cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

add_library(jelly-thingy STATIC IMPORTED)
 
set_property(TARGET jelly-thingy PROPERTY IMPORTED_LOCATION ../lib/libjelly-thingy.a)

   
project(demo_bunny)

add_executable(demo_bunny demo_bunny.cpp)

install(TARGETS demo_bunny RUNTIME DESTINATION bin)

set_target_properties(demo_bunny PROPERTIES COMPILE_FLAGS " -frounding-math")

include_directories(/usr/include/GL/ ../inc/ ../ext/)

link_directories(/usr/local/lib)

target_link_libraries(demo_bunny jelly-thingy Irrlicht CGAL boost_thread boost_system glut GL GLU gmp gomp lapack armadillo)


project(demo_cube)

add_executable(demo_cube demo_cube.cpp)

install(TARGETS demo_cube RUNTIME DESTINATION bin)

set_target_properties(demo_cube PROPERTIES COMPILE_FLAGS " -frounding-math")

include_directories(/usr/include/GL/ ../inc/ ../ext/)

link_directories(/usr/local/lib)

target_link_libraries(demo_cube jelly-thingy Irrlicht CGAL boost_thread boost_system glut GL GLU gmp gomp lapack armadillo)
