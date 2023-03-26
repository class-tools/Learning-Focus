/*
Learning Focus Functions Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

void LF_exit() {
	// Exit program.
	SPDLOG_INFO(fmt::format("Exiting \"Learning Focus {}\"", LF_version));
	_exit(0);
}

void LF_Log_Args() {
	// Print command line arguments into log file.
	SPDLOG_INFO(fmt::format("Camera id: {}", ARG_parser.get<int32_t>("-c")));
	SPDLOG_INFO(fmt::format("Debug mode: {}", ARG_parser.get<bool>("-d") ? "Enabled" : "Disabled"));
}
