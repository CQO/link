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

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************ע������********************/

	/**********************����**********************/
	
	/**
	* \brief �ϴ�ͷ��
	* @param[in] path ����ͷ�񱾵�·��
	* @param[in] cb  ������ս���ص� _1������Ϣ  _2��ͼ��ַjson _3����ͼ��ַjson
	*/
	static void uploadAvatar(const v8::FunctionCallbackInfo<v8::Value> &args);
	void uploadAvatarHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �ϴ��ļ� 
	* @param[in] fileProperty �ϴ��ļ�������
	* @param[in] cb  ������ս���ص� _1������Ϣ _2Ŀ��Id _3���������ص�json
	* @param[in] pro  ���Ȼص� _1 extra_req  _2process
	* @return ÿ���ļ���Ӧ��ΨһlocalID
	*/
	static void uploadFile(const v8::FunctionCallbackInfo<v8::Value> &args);
	void uploadFileHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �����ļ�
	* @param[in] targetId �����ߵ�targetId
	* @param[in] fileMsgId �ļ���Ϣ��msgID
	* @param[in] path ��������·��
	* @param[in] url ����url
	* @param[in] cb  ������ս���ص� _1������Ϣ  _2����·�� _3������ID
	* @param[in] pro  ���Ȼص�
	* @return ÿ���ļ���Ӧ��ΨһlocalID
	*/
	static void downloadFile(const v8::FunctionCallbackInfo<v8::Value> &args);
	void downloadFileHandler(void *data, int len, PersistentPtr cb);
	
	/**
	* \brief �ϴ�ͼƬ
	* @param[in] targetId �˻�Ⱥ��id
	* @param[in] thumbimg ��������ͼ
	* @param[in] srcimg ����ԭͼ
	* @param[in] encryptkey �����������
	* @param[in] cb  ������ս���ص� _1������Ϣ _2Ŀ��ID�� _3ԭͼJSON�� _4����ͼJSON
	*/
	static void uploadImage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void uploadImageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����ͼƬ
	* @param[in] targetId �˻�Ⱥ��id
	* @param[in] url ����ͼƬurl
	* @param[in] cb  ������ս���ص� _1������Ϣ  _2ͼƬ��  _3�Է�ID
	*/
	static void downloadImage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void downloadImageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �����ļ�
	* @param[in] encryptkey �����������
	* @param[in] srcpath ����ԭͼ·��
	* @param[in] destpath ������ܺ�ͼƬ·��
	*/
	static void decryptFile(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ���Ȼص�
	* @param[in] extra �������
	* @param[in] progess ���Ȱٷֱ�
	* @param[in] info ������Ϣ
	*/
	void progressValueHandler(void *data, int len, PersistentPtr cb);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

