#pragma once
#include "MyParamParse.h"

/*
	@该文件定义整个框架所有算法处理的基类
	@author:mayabin
	@date:2023/04/22
	@description:整个框架所有算法的基类，具体算法注册时必须继承该类并重写该类的两个纯虚函数
*/

class AlgProcessBase
{
public:
	AlgProcessBase() = default;
	virtual ~AlgProcessBase() = default;

	/*
		@InitializeParamParse:定义参数解析处理的纯虚函数
		@ptrParamPro:参数处理类对象指针
	*/
	virtual bool InitializeParamParse(ParamParseProc *ptrParamPro) = 0;

	/*
		@AlgorithmProcess:定义算法处理的纯虚函数
	*/
	virtual bool AlgorithmProcess() = 0;

private:

};
