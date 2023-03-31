#pragma once

/*
	@限定作用域的枚举类型定义
	@author:mayabin
	@date:2023/03/15
	@description:限定作用域的枚举变量必须以"类型名::变量名"的方式使用该变量;因为该变量的作用域仅在{}以内
*/

//遍历文件目录的层级深度
enum class LOOP_DEPTH {
	CURRENT_PATH,
	ALL_PATH
};

//时间戳的时间级别
enum class TSTAMP_LEVEL {
	TS_NANOSEC,		//nano second
	TS_MICROSEC,	//micro second
	TS_MILLISEC,	//milli second
	TS_SECOND,		//second
	TS_MINUTES,		//minutes
	TS_HOUR			//hour
};

//文件大小的取值单位
enum class FSIZE_UNIT {
	FUNIT_BYTE,
	FUNIT_KB,
	FUNIT_MB,
	FUNIT_GB
};

//json对象展开层级
enum class EXPAND_LEVEL {
	FIRST_LEVEL,	//展开第一层级
	SECOND_LEVEL	//展开所有层级
};
