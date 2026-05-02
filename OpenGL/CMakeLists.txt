include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable (finalRender
  finalRender.cpp 
  GLSL.cpp GLSL.h
)

target_link_libraries (finalRender PRIVATE GLEW::GLEW)
target_link_libraries (finalRender PRIVATE glfw)

if(APPLE)
    target_link_libraries(finalRender PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(finalRender PRIVATE OpenGL::GL)
endif()

target_link_libraries (finalRender PRIVATE PNG::PNG)
target_link_libraries (finalRender PRIVATE ZLIB::ZLIB)
target_link_libraries(finalRender PRIVATE cs4212-renderlib)

