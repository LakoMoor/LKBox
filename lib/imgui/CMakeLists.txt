cmake_minimum_required(VERSION 3.0)
project(imgui)

set(IMGUI_DIR .)
add_library(imgui SHARED
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
        ${IMGUI_DIR}/imgui.cpp 
        ${IMGUI_DIR}/imgui_draw.cpp 
        ${IMGUI_DIR}/imgui_demo.cpp 
        ${IMGUI_DIR}/imgui_tables.cpp 
        ${IMGUI_DIR}/imgui_widgets.cpp
        )
#glfw
#add_subdirectory(../glfw)

include_directories(../glfw/include)
link_directories(../glfw/lib)

include_directories(lib/glad/include)

target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
target_link_libraries(imgui PUBLIC glfw ${GLFW_LIBRARY})