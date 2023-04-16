/*
Learning Focus File Processing Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/
void LF_Data_List() {
	// List data from file.
	std::string FilePath = fmt::format("{}/data.json", LF_Path_Data);
	Json::Value val;
	std::ifstream fin;
	SPDLOG_INFO(fmt::format("Reading file \"{}\"", FilePath));
	fin.open(FilePath);
	if (!fin.is_open()) {
		goto ERR;
	}
	if (Json_Reader.parse(fin, val)) {
		try {
			for (auto it = val.begin(); it != val.end(); it++) {
				std::cout << it.key().asString() << std::endl;
			}
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		fin.close();
		SPDLOG_WARN(fmt::format("Data in file \"{}\" corrupted", FilePath));
		std::cout << "Data corrupted." << std::endl;
	}
}

std::pair<double, double> LF_Data_Get(std::string key) {
	// Get data from file.
	std::string FilePath = fmt::format("{}/data.json", LF_Path_Data);
	Json::Value val;
	std::ifstream fin;
	SPDLOG_INFO(fmt::format("Reading file \"{}\"", FilePath));
	fin.open(FilePath);
	if (!fin.is_open()) {
		goto ERR;
	}
	if (Json_Reader.parse(fin, val)) {
		try {
			return std::make_pair(val[key][0].asDouble(), val[key][1].asDouble());
		} catch (...) {
			goto ERR;
		}
	} else {
	ERR:
		fin.close();
		SPDLOG_WARN(fmt::format("Data in file \"{}\" corrupted", FilePath));
		return std::make_pair(static_cast<double>(0), static_cast<double>(0));
	}
}

void LF_Data_Save(double good, double bad, std::string key) {
	// Save data to file.
	std::string FilePath = fmt::format("{}/data.json", LF_Path_Data);
	Json::Value val;
	std::ifstream fin;
	std::ofstream fout;
	SPDLOG_INFO(fmt::format("Reading file \"{}\"", FilePath));
	fin.open(FilePath);
	if (fin.is_open()) {
		Json_Reader.parse(fin, val);
	}
	fin.close();
	val[key][0] = good;
	val[key][1] = bad;
	fout.open(FilePath);
	Json_SW->write(val, &fout);
	fout.close();
	SPDLOG_INFO(fmt::format("Writing file \"{}\"", FilePath));
}
