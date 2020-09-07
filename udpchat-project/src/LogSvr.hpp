#pragma once 
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<sys/time.h>
#include<iostream>

//日志格式 
// [时间   日志等级：info(普通输出信息)/warning(警告信息)/error(错误信息)/fatal(致命错误信息)/debug(调试信息)  文件  错误所在行号] 具体错误信息

//字符指针数组： 存放日志等级划分
const char* Level[] = 
{
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
    "DEBUG"
};

enum LOGLevel
{
    INFO = 0,
    WARNING,
    ERROR,
    FATAL,
    DEBUG
};

//获取时间
class LogTime
{
    public:
        
        static int64_t GetTimeStamp()
        {
            //第一个参数是一个结构体  struct timeval(秒，毫秒) 出参。使用后可返回
            //第二个参数是时区，一般传NULL,采用系统时区  如CST:中国标准时间 东八区
            struct  timeval tv;
            gettimeofday(&tv,NULL);
            //返回一个大的整数 无法代表年月日  不太方便用户使用读懂
            return tv.tv_sec;
        }
        
        static void GetTimeStamp(std::string& timestamp)
        {
            //返回 年 月 日 时 分 秒
            time_t SysTime;
            time(&SysTime);
            struct tm* ST =  localtime(&SysTime);
            
            //格式化字符串
            //格式化字符串函数: snprintf
            //[YYYY-MM-DD-HH-MM-SS]大概23个字节
            char TimeNow[23] = {"\0"};
            snprintf(TimeNow, sizeof(TimeNow) - 1,"%04d-%02d-%02d %02d:%02d:%02d",ST->tm_year + 1900, ST->tm_mon + 1,ST->tm_mday,ST->tm_hour,ST->tm_min,ST->tm_sec);
            timestamp.assign(TimeNow,strlen(TimeNow));
        }
};

//声明成内联函数
inline std::ostream& Log(LOGLevel lev, const char* file, int line,const std::string& logmsg)
{
    std::string level_info = Level[lev];
    std::string timer_stamp;

    LogTime::GetTimeStamp(timer_stamp);
    std::cout<<"[" <<timer_stamp << " " <<level_info << " " <<file<<":"<<line <<"]" <<logmsg ;    
    return std::cout;
}

#define LOG(lev,msg) Log(lev,__FILE__,__LINE__,msg)






