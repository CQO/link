#ifndef NODE_GROUP_SERVICE_
#define NODE_GROUP_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class GroupService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit GroupService(SdkChannel *channel);
	~GroupService();

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************ע������********************/

	/**
	* \brief ����Ⱥͷ�����
	* @param[in] _1 ����Ŀ��ID���˻���Ⱥ number
	* @param[in] _2	  ����ͷ�� string
	*/
	static void regGroupHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����Ⱥ��Աͷ�����
	* @param[in] _1 ����Ŀ��ID���˻���Ⱥ number
	* @param[in] _2	  ����ͷ��  string
	*/
	static void regMemberHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ����Ⱥ��������
	* @param[in] _1 ����Ŀ��ID���˻���Ⱥ number
	* @param[in] _2	  ����ͷ�� string
	*/
	static void regGroupBackImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ���ü���Ⱥ��Ϣ���µĻص�
	* @param[in] _1 �������� 1.��� 2.���£�31.��ɢ 32.�Ƴ� 33.�˳� 34.�Զ���ɢ  number
	* @param[in] _2 Ⱥ��Ϣ  object
	*/
	static void regGroupRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void groupRefreshHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ���ü���Ⱥ�����µĻص�
	* @param[in] _1 Ⱥ��ID  number
	* @param[in] _2 Ⱥ���� string
	* @param[in] _3 ȺID number
	* @param[in] _4 Ⱥ�� string
	*/
	static void regTransferGroupCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void transferGroupHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ���ü���Ⱥ��Ա��Ϣ���µĻص�
	* @param[in] _1 ȺID  number
	* @param[in] _2 Ⱥ��Ա��Ϣ string
	*/
	static void regGrpMemInfoRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void grpMemInfoRefreshHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  ����Ⱥ��Ա�б���µĻص�
	* @param[in] _1 ��������  1 ��ӣ� 32 �Ƴ��� 33 �˳�  number
	* @param[in] _2 ȺID  number
	* @param[in] _3 �����Ⱥ��Ա���� object
	*/
	static void regGrpMemRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void grpMemRefreshHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  ���û�ȡȺ��Ա�б�Ļص�
	* @param[in] _1 ������Ϣ����ʹ�ã�ֻ��Ϊ�˺�getMemberList�ص�����һ��  number
	* @param[in] _2 Ⱥ��Ա�б� object
	*/
	static void regGetGrpMemListCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGrpMemListHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  ����Ⱥ�б�ˢ��
	* @param[in] _1 Ⱥ�б� object
	*/
	static void regGroupListCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void groupListHandler(void *data, int len, PersistentPtr cb);

	/**********************����**********************/
	
	/**
	* \brief ����Ⱥ
	* @param[in] level ����Ⱥ�ȼ�  number
	* @param[in] name ����Ⱥ����   string
	* @param[in] members   ����Ⱥ��Ա  array
	* @param[in] cb  ������ս���ص�  _1������Ϣ  _2ȺID  object
	*/
	static void createGroup(const v8::FunctionCallbackInfo<v8::Value> &args);
	void createGroupHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ��Ⱥ
	* @param[in] groupId ����Ⱥid  number
	* @param[in] verify_info ������֤��Ϣ string
	* @param[in] cb  ������ս���ص�  _1������Ϣ objcet
	*/
	static void addGroup(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ��ɢȺ
	* @param[in] type �����������  number
	* @param[in] groupid ����Ⱥid  number
	* @param[in] cb  ������ս���ص�  _1������Ϣ object
	*/
	static void removeGroup(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ת��Ⱥ
	* @param[in] groupId ����Ⱥid  number
	* @param[in] userId ������Ⱥ����id number
	* @param[in] cb  ������ս���ص�  _1������Ϣ object
	*/
	static void transferGroup(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ��ȡȺ����
	* @param[in] groupid ����Ⱥid  number
	* @param[in] cb  ������ս���ص�  _1������Ϣ  
	*    _2  ��֤����: 1.�������κ������, 2.��Ҫ��֤��Ϣ, 3.�����κ������.
	*	 _3	 �Ƿ�����Ⱥ��Ա������Ѽ���Ⱥ: 1.���� 2.������. 
	*/
	static void getGroupSet(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGroupSetHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����Ⱥ����
	* @param[in] groupId ����Ⱥid  number
	* @param[in] verify_type ������֤����  number
	* @param[in] is_allow �����Ƿ������Ա�����û�  boolean
	* @param[in] cb  ������ս���ص�  _1������Ϣ  object
	*/
	static void setGroupSet(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡȺ��Ϣ
	* @param[in] groupid ����Ⱥid  number
	* @param[out] groupInfo Ⱥ��Ϣ object
	*/
	static void getGroupInfo(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGroupInfoHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����Ⱥ��Ϣ
	* @param[in] groupId ���õ�ȺID  number
	* @param[in] group  �����õ�Ⱥ��Ϣ  object
	* @param[in] cb  ������ս���ص�  _1������Ϣ object
	*/
	static void setGroupInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡȺ�б� ��ͬ���ӿڣ�
	* @param[in] cb  ������ս���ص�  _1Ⱥ��Ϣ����;  object
	*/
	static void getGroupList(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief  ��ȡȺ��Ա�б�
	* @param[in] groupId ����Ⱥid   number
	* @param[in] cb  ������ս���ص�   _1������Ϣ  _2Ⱥ��Ա��Ϣ����  object
	*/
	static void getMemberList(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMemberListHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����Ⱥ��Ա
	* @param[in] groupId ����Ⱥid  number
	* @param[in] members �����Ա����  array
	* @param[in] cb  ������ս���ص�   _1������Ϣ  object
	*/
	static void inviteMember(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief �Ƴ�Ⱥ��Ա
	* @param[in] groupId ����Ⱥid  number
	* @param[in] userId ������Ҫ�Ƴ��ĳ�Աid  number
	* @param[in] cb  ������ս���ص�   _1������Ϣ  object
	*/
	static void removeMember(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡȺ��Ա��Ϣ
	* @param[in] groupId ����Ⱥid  number
	* @param[in] userId �����Աid number
	* @param[out] member���س�Ա��Ϣ object
	* @return false����û�д���Ϣ
	*/
	static void getMemberInfo(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMemberInfoHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ����Ⱥ��Ա��Ϣ
	* @param[in] member �����Ա��Ϣ  object
	* @param[in] cb  ������ս���ص� objcet
	*/
	static void setMemberInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief �ж��û��Ƿ���Ⱥ�� (ͬ���ӿ�)
	* @param[in] groupid ����Ⱥid  number
	* @param[in] userid �����Աid  number
	* @return false������Ⱥ��Ա
	*/
	static void isInGroup(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡȺ�ļ��б�
	* @param[in] groupId ����Ⱥid  number
	* @param[in] beginId ������ʼid number
	* @param[in] count ��������  number
	* @param[in] flag ����ƫ�Ʊ�־0Ϊ����1Ϊ����  number
	* @param[in] cb  ������ս���ص�   _1������Ϣ _2�ļ���Ϣ����  object
	*/
	static void getGroupFileList(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGroupFileListHandler(void *data, int len, PersistentPtr cb);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

