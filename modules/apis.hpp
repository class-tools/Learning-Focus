/*
Learning Focus APIs Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/
timestamp GetTimestamp() {
	// Get current time and return a timestamp object.
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	time_t rawtime = std::chrono::system_clock::to_time_t(now);
	tm ptminfo = {};
	timestamp NowTime = {};
#ifdef _WIN32
	localtime_s(&ptminfo, &rawtime);
#elif linux
	localtime_r(&rawtime, &ptminfo);
#endif
	NowTime.Year = ptminfo.tm_year + 1900;
	NowTime.Month = ptminfo.tm_mon + 1;
	NowTime.Day = ptminfo.tm_mday;
	NowTime.Hour = ptminfo.tm_hour;
	NowTime.Minute = ptminfo.tm_min;
	NowTime.Second = ptminfo.tm_sec;
	NowTime.MilliSecond = (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000).count();
	NowTime.MicroSecond = (std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000).count() % 1000;
	NowTime.NanoSecond = (std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000).count() % 1000;
	return NowTime;
}

std::string GetTimestampString() {
	// Get current time and return a string of timestamp.
	timestamp now = GetTimestamp();
	return fmt::format("{:04}{:02}{:02}{:02}{:02}{:02}", now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second);
}

std::string GetExecDir() {
	// Get program executable directory.
#ifdef _WIN32
	std::string _res = _pgmptr;
	char _spilt = '\\';
#elif linux
	char _path[4096] = {};
	readlink("/proc/self/exe", _path, 4096);
	std::string _res = _path;
	char _spilt = '/';
#endif
	int32_t _pos = 0;
	for (int32_t i = static_cast<int32_t>(_res.size() - 1); i >= 0; i--) {
		if (_res[i] == _spilt) {
			_pos = i;
			break;
		}
	}
	return _res.substr(0, _pos);
}
