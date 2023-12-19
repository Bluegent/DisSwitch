cmake_minimum_required(VERSION 3.2)
project(DisSwitchLib)

# Add your source files from the source folder
file(GLOB_RECURSE PROJECT_SRC RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        include/*.cpp
        include/*.hpp
        src/*.cpp
        src/*.hpp
        )
message("${PROJECT_NAME} Sources: ${PROJECT_SRC}")
foreach(source IN LISTS PROJECT_SRC)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC include)
include_directories(include)
