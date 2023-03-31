#include "MyUtility.h"
//#include "cpl_string.h"

std::string CommUtility::ToLower(const std::string &instr) noexcept
{
	std::string sTmpStr = instr;
	std::transform(sTmpStr.begin(), sTmpStr.end(), sTmpStr.begin(), ::tolower);
	return sTmpStr;
}

void CommUtility::ToLowerInPlace(std::string &instr) noexcept
{
	std::transform(instr.begin(), instr.end(), instr.begin(), ::tolower);
	return;
}

std::string CommUtility::ToUpper(const std::string &instr) noexcept
{
	std::string sTmpStr = instr;
	std::transform(sTmpStr.begin(), sTmpStr.end(), sTmpStr.begin(), ::toupper);
	return sTmpStr;
}

void CommUtility::ToUpperInPlace(std::string &instr) noexcept
{
	std::transform(instr.begin(), instr.end(), instr.begin(), ::toupper);
	return;
}

bool CommUtility::IsStartWith(const std::string &str, const std::string &subStr, bool bIgnoreCase /*= false*/)
{
	std::string sTmpStr = str;
	std::string sTmpSubStr = subStr;

	if (bIgnoreCase) {
		ToLowerInPlace(sTmpStr);
		ToLowerInPlace(sTmpSubStr);
	}

	size_t npos = sTmpStr.find(sTmpSubStr);
	if (npos == std::string::npos || npos > 0) return false;

	return sTmpSubStr == sTmpStr.substr(0, sTmpSubStr.length());
}

bool CommUtility::IsEndWith(const std::string &str, const std::string &subStr, bool bIgnoreCase /*= false*/)
{
	std::string sTmpStr = str;
	std::string sTmpSubStr = subStr;

	if (bIgnoreCase) {
		ToLowerInPlace(sTmpStr);
		ToLowerInPlace(sTmpSubStr);
	}

	size_t npos = sTmpStr.rfind(sTmpSubStr);
	if (npos == std::string::npos) return false;

	return sTmpSubStr == sTmpStr.substr(npos);
}

void CommUtility::RemoveCharFromStr(std::string &str, char c)
{
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
	return;
}

void CommUtility::TrimSpace(std::string &str)
{
	if (str.empty()) {
		return;
	}
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
}

void CommUtility::GetFiles(std::string const &sInputPath, std::vector<std::string> &retFileVec, std::string const &sSuffix /*= "*.*"*/, bool bIsFilter /*= false*/, LOOP_DEPTH nDepth /*= LOOP_DEPTH::ALL_PATH*/)
{
	if (sInputPath.empty()) return;

	std::string sExtrPath = sInputPath;
	if (sExtrPath.back() != DIR_SEP) sExtrPath += DIR_SEP;

	if (bIsFilter && sSuffix == "*.*") return;

	std::string sResetSuffix;
	if (nDepth == LOOP_DEPTH::ALL_PATH) {
		sResetSuffix = "*.*";
	}
	else {
		if (sSuffix != "*.*") {
			if (!bIsFilter) {
				sSuffix.find(".") != std::string::npos ? sResetSuffix.append("*").append(sSuffix) : sResetSuffix.append("*.").append(sSuffix);
			}
			else
				sResetSuffix = "*.*";
		}
		else {
			sResetSuffix = sSuffix;
		}
	}

	auto fnFileProcess = [&](std::string const & sFilename) {
		if (sSuffix == "*.*") {
			retFileVec.emplace_back(sFilename);
			return;
		}

		if (bIsFilter) {		//filter specified suffix
			if (!IsEndWith(sFilename, sSuffix)) {
				retFileVec.emplace_back(sFilename);
			}
		}
		else {		//extract specified suffix
			if (IsEndWith(sFilename, sSuffix)) {
				retFileVec.emplace_back(sFilename);
			}
		}
	};

	std::string sInitExtrPath = sExtrPath;

#ifdef _MSC_VER
	sExtrPath += sResetSuffix;
	_finddata_t fileInfo;
	intptr_t retHandle = _findfirst(sExtrPath.c_str(), &fileInfo);
	if (retHandle == std::string::npos) {
		MYLOG_WARN << "Invalid return handle.";
		return;
	}
	
	do 
	{
		if (fileInfo.attrib & _A_SUBDIR) {
			if (!strcmp(fileInfo.name, ".") || !strcmp(fileInfo.name, "..")) continue;

			std::string sSubDirPath = sInitExtrPath + fileInfo.name + DIR_SEP;
			if (nDepth == LOOP_DEPTH::CURRENT_PATH) {
				fnFileProcess(sSubDirPath);
			}
			else {
				GetFiles(sSubDirPath, retFileVec, sSuffix, bIsFilter, nDepth);
			}
		}
		else {
			std::string sFileName = sInitExtrPath + fileInfo.name;
			fnFileProcess(sFileName);
		}
	} while (_findnext(retHandle, &fileInfo) == 0);
	
	_findclose(retHandle);
#else
	DIR *ptrDir = nullptr;
	struct dirent *ptrFile = nullptr;
	ptrDir = opendir(sInitExtrPath.c_str());
	if (!ptrDir) {
		MYLOG_ERROR << "Open input directory path error.";
		return;
	}

	while ((ptrFile = readdir(ptrDir)) != nullptr) {
		if (!strcmp(ptrFile->d_name, ".") || !strcmp(ptrFile->d_name, "..")) continue;

		if (ptrFile->d_type == 4) {		//sub directory
			std::string sSubDirPath = sInitExtrPath + ptrFile->d_name + DIR_SEP;
			if (nDepth == LOOP_DEPTH::CURRENT_PATH) {
				fnFileProcess(sSubDirPath);
			}
			else {
				GetFiles(sSubDirPath, retFileVec, sSuffix, bIsFilter, nDepth);
			}
		}
		else if (ptrFile->d_type == 8) {	//sub file
			std::string sFileName = sInitExtrPath + ptrFile->d_name;
			fnFileProcess(sFileName);
		}
	}

	closedir(ptrDir);
#endif

	return;
}

void CommUtility::GetDirs(std::string const &sInputPath, std::vector<std::string> &retDirsVec, std::string const &sSuffix /*= "*.*"*/, bool bIsFilter /*= false*/, LOOP_DEPTH nDepth /*= LOOP_DEPTH::ALL_PATH*/)
{
	if (sInputPath.empty()) return;

	std::string sExtrPath = sInputPath;
	if (sExtrPath.back() != DIR_SEP) sExtrPath += DIR_SEP;

	if (bIsFilter && sSuffix == "*.*") return;		//must specified suffix when bIsFilter is true

	auto fnDirProcess = [&](const std::string &sDirName) {
		if (sSuffix == "*.*") {
			retDirsVec.emplace_back(sDirName);
			return;
		}

		if (bIsFilter) {
			if (!IsEndWith(sDirName, sSuffix)) {
				retDirsVec.emplace_back(sDirName);
			}
		}
		else {
			if (IsEndWith(sDirName, sSuffix)) {
				retDirsVec.emplace_back(sDirName);
			}
		}
	};

	std::string sInitDirPath = sExtrPath;

#ifdef _MSC_VER
	sExtrPath += "*.*";
	_finddata_t fileDirInfo;
	intptr_t fileDirHandle = _findfirst(sExtrPath.c_str(), &fileDirInfo);
	if (fileDirHandle == std::string::npos) {
		MYLOG_WARN << "Invalid return fileDir handle.";
		return;
	}

	do 
	{
		if (fileDirInfo.attrib & _A_SUBDIR) {
			if (!strcmp(fileDirInfo.name, ".") || !strcmp(fileDirInfo.name, "..")) continue;

			std::string sSubDirPath = sInitDirPath + fileDirInfo.name;
			fnDirProcess(sSubDirPath);

			if (nDepth == LOOP_DEPTH::CURRENT_PATH) continue;

			GetDirs(sSubDirPath, retDirsVec, sSuffix, bIsFilter, nDepth);
		}
	} while (_findnext(fileDirHandle, &fileDirInfo) == 0);

	_findclose(fileDirHandle);
#else
	DIR *ptrDir = nullptr;
	struct dirent *ptrFile = nullptr;
	ptrDir = opendir(sInitDirPath.c_str());
	if (!ptrDir) {
		MYLOG_ERROR << "Open input directory path error.";
		return;
	}

	while ((ptrFile = readdir(ptrDir)) != nullptr)
	{
		if (!strcmp(ptrFile->d_name, ".") || !strcmp(ptrFile->d_name, "..")) continue;

		if (ptrFile->d_type == 4) {		//sub directory
			std::string sSubDirPath = sInitDirPath + ptrFile->d_name;
			fnDirProcess(sSubDirPath);

			if (nDepth == LOOP_DEPTH::CURRENT_PATH) continue;

			GetDirs(sSubDirPath, retDirsVec, sSuffix, bIsFilter, nDepth);
		}
	}

	closedir(ptrDir);
#endif

	return;
}

double CommUtility::GetFileSize(std::string const &filepath, FSIZE_UNIT funit /*= FSIZE_UNIT::FUNIT_BYTE*/)
{
	double dfRetFileSize = -1.0;
	if (!IsPathExists(filepath)) return dfRetFileSize;

	struct stat _fileInfo;
	int nret = stat(filepath.c_str(), &_fileInfo);	//default byte;
	if (nret != 0) {
		MYLOG_ERROR << "Get file size error.";
		return dfRetFileSize;
	}

	dfRetFileSize = _fileInfo.st_size;
	switch (funit)
	{
	case FSIZE_UNIT::FUNIT_BYTE:
		break;
	case FSIZE_UNIT::FUNIT_KB: {
		dfRetFileSize /= 1024.0;
		break;
	}
	case FSIZE_UNIT::FUNIT_MB: {
		dfRetFileSize /= (1024.0 * 1024.0);
		break;
	}
	case FSIZE_UNIT::FUNIT_GB: {
		dfRetFileSize /= (1024.0 * 1024.0 * 1024.0);
		break;
	}
	default:
		break;
	}

	return dfRetFileSize;
}

std::string CommUtility::ExtractMiddleStr(std::string const &src, std::string const &sub_left, std::string const &sub_right)
{
	std::string sReturnStr;
	if (src.empty() || sub_left.empty() || sub_right.empty()) return sReturnStr;

	size_t npos_left = src.find(sub_left);
	if (npos_left == std::string::npos) return sReturnStr;

	npos_left += sub_left.size();
	size_t npos_right = src.find(sub_right, npos_left);

	if (npos_right == std::string::npos || npos_left >= npos_right) return sReturnStr;

	sReturnStr = src.substr(npos_left, npos_right - npos_left);
	return sReturnStr;
}

void CommUtility::SplitStr(std::string const &str, std::vector<std::string> &retVecRes, std::string const &sepStr)
{
	if (str.empty() || sepStr.empty()) return;

	size_t npos_begin = 0;
	size_t npos_end = str.find(sepStr, npos_begin);
	if (npos_end == std::string::npos) {
		retVecRes.emplace_back(str);
		return;
	}

	std::string sTmpExtrStr;
	while (npos_end != std::string::npos)
	{
		sTmpExtrStr = str.substr(npos_begin, npos_end - npos_begin);
		retVecRes.emplace_back(std::move(sTmpExtrStr));

		npos_begin = npos_end + sepStr.size();
		npos_end = str.find(sepStr, npos_begin);
	}

	sTmpExtrStr = str.substr(npos_begin);
	retVecRes.emplace_back(std::move(sTmpExtrStr));

	return;
}

//std::wstring CommUtility::StringToWString(const std::string &instr)
//{
//	wchar_t* pszOutStr = CPLRecodeToWChar(instr.c_str(), CPL_ENC_UTF8, CPL_ENC_UCS2);
//	std::wstring outstr = pszOutStr;
//	CPLFree(pszOutStr);
//	return outstr;
//}
//
//std::string CommUtility::WStringToString(const std::wstring &instr)
//{
//	char* pszOutStr = CPLRecodeFromWChar(instr.c_str(), CPL_ENC_UCS2, CPL_ENC_UTF8);
//	std::string outstr = pszOutStr;
//	CPLFree(pszOutStr);
//	return outstr;
//}

void CommUtility::ReplaceStr(std::string &str, const std::string &oldstr, const std::string &newstr, bool bIsReplaceAll /*= true*/)
{
	if (str.empty() || oldstr.empty() || newstr.empty()) return;

	size_t npos_begin = 0;
	size_t npos_end = str.find(oldstr, npos_begin);
	if (npos_end == std::string::npos) return;

	while (npos_end != std::string::npos)
	{
		str.replace(npos_end, oldstr.size(), newstr);

		if(!bIsReplaceAll) break;

		npos_begin = npos_end + newstr.size();
		npos_end = str.find(oldstr, npos_begin);
	}

	return;
}

std::string CommUtility::DoubleToStringSetBits(double df_in, int bits)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(bits) << df_in;
	std::string sret;
	ss >> sret;
	return sret;
}

std::string CommUtility::DigitToString(GUIntBig num, int n)
{
	std::string s_ret;
	double df_out = num / pow(10, n);
	s_ret = DoubleToStringSetBits(df_out, n);
	return s_ret;
}

std::string CommUtility::StrToStrRoundN(std::string const &s_in, int n)
{
	std::string sret;
	double dfTmp = atof(s_in.c_str());
	if (n == 0) {
		int nret = (int)dfTmp;
		sret = std::to_string(nret);
	}
	else {
		double dfRet = round(dfTmp * pow(10, n)) / pow(10, n);
		sret = DoubleToStringSetBits(dfRet, n);
	}
	return sret;
}

std::string CommUtility::StrToStrReaminN(std::string const &s, int n)
{
	std::string sval;
	int npos = s.find_first_of(".");
	if (npos == std::string::npos) {
		sval = s + "." + std::string(n, '0');
	}
	else {
		int num = s.substr(npos + 1).size();
		if (num <= n) {
			std::string sbefore = s.substr(0, npos);
			std::string safter = (s.substr(npos + 1) + std::string(n - num, '0'));
			sval = sbefore + "." + safter;
		}
		else {
			sval = StrToStrRoundN(s, n);
		}
	}
	return sval;
}

std::string CommUtility::ConvertToUtf8(std::string const &str)
{
	std::string sRetUtf8Str = str;
	if (IsUtf8Str(sRetUtf8Str.c_str())) {
		return sRetUtf8Str;
	}

#ifdef _MSC_VER
	return ToUtf8OnWindows(sRetUtf8Str);
#else
	return ToUtf8OnLinux(sRetUtf8Str);
#endif
}

bool CommUtility::IsUtf8Str(const char* str)
{
	unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	unsigned char chr = *str;
	bool bAllAscii = true;
	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx 
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}
		if (nBytes == 0) {
			//如果不是ASCII码,应该是多字节符,计算字节数  
			if (chr >= 0x80) {

				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				}
				else if (chr >= 0xF8) {
					nBytes = 5;
				}
				else if (chr >= 0xF0) {
					nBytes = 4;
				}
				else if (chr >= 0xE0) {
					nBytes = 3;
				}
				else if (chr >= 0xC0) {
					nBytes = 2;
				}
				else {
					return false;
				}

				nBytes--;
			}
		}
		else {
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			nBytes--;
		}
	}
	//违返UTF8编码规则 
	if (nBytes != 0) {
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
		return true;
	}
	return true;
}

#ifdef _MSC_VER
std::string CommUtility::ToUtf8OnWindows(const std::string &str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
#else
std::string CommUtility::ToUtf8OnLinux(const std::string &str)
{
	return g2u(str);
}

int CommUtility::code_convert(char * from_charset, char * to_charset, char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;

	memset(outbuf, 0, outlen);

	if ((int)iconv(cd, pin, &inlen, pout, &outlen) == -1)
	{
		iconv_close(cd);
		return -1;
	}
	iconv_close(cd);
	*pout = '\0';

	return 0;
}

std::string CommUtility::u2g(std::string const &instr)
{
	int inlen = instr.length();
	int outlen = inlen * 2 + 1;
	std::string retstr = "";
	retstr.resize(outlen);
	code_convert("utf-8", "gb2312", const_cast<char*>(instr.c_str()), inlen, const_cast<char*>(retstr.data()), retstr.size());

	retstr = retstr.c_str();
	return retstr;
}

std::string CommUtility::g2u(std::string const &instr)
{
	int inlen = instr.length();
	int outlen = inlen * 2 + 1;
	std::string retstr = "";
	retstr.resize(outlen);
	code_convert("gb2312", "utf-8", const_cast<char*>(instr.c_str()), inlen, const_cast<char*>(retstr.data()), retstr.size());

	retstr = retstr.c_str();
	return retstr;
}
#endif

bool CommUtility::IsPathExists(const std::string &filepath)
{
#ifdef _MSC_VER
	return 0 == _access(filepath.c_str(), 0);
#else
	return 0 == access(filepath.c_str(), 0);
#endif
}

bool CommUtility::CreateDir(const std::string &dirpath)
{
	if (IsPathExists(dirpath)) {
		MYLOG_INFO << "Directory already exists.";
		return true;
	}

	std::string sCommondLine;
#ifdef _MSC_VER
	sCommondLine = std::string("mkdir ") + "\"" + dirpath + "\"";	//Path contains spaces are supported on windows, so add quotes to the path.
 #else
	sCommondLine = std::string("mkdir -p ") + dirpath;
#endif

	return 0 == system(sCommondLine.c_str());
}

bool CommUtility::DeleteDir(const std::string &dirpath)
{
	if (!IsPathExists(dirpath)) {
		MYLOG_INFO << "Directory not exists.";
		return true;
	}

	std::string sCommondLine;
#ifdef _MSC_VER
	sCommondLine = std::string("rmdir ") + "\"" + dirpath + "\"" + " /s /q";	//Path contains spaces are supported on windows, so add quotes to the path.
#else
	sCommondLine = std::string("rm -rf ") + dirpath;
#endif

	return 0 == system(sCommondLine.c_str());
}

std::string CommUtility::GetFilePathFromFullPath(const std::string &fileFullPath) noexcept
{
	std::string sRetDirPath;
	if (fileFullPath.empty()) return sRetDirPath;

	size_t npos = fileFullPath.find_last_of("\\/");
	if (npos == std::string::npos) return fileFullPath;

	sRetDirPath = fileFullPath.substr(0, npos);
	return sRetDirPath;
}

std::string CommUtility::GetFileNameFromFullPath(const std::string &fileFullPath) noexcept
{
	std::string sRetFileName;
	if (fileFullPath.empty()) return sRetFileName;

	size_t npos_dot = std::string::npos;
	size_t npos_slash = fileFullPath.find_last_of("\\/");
	if (npos_slash == std::string::npos) {
		npos_dot = fileFullPath.find_last_of(".");
		if (npos_dot == std::string::npos) {
			return fileFullPath;
		}
		else {
			sRetFileName = fileFullPath.substr(0, npos_dot);
			return sRetFileName;
		}
	}
	else {
		std::string sTmpFilename = fileFullPath.substr(npos_slash + 1);
		npos_dot = sTmpFilename.find_last_of(".");
		if (npos_dot == std::string::npos) {
			return sTmpFilename;
		}
		else {
			sRetFileName = sTmpFilename.substr(0, npos_dot);
			return sRetFileName;
		}
	}
}

std::string CommUtility::GetFileExtensionFromFullPath(const std::string &fileFullPath, bool bWithDot /*= false*/) noexcept
{
	std::string sRetExtension;
	if (fileFullPath.empty()) return sRetExtension;

	size_t npos_dot = fileFullPath.find_last_of(".");
	if (npos_dot == std::string::npos) {
		MYLOG_ERROR << "Input file path format error.";
		return sRetExtension;
	}

	if (bWithDot) {
		sRetExtension = fileFullPath.substr(npos_dot);
	}
	else {
		sRetExtension = fileFullPath.substr(npos_dot + 1);
	}

	return sRetExtension;
}

bool CommUtility::ReadTextFileInfo(const std::string &filepath, std::vector<std::string> &retFileContent)
{
	if (!IsPathExists(filepath)) {
		MYLOG_ERROR << "File not exists!";
		return false;
	}

	std::fstream infile(filepath, std::ios::in);

	if (!infile) {
		MYLOG_ERROR << "Open file error.";
		return false;
	}

	std::string sEachLineInfo;
	while (infile >> sEachLineInfo)		// while (getline(infile, sEachLineInfo))  or while(!infile.eof())
	{
		if (!sEachLineInfo.empty()) retFileContent.emplace_back(std::move(sEachLineInfo));
	}

	infile.close();
	return true;
}

bool CommUtility::WriteTextFileInfo(const std::string &filepath, std::string const &fileInfo, bool bIsAppend /*= false*/)
{
	std::string sFileDir = GetFilePathFromFullPath(filepath);
	CreateDir(sFileDir);

	std::fstream outfile;
	if (bIsAppend) {
		outfile.open(filepath.c_str(), std::ios::out | std::ios::app);
	}
	else {
		outfile.open(filepath.c_str(), std::ios::out | std::ios::trunc);
	}

	if (!outfile.is_open()) {
		MYLOG_ERROR << "Open file for write error.";
		return false;
	}

	if (!outfile.good()) {		//good() member function check stream status is ok
		MYLOG_ERROR << "Out stream status abnormal.[eofbit=" << outfile.eof() << " badbit=" << outfile.bad() << " failbit=" << outfile.fail() << "]";
		return false;
	}

	outfile << fileInfo;
	outfile.close();

	return true;
}

std::string CommUtility::FormatFullFileName(const std::string &filepath, const std::string &filename, const std::string &filextension)
{
	std::string sRetFileName;
	if (filepath.empty() && filename.empty() && filextension.empty()) return sRetFileName;

	if (!filepath.empty()) {
		if (filepath.back() != DIR_SEP) sRetFileName = filepath + DIR_SEP;
		else sRetFileName = filepath;
	}

	if (!filename.empty()) sRetFileName += filename;

	if (!filextension.empty()) {
		size_t npos_dot = filextension.find(".");
		if (npos_dot == std::string::npos) {
			sRetFileName = sRetFileName + "." + filextension;
		}
		else {
			sRetFileName += filextension;
		}
	}

	return sRetFileName;
}

std::string CommUtility::GetNowTimeStr(bool bWithHMS /*= true*/, bool bWithMilliSec /*= true*/)
{
	timeb tb;
	ftime(&tb);
	char cTimeStr[128] = { 0 };
	if (bWithHMS) {
		strftime(cTimeStr, sizeof(cTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&tb.time));
		if (bWithMilliSec) {
			sprintf(cTimeStr + strlen(cTimeStr), ".%d", tb.millitm);
		}
	}
	else {
		strftime(cTimeStr, sizeof(cTimeStr), "%Y-%m-%d", localtime(&tb.time));
	}

	std::string sNowTime = cTimeStr;
	return sNowTime;
}

std::string CommUtility::GetNowTimeStamp(TSTAMP_LEVEL tsFlag /*= TSTAMP_LEVEL::TS_MILLISEC*/)
{
	std::string sTimeStamps;
	auto nowTime = std::chrono::system_clock::now();  //time_point
	int64_t nTimeSeconds = 0;
	switch (tsFlag)
	{
	case TSTAMP_LEVEL::TS_NANOSEC:
		nTimeSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime.time_since_epoch()).count();
		break;
	case TSTAMP_LEVEL::TS_MICROSEC:
		nTimeSeconds = std::chrono::duration_cast<std::chrono::microseconds>(nowTime.time_since_epoch()).count();
		break;
	case TSTAMP_LEVEL::TS_MILLISEC:
		nTimeSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime.time_since_epoch()).count();
		break;
	case TSTAMP_LEVEL::TS_SECOND:
		nTimeSeconds = std::chrono::duration_cast<std::chrono::seconds>(nowTime.time_since_epoch()).count();
		break;
	case TSTAMP_LEVEL::TS_MINUTES:
		nTimeSeconds = std::chrono::duration_cast<std::chrono::minutes>(nowTime.time_since_epoch()).count();
		break;
	case TSTAMP_LEVEL::TS_HOUR:
		nTimeSeconds = std::chrono::duration_cast<std::chrono::hours>(nowTime.time_since_epoch()).count();
		break;
	default:
		break;
	}

	std::stringstream ss;
	ss << nTimeSeconds;
	sTimeStamps = ss.str();

	return sTimeStamps;
}

time_t CommUtility::TimeStringToTimeT(const std::string &time_str)
{
	struct tm _tm;
	int year = 0, month = 0, day = 0, hour = 0, mintue = 0, second = 0;
	sscanf(time_str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &mintue, &second);

	_tm.tm_year = year - 1900;
	_tm.tm_mon = month - 1;
	_tm.tm_mday = day;
	_tm.tm_hour = hour;
	_tm.tm_min = mintue;
	_tm.tm_sec = second;
	_tm.tm_isdst = 0;		/*夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。*/

	time_t _t = mktime(&_tm);
	return _t;
}

std::string CommUtility::TimeToTimeString(time_t &timepass)
{
	struct tm _tm;
#if defined(_MSC_VER)
	localtime_s(&_tm, &timepass);
#else
	localtime_r(&timepass, &_tm);
#endif

	char cTimeStr[64] = { 0 };
	strftime(cTimeStr, sizeof(cTimeStr), "%Y-%m-%d %H:%M:%S", &_tm);

	std::string sRetTimeStr = cTimeStr;
	return sRetTimeStr;
}

bool CommUtility::ParseJsonObjectToMap(const std::string &json_str, MapStrStrT &retJsonMap, EXPAND_LEVEL nExpLevel /*= EXPAND_LEVEL::FIRST_LEVEL*/)
{
	if (json_str.empty()) {
		MYLOG_WARN << "Input json string is empty.";
		return true;
	}

	jsoncons::json j_parse_res = jsoncons::json::parse(json_str);
	if (!j_parse_res.is_object()) {
		MYLOG_ERROR << "Input string is not json object.";
		return false;
	}

	for (auto tmpjsonItem : j_parse_res.object_range())
	{
		std::string jsonKey = tmpjsonItem.key().data();
		std::string jsonValue = tmpjsonItem.value().as<std::string>();

		if (tmpjsonItem.value().is_object()) {
			if (nExpLevel == EXPAND_LEVEL::SECOND_LEVEL) {
				if (!ParseJsonObjectToMap(jsonValue, retJsonMap, nExpLevel)) {
					MYLOG_ERROR << "Parse json error.[" << jsonValue << "]";
					return false;
				}
			}
			else {
				retJsonMap.emplace(std::move(jsonKey), std::move(jsonValue));
			}
		}
		else {
			retJsonMap.emplace(std::move(jsonKey), std::move(jsonValue));
		}
	}
	
	return true;
}

bool CommUtility::ParseJsonArrayToMap(const std::string &json_str, VecMapStrStrT &retJsonMapVec, EXPAND_LEVEL nExpLevel /*= EXPAND_LEVEL::FIRST_LEVEL*/)
{
	if (json_str.empty()) {
		MYLOG_ERROR << "Input json string is empty.";
		return false;
	}

	jsoncons::json j_parse_res = jsoncons::json::parse(json_str);
	if (!j_parse_res.is_array()) {
		MYLOG_ERROR << "Input string is not json array.";
		return false;
	}

	for (auto jsonItem : j_parse_res.array_range())
	{
		MapStrStrT pTmpParseMap;
		std::string sJsonStr = jsonItem.as_string();
		if (jsonItem.is_object()) {
			if (!ParseJsonObjectToMap(sJsonStr, pTmpParseMap, nExpLevel)) {
				MYLOG_ERROR << "Parse json object error. [" << sJsonStr << "]";
				return false;
			}
		}
		else {
			MYLOG_WARN << "Not json object, key: " << sJsonStr << " and value: ";
			pTmpParseMap.emplace(sJsonStr, "");
		}
		retJsonMapVec.emplace_back(std::move(pTmpParseMap));
	}
	
	return true;
}

bool CommUtility::ParseJsonStrToVec(const std::string &json_str, VecMapStrStrT &retJsonMapVec, EXPAND_LEVEL nExpLevel /*= EXPAND_LEVEL::FIRST_LEVEL*/)
{
	if (json_str.empty()) {
		MYLOG_ERROR << "Input string is empty.";
		return false;
	}

	jsoncons::json j_parse_res = jsoncons::json::parse(json_str);
	if (j_parse_res.is_object()) {
		MapStrStrT pTmpParseRes;
		if (!ParseJsonObjectToMap(json_str, pTmpParseRes, nExpLevel)) {
			return false;
		}
		retJsonMapVec.emplace_back(std::move(pTmpParseRes));
	}
	else if (j_parse_res.is_array()) {
		if (!ParseJsonArrayToMap(json_str, retJsonMapVec, nExpLevel)) {
			return false;
		}
	}
	else {
		MYLOG_ERROR << "Input json string format error.Cannot parse successfully.";
		return false;
	}

	return true;
}

std::string CommUtility::ToJsonArrayStr(const VecMapStrStrT &inJsonMapVec)
{
	std::string sRetString;
	if (inJsonMapVec.empty()) return sRetString;

	jsoncons::json_array<jsoncons::json> j_ret_array;
	for (auto tmpJsonMap : inJsonMapVec)
	{
		jsoncons::json jsonItem;
		std::for_each(tmpJsonMap.begin(), tmpJsonMap.end(), [&jsonItem](std::pair<std::string, std::string> const &vTmpPair) {
			jsonItem.insert_or_assign(vTmpPair.first, vTmpPair.second);
		});

		j_ret_array.emplace_back(std::move(jsonItem));
	}

	jsoncons::json jResult = std::move(j_ret_array);
	sRetString = jResult.as_string().c_str();

	return sRetString;
}

std::string CommUtility::ToJsonObjectStr(const MapStrStrT &inJsonMap)
{
	std::string sRetString;
	if (inJsonMap.empty()) return sRetString;

	jsoncons::json jsonItem;
	for (auto tmppair : inJsonMap)
	{
		jsonItem.insert_or_assign(tmppair.first, tmppair.second);
	}

	sRetString = jsonItem.as_string().c_str();
	return sRetString;
}

std::string CommUtility::ToJsonArrayStr(const VecStr &inJsonVec, std::string const &keyName /*= ""*/)
{
	std::string sRetString;
	if (inJsonVec.empty()) return sRetString;

	std::string sTmpKeyName = keyName;
	if (sTmpKeyName.empty()) sTmpKeyName = "Index";

	size_t nIndex = 0;
	jsoncons::json_array<jsoncons::json> j_array;
	for (auto tmpstr : inJsonVec)
	{
		jsoncons::json jsonItem;
		std::string s_tmp_key;
		if (keyName.empty()) s_tmp_key.append(sTmpKeyName).append(std::to_string(nIndex++));
		else s_tmp_key = sTmpKeyName;

		jsonItem.insert_or_assign(s_tmp_key, tmpstr);
		j_array.emplace_back(std::move(jsonItem));
	}

	jsoncons::json jResult = j_array;
	sRetString = jResult.as_string().c_str();
	return sRetString;
}
