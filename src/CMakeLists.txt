add_subdirectory(renderlib)

include_directories(${CMAKE_SOURCE_DIR}/src/renderlib)
add_library (cs4212-util
  ArgumentParsing.cpp ArgumentParsing.h
  handleGraphicsArgs.cpp handleGraphicsArgs.h
  model_obj.cpp model_obj.h
)
add_executable(
  ani_pg ani_pg.cpp
)
target_link_libraries(ani_pg PNG::PNG cs4212-renderlib cs4212-util)
target_compile_definitions(cs4212-util PUBLIC HAS_GLM)
target_link_libraries(cs4212-util PRIVATE Boost::program_options)
target_link_libraries(cs4212-util PUBLIC glm::glm)
target_link_libraries(cs4212-util PRIVATE cs4212-renderlib)