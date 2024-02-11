cmake_minimum_required(VERSION 3.15)

project(HTTPServer_example CXX)

file(GLOB HEADERS "*.hpp")
file(GLOB SOURCES "*.cpp")
list(REMOVE_ITEM SOURCES "/home/andrey/Desktop/HTTPServer/http/main.cpp")

add_executable(http-server main.cpp ${SOURCES} ${HEADERS})

find_package (Boost REQUIRED)
target_include_directories(http-server PRIVATE
	${Boost_INCLUDE_DIR}
)

target_link_libraries(http-server PRIVATE
    ${Boost_LIBRARIES}
)

add_compile_options(-g)