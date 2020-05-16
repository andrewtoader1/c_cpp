/*
    mm_test.cpp
    @Author: Haihui Zhu and Andrew Toader
    @Purpose: Test the memory model simulation
*/

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>

#include "memory_model.h"

// Test-1: no miss
TEST_CASE( "Test-1", "[MemoryModel]" ) {
    MemoryModel mm(1);
    int L1_set[] = {1,2,3};
    int L2_set[] = {4,5,6};
    int L3_set[] = {7,8,9};
    mm.initMemory(std::set<int>(L1_set, L1_set+3), std::set<int>(L2_set, L2_set+3), std::set<int>(L3_set, L3_set+3), std::set<int>());
    mm.add(1, 1);
    mm.add(2, 2);
    mm.add(3, 3);
    mm.add(4, 4);
    mm.add(5, 5);
    mm.add(6, 6);
    mm.add(7, 7);
    mm.add(8, 8);
    mm.add(9, 9);
    std::string result = mm.exec();

    std::ifstream fs("./test/1.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}

// Test-2: mix
TEST_CASE( "Test-2", "[MemoryModel]" ) {
    MemoryModel mm(1);
    int L1_set[] = {1,2,3};
    int L2_set[] = {4,5,6};
    int L3_set[] = {7,8,9};
    mm.initMemory(std::set<int>(L1_set, L1_set+3), std::set<int>(L2_set, L2_set+3), std::set<int>(L3_set, L3_set+3), std::set<int>());
    mm.add(1, 1);
    mm.add(2, 4);
    mm.add(2, 3);
    mm.add(5, 5);
    mm.add(10, 2);
    mm.add(11, 8);
    mm.add(15, 9);
    mm.add(16, 7);
    mm.add(20, 10);
    std::string result = mm.exec();

    std::ifstream fs("./test/2.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}

// Test-3: mix
TEST_CASE( "Test-3", "[MemoryModel]" ) {
    MemoryModel mm(1);
    int L1_set[] = {5,6,7};
    int L2_set[] = {8,9};
    int L3_set[] = {10};
    int MM_set[] = {11, 12, 13};
    mm.initMemory(std::set<int>(L1_set, L1_set+3), std::set<int>(L2_set, L2_set+2), std::set<int>(L3_set, L3_set+1), std::set<int>(MM_set, MM_set+3));
    mm.add(5, 2);
    mm.add(7, 5);
    mm.add(7, 7);
    mm.add(10, 9);
    mm.add(12, 10);
    mm.add(20, 11);
    mm.add(20, 12);
    mm.add(21, 13);
    std::string result = mm.exec();

    std::ifstream fs("./test/3.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}

// Test variant 2 mix
TEST_CASE( "Test-4", "[MemoryModel]" ) {
    MemoryModel mm(2);
    int L1_set[] = {1,2,3};
    int L2_set[] = {4,5,6};
    int L3_set[] = {7,8};
    int MM_set[] = {11};
    mm.initMemory(std::set<int>(L1_set, L1_set+3), std::set<int>(L2_set, L2_set+3), std::set<int>(L3_set, L3_set+2), std::set<int>(MM_set, MM_set+1));
    mm.add(1,1);
    mm.add(1,4);
    mm.add(2,3);
    mm.add(5,7);
    mm.add(6,2);

    std::string result = mm.exec();

    std::ifstream fs("./test/4.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}


// Test-5: student test variant 1
TEST_CASE( "Test-5", "[MemoryModel]" ) {
    MemoryModel mm(1);
    int L1_set[] = {3};
    int L2_set[] = {1};
    int L3_set[] = {6};
    int MM_set[] = {5};
    mm.initMemory(std::set<int>(L1_set, L1_set+1), std::set<int>(L2_set, L2_set+1), std::set<int>(L3_set, L3_set+1), std::set<int>(MM_set, MM_set+1));
    mm.add(1, 3);
    mm.add(8, 1);
    mm.add(23, 6);
    mm.add(57, 5);

    std::string result = mm.exec();

    std::ifstream fs("./test/5.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}

// Test-6: test 5 using variant 2
TEST_CASE( "Test-6", "[MemoryModel]" ) {
    MemoryModel mm(2);
    int L1_set[] = {3};
    int L2_set[] = {1};
    int L3_set[] = {6};
    int MM_set[] = {5};
    mm.initMemory(std::set<int>(L1_set, L1_set+1), std::set<int>(L2_set, L2_set+1), std::set<int>(L3_set, L3_set+1), std::set<int>(MM_set, MM_set+1));
    mm.add(1, 3);
    mm.add(8, 1);
    mm.add(23, 6);
    mm.add(57, 5);

    std::string result = mm.exec();

    std::ifstream fs("./test/6.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}

// Test-7: test 7 using variant 1
TEST_CASE( "Test-7", "[MemoryModel]" ) {
    MemoryModel mm(1);
    int L1_set[] = {6, 10};
    int L2_set[] = {5};
    int L3_set[] = {7, 12};
    int MM_set[] = {20};
    mm.initMemory(std::set<int>(L1_set, L1_set+2), std::set<int>(L2_set, L2_set+1), std::set<int>(L3_set, L3_set+2), std::set<int>(MM_set, MM_set+1));
    mm.add(2, 12);
    mm.add(4, 5);
    mm.add(4, 20);
    mm.add(4, 6);
    mm.add(10, 7);
    mm.add(11, 10);

    std::string result = mm.exec();

    std::ifstream fs("./test/7.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}

// Test-8: test 7 using variant 2
TEST_CASE( "Test-8", "[MemoryModel]" ) {
    MemoryModel mm(2);
    int L1_set[] = {6, 10};
    int L2_set[] = {5};
    int L3_set[] = {7, 12};
    int MM_set[] = {20};
    mm.initMemory(std::set<int>(L1_set, L1_set+2), std::set<int>(L2_set, L2_set+1), std::set<int>(L3_set, L3_set+2), std::set<int>(MM_set, MM_set+1));
    mm.add(2, 12);
    mm.add(4, 5);
    mm.add(4, 20);
    mm.add(4, 6);
    mm.add(10, 7);
    mm.add(11, 10);

    std::string result = mm.exec();

    std::ifstream fs("./test/8.txt");
    std::string ans((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());  
    REQUIRE(result.compare(0, result.length()-2, ans, 0, result.length()-2) == 0);
}