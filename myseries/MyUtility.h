#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <map>
#include <sys/timeb.h>
#include <sys/stat.h>
#include "jsoncons/json.hpp"
#include "MyTypeAlias.h"
#include "MyEnumType.h"
#include "MyLogFile.h"

#ifdef _MSC_VER
#define DIR_SEP		'\\'
#include <io.h>
#include <windows.h>
#else
#define DIR_SEP		'/'
#include <dirent.h>
#include <string.h>
#include <iconv.h>
#include <unistd.h>
#endif

/*
	@通用的接口函数的实现
	@author:mayabin
	@date:2023/03/15
	@description:包含字符串处理，时间处理，文件文件夹处理，编码转换处理，json解析处理等
*/

class CommUtility
{
public:
	CommUtility() = default;
	~CommUtility() = default;


	/************************************************************************/
	/*                        字符串处理接口                                 */
	/************************************************************************/

	/*
		@ToLower:将字符串转成小写并返回
		@instr:待转换的字符串
	*/
	static std::string ToLower(const std::string &instr) noexcept;

	/*
		@ToLower:将字符串转成小写(原地修改)
		@instr:待转换的字符串
	*/
	static void ToLowerInPlace(std::string &instr) noexcept;

	/*
		@ToLower:将字符串转成大写并返回
		@instr:待转换的字符串
	*/
	static std::string ToUpper(const std::string &instr) noexcept;

	/*
		@ToLower:将字符串转成大写(原地修改)
		@instr:待转换的字符串
	*/
	static void ToUpperInPlace(std::string &instr) noexcept;

	/*
		@IsStartWith:判断str是否以subStr开始
		@str: 待检测源字符串
		@subStr: 待检测子字符串
		@bIgnoreCase: 是否忽略大小写(默认区分大小写)
	*/
	static bool IsStartWith(const std::string &str, const std::string &subStr, bool bIgnoreCase = false);

	/*
		@IsEndWith:判断str是否以subStr结尾
		@str: 待检测源字符串
		@subStr: 待检测子字符串
		@bIgnoreCase: 是否忽略大小写(默认区分大小写)
	*/
	static bool IsEndWith(const std::string &str, const std::string &subStr, bool bIgnoreCase = false);

	/*
		@RemoveCharFromStr: 从str字符串中移除字符c
		@str: 待处理的输入字符串
		@c: 待移除的字符c
	*/
	static void RemoveCharFromStr(std::string &str, char c);

	/*
		@TrimSpace:删除字符串两边的空格
		@str:待处理字符串
	*/
	static void TrimSpace(std::string &str);

	/*
		@ExtractMiddleStr: 提取左子串与右子串中间的子字符串并返回
		@src: 待提取的源字符串
		@sub_left: 左边的子字符串
		@sub_right: 右边的子字符串
	*/
	static std::string ExtractMiddleStr(std::string const &src, std::string const &sub_left, std::string const &sub_right);

	/*
		@SplitStr: 字符串分割处理并将结果存储vector返回
		@str: 待分割字符串
		@retVecRes: 存储分割结果的容器
		@sepStr: 指定分割符(可以是单字符也可以是子字符串)
	*/
	static void SplitStr(std::string const &str, std::vector<std::string> &retVecRes, std::string const &sepStr);

	///*
	//	@StringToWString: 将string转为wstring
	//	@instr: 待转换字符串
	//*/
	//static std::wstring StringToWString(const std::string &instr);

	///*
	//	@StringToWString: 将wstring转为string
	//	@instr: 待转换字符串
	//*/
	//static std::string WStringToString(const std::wstring &instr);

	/*
		@ReplaceStr: 用新子串替换输入字符串中的旧子串
		@str: 输入的源字符串
		@oldstr: 待替换的旧子串
		@newstr: 用于替换的新子串
		@bIsReplaceAll: 默认为true全部替换;false时只替换第一次出现的旧子串
	*/
	static void ReplaceStr(std::string &str, const std::string &oldstr, const std::string &newstr, bool bIsReplaceAll = true);


	/************************************************************************/
	/*                   字符串与数字相关转换处理接口                         */
	/************************************************************************/

	/*
		@DoubleToStringSetBits: 将double数据按照指定的小数位数四舍五入后转为string输出
		@df_in: 输入的double类型数据
		@bits: 指定的小数位数
	*/
	static std::string DoubleToStringSetBits(double df_in, int bits);

	/*
		@DigitToString: 数字转字符串并保留n位小数
		@GUIntBig: 输入待转换的整数
		@int n: 需要保留的小数位
		@return: 返回转换后的字符串	e.g: (23459874098,4) ---> "2345987.4098"
	*/
	static std::string DigitToString(GUIntBig num, int n);

	/*
		@StrToStrRoundN: 将输入字符串表示的数字按照需要保留的位数四舍五入转换成新的字符串并返回
		@std::string const &s_in: 输入的表示数字的字符串
		@int n: 需要保留的位数
		@return: 返回新的处理后的字符串	e.g: "45263.2165" --->  "45263.217"
	*/
	static std::string StrToStrRoundN(std::string const &s_in, int n);

	/*
		@StrToStrReaminN: 将输入字符串强制按照需要保留位数转换并返回
		@std::string const &s: 输入待转换的字符串
		@int n: 需要保留的位数
		@return: 返回处理好的字符串
		e.g: ("4524",3) --->  "4524.000"; ("4524.346",5) --->  "4524.34600"; ("4524.346",2) --->  "4524.35"
	*/
	static std::string StrToStrReaminN(std::string const &s, int n);



	/************************************************************************/
	/*                 编码转换处理接口                                      */
	/************************************************************************/


	/*
		@ConvertToUtf8: 将输入字符串转为UTF8编码并返回
		@str: 待转换的字符串
	*/
	static std::string ConvertToUtf8(std::string const &str);

	/*
		@IsUtf8Str:判断输入的字符串是否为UTF8编码
		@str: 待检测字符串
	*/
	static bool IsUtf8Str(const char* str);

#ifdef _MSC_VER
	/*
		@ToUtf8OnWindows:在Windows平台上将字符串转为UTF8编码
		@str: 待转换的字符串
	*/
	static std::string ToUtf8OnWindows(const std::string &str);
#else
	/*
		@ToUtf8OnLinux:在Linux平台上将字符串转为UTF8编码
		@str: 待转换的字符串
	*/
	static std::string ToUtf8OnLinux(const std::string &str);

	/*
		@code_convert: 由源字符集编码转换为目标字符集编码
	*/
	static int code_convert(char * from_charset, char * to_charset, char * inbuf, size_t inlen, char * outbuf, size_t outlen);

	/*
		@u2g: utf-8 转 gb2312
	*/
	static std::string u2g(std::string const &instr);

	/*
		@g2u: gb2312 转 utf-8
	*/
	static std::string g2u(std::string const &instr);
#endif


	/************************************************************************/
	/*                       文件文件夹处理接口                              */
	/************************************************************************/

	/*
		@IsPathExists: 检测文件或文件夹是否存在
		@filepath: 待检测的文件路径
	*/
	static bool IsPathExists(const std::string &filepath);

	/*
		@CreateDir: 创建子目录
		@dirpath: 指定待创建的子目录路径
		@注：mkdir(OnWindows)与mkdir -p(OnLinux)均可创建多级目录
	*/
	static bool CreateDir(const std::string &dirpath);

	/*
		@DeleteDir: 删除指定目录
		@dirpath: 待处理的目录路径
	*/
	static bool DeleteDir(const std::string &dirpath);

	/*
		@GetFilePathFromFullPath: 从文件全路径获取所属目录路径
		@fileFullPath: 指定的文件全路径信息
	*/
	static std::string GetFilePathFromFullPath(const std::string &fileFullPath) noexcept;

	/*
		@GetFileNameFromFullPath: 从文件全路径获取文件名
		@fileFullPath: 指定的文件全路径信息
	*/
	static std::string GetFileNameFromFullPath(const std::string &fileFullPath) noexcept;

	/*
		@GetFileExtensionFromFullPath: 从文件全路径获取文件扩展名
		@fileFullPath: 指定的文件全路径信息
		@bWithDot: 返回的扩展名是否带点，默认不带
	*/
	static std::string GetFileExtensionFromFullPath(const std::string &fileFullPath, bool bWithDot = false) noexcept;

	/*
		@ReadFileInfo: 读取文本文件信息到容器并返回(默认为文本文件，按行读取的方式处理)
		@filepath: 指定文本文件路径信息
		@retFileContent: 返回存放文件内容的容器(按行为单位存储)
	*/
	static bool ReadTextFileInfo(const std::string &filepath, std::vector<std::string> &retFileContent);

	/*
		@ReadBinaryFileInfo: 读取二进制文件信息(二进制文件信息通常是按照特定的数据块结构来存储或读取的，fstream.read())
		@filepath: 指定的二进制文件路径
		@retFileBlockInfo: 返回获取的二进制文件信息
	*/
	//static bool ReadBinaryFileInfo(const std::string &filepath, std::string &retFileBlockInfo);

	/*
		@WriteTextFileInfo: 写文件信息到文本文件
		@filepath: 指定输出的文件路径
		@fileInfo: 需要写入的文件内容
		@bIsAppend: 是否以追加方式写入

	*/
	static bool WriteTextFileInfo(const std::string &filepath, std::string const &fileInfo, bool bIsAppend = false);

	/*
		@WriteBinaryFileInfo: 写二进制文件信息到文件(二进制文件信息通常是按照特定的数据块结构来存储或读取的，fstream.write())
		@filepath: 指定输出的二进制文件路径
		@fileBlockInfo: 待写入的二进制文件信息
	*/
	//static bool WriteBinaryFileInfo(const std::string &filepath, const std::string &fileBlockInfo);

	/*
		@FormatFullFileName: 由文件路径文件名称以及文件扩展名构造完整的全路径文件名称
		@filepath: 指定文件路径(可能为空)
		@filename: 指定的文件名称(有可能直接带后缀也有可能为空)
		@filextension: 指定文件扩展名(有可能为空)
	*/
	static std::string FormatFullFileName(const std::string &filepath, const std::string &filename, const std::string &filextension);

	/*
		@GetFiles: 提取或过滤指定路径下的指定后缀文件
		@sInputPath: 待遍历的指定目录
		@retFileVec: 返回提取到的文件的容器
		@sSuffix: 指定待提取的特定后缀的文件，默认为"*.*"代表所有文件，若指定后缀需形如".shp" or "shp" 两种形式都可以
		@bIsFilter: 默认为 false 代表提取指定后缀文件；为 true 时，sSuffix参数必须有指定后缀，过滤掉指定后缀文件而将其他文件返回；否则直接返回
		@nDepth: 遍历深度，默认为包含所有子目录，也可修改为只遍历当前路径 LOOP_DEPTH::CURRENT_PATH
	*/
	static void GetFiles(std::string const &sInputPath, std::vector<std::string> &retFileVec, std::string const &sSuffix = "*.*", bool bIsFilter = false, LOOP_DEPTH nDepth = LOOP_DEPTH::ALL_PATH);

	/*
		@GetDirs: 提取或过滤指定路径下的指定后缀子目录
		@sInputPath: 待遍历的指定目录
		@retDirsVec: 返回提取到的子目录的容器
		@sSuffix: 指定待提取的特定后缀的子目录，默认为"*.*"代表所有文件与目录，若指定后缀需形如"gdb" 不包含.的扩展名
		@bIsFilter: 默认为 false 代表提取指定后缀子目录；为 true 时，sSuffix参数必须有指定后缀，过滤掉指定后缀子目录而将其他子目录返回；否则直接返回
		@nDepth: 遍历深度，默认为包含所有子目录，也可修改为只遍历当前路径 LOOP_DEPTH::CURRENT_PATH
	*/
	static void GetDirs(std::string const &sInputPath, std::vector<std::string> &retDirsVec, std::string const &sSuffix = "*.*", bool bIsFilter = false, LOOP_DEPTH nDepth = LOOP_DEPTH::ALL_PATH);

	/*
		@GetFileSize: 获取指定文件的文件大小
		@funit: 文件大小取值单位，默认以字节为单位
	*/
	static double GetFileSize(std::string const &filepath, FSIZE_UNIT funit = FSIZE_UNIT::FUNIT_BYTE);


	/************************************************************************/
	/*                      时间处理接口                                     */
	/************************************************************************/

	/*
		@GetNowTimeStr: 获取当前时间字符串并返回
		@bWithHMS: 时间字符串是否精确到时分秒，默认为true
		@bWithMilliSec: 是否精确到毫秒，默认为true
	*/
	static std::string GetNowTimeStr(bool bWithHMS = true, bool bWithMilliSec = true);
	
	/*
		@GetNowTimeStamp: 获取当前时间时间戳字符串
		@tsFlag: 时间戳的时间类型，默认为毫秒
	*/
	static std::string GetNowTimeStamp(TSTAMP_LEVEL tsFlag = TSTAMP_LEVEL::TS_MILLISEC);
	
	/*
		@TimeStringToTimeT: 将时间字符串转换为time_t类型的秒数
		@time_str: 待转换的时间字符串，默认时间格式形如: %d-%d-%d %d:%d:%d
	*/
	static time_t TimeStringToTimeT(const std::string &time_str);

	/*
		@TimeToTimeString: 将time_t类型转换为时间字符串返回
		@timepass: 待转换的time_t数据
	*/
	static std::string TimeToTimeString(time_t &timepass);


	/************************************************************************/
	/*				      json 处理接口                                      */
	/************************************************************************/

	/*
		@ParseJsonObjectToMap: 解析json对象字符串并将结果存至Map中
		@json_str：待解析的json对象字符串,形如："{\"four\":\"ffg\",\"one\":{\"four\":\"7\",\"one\":\"8\"},\"three\":\"messi\",\"two\":\"heib\"}"
		@retJsonMap：存放解析结果的Map
		@nExpLevel: 解析json时的展开级别
	*/
	static bool ParseJsonObjectToMap(const std::string &json_str, MapStrStrT &retJsonMap, EXPAND_LEVEL nExpLevel = EXPAND_LEVEL::FIRST_LEVEL);

	/*
		@ParseJsonArrayToMap: 解析json数组并将结果存至Map中
		@json_str: 待解析的json字符串
		@retJsonMapVec：存放解析结果的Vector,里面每个元素是每个对象的解析结果Map
		@nExpLevel: 解析json时的展开级别
	*/
	static bool ParseJsonArrayToMap(const std::string &json_str, VecMapStrStrT &retJsonMapVec, EXPAND_LEVEL nExpLevel = EXPAND_LEVEL::FIRST_LEVEL);

	/*
		@ParseJsonStrToVec: 解析json字符串并将结果存至Vector
		@json_str: 待解析的json字符串
		@retJsonMapVec：存放解析结果的Vector,里面每个元素是每个对象的解析结果Map
		@nExpLevel: 解析json时的展开级别
	*/
	static bool ParseJsonStrToVec(const std::string &json_str, VecMapStrStrT &retJsonMapVec, EXPAND_LEVEL nExpLevel = EXPAND_LEVEL::FIRST_LEVEL);

	/*
		@ToJsonArrayStr: 将指定存放的数据结构转换成json数组字符串
		@inJsonMapVec: 待转换的数据结构
	*/
	static std::string ToJsonArrayStr(const VecMapStrStrT &inJsonMapVec);

	/*
		@ToJsonObjectStr: 将指定Vector中的所有值按指定的key名称输出为json数组字符串
		@keyName: 指定Key名称，默认为空时，将以Index0,Index1...递增
	*/
	static std::string ToJsonArrayStr(const VecStr &inJsonVec, std::string const &keyName = "");

	/*
		@ToJsonObjectStr: 将指定数据结构转换成json对象字符串
		@inJsonMap: 待转换的数据结构
	*/
	static std::string ToJsonObjectStr(const MapStrStrT &inJsonMap);

	

private:

};