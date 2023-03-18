/*
Learning Focus Initialization Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

void LF_Init_Dir() {
	// Initialize directories.
	if (_waccess(LF_path_data.c_str(), 0) != 0) {
		CreateDirectoryW(LF_path_data.c_str(), NULL);
	}
	if (_waccess((LF_path_data + L"\\logs").c_str(), 0) != 0) {
		CreateDirectoryW(LF_path_data.c_str(), NULL);
	}
}

void LF_Init_Bind() {
	// Initialize bindings.
	JSON_SWB.settings_ = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();
	std::ignore = _setmode(_fileno(stdin), _O_WTEXT);
	std::ignore = _setmode(_fileno(stdout), _O_WTEXT);
	wfin.imbue(std::locale(".UTF-8", LC_CTYPE));
	wfout.imbue(std::locale(".UTF-8", LC_CTYPE));
	_wsystem(L"CHCP 65001 > NUL");
}

void LF_Init_Sig() {
	// Initialize signals.
	signal(SIGINT, []([[maybe_unused]] int32_t signum) {
		exit(0);
	});
	signal(SIGBREAK, []([[maybe_unused]] int32_t signum) {
		exit(0);
	});
	signal(SIGABRT, [](int32_t signum) {
		SPDLOG_CRITICAL(std::format(L"Signal {} detected (Program aborted)", signum));
		exit(signum);
	});
	signal(SIGFPE, [](int32_t signum) {
		SPDLOG_CRITICAL(std::format(L"Signal {} detected (Operation overflow)", signum));
		exit(signum);
	});
	signal(SIGILL, [](int32_t signum) {
		SPDLOG_CRITICAL(std::format(L"Signal {} detected (Illegal instruction)", signum));
		exit(signum);
	});
	signal(SIGSEGV, [](int32_t signum) {
		SPDLOG_CRITICAL(std::format(L"Signal {} detected (Access to illegal memory)", signum));
		exit(signum);
	});
}

void LF_Init_Log() {
	// Initialize logger.
	timestamp now = GetTimestamp();
	LOG_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::format(L"{}\\logs\\{:04}{:02}{:02}{:02}{:02}{:02}.log", LF_path_data, now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second));
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
	LF_version.append(L" Alpha");
	#elif LF_VER_TYPE == 2
	LF_version.append(L" Beta");
	#elif LF_VER_TYPE == 3
	LF_version.append(L" Rc");
	#endif
	LF_version.append(std::format(L" {}", LF_VER_BUILD));
#endif
	LF_version.append(std::format(L" ({})", LF_Framework));
	SPDLOG_INFO(std::format(L"Starting \"Learning Focus {}\"", LF_version));
}

void LF_Init() {
	// Initialize program.
	atexit(LF_exit);
	LF_Init_Dir();
	LF_Init_Bind();
	LF_Init_Sig();
	LF_Init_Log();
}
