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
else()
	target_compile_options(LF PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(NOT DEFINED VCPKG_ROOT)
	if(DEFINED ENV{VCPKG_ROOT})
		set(VCPKG_ROOT ENV{VCPKG_ROOT})
	else()
		if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
			set(VCPKG_ROOT "D:/vcpkg")
		else()
			set(VCPKG_ROOT "$ENV{HOME}/vcpkg")
		endif()
	endif()
endif()

include("archdetect.cmake")
target_architecture(TARGET_ARCH)
message(STATUS "Target architecture: ${TARGET_ARCH}")
add_compile_definitions(LF_Framework=\"${TARGET_ARCH}\")
include_directories("${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
find_package(jsoncpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(LF jsoncpp_static JsonCpp::JsonCpp spdlog::spdlog)
