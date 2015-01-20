cmake_minimum_required(VERSION 2.8.9)

project(JavaLauncher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/lib/jvm/java-8-openjdk/include /usr/lib/jvm/java-8-openjdk/include/linux)
include_directories(src)

set(JVM_LIBRARY_PATH "" CACHE FILEPATH "Path to the JVM library (libjvm.so/jvm.dll/libjvm.dylib)")

add_definitions(-DJVM_LIBRARY_PATH="${JVM_LIBRARY_PATH}")

add_library(javalauncher STATIC src/JavaLauncher.cpp)

add_executable(tst_javalauncher tests/main.cpp)
target_link_libraries(tst_javalauncher javalauncher dl)
