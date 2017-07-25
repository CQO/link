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

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);


	/**********************����**********************/
	
	/**
	* \brief <U>��ȡ��½��Ϣ
	* @return ��¼��Ϣ��������
	*/
	static void getLoginInfos(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>���õ�½��Ϣ
	* @param[in] ���õĵ�¼��Ϣ
	* @return   ���ý��
	*/
	static void setLoginInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>��ȡ���û�������ͼƬ��Ϣ�ı���·��
	* @return   ͼƬ��Ϣ�ı���·��
	*/
	static void userImgPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>��ȡ���û���������Ƶ��Ϣ�ı���·��
	* @return   ��Ƶ��Ϣ�ı���·��
	*/
	static void userAudioPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>��ȡ���û���������Ƶ��Ϣ�ı���·��
	* @return   ��Ƶ��Ϣ�ı���·��
	*/
	static void userVideoPath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <U>��ȡ���û��������ļ���Ϣ�ı���·��
	* @return   �ļ���Ϣ�ı���·��
	*/
	static void userFilePath(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief <G>��ȡͷ��·��
	* @return   ͷ��·��
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

