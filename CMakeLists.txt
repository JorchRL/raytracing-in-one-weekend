cmake_minimum_required(VERSION 3.30)
project(simpleGame LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 20)

## Specify the CUDA standard
#set(CMAKE_CUDA_STANDARD 20)
#set(CMAKE_CUDA_STANDARD_REQUIRED YES)
#
## Enable modern architectures for the NVIDIA GPU
#set(CMAKE_CUDA_ARCHITECTURES 50 60 70 75 80)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    endif ()
endif ()

# project (windows)
add_executable(${PROJECT_NAME} main.cpp
        Framebuffer.cpp
        Framebuffer.h
        vec3.h
        ray.h
        hittable.h
        sphere.h
        hittable_list.h
        rtweekend.h
        interval.h
        camera.h
        material.h
        #        RayTracing.cu
)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

#set_source_files_properties(RayTracing.cu PROPERTIES LANGUAGE CUDA)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()