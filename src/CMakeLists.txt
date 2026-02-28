add_subdirectory(renderlib)

include_directories(${CMAKE_SOURCE_DIR}/src/renderlib)
add_library (cs4212-util
  ArgumentParsing.cpp ArgumentParsing.h
  handleGraphicsArgs.cpp handleGraphicsArgs.h
  model_obj.cpp model_obj.h
)
add_executable(
  render_main render_main.cpp
)

set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
set(OpenMP_CXX_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY /opt/homebrew/opt/libomp/lib/libomp.dylib)

find_package(OpenMP REQUIRED)
target_link_libraries(render_main OpenMP::OpenMP_CXX)
target_link_libraries(render_main cs4212-renderlib cs4212-util)
target_compile_definitions(cs4212-util PUBLIC HAS_GLM)
target_link_libraries(cs4212-util PRIVATE Boost::program_options)
target_link_libraries(cs4212-util PUBLIC glm::glm)
target_link_libraries(cs4212-util PRIVATE cs4212-renderlib)