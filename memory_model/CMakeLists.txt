cmake_minimum_required(VERSION 3.5)
project(P0 CXX)

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# create unittests

add_executable(memoryQueueTests test_memory_queue.cpp)
add_executable(memoryModelTests test_mm.cpp memory_model.cpp)

enable_testing()

# unit tests
add_test(TestingMemoryQueue memoryQueueTests)
add_test(TestingMemoryModel memoryModelTests)
