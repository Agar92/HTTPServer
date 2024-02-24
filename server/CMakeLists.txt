cmake_minimum_required(VERSION 3.15)

project(HTTPServer_example CXX)

file(GLOB HEADERS "*.hpp")
file(GLOB SOURCES "*.cpp")
list(REMOVE_ITEM SOURCES "/home/andrey/Desktop/HTTPServer/http/main.cpp")

set(TARGET "http-server")
add_executable(${TARGET} main.cpp ${SOURCES} ${HEADERS})

find_package (Boost REQUIRED)
target_include_directories(${TARGET} PRIVATE
	${Boost_INCLUDE_DIR}
)

target_link_libraries(${TARGET} PRIVATE
    ${Boost_LIBRARIES}
)

add_compile_options(-g)



find_program(CLANG_FORMAT_APP clang-format)
###message("CLANG_FORMAT_APP=${CLANG_FORMAT_APP}")
include(ClangFormat.cmake)
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.cxx *.h *.hpp)
add_custom_target(
        clangformat
        COMMAND ${CLANG_FORMAT_APP}
        -style=file
        -i
        ${ALL_SOURCE_FILES}
)