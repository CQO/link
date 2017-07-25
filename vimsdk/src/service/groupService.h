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

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************注册推送********************/

	/**
	* \brief 监听群头像更新
	* @param[in] _1 传入目标ID，人或者群 number
	* @param[in] _2	  传入头像 string
	*/
	static void regGroupHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 监听群成员头像更新
	* @param[in] _1 传入目标ID，人或者群 number
	* @param[in] _2	  传入头像  string
	*/
	static void regMemberHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 监听群背景更新
	* @param[in] _1 传入目标ID，人或者群 number
	* @param[in] _2	  传入头像 string
	*/
	static void regGroupBackImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 设置监听群信息更新的回调
	* @param[in] _1 操作类型 1.添加 2.更新，31.解散 32.移除 33.退出 34.自动解散  number
	* @param[in] _2 群信息  object
	*/
	static void regGroupRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void groupRefreshHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置监听群主更新的回调
	* @param[in] _1 群主ID  number
	* @param[in] _2 群主名 string
	* @param[in] _3 群ID number
	* @param[in] _4 群名 string
	*/
	static void regTransferGroupCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void transferGroupHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置监听群成员信息更新的回调
	* @param[in] _1 群ID  number
	* @param[in] _2 群成员信息 string
	*/
	static void regGrpMemInfoRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void grpMemInfoRefreshHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  设置群成员列表更新的回调
	* @param[in] _1 操作类型  1 添加， 32 移除， 33 退出  number
	* @param[in] _2 群ID  number
	* @param[in] _3 变更的群成员集合 object
	*/
	static void regGrpMemRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void grpMemRefreshHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  设置获取群成员列表的回调
	* @param[in] _1 错误信息，不使用，只是为了和getMemberList回调函数一致  number
	* @param[in] _2 群成员列表 object
	*/
	static void regGetGrpMemListCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGrpMemListHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  监听群列表刷新
	* @param[in] _1 群列表 object
	*/
	static void regGroupListCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void groupListHandler(void *data, int len, PersistentPtr cb);

	/**********************请求**********************/
	
	/**
	* \brief 创建群
	* @param[in] level 传入群等级  number
	* @param[in] name 传入群名称   string
	* @param[in] members   传入群成员  array
	* @param[in] cb  传入接收结果回调  _1错误信息  _2群ID  object
	*/
	static void createGroup(const v8::FunctionCallbackInfo<v8::Value> &args);
	void createGroupHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 加群
	* @param[in] groupId 传入群id  number
	* @param[in] verify_info 传入验证信息 string
	* @param[in] cb  传入接收结果回调  _1错误信息 objcet
	*/
	static void addGroup(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 解散群
	* @param[in] type 传入操作类型  number
	* @param[in] groupid 传入群id  number
	* @param[in] cb  传入接收结果回调  _1错误信息 object
	*/
	static void removeGroup(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 转让群
	* @param[in] groupId 传入群id  number
	* @param[in] userId 传入新群主的id number
	* @param[in] cb  传入接收结果回调  _1错误信息 object
	*/
	static void transferGroup(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 获取群设置
	* @param[in] groupid 传入群id  number
	* @param[in] cb  传入接收结果回调  _1错误信息  
	*    _2  验证类型: 1.不允许任何人添加, 2.需要验证信息, 3.允许任何人添加.
	*	 _3	 是否允许群成员邀请好友加入群: 1.允许 2.不允许. 
	*/
	static void getGroupSet(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGroupSetHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置群设置
	* @param[in] groupId 传入群id  number
	* @param[in] verify_type 传入验证类型  number
	* @param[in] is_allow 传入是否允许成员邀请用户  boolean
	* @param[in] cb  传入接收结果回调  _1错误信息  object
	*/
	static void setGroupSet(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取群信息
	* @param[in] groupid 传入群id  number
	* @param[out] groupInfo 群信息 object
	*/
	static void getGroupInfo(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGroupInfoHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置群信息
	* @param[in] groupId 设置的群ID  number
	* @param[in] group  可设置的群信息  object
	* @param[in] cb  传入接收结果回调  _1错误信息 object
	*/
	static void setGroupInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取群列表 （同步接口）
	* @param[in] cb  传入接收结果回调  _1群信息集合;  object
	*/
	static void getGroupList(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief  获取群成员列表
	* @param[in] groupId 传入群id   number
	* @param[in] cb  传入接收结果回调   _1错误信息  _2群成员信息集合  object
	*/
	static void getMemberList(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMemberListHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 邀请群成员
	* @param[in] groupId 传入群id  number
	* @param[in] members 传入成员名单  array
	* @param[in] cb  传入接收结果回调   _1错误信息  object
	*/
	static void inviteMember(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 移除群成员
	* @param[in] groupId 传入群id  number
	* @param[in] userId 传入需要移除的成员id  number
	* @param[in] cb  传入接收结果回调   _1错误信息  object
	*/
	static void removeMember(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取群成员信息
	* @param[in] groupId 传入群id  number
	* @param[in] userId 传入成员id number
	* @param[out] member返回成员信息 object
	* @return false代表没有此信息
	*/
	static void getMemberInfo(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMemberInfoHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置群成员信息
	* @param[in] member 传入成员信息  object
	* @param[in] cb  传入接收结果回调 objcet
	*/
	static void setMemberInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 判断用户是否在群里 (同步接口)
	* @param[in] groupid 传入群id  number
	* @param[in] userid 传入成员id  number
	* @return false代表不是群成员
	*/
	static void isInGroup(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取群文件列表
	* @param[in] groupId 传入群id  number
	* @param[in] beginId 传入起始id number
	* @param[in] count 传入数量  number
	* @param[in] flag 传入偏移标志0为向上1为向下  number
	* @param[in] cb  传入接收结果回调   _1错误信息 _2文件信息集合  object
	*/
	static void getGroupFileList(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGroupFileListHandler(void *data, int len, PersistentPtr cb);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

