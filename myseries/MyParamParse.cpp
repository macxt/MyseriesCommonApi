#include "MyParamParse.h"
#include <sstream>

//1.入参必须以 --key=value 形式出现，且"="前后不能有空格
//2.必须有 --appname=xxx 这个参数，代表后期程序执行的算法
bool ParamParseProc::ParamParse(int argc, char** argv)
{
	std::stringstream osstr;
	bool bIsFindAppName = false;
	for (size_t i = 1; i < argc; i++)
	{
		std::string sTmpParam = argv[i];
		osstr << sTmpParam << " ";

		std::vector<std::string> vTmpParamVec;
		CommUtility::SplitStr(sTmpParam, vTmpParamVec, "=", true);

		//size != 2 format error
		if (vTmpParamVec.size() != 2) {
			MYLOG_ERROR << "This parameter format error. [" << sTmpParam << "]";
			return false;
		}

		//param not start with "--" format error
		if (!CommUtility::IsStartWith(vTmpParamVec.at(0), "--")) {
			MYLOG_ERROR << "This parameter does not start with -- format error. [" << sTmpParam << "]";
			return false;
		}

		//remove --
		std::string sTmpKey = vTmpParamVec.at(0).substr(2);
		
		//sTmpKey already exists, error
		if (m_pParaParseResMap.count(sTmpKey)) {
			MYLOG_ERROR << "The key of this parameter already exists. Not repeat.";
			return false;
		}

		if (sTmpKey == "appname") bIsFindAppName = true;

		MYLOG_INFO << "Succeed insert [key = " << sTmpKey << ", value = " << vTmpParamVec.at(1) << "]";
		m_pParaParseResMap.emplace(std::move(sTmpKey), std::move(vTmpParamVec.at(1)));
	}

	if (!bIsFindAppName) {
		MYLOG_ERROR << "Input parameters error. Must have [--appname=xxx]";
		return false;
	}

	m_PrintParamInfo = osstr.str();

	MYLOG_INFO << "Total input " << m_pParaParseResMap.size() << " parameters.";
	return true;
}

bool ParamParseProc::AddParamToMap(std::string &sKey, std::string &sValue)
{
	auto iterRes = m_pParaParseResMap.find(sKey);
	if (iterRes != m_pParaParseResMap.end()) {
		MYLOG_ERROR << "This key already exists in map.";
		return false;
	}

	m_pParaParseResMap.emplace(std::move(sKey), std::move(sValue));
	return true;
}

std::string ParamParseProc::GetValueFromMap(std::string const &sKey)
{
	std::string sRetValue = "";
	if (m_pParaParseResMap.find(sKey) != m_pParaParseResMap.end()) {
		sRetValue = m_pParaParseResMap[sKey];
	}

	return sRetValue;
}

std::string ParamParseProc::GetPrintParamInfo()
{
	return m_PrintParamInfo;
}

