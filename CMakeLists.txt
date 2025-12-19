cmake_minimum_required(VERSION 3.18)
project(FB_to_SVG VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.14
)

FetchContent_MakeAvailable(pugixml)

set(YOUR_SOURCES
    src/main.cpp
    src/svg_generator.cpp
    src/xml_parser.cpp
)

set(YOUR_HEADERS
    include/fb_types.h
)

add_executable(FB_to_SVG ${YOUR_SOURCES} ${YOUR_HEADERS})

target_include_directories(FB_to_SVG PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${pugixml_SOURCE_DIR}/src
)

target_link_libraries(FB_to_SVG PRIVATE 
    pugixml
)