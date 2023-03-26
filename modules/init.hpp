/*
Learning Focus Initialization Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

void LF_Init_Dir() {
	// Initialize directories.
	if (std::filesystem::is_directory(LF_path_data)) {
		std::filesystem::create_directory(LF_path_data);
	}
	if (std::filesystem::is_directory(LF_path_data + "/logs")) {
		std::filesystem::create_directory(LF_path_data + "/logs");
	}
}

void LF_Init_Bind() {
	// Initialize bindings.
	ARG_parser.add_argument("-c", "--camera").default_value<int32_t>(0).nargs(1).help("specify the camera id of the video capturing device to use").scan<'i', int32_t>();
	ARG_parser.add_argument("-d", "--debug").default_value<bool>(false).implicit_value(true).nargs(0).help("enable debug mode to view more output");
	JSON_SWB.settings_ = []() {
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
	LOG_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("{}/logs/{:04}{:02}{:02}{:02}{:02}{:02}.log", LF_path_data, now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second));
	LOG_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] [%s:%# %!] [Process %P] [Thread %t]: %v.");
	LOG_logger = make_shared<spdlog::logger>("LF", LOG_sink);
	LOG_logger->flush_on(spdlog::level::debug);
	spdlog::register_logger(LOG_logger);
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif
	spdlog::set_default_logger(LOG_logger);
	LF_version = LF_VER_MAIN;
#if LF_VER_TYPE != 0
	#if LF_VER_TYPE == 1
	LF_version.append(" Alpha");
	#elif LF_VER_TYPE == 2
	LF_version.append(" Beta");
	#elif LF_VER_TYPE == 3
	LF_version.append(" Rc");
	#endif
	LF_version.append(fmt::format(" {}", LF_VER_BUILD));
#endif
#ifdef _WIN32
	LF_version.append(" (Windows)");
#elif linux
	LF_version.append(" (Linux)");
#endif
	SPDLOG_INFO(fmt::format("Starting \"Learning Focus {}\"", LF_version));
}

void LF_Init() {
	// Initialize program.
	atexit(LF_exit);
	LF_Init_Dir();
	LF_Init_Bind();
	LF_Init_Sig();
	LF_Init_Log();
}
