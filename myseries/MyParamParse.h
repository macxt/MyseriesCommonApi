#pragma once
#include "MyUtility.h"

/*
	@ͨ�õĽӿں�����ʵ��
	@author:mayabin
	@date:2023/04/13
	@description:����������Σ����������������Map�У�Ĭ�ϲ�������"--paramName=paramValue"������ʽ������
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
		@ParamParse:���ܳ�����ڲ��������������Map
		@argc:��������
		@argv:ָ�����в������ַ���ָ��
	*/
	bool ParamParse(int argc, char** argv);

	/*
		@AddParamToMap:��Map������µĲ���
		@sKey:ָ��keyֵ
		@sValue:ָ��valueֵ���β������������ const ���δʣ���ʹʹ���� std::move() Ҳ���ܴ����ƶ����壩
	*/
	bool AddParamToMap(std::string &sKey, std::string &sValue);

	/*
		@GetValueFromMap:����ָ��Keyֵ��ȡ��Ӧvalueֵ
		@sKey:ָ��sKeyֵ
		@return:���ض�Ӧvalueֵ�����ָ��sKey�������򷵻ؿ�
	*/
	std::string GetValueFromMap(std::string const &sKey);

	/*
		@GetPrintParamInfo:�����ṩ��ȡ˽�б����Ľӿ�
	*/
	std::string GetPrintParamInfo();

private:

	std::string m_PrintParamInfo;	//������ն˵Ĳ����ַ���

	std::map<std::string, std::string> m_pParaParseResMap;		//��"{paramName,paramValue}"��ʽ�洢�����������
};
