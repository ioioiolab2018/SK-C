cmake_minimum_required(VERSION 3.12)
project(SK)

set(CMAKE_CXX_STANDARD 11)

add_executable(SK main.cpp Commons.h Commons.cpp messageTools.cpp messageTools.h serverStatements.h connectionHandler.cpp connectionHandler.h connectionFunctions.cpp connectionFunctions.h)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SK Threads::Threads)