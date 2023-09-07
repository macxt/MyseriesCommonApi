#pragma once
#include "MyUtility.h"

/*
	@通用的接口函数的实现
	@author:mayabin
	@date:2023/04/13
	@description:解析程序入参，并将解析结果存入Map中；默认参数均以"--paramName=paramValue"这种形式给出；
*/

#ifdef _MSC_VER
#define	STR_COMPARE(str1, str2)			(stricmp(str1, str2))
#define STR_COMPARE_N(str1, str2, n)	(strnicmp(str1, str2, n))
#else
#define STR_COMPARE(str1, str2)			(strcasecmp(str1, str2))
#define STR_COMPARE_N(str1, str2, n)	(strncasecmp(str1, str2, n))
#endif

#define CHECK_PARAMS_IS_EMPTY(param) if (param.empty()) {std::cerr << #param " is empty, and return."<< std::endl;return false;}

class ParamParseProc
{
public:
	ParamParseProc() = default;
	~ParamParseProc() = default;

	/*
		@ParamParse:接受程序入口参数并解析存放至Map
		@argc:参数个数
		@argv:指向所有参数的字符串指针
	*/
	bool ParamParse(int argc, char** argv);

	/*
		@AddParamToMap:向Map中添加新的参数
		@sKey:指定key值
		@sValue:指定value值（形参类型如果加了 const 修饰词，则即使使用了 std::move() 也不能触发移动语义）
	*/
	bool AddParamToMap(std::string &sKey, std::string &sValue);

	/*
		@GetValueFromMap:根据指定Key值获取相应value值
		@sKey:指定sKey值
		@return:返回对应value值，如果指定sKey不存在则返回空
	*/
	std::string GetValueFromMap(std::string const &sKey);

	/*
		@GetPrintParamInfo:对外提供获取私有变量的接口
	*/
	std::string GetPrintParamInfo();

private:

	std::string m_PrintParamInfo;	//输出到终端的参数字符串

	std::map<std::string, std::string> m_pParaParseResMap;		//以"{paramName,paramValue}"形式存储参数解析结果
};
