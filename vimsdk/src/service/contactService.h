#ifndef NODE_CONTACT_SERVICE_
#define NODE_CONTACT_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class ContactService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit ContactService(SdkChannel *channel);
	~ContactService();

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************ע������********************/

	/**
	* \brief ������ϵ��ͷ�����
	* @param[in]  cb _1 ����Ŀ��ID���˻���Ⱥ;  _2	  ����ͷ��;
	*/
	static void regContactHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ������ϵ�����챳������
	* @param[in]  cb _1 ����Ŀ��ID���˻���Ⱥ;  _2	  ����ͷ��;
	*/
	static void regContactBackImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ���ü�������״̬���µĻص�
	* @param[in] cb _1 ����״̬
	*/
	static void regPresentCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void presentHandler(void *data, int len, PersistentPtr cb);

	/**
	*  \brief ���ú�����Ϣ���µĻص�
	* @param[in] cb _1 �����������  1: ���� 2: ���£�3: ɾ��; _2  ����������ϵ��;
	*/
	static void regOperateContactCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void operateContactHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��������ˢ��
	* @param[in] cb _1 ����������� \n
	* 0x01 �����˱�ʶ����գգ��б���������б��� \n
	* 0x02�������˱�ʶ���գ��б�β����������б��� \n
	* 0x04 �����˱�ʶ�����к��������� \n
	* 0x08 �����˱�ʶ���ӵ�ǰ�գ��б��и��������б������������ʶ��� \n
	* 0x10 �����˱�ʶ���ӵ�ǰ�գ��б���ɾ�������б������������ʶ��� \n
	* [in] _2	  ����������ϵ��
	*/
	static void regContactListCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void contactListHandler(void *data, int len, PersistentPtr cb);

	/**********************����**********************/
	
	/**
	* \brief �����ϵ��
	* @param[in] userid ������ϵ��ID
	* @param[in] remark ������ϵ�˱�ע������Ϊ��
	* @param[in] info   ������֤��Ϣ
	* @param[in] cb  ������ս���ص� _1������Ϣ
	*/
	static void addContact(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ��ȡ��ϵ����֤��ʽ
	* @param[in] userid ������ϵ��ID
	* @param[in] cb  ������ս���ص� _1 ������Ϣ  _2��֤��ʽ 1��Ҫ��֤��Ϣ:2:�������κ������,3:�����κ������
	*/
	static void getVerifyType(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getVerifyTypeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ɾ����ϵ��
	* @param[in] userid ������ϵ��ID
	* @param[in] cb  ������ս���ص� _1 ������Ϣ
	*/
	static void removeContact(const v8::FunctionCallbackInfo<v8::Value> &args);
	void signupHandler(void *data, int len, PersistentPtr cb);
	
	/**
	* \brief ������ϵ����Ϣ���Ǳ�&V��&��ע��
	* @param[in] info ������ϵ����Ϣ
	* @param[in] cb  ������ս���ص� _1������Ϣ
	*/
	static void updateContactInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡ��ϵ���б� ��ͬ���ӿڣ�
	* @param[in] cb ������ս���ص� _1 contacts ���е���ϵ����Ϣ;
	*/
	static void getContactList(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getContactListHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��ȡ��ϵ������״̬
	* @param[in] users ������ϵ�˼��� ���Ϊ�����������ϵ��״̬
	* @param[in] cb  ������ս���ص� _1 ������Ϣ; _2 ����״̬
	*/
	static void getContactOnline(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getContactOnlineHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��ȡ��ϵ����Ϣ ( ͬ���ӿ� )
	* @param[in] userid �û�ID
	* @param[out] contact  ��ϵ����Ϣ
	* @return �������
	*/
	static void getContactInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

