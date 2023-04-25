#pragma once
#include "MyAlgProBase.h"
#include <functional>
#include <map>

/*
	@该文件定义算法注册处理类
	@author:mayabin
	@date:2023/04/22
	@description:此类包含一个内部模板类，便于在编译期就将算子名称与相应可调用类对象放入Map中
*/

constexpr auto PRINT_PARAM_TO_TERMINAL = "print_param_to_terminal";

class AlgRegisterPro
{
public:
	AlgRegisterPro() = default;
	~AlgRegisterPro() = default;

	/*
		@AlgProcessFunc:算子处理入口函数
		@passInParamParse:参数解析类对象指针
	*/
	bool AlgProcessFunc(ParamParseProc *passInParamParse);

	/*
		@ImplAlgRegister:该模板类实现具体的类注册处理，将算子名称与相对应类对象指针存入Map
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
		@GenerateAlgBasePtr:根据算子名称查找具体的算子类对象指针，并将其赋值给基类指针
		@strAppname:待查找的算子名称
	*/
	bool GenerateAlgBasePtr(std::string &strAppname);

	static std::map<std::string, std::function<AlgProcessBase*()>>& GenerateCalledAlgMap() {
		static std::map<std::string, std::function<AlgProcessBase*()>> m_CalledAlgNameObjMap = std::map<std::string, std::function<AlgProcessBase*()>>();
		return m_CalledAlgNameObjMap;		//该局部静态变量只会被初始化一次
	}

	void PrintParamToConsole();

	std::map<std::string, std::function<void()>> m_BuiltinProAlgMap = { std::make_pair(PRINT_PARAM_TO_TERMINAL, [this]() { PrintParamToConsole(); }) };

	ParamParseProc *m_RegisterParamPro = nullptr;

	AlgProcessBase *m_RegisterAlgBasePtr = nullptr;

};

//具体算子类的宏注册
#define IMPLALGORITHM_REGISTER_NAME(ALGIMPL)		reg_msg_##ALGIMPL##_
#define IMPLALGORITHM_REGISTER(ALGIMPL, ALGNAME)	static AlgRegisterPro::ImplAlgRegister<ALGIMPL> IMPLALGORITHM_REGISTER_NAME(ALGIMPL)(ALGNAME);