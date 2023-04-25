#include "MyAlgRegisterPro.h"

bool AlgRegisterPro::AlgProcessFunc(ParamParseProc *passInParamParse)
{
	m_RegisterParamPro = passInParamParse;

	std::string strAppName = m_RegisterParamPro->GetValueFromMap("appname");
	std::vector<std::string> vAppNameVec;
	CommUtility::SplitStr(strAppName, vAppNameVec, "|");	//支持一次性传入多个算子名称并用"|"分隔

	if (vAppNameVec.empty()) {
		MYLOG_INFO << "Pass in algorithm name is empty, and no need to process.";
		return true;
	}

	//内置算子需要优先执行
	for (auto iterOps = m_BuiltinProAlgMap.begin(); iterOps != m_BuiltinProAlgMap.end(); iterOps++)
	{
		auto appIter = std::find(vAppNameVec.begin(), vAppNameVec.end(), iterOps->first);
		if (appIter != vAppNameVec.end()) {
			m_BuiltinProAlgMap[iterOps->first]();
			vAppNameVec.erase(appIter);
		}
	}

	for (auto TmpAppname : vAppNameVec)
	{
		if (!GenerateAlgBasePtr(TmpAppname)) {
			return false;
		}

		if (!m_RegisterAlgBasePtr->InitializeParamParse(m_RegisterParamPro)) {
			MYLOG_ERROR << "Input command line parameters parse failed.";
			return false;
		}

		if (!m_RegisterAlgBasePtr->AlgorithmProcess()) {
			MYLOG_ERROR << "Process algorithm [" << TmpAppname << "] failed.";
			return false;
		}

		MYLOG_INFO << "Process algorithm [" << TmpAppname << "] success.";
	}

	MYLOG_INFO << "All of algorithms process done.";
	return true;
}

bool AlgRegisterPro::GenerateAlgBasePtr(std::string &strAppname)
{
	auto calledAlgorithmMap = GenerateCalledAlgMap();
	if (calledAlgorithmMap.find(strAppname) == calledAlgorithmMap.end()) {
		MYLOG_ERROR << "This appname is not registered and not found.";
		return false;
	}
	
	m_RegisterAlgBasePtr = calledAlgorithmMap[strAppname]();
	return true;
}

void AlgRegisterPro::PrintParamToConsole()
{
	MYLOG_INFO << "[" << m_RegisterParamPro->GetPrintParamInfo() << "].";
	return;
}
