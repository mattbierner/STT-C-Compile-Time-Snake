cmake_minimum_required(VERSION 3.0.2)
project(stt-snake)

# Visual Studio 2015 defaults to C++11
if (NOT WIN32)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14")
endif()

file(GLOB stt_snake "${CMAKE_SOURCE_DIR}/stt-snake/*.*")

add_executable(stt-snake ${stt_snake})