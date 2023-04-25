#pragma once
#include "MyAlgProBase.h"
#include <functional>
#include <map>

/*
	@���ļ������㷨ע�ᴦ����
	@author:mayabin
	@date:2023/04/22
	@description:�������һ���ڲ�ģ���࣬�����ڱ����ھͽ�������������Ӧ�ɵ�����������Map��
*/

constexpr auto PRINT_PARAM_TO_TERMINAL = "print_param_to_terminal";

class AlgRegisterPro
{
public:
	AlgRegisterPro() = default;
	~AlgRegisterPro() = default;

	/*
		@AlgProcessFunc:���Ӵ�����ں���
		@passInParamParse:�������������ָ��
	*/
	bool AlgProcessFunc(ParamParseProc *passInParamParse);

	/*
		@ImplAlgRegister:��ģ����ʵ�־������ע�ᴦ�����������������Ӧ�����ָ�����Map
	*/
	template<class DetailAlgType>
	class ImplAlgRegister
	{
	public:
		ImplAlgRegister(std::string const &sTmpAppname) {
			AlgRegisterPro::GenerateCalledAlgMap().emplace(sTmpAppname, [this]() { return new DetailAlgType(); });
		}
	};

private:

	/*
		@GenerateAlgBasePtr:�����������Ʋ��Ҿ�������������ָ�룬�����丳ֵ������ָ��
		@strAppname:�����ҵ���������
	*/
	bool GenerateAlgBasePtr(std::string &strAppname);

	static std::map<std::string, std::function<AlgProcessBase*()>>& GenerateCalledAlgMap() {
		static std::map<std::string, std::function<AlgProcessBase*()>> m_CalledAlgNameObjMap = std::map<std::string, std::function<AlgProcessBase*()>>();
		return m_CalledAlgNameObjMap;		//�þֲ���̬����ֻ�ᱻ��ʼ��һ��
	}

	void PrintParamToConsole();

	std::map<std::string, std::function<void()>> m_BuiltinProAlgMap = { std::make_pair(PRINT_PARAM_TO_TERMINAL, [this]() { PrintParamToConsole(); }) };

	ParamParseProc *m_RegisterParamPro = nullptr;

	AlgProcessBase *m_RegisterAlgBasePtr = nullptr;

};

//����������ĺ�ע��
#define IMPLALGORITHM_REGISTER_NAME(ALGIMPL)		reg_msg_##ALGIMPL##_
#define IMPLALGORITHM_REGISTER(ALGIMPL, ALGNAME)	static AlgRegisterPro::ImplAlgRegister<ALGIMPL> IMPLALGORITHM_REGISTER_NAME(ALGIMPL)(ALGNAME);