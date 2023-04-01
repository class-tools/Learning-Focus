# Learning Focus CMake Lists File 2.0.0
# This source code file is under MIT License.
# Copyright (c) 2022 - 2023 Class Tools Develop Team
# Contributors: ren-yc
cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(LF)

add_executable(LF "LF.cpp" "version.hpp" "modules/apis.hpp" "modules/basic.hpp" "modules/file-process.hpp" "modules/functions.hpp" "modules/init.hpp" "$<$<BOOL:MSVC>:LF.rc>")

set_property(TARGET LF PROPERTY CXX_STANDARD 20)

if(MSVC)
	if(POLICY CMP0091)
		cmake_policy(SET CMP0091 NEW)
	endif()
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set_property(TARGET LF PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		set_property(TARGET LF PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	endif()
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	target_compile_options(LF PRIVATE /W3)
	set(VCPKG_TARGET_TRIPLET "x64-windows-static")
else()
	target_compile_options(LF PRIVATE -Wall -Wextra -Wpedantic)
	set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

if(NOT DEFINED VCPKG_ROOT)
	if(DEFINED ENV{VCPKG_ROOT})
		set(VCPKG_ROOT ENV{VCPKG_ROOT})
	else()
		message(FATAL_ERROR "VCPKG_ROOT is not defined.")
	endif()
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include")
find_package(argparse CONFIG REQUIRED)
find_package(dlib CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(LF argparse::argparse dlib::dlib fmt::fmt jsoncpp_static JsonCpp::JsonCpp opencv_core opencv_highgui opencv_imgproc opencv_videoio spdlog::spdlog)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/shape_predictor_68_face_landmarks.dat)
	message(FATAL_ERROR "Cannot find shape_predictor_68_face_landmarks.dat.\nDownload and decompress it from http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2.")
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shape_predictor_68_face_landmarks.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
