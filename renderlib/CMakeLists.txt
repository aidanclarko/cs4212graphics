include_directories(${CMAKE_SOURCE_DIR}/src)

add_library (cs4212-renderlib
  vec3.h
  Framebuffer.cpp Framebuffer.h
)

target_link_libraries(cs4212-renderlib PRIVATE PNG::PNG)