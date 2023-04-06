/*
Learning Focus Basic Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef linux
	#include <unistd.h>
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <argparse/argparse.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/misc_api.h>
#include <dlib/opencv.h>
#include <fmt/format.h>
#include <json/json.h>
#include <mgl2/mgl.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

struct timestamp {
	int32_t Year = 0;
	int32_t Month = 0;
	int32_t Day = 0;
	int32_t Hour = 0;
	int32_t Minute = 0;
	int32_t Second = 0;
	int32_t MilliSecond = 0;
	int32_t MicroSecond = 0;
	int32_t NanoSecond = 0;
};

#ifdef _WIN32
const std::string LF_Path_Data = fmt::format("{}/.class-tools/LF", getenv("APPDATA"));
#elif linux
const std::string LF_Path_Data = fmt::format("{}/.class-tools/LF", getenv("HOME"));
#endif
std::string LF_Version;
argparse::ArgumentParser Arg_Parser("LF", LF_VER_MAIN, argparse::default_arguments::all);
Json::Reader Json_Reader;
Json::StreamWriterBuilder Json_SWB;
std::unique_ptr<Json::StreamWriter> Json_SW(Json_SWB.newStreamWriter());
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Log_Sink;
std::shared_ptr<spdlog::logger> Log_Logger;
