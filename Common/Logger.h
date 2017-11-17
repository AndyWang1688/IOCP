#pragma once
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

#ifdef _MSC_VER  
#include <Windows.h>
#include <direct.h> 
#include <tchar.h>
#elif __GNUC__
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

//日志输出等级，大于等于的级别的日志才输出
enum LOG_LEVEL
{
	//所以日志信息
	LL_ALL,
	//调试信息
	LL_DEBUG,
	//系统运行时一般记录信息
	LL_INFO,
	//表明会出现潜在错误的情形
	LL_WARN,
	//虽然发生错误事件，但仍然不影响系统的继续运行
	LL_ERROR,
	//指出每个严重的错误事件将会导致应用程序的退出
	LL_FATAL,
	//最高等级的，用于关闭所有日志记录
	LL_OFF
};

class Logger
{
public:
	Logger();
	~Logger();

	//初始化日志系统，指定日志存储路径, 输出日志级别和日志存储天数
	static bool InitLogger(string strLogDir = "..\\logs", LOG_LEVEL logLevel = LOG_LEVEL::LL_OFF);
	//删除存储满指定天数日志，默认日志存放30天,初始化日志系统后需调用该函数进行日志清理
	static void DeleteLog(int nStorageDays = 30);
	//设置存储路径
	static void SetLogDir(string strLogDir = "..\\logs");
	//设置日志等级,大于等于指定级别的日志才能输出
	static void SetLogLevel(LOG_LEVEL logLevel = LOG_LEVEL::LL_OFF);
	//输出日志
	static void log(LOG_LEVEL logLevel, string strLog);
	//输出DEBUG级别的日志
	static void debug(string strLog);
	//输出INFO级别的日志
	static void info(string strLog);
	//输出WARN级别的日志
	static void warn(string strLog);
	//输出ERROR级别的日志
	static void error(string strLog);
	//输出FATAL级别的日志
	static void fatal(string strLog);

private:
	//判断字符串是否是日期格式yyyy-MM-dd
	static bool IsDateFormat(const char *pszParam);
	//获取当日日期,日期格式yyyy-MM-dd
	static string GetDate();
	//获取当前时间，时间格式HH:mm:ss
	static string GetTime();
	//判断是否是闰年
	static int IsLeapYear(const int year);
	//获取日期strLaterDate到日期strPreDate相隔的天数，strLaterDate为较晚的日期，日期格式yyyy-MM-dd
	static int GetDays(const string& strPreDate, const string& strLaterDate);
	//调试信息日子的输出流
	static ofstream m_debug_log_file;
	//信息日子的输出流
	static ofstream m_info_log_file;
	//警告信息的输出流
	static ofstream m_warn_log_file;
	//错误信息的输出流
	static ofstream m_error_log_file;
	//严重错误信息的输出流
	static ofstream m_fatal_log_file;
	//初始化时输出日志信息的等级
	static LOG_LEVEL m_logLevel;
	//日志存储路径
	static string m_strLogDir;
};