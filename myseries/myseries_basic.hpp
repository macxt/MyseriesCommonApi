#pragma once

/*
	Desc: A series of code files starting with the prefix "My" implements the following function interface:
		Processing for files and directories;
		Processing for string;
		Processing for time;
		Processing for encoding translate betweent utf-8 and gb2312 on linux/windows platform;
		Processing for parse json string;
		Processing for thread pool;
		Processing for output log message to terminal;
	Usage: #include "myseries_basic.hpp" in your projects.
	Author: mayabin
	Date: 2023/03/19
	See: https://github.com/macxt/MyseriesCommonApi.git

	Update Date: 2023/04/24
	Update Content:
		To complete the basic framework of algorithm processing, specific operators need to register and rewrite two 
		pure virtual function of base class to achieve the corresponding functions.
	New files:
		MyAlgProBase.h
		MyParamParse.h
		MyParamParse.cpp
		MyAlgRegisterPro.h
		MyAlgRegisterPro.cpp
*/

#include "MyUtility.h"
#include "MyThreadPool.h"
#include "MyAlgRegisterPro.h"