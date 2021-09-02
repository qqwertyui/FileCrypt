cmake_minimum_required(VERSION 3.19.4)
project(Filecrypt VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC_FILES
    "src/*.cpp"
)
add_executable(fcrypt)

target_sources(fcrypt PUBLIC ${SRC_FILES})
target_include_directories(fcrypt PUBLIC include)
configure_file(include/Version.hpp.in ${CMAKE_SOURCE_DIR}/include/Version.hpp)

target_link_libraries(fcrypt PUBLIC cryptopp)
target_link_libraries(fcrypt PUBLIC boost_filesystem)
target_link_libraries(fcrypt PUBLIC gflags_static)
target_link_libraries(fcrypt PUBLIC shlwapi)
