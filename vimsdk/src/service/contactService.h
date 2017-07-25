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

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************注册推送********************/

	/**
	* \brief 监听联系人头像更新
	* @param[in]  cb _1 传入目标ID，人或者群;  _2	  传入头像;
	*/
	static void regContactHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 监听联系人聊天背景更新
	* @param[in]  cb _1 传入目标ID，人或者群;  _2	  传入头像;
	*/
	static void regContactBackImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 设置监听在线状态更新的回调
	* @param[in] cb _1 在线状态
	*/
	static void regPresentCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void presentHandler(void *data, int len, PersistentPtr cb);

	/**
	*  \brief 设置好友信息更新的回调
	* @param[in] cb _1 传入操作类型  1: 增加 2: 更新，3: 删除; _2  传入变更的联系人;
	*/
	static void regOperateContactCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void operateContactHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 监听好友刷新
	* @param[in] cb _1 传入操作类型 \n
	* 0x01 包含此标识，清空ＵＩ列表插入下面列表项 \n
	* 0x02　包含此标识，ＵＩ列表尾部添加下面列表项 \n
	* 0x04 包含此标识，所有好友添加完毕 \n
	* 0x08 包含此标识，从当前ＵＩ列表中更新下面列表项　不和其他标识组合 \n
	* 0x10 包含此标识，从当前ＵＩ列表中删除下面列表项　不和其他标识组合 \n
	* [in] _2	  传入变更的联系人
	*/
	static void regContactListCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void contactListHandler(void *data, int len, PersistentPtr cb);

	/**********************请求**********************/
	
	/**
	* \brief 添加联系人
	* @param[in] userid 传入联系人ID
	* @param[in] remark 传入联系人备注，可以为空
	* @param[in] info   传入验证信息
	* @param[in] cb  传入接收结果回调 _1错误信息
	*/
	static void addContact(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 获取联系人验证方式
	* @param[in] userid 传入联系人ID
	* @param[in] cb  传入接收结果回调 _1 错误信息  _2验证方式 1需要验证信息:2:不允许任何人添加,3:允许任何人添加
	*/
	static void getVerifyType(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getVerifyTypeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 删除联系人
	* @param[in] userid 传入联系人ID
	* @param[in] cb  传入接收结果回调 _1 错误信息
	*/
	static void removeContact(const v8::FunctionCallbackInfo<v8::Value> &args);
	void signupHandler(void *data, int len, PersistentPtr cb);
	
	/**
	* \brief 更新联系人信息（星标&V标&备注）
	* @param[in] info 传入联系人信息
	* @param[in] cb  传入接收结果回调 _1错误信息
	*/
	static void updateContactInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取联系人列表 （同步接口）
	* @param[in] cb 传入接收结果回调 _1 contacts 所有的联系人信息;
	*/
	static void getContactList(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getContactListHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 获取联系人在线状态
	* @param[in] users 传入联系人集合 如果为空则查所有联系人状态
	* @param[in] cb  传入接收结果回调 _1 错误信息; _2 在线状态
	*/
	static void getContactOnline(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getContactOnlineHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 获取联系人信息 ( 同步接口 )
	* @param[in] userid 用户ID
	* @param[out] contact  联系人信息
	* @return 操作结果
	*/
	static void getContactInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

