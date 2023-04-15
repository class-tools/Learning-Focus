/*
Learning Focus Functions Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

void LF_Exit() {
	// Exit program.
	SPDLOG_INFO(fmt::format("Exiting \"Learning Focus {}\"", LF_Version));
	_exit(0);
}

void LF_Log_Args() {
	// Print command line arguments into log file.
	SPDLOG_INFO(fmt::format("Camera id: {}", Arg_Parser.get<int32_t>("-c")));
	SPDLOG_INFO(fmt::format("Data list: {}", Arg_Parser.get<bool>("-dl") ? "Yes" : "No"));
	SPDLOG_INFO(fmt::format("Data show: \"{}\"", Arg_Parser.get<std::string>("-ds")));
	SPDLOG_INFO(fmt::format("Extra information: {}", Arg_Parser.get<bool>("-e") ? "Enabled" : "Disabled"));
	SPDLOG_INFO(fmt::format("Window width: {}", Arg_Parser.get<int32_t>("-ww")));
	SPDLOG_INFO(fmt::format("Window height: {}", Arg_Parser.get<int32_t>("-wh")));
}

void LF_Func_ModelExistence() {
	// Check model file existence.
	if (!std::filesystem::is_regular_file(fmt::format("{}/shape_predictor_68_face_landmarks.dat", GetExecDir()))) {
		SPDLOG_CRITICAL("Cannot find \"shape_predictor_68_face_landmarks.dat\"");
		std::cout << "Cannot find shape_predictor_68_face_landmarks.dat." << std::endl;
		std::cout << "Download and decompress it from http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2." << std::endl;
		std::exit(0);
	}
}

void LF_Func_Visualization(double good, double bad, std::string title = "Statistics") {
	// Visualize the result.
	double para[2] = {good, bad};
	mglGraph gr(0, Arg_Parser.get<int32_t>("-ww"), Arg_Parser.get<int32_t>("-wh"));
	mglData dt(2, para);
	gr.Title(title.c_str(), "", -1.5);
	gr.SetRanges(0, 2, 0, static_cast<int>(std::ceil(std::max(good, bad)) + 1));
	gr.SetOrigin(0, 0);
	gr.Axis("y");
	gr.Label('y', "Time");
	gr.Box();
	gr.Bars(dt, "W");
	gr.Puts(mglPoint(0.5, -0.1 * std::ceil(std::max(good, bad) + 1)), "Good");
	gr.Puts(mglPoint(1.5, -0.1 * std::ceil(std::max(good, bad) + 1)), "Bad");
	gr.Puts(mglPoint(0.5, good + 0.05 * std::ceil(std::max(good, bad) + 1)), fmt::format("{:.5f}", good).c_str());
	gr.Puts(mglPoint(1.5, bad + 0.05 * std::ceil(std::max(good, bad) + 1)), fmt::format("{:.5f}", bad).c_str());
	cv::namedWindow(title, cv::WINDOW_KEEPRATIO);
	cv::resizeWindow(title, Arg_Parser.get<int32_t>("-ww"), Arg_Parser.get<int32_t>("-wh"));
	cv::Mat pic(gr.GetHeight(), gr.GetWidth(), CV_8UC3);
	pic.data = const_cast<uchar*>(gr.GetRGB());
	cv::imshow(title, pic);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
