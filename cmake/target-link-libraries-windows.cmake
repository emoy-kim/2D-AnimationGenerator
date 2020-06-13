target_link_libraries(2DAnimationGenerator glad glfw3dll)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
   target_link_libraries(2DAnimationGenerator FreeImaged)
else()
   target_link_libraries(2DAnimationGenerator FreeImage)
endif()