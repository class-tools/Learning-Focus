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
		ARG_parser.parse_args(_argc, _argv);
	} catch (...) {
		SPDLOG_CRITICAL("Invalid command line arguments");
		std::cout << "Invalid command line arguments." << std::endl;
		std::cout << ARG_parser << std::endl;
		std::exit(0);
	}
	LF_Log_Args();
	cv::VideoCapture cap;
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	cap.open(ARG_parser.get<int32_t>("-c"), cv::CAP_DSHOW);
	dlib::sleep(1000);
	if (!cap.isOpened()) {
		SPDLOG_CRITICAL("Cannot open camera");
		std::cout << "Cannot open camera." << std::endl;
		std::exit(0);
	}
	if (!std::filesystem::is_regular_file(fmt::format("{}/shape_predictor_68_face_landmarks.dat", GetExecDir()))) {
		SPDLOG_CRITICAL("Cannot find shape_predictor_68_face_landmarks.dat");
		std::cout << "Cannot find shape_predictor_68_face_landmarks.dat." << std::endl;
		std::cout << "Download and decompress it from http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2." << std::endl;
		std::exit(0);
	}
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::shape_predictor predictor;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> predictor;
	dlib::correlation_tracker tracker;
	bool status = false;
	int32_t totalgood = 0;
	int32_t totalbad = 0;
	time_t timestart = 0;
	time_t timeend = 0;
	cv::namedWindow("Face", cv::WINDOW_AUTOSIZE);
	while (true) {
		time(&timestart);
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
			if (ARG_parser.get<bool>("-d")) {
				for (int32_t i = 0; i < 68; i++) {
					cv::circle(frame, cv::Point(landmarks.part(i).x(), landmarks.part(i).y()), 2, cv::Scalar(255, 255, 255), -1);
				}
			}
		}
		time(&timeend);
		if (!status) {
			totalbad += difftime(timeend, timestart);
		} else {
			totalgood += difftime(timeend, timestart);
		}
		cv::imshow("Face", frame);
		if (cv::waitKey(1) == 27) {
			break;
		}
	}
	cap.release();
	cv::destroyAllWindows();
	std::cout << fmt::format("Total good studying time (Minute): {:.5f}", static_cast<double>(totalgood) / 60) << std::endl;
	std::cout << fmt::format("Total bad studying time (Minute): {:.5f}", static_cast<double>(totalbad) / 60) << std::endl;
	return 0;
}
