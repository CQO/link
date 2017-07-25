/*
 * config.h
 *
 *  Created on: 2015年7月22日
 *      Author: sharp
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef  _WIN32
typedef   char   int8;
typedef   unsigned char  uint8;
typedef   short  int16 ;
typedef   unsigned short uint16 ;
typedef   int    int32 ;
typedef   unsigned int   uint32 ;
typedef   __int64   int64 ;
typedef   unsigned  __int64  uint64 ;
#elif  defined(__linux__)
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <stdarg.h>

typedef   int8_t     int8   ;
typedef   uint8_t    uint8  ;
typedef   int16_t    int16  ;
typedef   uint16_t   uint16 ;
typedef   int32_t    int32  ;
typedef   uint32_t   uint32  ;
typedef   int64_t    int64  ;
typedef   uint64_t   uint64 ;
#elif  defined(__APPLE__)
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <stdarg.h>

typedef   int8_t     int8   ;
typedef   uint8_t    uint8  ;
typedef   int16_t    int16  ;
typedef   uint16_t   uint16 ;
typedef   int32_t    int32  ;
typedef   uint32_t   uint32  ;
typedef   int64_t    int64  ;
typedef   uint64_t   uint64 ;
#endif

#ifdef  _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>

	#include <process.h>
	#define  THRED_RETURN  uint32
#else
	#define __stdcall
	#define  THRED_RETURN  void *
#endif

struct thread_info {
	///线程ID
#ifndef _WIN32
	pthread_t tid ;
#else
	uint32     tid ;
	HANDLE    handle ;
#endif
#ifndef   __APPLE__
	thread_info() {
		tid = 0 ;
	}
#endif
};

/**
 * 最大终端数量
 */
#define  MAX_CLI_CNT      5

//#define  ENCRYPT_DB

///日志级别
enum  enlogLvl {
	enlog_debug,  ///< 调试日志 详细情况
	enlog_trace,  ///< 输出 运行状态信息
	enlog_notice,  ///< 注意 有可能出错 ,比如内存占用高等
	enlog_warn,  ///< 除错,但可以自行修复
	enlog_err,  ///< 出错,要挂掉.
	enlog_count  ///< 无意义,标识级别数量
};

///日志模块
enum enLogModel
{
	enlogm_comm,  ///< 通用模块
	enlogm_ldd,  ///< 豆豆插件
	enlogm_ga,  ///< 公安插件
	enlogm_av,  ///< 音视频插件
    enlogm_rtc,  ///<多点音视频
	enlogm_sopstore,  ///<sopstore插件
    enlogm_external,  ///外部模块, 安卓 ios ,pc等平台输出的日志
};

static const char *LogTagAuth = "auth";				///< 认证
static const char *LogTagUser = "user";				///< 用户
static const char *LogTagBuddy = "buddy";			///< 好友
static const char *LogTagGroup = "group";			///< 群组
static const char *LogTagRecontact = "recontact";	///< 最近联系人
static const char *LogTagEnt = "ent";				///< 企业相关
static const char *LogTagChat = "chat";				///< 聊天
static const char *LogTagFileInfo = "fileInfo";		///< 文件
static const char *LogTagSysConfig = "sysConfig";	///< 系统配置
static const char *LogTagSysMsg = "sysMsg";			///< 系统信息
static const char *LogTagTimestamp = "timestamp";	///< 时间戳
static const char *LogTagApp = "app";				///< app
static const char *LogTagOther = "other";			///< 其它功能(内部功能，初始化功能等)


#ifdef _WIN32
	#ifdef IMSDK_EXPORT
        #define  IMSDK_EXPORT_API __declspec(dllexport)
	#else
        #define  IMSDK_EXPORT_API __declspec(dllimport)
	#endif
#else
	#define  IMSDK_EXPORT_API
#endif

#ifdef _WIN32
    #ifdef  PLUGIN_EXPORT
        #define EXPORT_PLUGIN extern "C" _declspec(dllexport)
    #else
        #define EXPORT_PLUGIN extern "C" _declspec(dllimport)
    #endif
#else
	#define EXPORT_PLUGIN extern "C"
#endif

/**
 *    终端类型宏定义
 */
//#define  SDK_FOR_IOS
//#define  SDK_FOR_ANDROID
//#define  SDK_FOR_PC
//#define  WITHOUT_SSL  ///打开该宏，就是不使用SSL
//#define OEM_OS_YUANXIN //元心系统专用

#endif /* CONFIG_H_ */
