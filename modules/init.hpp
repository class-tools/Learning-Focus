/*
Learning Focus Initialization Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

void LF_Init_Dir() {
	// Initialize directories.
	if (std::filesystem::is_directory(LF_Path_Data)) {
		std::filesystem::create_directory(LF_Path_Data);
	}
	if (std::filesystem::is_directory(LF_Path_Data + "/logs")) {
		std::filesystem::create_directory(LF_Path_Data + "/logs");
	}
}

void LF_Init_Bind() {
	// Initialize bindings.
	Arg_Parser.add_argument("-c", "--camera").default_value<int32_t>(0).nargs(1).help("specify the camera id of the video capturing device to use").scan<'i', int32_t>();
	Arg_Parser.add_argument("-dl", "--datalist").default_value<bool>(false).implicit_value(true).nargs(0).help("show data list");
	Arg_Parser.add_argument("-ds", "--datashow").default_value<std::string>("").nargs(1).help("visualize data with the key specified");
	Arg_Parser.add_argument("-e", "--extra").default_value<bool>(false).implicit_value(true).nargs(0).help("show extra information");
	Json_SWB.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();
}

void LF_Init_Sig() {
	// Initialize signals.
	signal(SIGINT, []([[maybe_unused]] int32_t signum) {
		exit(0);
	});
	signal(SIGABRT, [](int32_t signum) {
		SPDLOG_CRITICAL(fmt::format("Signal {} detected (Program aborted)", signum));
		exit(signum);
	});
	signal(SIGFPE, [](int32_t signum) {
		SPDLOG_CRITICAL(fmt::format("Signal {} detected (Operation overflow)", signum));
		exit(signum);
	});
	signal(SIGILL, [](int32_t signum) {
		SPDLOG_CRITICAL(fmt::format("Signal {} detected (Illegal instruction)", signum));
		exit(signum);
	});
	signal(SIGSEGV, [](int32_t signum) {
		SPDLOG_CRITICAL(fmt::format("Signal {} detected (Access to illegal memory)", signum));
		exit(signum);
	});
}

void LF_Init_Log() {
	// Initialize logger.
	timestamp now = GetTimestamp();
	Log_Sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("{}/logs/{}.log", LF_Path_Data, GetTimestampString()));
	Log_Sink->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] [%s:%# %!] [Process %P] [Thread %t]: %v.");
	Log_Logger = make_shared<spdlog::logger>("LF", Log_Sink);
	Log_Logger->flush_on(spdlog::level::debug);
	spdlog::register_logger(Log_Logger);
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif
	spdlog::set_default_logger(Log_Logger);
	LF_Version = LF_VER_MAIN;
#if LF_VER_TYPE != 0
	#if LF_VER_TYPE == 1
	LF_Version.append(" Alpha");
	#elif LF_VER_TYPE == 2
	LF_Version.append(" Beta");
	#elif LF_VER_TYPE == 3
	LF_Version.append(" Rc");
	#endif
	LF_Version.append(fmt::format(" {}", LF_VER_BUILD));
#endif
#ifdef _WIN32
	LF_Version.append(" (Windows)");
#elif linux
	LF_Version.append(" (Linux)");
#endif
	SPDLOG_INFO(fmt::format("Starting \"Learning Focus {}\"", LF_Version));
}

void LF_Init() {
	// Initialize program.
	atexit(LF_Exit);
	LF_Init_Dir();
	LF_Init_Bind();
	LF_Init_Sig();
	LF_Init_Log();
}
