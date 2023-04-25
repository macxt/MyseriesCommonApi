#pragma once
#include "MyParamParse.h"

/*
	@���ļ�����������������㷨����Ļ���
	@author:mayabin
	@date:2023/04/22
	@description:������������㷨�Ļ��࣬�����㷨ע��ʱ����̳и��ಢ��д������������麯��
*/

class AlgProcessBase
{
public:
	AlgProcessBase() = default;
	virtual ~AlgProcessBase() = default;

	/*
		@InitializeParamParse:���������������Ĵ��麯��
		@ptrParamPro:�������������ָ��
	*/
	virtual bool InitializeParamParse(ParamParseProc *ptrParamPro) = 0;

	/*
		@AlgorithmProcess:�����㷨����Ĵ��麯��
	*/
	virtual bool AlgorithmProcess() = 0;

private:

};
