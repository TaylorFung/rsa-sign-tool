cmake_minimum_required(VERSION 3.10)
project(rsa_sign_tool)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
   pkg_search_module(OPENSSL REQUIRED openssl)
   include_directories(${OPENSSL_INCLUDE_DIRS})
   target_link_libraries(rsa_sign_tool ${OPENSSL_LIBRARIES})
# 包含头文件目录
include_directories(include)

# 添加可执行文件
add_executable(rsa_sign_tool 
    src/rsa_tool.cpp 
    main.cpp)

# 链接OpenSSL库
target_link_libraries(rsa_sign_tool OpenSSL::SSL OpenSSL::Crypto)
