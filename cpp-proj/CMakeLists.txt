cmake_minimum_required(VERSION 3.15)
project(cpp_proj)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_proj main.cpp util/result.h util/clause.cpp util/clause.h util/bitset.h util/bitset.cpp util/random.h util/random.cpp src/methods.cpp src/methods.h src/testRunner.cpp src/testRunner.h config.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cpp_proj Threads::Threads)