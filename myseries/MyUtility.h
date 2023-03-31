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
	@ͨ�õĽӿں�����ʵ��
	@author:mayabin
	@date:2023/03/15
	@description:�����ַ�������ʱ�䴦���ļ��ļ��д�������ת������json���������
*/

class CommUtility
{
public:
	CommUtility() = default;
	~CommUtility() = default;


	/************************************************************************/
	/*                        �ַ�������ӿ�                                 */
	/************************************************************************/

	/*
		@ToLower:���ַ���ת��Сд������
		@instr:��ת�����ַ���
	*/
	static std::string ToLower(const std::string &instr) noexcept;

	/*
		@ToLower:���ַ���ת��Сд(ԭ���޸�)
		@instr:��ת�����ַ���
	*/
	static void ToLowerInPlace(std::string &instr) noexcept;

	/*
		@ToLower:���ַ���ת�ɴ�д������
		@instr:��ת�����ַ���
	*/
	static std::string ToUpper(const std::string &instr) noexcept;

	/*
		@ToLower:���ַ���ת�ɴ�д(ԭ���޸�)
		@instr:��ת�����ַ���
	*/
	static void ToUpperInPlace(std::string &instr) noexcept;

	/*
		@IsStartWith:�ж�str�Ƿ���subStr��ʼ
		@str: �����Դ�ַ���
		@subStr: ��������ַ���
		@bIgnoreCase: �Ƿ���Դ�Сд(Ĭ�����ִ�Сд)
	*/
	static bool IsStartWith(const std::string &str, const std::string &subStr, bool bIgnoreCase = false);

	/*
		@IsEndWith:�ж�str�Ƿ���subStr��β
		@str: �����Դ�ַ���
		@subStr: ��������ַ���
		@bIgnoreCase: �Ƿ���Դ�Сд(Ĭ�����ִ�Сд)
	*/
	static bool IsEndWith(const std::string &str, const std::string &subStr, bool bIgnoreCase = false);

	/*
		@RemoveCharFromStr: ��str�ַ������Ƴ��ַ�c
		@str: ������������ַ���
		@c: ���Ƴ����ַ�c
	*/
	static void RemoveCharFromStr(std::string &str, char c);

	/*
		@TrimSpace:ɾ���ַ������ߵĿո�
		@str:�������ַ���
	*/
	static void TrimSpace(std::string &str);

	/*
		@ExtractMiddleStr: ��ȡ���Ӵ������Ӵ��м�����ַ���������
		@src: ����ȡ��Դ�ַ���
		@sub_left: ��ߵ����ַ���
		@sub_right: �ұߵ����ַ���
	*/
	static std::string ExtractMiddleStr(std::string const &src, std::string const &sub_left, std::string const &sub_right);

	/*
		@SplitStr: �ַ����ָ��������洢vector����
		@str: ���ָ��ַ���
		@retVecRes: �洢�ָ���������
		@sepStr: ָ���ָ��(�����ǵ��ַ�Ҳ���������ַ���)
	*/
	static void SplitStr(std::string const &str, std::vector<std::string> &retVecRes, std::string const &sepStr);

	///*
	//	@StringToWString: ��stringתΪwstring
	//	@instr: ��ת���ַ���
	//*/
	//static std::wstring StringToWString(const std::string &instr);

	///*
	//	@StringToWString: ��wstringתΪstring
	//	@instr: ��ת���ַ���
	//*/
	//static std::string WStringToString(const std::wstring &instr);

	/*
		@ReplaceStr: �����Ӵ��滻�����ַ����еľ��Ӵ�
		@str: �����Դ�ַ���
		@oldstr: ���滻�ľ��Ӵ�
		@newstr: �����滻�����Ӵ�
		@bIsReplaceAll: Ĭ��Ϊtrueȫ���滻;falseʱֻ�滻��һ�γ��ֵľ��Ӵ�
	*/
	static void ReplaceStr(std::string &str, const std::string &oldstr, const std::string &newstr, bool bIsReplaceAll = true);


	/************************************************************************/
	/*                   �ַ������������ת������ӿ�                         */
	/************************************************************************/

	/*
		@DoubleToStringSetBits: ��double���ݰ���ָ����С��λ�����������תΪstring���
		@df_in: �����double��������
		@bits: ָ����С��λ��
	*/
	static std::string DoubleToStringSetBits(double df_in, int bits);

	/*
		@DigitToString: ����ת�ַ���������nλС��
		@GUIntBig: �����ת��������
		@int n: ��Ҫ������С��λ
		@return: ����ת������ַ���	e.g: (23459874098,4) ---> "2345987.4098"
	*/
	static std::string DigitToString(GUIntBig num, int n);

	/*
		@StrToStrRoundN: �������ַ�����ʾ�����ְ�����Ҫ������λ����������ת�����µ��ַ���������
		@std::string const &s_in: ����ı�ʾ���ֵ��ַ���
		@int n: ��Ҫ������λ��
		@return: �����µĴ������ַ���	e.g: "45263.2165" --->  "45263.217"
	*/
	static std::string StrToStrRoundN(std::string const &s_in, int n);

	/*
		@StrToStrReaminN: �������ַ���ǿ�ư�����Ҫ����λ��ת��������
		@std::string const &s: �����ת�����ַ���
		@int n: ��Ҫ������λ��
		@return: ���ش���õ��ַ���
		e.g: ("4524",3) --->  "4524.000"; ("4524.346",5) --->  "4524.34600"; ("4524.346",2) --->  "4524.35"
	*/
	static std::string StrToStrReaminN(std::string const &s, int n);



	/************************************************************************/
	/*                 ����ת������ӿ�                                      */
	/************************************************************************/


	/*
		@ConvertToUtf8: �������ַ���תΪUTF8���벢����
		@str: ��ת�����ַ���
	*/
	static std::string ConvertToUtf8(std::string const &str);

	/*
		@IsUtf8Str:�ж�������ַ����Ƿ�ΪUTF8����
		@str: ������ַ���
	*/
	static bool IsUtf8Str(const char* str);

#ifdef _MSC_VER
	/*
		@ToUtf8OnWindows:��Windowsƽ̨�Ͻ��ַ���תΪUTF8����
		@str: ��ת�����ַ���
	*/
	static std::string ToUtf8OnWindows(const std::string &str);
#else
	/*
		@ToUtf8OnLinux:��Linuxƽ̨�Ͻ��ַ���תΪUTF8����
		@str: ��ת�����ַ���
	*/
	static std::string ToUtf8OnLinux(const std::string &str);

	/*
		@code_convert: ��Դ�ַ�������ת��ΪĿ���ַ�������
	*/
	static int code_convert(char * from_charset, char * to_charset, char * inbuf, size_t inlen, char * outbuf, size_t outlen);

	/*
		@u2g: utf-8 ת gb2312
	*/
	static std::string u2g(std::string const &instr);

	/*
		@g2u: gb2312 ת utf-8
	*/
	static std::string g2u(std::string const &instr);
#endif


	/************************************************************************/
	/*                       �ļ��ļ��д���ӿ�                              */
	/************************************************************************/

	/*
		@IsPathExists: ����ļ����ļ����Ƿ����
		@filepath: �������ļ�·��
	*/
	static bool IsPathExists(const std::string &filepath);

	/*
		@CreateDir: ������Ŀ¼
		@dirpath: ָ������������Ŀ¼·��
		@ע��mkdir(OnWindows)��mkdir -p(OnLinux)���ɴ����༶Ŀ¼
	*/
	static bool CreateDir(const std::string &dirpath);

	/*
		@DeleteDir: ɾ��ָ��Ŀ¼
		@dirpath: �������Ŀ¼·��
	*/
	static bool DeleteDir(const std::string &dirpath);

	/*
		@GetFilePathFromFullPath: ���ļ�ȫ·����ȡ����Ŀ¼·��
		@fileFullPath: ָ�����ļ�ȫ·����Ϣ
	*/
	static std::string GetFilePathFromFullPath(const std::string &fileFullPath) noexcept;

	/*
		@GetFileNameFromFullPath: ���ļ�ȫ·����ȡ�ļ���
		@fileFullPath: ָ�����ļ�ȫ·����Ϣ
	*/
	static std::string GetFileNameFromFullPath(const std::string &fileFullPath) noexcept;

	/*
		@GetFileExtensionFromFullPath: ���ļ�ȫ·����ȡ�ļ���չ��
		@fileFullPath: ָ�����ļ�ȫ·����Ϣ
		@bWithDot: ���ص���չ���Ƿ���㣬Ĭ�ϲ���
	*/
	static std::string GetFileExtensionFromFullPath(const std::string &fileFullPath, bool bWithDot = false) noexcept;

	/*
		@ReadFileInfo: ��ȡ�ı��ļ���Ϣ������������(Ĭ��Ϊ�ı��ļ������ж�ȡ�ķ�ʽ����)
		@filepath: ָ���ı��ļ�·����Ϣ
		@retFileContent: ���ش���ļ����ݵ�����(����Ϊ��λ�洢)
	*/
	static bool ReadTextFileInfo(const std::string &filepath, std::vector<std::string> &retFileContent);

	/*
		@ReadBinaryFileInfo: ��ȡ�������ļ���Ϣ(�������ļ���Ϣͨ���ǰ����ض������ݿ�ṹ���洢���ȡ�ģ�fstream.read())
		@filepath: ָ���Ķ������ļ�·��
		@retFileBlockInfo: ���ػ�ȡ�Ķ������ļ���Ϣ
	*/
	//static bool ReadBinaryFileInfo(const std::string &filepath, std::string &retFileBlockInfo);

	/*
		@WriteTextFileInfo: д�ļ���Ϣ���ı��ļ�
		@filepath: ָ��������ļ�·��
		@fileInfo: ��Ҫд����ļ�����
		@bIsAppend: �Ƿ���׷�ӷ�ʽд��

	*/
	static bool WriteTextFileInfo(const std::string &filepath, std::string const &fileInfo, bool bIsAppend = false);

	/*
		@WriteBinaryFileInfo: д�������ļ���Ϣ���ļ�(�������ļ���Ϣͨ���ǰ����ض������ݿ�ṹ���洢���ȡ�ģ�fstream.write())
		@filepath: ָ������Ķ������ļ�·��
		@fileBlockInfo: ��д��Ķ������ļ���Ϣ
	*/
	//static bool WriteBinaryFileInfo(const std::string &filepath, const std::string &fileBlockInfo);

	/*
		@FormatFullFileName: ���ļ�·���ļ������Լ��ļ���չ������������ȫ·���ļ�����
		@filepath: ָ���ļ�·��(����Ϊ��)
		@filename: ָ�����ļ�����(�п���ֱ�Ӵ���׺Ҳ�п���Ϊ��)
		@filextension: ָ���ļ���չ��(�п���Ϊ��)
	*/
	static std::string FormatFullFileName(const std::string &filepath, const std::string &filename, const std::string &filextension);

	/*
		@GetFiles: ��ȡ�����ָ��·���µ�ָ����׺�ļ�
		@sInputPath: ��������ָ��Ŀ¼
		@retFileVec: ������ȡ�����ļ�������
		@sSuffix: ָ������ȡ���ض���׺���ļ���Ĭ��Ϊ"*.*"���������ļ�����ָ����׺������".shp" or "shp" ������ʽ������
		@bIsFilter: Ĭ��Ϊ false ������ȡָ����׺�ļ���Ϊ true ʱ��sSuffix����������ָ����׺�����˵�ָ����׺�ļ����������ļ����أ�����ֱ�ӷ���
		@nDepth: ������ȣ�Ĭ��Ϊ����������Ŀ¼��Ҳ���޸�Ϊֻ������ǰ·�� LOOP_DEPTH::CURRENT_PATH
	*/
	static void GetFiles(std::string const &sInputPath, std::vector<std::string> &retFileVec, std::string const &sSuffix = "*.*", bool bIsFilter = false, LOOP_DEPTH nDepth = LOOP_DEPTH::ALL_PATH);

	/*
		@GetDirs: ��ȡ�����ָ��·���µ�ָ����׺��Ŀ¼
		@sInputPath: ��������ָ��Ŀ¼
		@retDirsVec: ������ȡ������Ŀ¼������
		@sSuffix: ָ������ȡ���ض���׺����Ŀ¼��Ĭ��Ϊ"*.*"���������ļ���Ŀ¼����ָ����׺������"gdb" ������.����չ��
		@bIsFilter: Ĭ��Ϊ false ������ȡָ����׺��Ŀ¼��Ϊ true ʱ��sSuffix����������ָ����׺�����˵�ָ����׺��Ŀ¼����������Ŀ¼���أ�����ֱ�ӷ���
		@nDepth: ������ȣ�Ĭ��Ϊ����������Ŀ¼��Ҳ���޸�Ϊֻ������ǰ·�� LOOP_DEPTH::CURRENT_PATH
	*/
	static void GetDirs(std::string const &sInputPath, std::vector<std::string> &retDirsVec, std::string const &sSuffix = "*.*", bool bIsFilter = false, LOOP_DEPTH nDepth = LOOP_DEPTH::ALL_PATH);

	/*
		@GetFileSize: ��ȡָ���ļ����ļ���С
		@funit: �ļ���Сȡֵ��λ��Ĭ�����ֽ�Ϊ��λ
	*/
	static double GetFileSize(std::string const &filepath, FSIZE_UNIT funit = FSIZE_UNIT::FUNIT_BYTE);


	/************************************************************************/
	/*                      ʱ�䴦��ӿ�                                     */
	/************************************************************************/

	/*
		@GetNowTimeStr: ��ȡ��ǰʱ���ַ���������
		@bWithHMS: ʱ���ַ����Ƿ�ȷ��ʱ���룬Ĭ��Ϊtrue
		@bWithMilliSec: �Ƿ�ȷ�����룬Ĭ��Ϊtrue
	*/
	static std::string GetNowTimeStr(bool bWithHMS = true, bool bWithMilliSec = true);
	
	/*
		@GetNowTimeStamp: ��ȡ��ǰʱ��ʱ����ַ���
		@tsFlag: ʱ�����ʱ�����ͣ�Ĭ��Ϊ����
	*/
	static std::string GetNowTimeStamp(TSTAMP_LEVEL tsFlag = TSTAMP_LEVEL::TS_MILLISEC);
	
	/*
		@TimeStringToTimeT: ��ʱ���ַ���ת��Ϊtime_t���͵�����
		@time_str: ��ת����ʱ���ַ�����Ĭ��ʱ���ʽ����: %d-%d-%d %d:%d:%d
	*/
	static time_t TimeStringToTimeT(const std::string &time_str);

	/*
		@TimeToTimeString: ��time_t����ת��Ϊʱ���ַ�������
		@timepass: ��ת����time_t����
	*/
	static std::string TimeToTimeString(time_t &timepass);


	/************************************************************************/
	/*				      json ����ӿ�                                      */
	/************************************************************************/

	/*
		@ParseJsonObjectToMap: ����json�����ַ��������������Map��
		@json_str����������json�����ַ���,���磺"{\"four\":\"ffg\",\"one\":{\"four\":\"7\",\"one\":\"8\"},\"three\":\"messi\",\"two\":\"heib\"}"
		@retJsonMap����Ž��������Map
		@nExpLevel: ����jsonʱ��չ������
	*/
	static bool ParseJsonObjectToMap(const std::string &json_str, MapStrStrT &retJsonMap, EXPAND_LEVEL nExpLevel = EXPAND_LEVEL::FIRST_LEVEL);

	/*
		@ParseJsonArrayToMap: ����json���鲢���������Map��
		@json_str: ��������json�ַ���
		@retJsonMapVec����Ž��������Vector,����ÿ��Ԫ����ÿ������Ľ������Map
		@nExpLevel: ����jsonʱ��չ������
	*/
	static bool ParseJsonArrayToMap(const std::string &json_str, VecMapStrStrT &retJsonMapVec, EXPAND_LEVEL nExpLevel = EXPAND_LEVEL::FIRST_LEVEL);

	/*
		@ParseJsonStrToVec: ����json�ַ��������������Vector
		@json_str: ��������json�ַ���
		@retJsonMapVec����Ž��������Vector,����ÿ��Ԫ����ÿ������Ľ������Map
		@nExpLevel: ����jsonʱ��չ������
	*/
	static bool ParseJsonStrToVec(const std::string &json_str, VecMapStrStrT &retJsonMapVec, EXPAND_LEVEL nExpLevel = EXPAND_LEVEL::FIRST_LEVEL);

	/*
		@ToJsonArrayStr: ��ָ����ŵ����ݽṹת����json�����ַ���
		@inJsonMapVec: ��ת�������ݽṹ
	*/
	static std::string ToJsonArrayStr(const VecMapStrStrT &inJsonMapVec);

	/*
		@ToJsonObjectStr: ��ָ��Vector�е�����ֵ��ָ����key�������Ϊjson�����ַ���
		@keyName: ָ��Key���ƣ�Ĭ��Ϊ��ʱ������Index0,Index1...����
	*/
	static std::string ToJsonArrayStr(const VecStr &inJsonVec, std::string const &keyName = "");

	/*
		@ToJsonObjectStr: ��ָ�����ݽṹת����json�����ַ���
		@inJsonMap: ��ת�������ݽṹ
	*/
	static std::string ToJsonObjectStr(const MapStrStrT &inJsonMap);

	

private:

};