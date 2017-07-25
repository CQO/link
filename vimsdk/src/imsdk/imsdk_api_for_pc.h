/*
 * imsdk_api_for_pc.h
 * imsdk 在pc上的导出文件
 *  Created on: 2015年7月28日
 *      Author: sharp
 */

#ifndef SRC_SDKAPI_PC_IMSDK_API_FOR_PC_H_
#define SRC_SDKAPI_PC_IMSDK_API_FOR_PC_H_

#if defined(SDK_FOR_ANDROID)
#include <jni.h>
#endif

#include <string>
#include "config.h"

/**
 * @nCliid 客户端id
 * @msg 消息编号
 * @buffer 消息结构体 在cmdExport里面有定义
 * @len BUFFER buffer 长度
 */
class  imMsgCallBack {

public:
	virtual bool    		  onMsgcallBack(unsigned short msg , void * pBuffer,int len) = 0;
};

#ifndef _WIN32
#ifdef __cplusplus
extern "C" {
#endif
#else
/*#ifdef __cplusplus
extern "C" {
#endif*/
#endif

/**
 * 初始化
 * @pRootPath 为可读写的路径
 * @return    返回绑定的客户端ID,返回零为失败.
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT int          initClient(const char * pRootPath,imMsgCallBack * pCallBack);
#else
IMSDK_EXPORT_API int          initClient(const char * pRootPath,imMsgCallBack * pCallBack);
#endif

/**
 * 反初始化
 * @nCliid 客户端ID
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT void         uninitClient(int nCliid);
#else
IMSDK_EXPORT_API void         uninitClient(int nCliid);
#endif

/**
 * 发送请求
 * @nCliid 客户端ID
 * @msg 发送消息号 cmdExport.h 有枚举定义
 * @pData 消息携带数据 cmdExport.h 有结构定义
 * @len 发送消息长度
 * ＠pOut 当前消息返回的数据，cmdExport.h　有定义以 resp_ 开头的数据结构。
 *       如果设置了，该条消息为同步消息，接口执行完毕，请求也处理完毕。如果不设置，则为异步消息。
 * ******同步消息使用注意，可能会造成系统整体性能的降低******
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT int           sendRequest(int nCliid,
	unsigned short msg,
	void * pData,
	int len, void * pOut = NULL);

JNIEXPORT int           sendRequestEx(int nCliid,
	unsigned short msg,
	void * pData,
	int len, void * pOut = NULL);
#else
IMSDK_EXPORT_API int           sendRequest(int nCliid,
		unsigned short msg,
		void * pData,
		int len,void * pOut = NULL);

IMSDK_EXPORT_API int           sendRequestEx(int nCliid,
	unsigned short msg,
	void * pData,
	int len, void * pOut = NULL);
#endif

/**
 * 获取配置
 * 阻塞调用
 * @nCliid   为零的时候获取的是全局的一些配置信息，随时可以获取；如果不为零的话，使用时必须登录以后
 * @type     配置类型
 * @pOut     输出数据，数据结构在cfgExport.h里面定义
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT bool     	   getConfig(int nCliid,int type, void * pOut) ;
#else
IMSDK_EXPORT_API bool     	   getConfig(int nCliid,int type, void * pOut) ;
#endif

/**
 * 设置配置
 * 阻塞调用
 * @nCliid   为零的时候获取的是全局的一些配置信息，随时可以设置；如果不为零的话，使用时必须登录以后
 * @type     配置类型
 * @pIn      输出数据，数据结构在cfgExport.h里面定义
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT bool     	   setConfig(int nCliid, int type, void * pIn);
#else
IMSDK_EXPORT_API bool     	   setConfig(int nCliid , int type, void * pIn) ;
#endif

#if defined(SDK_FOR_ANDROID)
JNIEXPORT bool     	   getReponse(int nCliid);
#else
IMSDK_EXPORT_API bool getReponse(int nCliid);
#endif

/**
 * 加密文件
 * @pKey 密钥
 * @pSrcPath 源文件路径
 * @pDestPath 目的文件路径
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT bool encryptFile(const char * pKey, const char  * pSrcPath, const char * pDestPath);
#else
IMSDK_EXPORT_API bool encryptFile(const char * pKey, const char  * pSrcPath, const char * pDestPath);
#endif

/**
 * 解密文件
 * @pKey 密钥
 * @pSrcPath 源文件路径
 * @pDestPath 目的文件路径
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT bool decryptFile(const char * pKey, const char  * pSrcPath, const char * pDestPath);
#else
IMSDK_EXPORT_API bool decryptFile(const char * pKey ,const char  * pSrcPath, const char * pDestPath);
#endif

/**
* 打印日志
* @nCliid 客户端ID
* @lvl 日志级别
* @model 所属模块
* @tag 日志所属功能
* @fmt 打印内容
*/
#if defined(SDK_FOR_ANDROID)
JNIEXPORT void loglog(int nCliid, enlogLvl lvl, enLogModel model, const char * tag, const char * fmt, ...);
#else
IMSDK_EXPORT_API void loglog(int nCliid, enlogLvl lvl, enLogModel model, const char * tag, const char * fmt, ...);
#endif
/**
* 获取一个请求或响应结构内存
* @nCliid 客户端ID
* @msg 消息号
* @bisReq true为请求结构，false为响应结构
*/
#if defined(SDK_FOR_ANDROID)
JNIEXPORT void *getCmdData(int nCliid, uint16 msg, bool bisReq = true);
#else
IMSDK_EXPORT_API  void *getCmdData(int nCliid, uint16 msg, bool bisReq = true);
#endif

/**
 * 解密文件到内存
 * @pKey 密钥
 * @pSrcPath 加密文件路径
 * @pOut 传出内存地址
 * @out_size 传出内存大小,如果大小不够,则赋值需要的大小返回
 */
#if defined(SDK_FOR_ANDROID)
JNIEXPORT bool decryptFileToMemory(const char * pKey, const char * pSrcPath, char * pOut, int64 &out_size);
#else
IMSDK_EXPORT_API bool decryptFileToMemory(const char * pKey, const char * pSrcPath, char * pOut, int64 &out_size);
#endif

/**
* 聊天中的图片，音频，视频（不包括头像）
  根据服务器名称，获取本地相对目录
 */

#if defined(SDK_FOR_ANDROID)
JNIEXPORT std::string getLPathByRUrl(std::string & rPath);
#else
#ifdef SDK_FOR_GA
IMSDK_EXPORT_API std::string getLPathByRUrl(std::string & rPath);
#endif
#endif

#ifndef _WIN32
#ifdef __cplusplus
} 
#endif
#else
/*#ifdef __cplusplus
}
#endif*/
#endif

#endif /* SRC_SDKAPI_PC_IMSDK_API_FOR_PC_H_ */
