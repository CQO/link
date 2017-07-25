#ifndef NODE_USER_SERVICE_
#define NODE_USER_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class UserService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit UserService(SdkChannel *channel);
	~UserService();

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************ע������********************/
	/**
	* \brief ��������ͷ�����
	* @param[in] _1 ����Ŀ��ID���˻���Ⱥ number
	* @param[in] _2	  ����ͷ�� string
	*/
	static void regUserHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ���ü����û���Ϣ���µĻص�������������
	* @param[in] _1 �û���Ϣ
	*/
	static void regOperateUserCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void operateUserHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �����û���Ϣ�޸ģ���¼���º����������
	* @param[in] _1 �û���Ϣ
	*/
	static void regGetMyselfCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMyselfHandler(void *data, int len, PersistentPtr cb);

	/**********************����**********************/
	
	/**
	* \brief ��ȡ�˻���Ϣ��ͬ���ӿ�
	* @param[in] info �û���Ϣ
	*/
	static void getAccountInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief �����û���Ϣ
	* @param[in] user �����û���Ϣ
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void updateAccountInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ���ӱ������ã��Լ�ֵ�Է�ʽ����
	* @param[in] items  ������Ϣ����
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void addLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ��ȡ��������
	* @param[in] keys ��ȡ��������Ϣ�ļ�����
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2��ȡ��������Ϣ����
	*/
	static void getLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getLocalSettingHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ���±�������
	* @param[in] newItems ������µ�������Ϣ����
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void updateLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ɾ����������
	* @param[in] keys ɾ����������Ϣ����
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void deleteLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ͨ��label��ѯ�����
	* @param[in] label�������ǩ
	* @param[in] cb ������ս���ص�  _1������Ϣ _2���صĲ�ѯ��Ϣ
	*/
	static void queryEmoticonPackageByLabel(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryEmoticonPackageByLabelHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ���������ѯ�����ݱ������ʶ��ѯ���飬��ѯ�����û��Զ������
	* @param[in] type  1, ������ѯ���� md5Code��ʾ���������md5, 6,���ݱ������ʶ��ѯ���б��� md5Code��ʾ�������md5, 7,��ѯ�û������Զ������ ����md5Code����ֵ
	* @param[in] md5   md5ֵ
	* @param[in] cb ������ս���ص�  _1������Ϣ _2���صĲ�ѯ��Ϣ
	*/
	static void queryEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryEmoticonHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ������ѯ�����
	* @param[in] md5   md5ֵ
	* @param[in] cb ������ս���ص�  _1������Ϣ _2���صĲ�ѯ��Ϣ
	*/
	static void singleQueryEmoticonPackage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void singleQueryEmoticonPackageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��ҳ��ѯ�����
	* @param[in] pageNum   ҳ��
	* @param[in] pageSize   ҳ��
	* @param[in] cb ������ս���ص�  _1������Ϣ _2���صĲ�ѯ��Ϣ
	*/
	static void pageQueryEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args);
	void pageQueryEmoticonHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��ɾ�Զ������
	* @param[in] type   ����  4 ���ӣ� 5ɾ��.
	* @param[in] emot   ����
	* @param[in] cb ������ս���ص�  _1������Ϣ _2resultCode _3resultMsg _4successList _5failedList
	*/
	static void addOrDeleteCustomEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args);
	void addOrDeleteCustomEmoticonHandler(void *data, int len, PersistentPtr cb);

	/**
	*\brief ��ѡ����������ڵĵ�������
	*@param[in] packageCode �����md5
	*@param[in] emotCode  ����md5
	*/
	static void queryEmotionInPackage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryEmotionInPackageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ͨ��Ӧ��ID��ȡӦ����Ϣ
	* @param[in] appId Ӧ�ó���ID
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2���ص�ֵ
	*/
	static void getAppInfo(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getAppInfoHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��ѯӦ���г�Ӧ����Ϣ
	* @param[in] qData ����Ĳ�ѯ��Ϣ
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2���ص�Ӧ����Ϣ
	*/
	static void queryMarketApplication(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryMarketApplicationHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ɾ�����Ӧ��
	* @param[in] type ����Ĳ�ѯ����
	* @param[in] appID ������appID
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void addOrDeleteApplication(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡ�Ѱ�װ��Ӧ��
	* @param[in] deviceType ��׿orIOS
	* @param[in] cb ������ս���ص�  _1������Ϣ _2���صĲ�ѯ��Ϣ
	*/
	static void getInstalledApplication(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getInstalledApplicationHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ���ø���������\n
	*		type: 1 (����)�������绰���������ʼ���\n
	* ������������value:  1�������˿ɼ� 2�������ѿɼ� 3�����Լ��ɼ���Ĭ��1\n
	*		type: 4 (������֤��ʽ)\n
	* ������������value:   1����Ҫ��֤��Ϣ,2:�������κ������,3:�����κ�����ӣ�Ĭ��1\n
	*		type: 5 V��\n
	*			value: 1:��ʾʼ�����������ѣ�2����ʾʼ������������ 3:��ʼ�����ѣ�Ĭ��1\n
	*		type: 6 @���������ģʽ\n
	*			value: 1:��ʾʼ�����������ѣ�2����ʾʼ������������ 3:��ʼ�����ѣ�Ĭ��1\n
	*		type: 7 ȫ��֪ͨ��Ϣ����չ��ģʽ\n
	*         value: 1:֪ͨ���飬2��֪ͨԴ���������� 3:��ȫ���أ�Ĭ��2\n
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void setPersonalData(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡ����������
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2���ص�ֵ
	*/
	static void getPersonalData(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getPersonalDataHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��ȡȫ������ģʽ
	* @param[in] cb ������ս���ص�  _1������Ϣ _2��ʼʱ��  _3����ʱ��  _4�Ƿ��
	*/
	static void getGlobalNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGlobalNoDisturbModeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����ȫ������ģʽ
	* @param[in] startTime  ��ʼʱ��
	* @param[in] endTime ����ʱ��
	* @param[in] isOpen �Ƿ��
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void setGolbalNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡ���Ŀ���û�������ģʽ
	* @param[in] targetId  �û�ID
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2�û�ID  _3���õ�ֵ:  1Ϊ�������� 2��ʾ�����ѽ���ʾ���� 3:Ϊ�����
	*/
	static void getUserNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getUserNoDisturbModeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �������Ŀ���û�������ģʽ
	* @param[in] targetId  �û�ID
	* @param[in] value ���õ�ֵ:  1Ϊ�������� 2��ʾ�����ѽ���ʾ���� 3:Ϊ�����, Ĭ��1
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void setUserNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

