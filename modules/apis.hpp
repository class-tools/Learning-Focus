/*
Learning Focus APIs Module Header File 2.0.0
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/

timestamp GetTimestamp() {
	// Get current time and return a timestamp object.
	time_t rawtime = 0;
	tm ptminfo = {};
	timestamp NowTime = {};
	time(&rawtime);
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
	return NowTime;
}
