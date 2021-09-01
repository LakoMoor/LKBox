cmake_minimum_required(VERSION 3.0.0)
project(LKBox VERSION 0.1.0)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON) 

# Dear ImGui
add_subdirectory(lib/imgui)

include_directories(lib/imgui/include)
link_directories(lib/imgui/lib)


#glfw
add_subdirectory(lib/glfw)

include_directories(lib/glfw/include)
link_directories(lib/glfw/lib)

include_directories(lib/glad/include)


#glad
add_subdirectory(lib/glad/)

#glm
add_subdirectory(lib/glm)

include_directories(lib/glm)
link_directories(lib/glm/lib)

#assimp
add_subdirectory(lib/assimp)

include_directories(lib/assimp/include)
link_directories(lib/assimp/lib)

#stb
add_subdirectory(lib/stb)

include_directories(lib/stb/include)
link_directories(lib/stb/lib)


#Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EXECUTABLE_OUTPUT_PATH "../Building/Debug/bin")
endif()

#Release (Hide console)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(EXECUTABLE_OUTPUT_PATH "../Building/Realese/bin")
        set(CMAKE_CXX_FLAGS "-mwindows")
endif()

#set(CMAKE_CXX_FLAGS "-mwindows")

#for icon =   windres LKBox.rc -o icon.o 

add_executable(
        LKBox
        src/Info/lkbox.o
        src/main.cpp 
        src/Core/Core.cpp

)
target_link_libraries(LKBox PUBLIC glfw ${GLFW_LIBRARY} glad imgui glm ${GLM_LIBRARY} assimp ${ASSIMP_LIBRARY} stb)