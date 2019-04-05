cmake_minimum_required (VERSION 2.8)

project (bplustree)

# 设置编译参数
set(CMAKE_C_FLAGS                     "-Wall -Werror -Wno-attributes")
set(CMAKE_C_FLAGS_DEBUG               "-g -DDEBUG") #-fstack-protector-all
set(CMAKE_C_FLAGS_RELEASE             "-O3")
set(CMAKE_C_FLAGS_MINSIZEREL          "-Os")
set(CMAKE_C_RELWITHDEBINFO_RELEASE    "-O2")
set(CMAKE_CXX_FLAGS                   " -O2 -g -std=\"c++11\"")
set(CMAKE_CXX_FLAGS_DEBUG             "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE           "-O3 -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS_MINSIZEREL        "-Os")
set(CMAKE_CXX_RELWITHDEBINFO_RELEASE  "-O2")
set(LIB_BPLUSTREE_NAME bplustree)

add_subdirectory(lib)
add_subdirectory(tests)