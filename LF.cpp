/*
Learning Focus Main File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/
#include "version.hpp"
#include "./modules/basic.hpp"
#include "./modules/apis.hpp"
#include "./modules/file-process.hpp"
#include "./modules/functions.hpp"
#include "./modules/init.hpp"

int32_t main(int32_t _argc, char* _argv[]) {
	LF_Init();
	try {
		Arg_Parser.parse_args(_argc, _argv);
	} catch (...) {
		SPDLOG_CRITICAL("Invalid command line arguments");
		std::cout << "Invalid command line arguments." << std::endl;
		std::cout << Arg_Parser << std::endl;
		std::exit(0);
	}
	LF_Log_Args();
	if (Arg_Parser.get<bool>("-dl")) {
		LF_Data_List();
		std::exit(0);
	}
	if (Arg_Parser.get<std::string>("-ds") != "") {
		std::pair<double, double> ret = LF_Data_Get(Arg_Parser.get<std::string>("-ds"));
		if (ret.first == 0 && ret.second == 0) {
			std::cout << "Cannot find data." << std::endl;
			std::exit(0);
		}
		LF_Func_Visualization(ret.first, ret.second, Arg_Parser.get<std::string>("-ds"));
		std::exit(0);
	}
	cv::VideoCapture cap;
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	cap.open(Arg_Parser.get<int32_t>("-c"), cv::CAP_DSHOW);
	dlib::sleep(1000);
	if (!cap.isOpened()) {
		SPDLOG_CRITICAL("Cannot open camera");
		std::cout << "Cannot open camera." << std::endl;
		std::exit(0);
	}
	LF_Func_ModelExistence();
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::shape_predictor predictor;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> predictor;
	dlib::correlation_tracker tracker;
	bool status = false;
	double totalgood = 0;
	double totalbad = 0;
	std::chrono::system_clock::time_point timestart;
	cv::namedWindow("Face", cv::WINDOW_AUTOSIZE);
	while (true) {
		timestart = std::chrono::system_clock::now();
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			break;
		}
		std::vector<dlib::rectangle> facelist = detector(dlib::cv_image<dlib::bgr_pixel>(frame));
		if (facelist.size() == 0) {
			status = true;
		} else {
			dlib::rectangle face = facelist[0];
			tracker.start_track(dlib::cv_image<dlib::bgr_pixel>(frame), face);
			tracker.update(dlib::cv_image<dlib::bgr_pixel>(frame));
			dlib::drectangle pos = tracker.get_position();
			cv::rectangle(frame, cv::Point(pos.left(), pos.top()), cv::Point(pos.right(), pos.bottom()), (status ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0)), 2);
			status = false;
			dlib::full_object_detection landmarks = predictor(dlib::cv_image<dlib::bgr_pixel>(frame), face);
			if (!status && 1.5 * (landmarks.part(37).y() - landmarks.part(19).y()) < (landmarks.part(8).y() - landmarks.part(57).y())) {
				int32_t axisx = landmarks.part(40).x() - 1;
				int32_t axisy = landmarks.part(40).y() - 3;
				if (axisy > frame.rows || axisx > frame.cols) {
					status = true;
				} else {
					cv::Vec3b pixel = frame.at<cv::Vec3b>(axisy, axisx);
					if (0.11 * pixel[0] + 0.59 * pixel[1] + 0.3 * pixel[2] >= 130) {
						status = true;
					}
				}
			}
			if (!status && 2 * (landmarks.part(29).x() - landmarks.part(1).x()) < (landmarks.part(15).x() - landmarks.part(29).x())) {
				int32_t axisx = landmarks.part(42).x() + 6;
				int32_t axisy = landmarks.part(42).y() - 3;
				if (axisy > frame.rows || axisx > frame.cols) {
					status = true;
				} else {
					cv::Vec3b pixel = frame.at<cv::Vec3b>(axisy, axisx);
					if (0.11 * pixel[0] + 0.59 * pixel[1] + 0.3 * pixel[2] >= 100) {
						status = true;
					}
				}
			}
			if (!status && (landmarks.part(29).x() - landmarks.part(1).x()) > 2 * (landmarks.part(15).x() - landmarks.part(29).x())) {
				int32_t axisx = landmarks.part(42).x() + 4;
				int32_t axisy = landmarks.part(39).y() - 3;
				if (axisy > frame.rows || axisx > frame.cols) {
					status = true;
				} else {
					cv::Vec3b pixel = frame.at<cv::Vec3b>(axisy, axisx);
					if (0.11 * pixel[0] + 0.59 * pixel[1] + 0.3 * pixel[2] <= 100) {
						status = true;
					}
				}
			}
			if (Arg_Parser.get<bool>("-e")) {
				for (uint8_t i = 0; i < 68; i++) {
					cv::circle(frame, cv::Point(landmarks.part(i).x(), landmarks.part(i).y()), 2, cv::Scalar(255, 255, 255), -1);
				}
			}
		}
		double differ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - std::chrono::duration_cast<std::chrono::milliseconds>(timestart.time_since_epoch())).count() / static_cast<double>(60000);
		if (!status) {
			totalgood += differ;
		} else {
			totalbad += differ;
		}
		cv::imshow("Face", frame);
		if (cv::waitKey(1) == 27) {
			break;
		}
	}
	cap.release();
	cv::destroyAllWindows();
	LF_Data_Save(totalgood, totalbad, GetTimestampString());
	LF_Func_Visualization(totalgood, totalbad);
	return 0;
}
