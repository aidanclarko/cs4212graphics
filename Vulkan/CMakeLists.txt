include_directories(${CMAKE_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${Vulkan_INCLUDE_DIRS})
LINK_LIBRARIES(${Vulkan_LIBRARY}) 

add_executable (basicCode
  basicCode.cpp
)
target_link_libraries (basicCode ${GLEW_LIBRARIES})
target_link_libraries (basicCode glfw ${GLFW_STATIC_LIBRARIES})
target_link_libraries (basicCode ${Vulkan_LIBRARIES})
target_link_libraries (basicCode ${PNG_LIBRARIES})
target_link_libraries (basicCode ${ZLIB_LIBRARIES})

add_executable (glfwVulkan
  glfwVulkan.cpp
)
target_link_libraries (glfwVulkan ${GLEW_LIBRARIES})
target_link_libraries (glfwVulkan glfw ${GLFW_STATIC_LIBRARIES})
target_link_libraries (glfwVulkan ${Vulkan_LIBRARIES})
target_link_libraries (glfwVulkan ${PNG_LIBRARIES})
target_link_libraries (glfwVulkan ${ZLIB_LIBRARIES})
