#ifndef NODE_FILE_SERVICE_
#define NODE_FILE_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class FileService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit FileService(SdkChannel *channel);
	~FileService();

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************注册推送********************/

	/**********************请求**********************/
	
	/**
	* \brief 上传头像
	* @param[in] path 传入头像本地路径
	* @param[in] cb  传入接收结果回调 _1错误信息  _2大图地址json _3缩略图地址json
	*/
	static void uploadAvatar(const v8::FunctionCallbackInfo<v8::Value> &args);
	void uploadAvatarHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 上传文件 
	* @param[in] fileProperty 上传文件的属性
	* @param[in] cb  传入接收结果回调 _1错误信息 _2目标Id _3服务器返回的json
	* @param[in] pro  进度回调 _1 extra_req  _2process
	* @return 每个文件对应的唯一localID
	*/
	static void uploadFile(const v8::FunctionCallbackInfo<v8::Value> &args);
	void uploadFileHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 下载文件
	* @param[in] targetId 发送者的targetId
	* @param[in] fileMsgId 文件消息的msgID
	* @param[in] path 传入下载路径
	* @param[in] url 传入url
	* @param[in] cb  传入接收结果回调 _1错误信息  _2本地路径 _3发送者ID
	* @param[in] pro  进度回调
	* @return 每个文件对应的唯一localID
	*/
	static void downloadFile(const v8::FunctionCallbackInfo<v8::Value> &args);
	void downloadFileHandler(void *data, int len, PersistentPtr cb);
	
	/**
	* \brief 上传图片
	* @param[in] targetId 人或群的id
	* @param[in] thumbimg 传入缩略图
	* @param[in] srcimg 传入原图
	* @param[in] encryptkey 传入解密密码
	* @param[in] cb  传入接收结果回调 _1错误信息 _2目标ID， _3原图JSON， _4缩略图JSON
	*/
	static void uploadImage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void uploadImageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 下载图片
	* @param[in] targetId 人或群的id
	* @param[in] url 传入图片url
	* @param[in] cb  传入接收结果回调 _1错误信息  _2图片名  _3对方ID
	*/
	static void downloadImage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void downloadImageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 解密文件
	* @param[in] encryptkey 传入解密密码
	* @param[in] srcpath 传入原图路径
	* @param[in] destpath 传入解密后图片路径
	*/
	static void decryptFile(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 进度回调
	* @param[in] extra 额外参数
	* @param[in] progess 进度百分比
	* @param[in] info 描述信息
	*/
	void progressValueHandler(void *data, int len, PersistentPtr cb);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

