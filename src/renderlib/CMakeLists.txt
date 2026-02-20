include_directories(${CMAKE_SOURCE_DIR}/src)

add_library (cs4212-renderlib
  vec3.h
  Framebuffer.cpp Framebuffer.h
  Ray.h
  Camera.h PerspectiveCamera.h
  Shape.h Sphere.h
  Scene.cpp Scene.h
)