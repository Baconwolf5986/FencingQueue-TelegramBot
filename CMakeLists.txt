# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project Name and Version
project(FencingQueue VERSION 1.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vcpkg to handle dependencies
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find the packages (dependencies)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# Add the executable (your main program)
add_executable(FencingQueue 
main.cpp
StripQueue.cpp
)

# Link libraries to the executable
target_link_libraries(FencingQueue
    C:/src/vcpkg/installed/x64-windows/lib/TgBot.lib
    Boost::system         # Linking Boost System
    OpenSSL::SSL          # Linking OpenSSL (SSL)
    OpenSSL::Crypto       # Linking OpenSSL (Crypto)
    ZLIB::ZLIB            # Linking Zlib
)
