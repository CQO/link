#ifndef NODE_CHAT_SERVICE_
#define NODE_CHAT_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class ChatService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit ChatService(SdkChannel *channel);
	~ChatService();

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************ע������********************/

	/**
	* \brief ע������ػ�ͷ�����
	* @param[in] cb _1 ����Ŀ��ID���˻���Ⱥ  _2 ����ͷ��
	*/
	static void regChatHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ע����������Ϣ�Ļص�
	* @param[in] cb _1 ��Ϣ�ṹ������ָ��
	*/
	static void regMsgNoticeCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void msgNoticeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ע��������Ϣ�Ļص�
	* @param[in] cb _1 ������Ϣ����
	*/
	static void regOfflineMsgCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void offlineMsgHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  ע��Ự�б����֪ͨ
	* @param[in] cb _1 �����б��־
	* ȫ������:
	*	0x01����һ����,���� ��Ҫ����ԭ��������
	*	0x02  �м�İ���������ԭ�������ݺ���׷��
	*	0x04  ���İ�����������Ự�������
	* ���ָ���:
	*   0x08  ����
	*	0x10  ����
	*	0x20  ɾ��
	* ȫ��ɾ��
	*   0x40 ɾ�����������ϵ��
	*  _2 ����Ự����
	*/
	static void regRecontactCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void recontactHandler(void *data, int len, PersistentPtr cb);

	/**********************����**********************/
	
	/**
	* \brief ��ȡ�Ự�б��˽ӿ�Ϊͬ���ӿڣ�����Ҫ�ص�
	* @param[in] cb  ������ս���ص� _1������Ϣ
	*/
	static void getChatList(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief �Ƴ��Ự
	* @param[in] targetid ����Ự��Ӧ��ID��Ⱥ������ 0Ϊɾ��ȫ����ϵ��
	* @param[in] cb  ������ս���ص�
	*/
	static void removeChat(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ������Ϣ������localid
	* @param[in] msg ������Ϣ
	* @param[in] cb  ������ս���ص� _1 ������Ϣ _2 ����ʱ�� _3��ϢID
	*/
	static void sendMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void sendMessageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ɾ��������Ϣ
	* @param[in] delChatList �Ƿ������ϵ���б�
	* @param[in] cb  ����ص� _1������Ϣ
	*/
	static void deleteAllMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ͨ��msgIdɾ����Ϣ
	* @param[in] targetid ����Ự��Ӧ��ID��Ⱥ������
	* @param[in] msgs ����Ҫɾ������ϢID���� msgsΪ�գ���ն�ӦtargetId��������Ϣ
	* @param[in] cb  ����ص� _1������Ϣ
	*/
	static void deleteMessageById(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ɾ��ʱ�����Ϣ
	* @param[in] targetid ����Ự��Ӧ��ID��Ⱥ������ targetIdΪ0������������û�
	* @param[in] beginTime ��ʼʱ��
	* @param[in] endTime ����ʱ��
	* @param[in] cb  ����ص� _1������Ϣ
	*/
	static void deleteMessageByTime(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ������Ϣ�Ѷ�
	* @param[in] targetid ����Ự��Ӧ��ID��Ⱥ������
	* @param[in] msgs ����Ҫɾ������ϢID����
	*/
	static void setMessageRead(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡ��Ϣ
	* @param[in] targetid ����Ự��Ӧ��ID��Ⱥ������
	* @param[in] msgid �����ѯ��Ϣ����ʼID�����Ӹ���Ϣ����һ����Ϣ��ʼ��ѯ
	* @param[in] count �����ѯ��Ϣ����
	* @param[in] flag  ������һҳ������һҳ ����ƫ�� 0������ƫ�� 1
	* @param[in] cb  ������ս���ص� _1 ������Ϣ _2�Ự��ID�� _3��Ϣ����
	*/
	static void getMessages(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMessagesHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �Ự�ö�
	* @param[in] targetid �ö���Ŀ��ID
	* @param[in] isTop  �Ƿ��ö�
	* @param[in] cb  ������ս���ص� _1 ������Ϣ
	*/
	//static void chatTop(const v8::FunctionCallbackInfo<v8::Value> &args);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

