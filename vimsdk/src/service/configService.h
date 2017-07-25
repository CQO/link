#ifndef NODE_CONFIG_SERVICE_
#define NODE_CONFIG_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class ConfigService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit ConfigService(SdkChannel *channel);
	~ConfigService();

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);


	/**********************请求**********************/
	
	/**
	* \brief <U>获取登陆信息
	* @return 登录信息对象数组
	*/
	static void getLoginInfos(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>设置登陆信息
	* @param[in] 设置的登录信息
	* @return   设置结果
	*/
	static void setLoginInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>获取该用户聊天中图片信息的保存路径
	* @return   图片信息的保存路径
	*/
	static void userImgPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>获取该用户聊天中音频信息的保存路径
	* @return   音频信息的保存路径
	*/
	static void userAudioPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>获取该用户聊天中视频信息的保存路径
	* @return   视频信息的保存路径
	*/
	static void userVideoPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>获取该用户聊天中文件信息的保存路径
	* @return   文件信息的保存路径
	*/
	static void userFilePath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <G>获取头像路径
	* @return   头像路径
	*/
	static void headImgPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	SdkChannel *channel_;
	int64_t registryId_;
	std::string rootPath_;
	std::string headImgPath_;
	std::string userImgPath_;
	std::string userAudioPath_;
	std::string userVideoPath_;
	std::string userCachePath_;
	std::string userFilePath_;
	std::string userEmotionPath_;
};

} /*namespace service*/

#endif

