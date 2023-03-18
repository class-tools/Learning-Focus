/*
Learning Focus Functions Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

void LF_exit() {
	// Exit program.
	SPDLOG_INFO(std::format(L"Exiting \"Learning Focus {}\"", LF_version));
	_exit(0);
}
