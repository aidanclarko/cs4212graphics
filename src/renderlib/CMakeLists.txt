include_directories(${CMAKE_SOURCE_DIR}/src)

add_library (cs4212-renderlib
  vec3.h
  Framebuffer.cpp Framebuffer.h
  Ray.h
  Camera.h PerspectiveCamera.h
  Shape.h Sphere.h
  Scene.cpp Scene.h
  Shader.cpp Shader.h
  SceneParser_JSON.cpp
  Particles.h 
  Fireworks.h
  ParticleObj.h
  ParticleWave.h
)

target_link_libraries(cs4212-renderlib PUBLIC PNG::PNG)
target_include_directories(cs4212-renderlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})