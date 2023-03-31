#include "MyLogFile.h"
#include <ctime>
#include <sys/timeb.h>

std::string LogFile::sLogFileName = "";
bool LogFile::bIsOutLogFile = false;

LogFile::LogFile(std::string const& fileInfo, int lineInfo, std::string const& funcInfo, MyLogLevel logLevel)
	: sFileInfo(fileInfo)
	, nLineInfo(lineInfo)
	, sFuncInfo(funcInfo)
	, ml_logLevel(logLevel)
{
	SetBaseLogMessage();
}

LogFile::~LogFile()
{
	if (IsOutLogFile()) {
		LogPrint::GetInstance().OutLogToFile(sBaseLogMsg, sUserLogMeg, sLogFileName);
	}

	LogPrint::GetInstance().OutLogToConsole(sBaseLogMsg, sUserLogMeg);
}

void LogFile::SetBaseLogMessage()
{
	// 2022-09-22 17:26:32 LOG_INFO FILE:LINE::FUNCTION log_message...
	sCurrNowTime = GetLocalCurrentTime();

	std::string strLogMsg;
	switch (ml_logLevel)
	{
	case MYLOG_INFO_LEVEL:
		strLogMsg = "LOG_INFO";
		break;
	case MYLOG_ERROR_LEVEL:
		strLogMsg = "LOG_ERROR";
		break;
	case MYLOG_WARN_LEVEL:
		strLogMsg = "LOG_WARN";
		break;
	default:
		break;
	}

	sBaseLogMsg = sCurrNowTime + " " + strLogMsg + " " + sFileInfo + ":" + std::to_string(nLineInfo) + "::" + sFuncInfo + " ";
	return;
}

std::string LogFile::GetLocalCurrentTime(std::string const& formats, std::string const& millformats)
{
	struct timeb tb_now;
	ftime(&tb_now);
	char cTimeStr[1024] = { 0 };
	struct tm tm_now;
#ifdef _MSC_VER
	localtime_s(&tm_now, &tb_now.time);
	strftime(cTimeStr, sizeof(cTimeStr), formats.c_str(), &tm_now);
	!millformats.empty() ? sprintf_s(cTimeStr + strlen(cTimeStr), sizeof(cTimeStr)-strlen(cTimeStr)-1, millformats.c_str(), tb_now.millitm) : NULL;
#else
	localtime_r(&tb_now.time, &tm_now);
	strftime(cTimeStr, sizeof(cTimeStr), formats.c_str(), &tm_now);
	!millformats.empty() ? sprintf(cTimeStr + strlen(cTimeStr), millformats.c_str(), tb_now.millitm) : NULL;
#endif
	std::string sretTime = cTimeStr;
	return sretTime;
}

void LogPrint::OutLogToConsole(const std::string &sBaseMessage, const std::string &sUserMessage)
{
	std::stringstream sOutLog;
	sOutLog << sBaseMessage << " " << sUserMessage;
	std::cout << sOutLog.str() << std::endl;
	return;
}

void LogPrint::OutLogToFile(const std::string &sBaseMessage, const std::string &sUserMessage, const std::string &sLogFile)
{
	std::ofstream outfile(sLogFile, std::ios::app | std::ios::out);		//ios::app ���ļ���������ĩβ׷�ӣ�ios::ate ���ļ�����������ļ���д��
	if (!outfile) {
		std::cerr << "open log file error." << std::endl;
		return;
	}
	outfile << sBaseMessage << " " << sUserMessage << std::endl;
	outfile.close();
	return;
}
