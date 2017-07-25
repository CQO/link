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

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************注册推送********************/

	/**
	* \brief 注册最近回话头像更新
	* @param[in] cb _1 传入目标ID，人或者群  _2 传入头像
	*/
	static void regChatHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 注册新聊天消息的回调
	* @param[in] cb _1 消息结构的智能指针
	*/
	static void regMsgNoticeCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void msgNoticeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 注册离线消息的回调
	* @param[in] cb _1 离线消息集合
	*/
	static void regOfflineMsgCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void offlineMsgHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief  注册会话列表更新通知
	* @param[in] cb _1 传入列表标志
	* 全部更新:
	*	0x01　第一个包,　　 需要清理原来的数据
	*	0x02  中间的包，　　在原来的数据后面追加
	*	0x04  最后的包，　　最近会话发送完毕
	* 部分更新:
	*   0x08  更新
	*	0x10  新增
	*	0x20  删除
	* 全部删除
	*   0x40 删除所有最近联系人
	*  _2 传入会话集合
	*/
	static void regRecontactCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void recontactHandler(void *data, int len, PersistentPtr cb);

	/**********************请求**********************/
	
	/**
	* \brief 获取会话列表，此接口为同步接口，不需要回调
	* @param[in] cb  传入接收结果回调 _1错误信息
	*/
	static void getChatList(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 移除会话
	* @param[in] targetid 传入会话对应的ID，群或者人 0为删除全部联系人
	* @param[in] cb  传入接收结果回调
	*/
	static void removeChat(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 发送消息，返回localid
	* @param[in] msg 传入消息
	* @param[in] cb  传入接收结果回调 _1 错误信息 _2 发送时间 _3消息ID
	*/
	static void sendMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void sendMessageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 删除所有消息
	* @param[in] delChatList 是否清空联系人列表
	* @param[in] cb  结果回调 _1错误信息
	*/
	static void deleteAllMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 通过msgId删除消息
	* @param[in] targetid 传入会话对应的ID，群或者人
	* @param[in] msgs 传入要删除的消息ID集合 msgs为空，清空对应targetId的所有消息
	* @param[in] cb  结果回调 _1错误信息
	*/
	static void deleteMessageById(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 删除时间段消息
	* @param[in] targetid 传入会话对应的ID，群或者人 targetId为0，则针对所有用户
	* @param[in] beginTime 起始时间
	* @param[in] endTime 结束时间
	* @param[in] cb  结果回调 _1错误信息
	*/
	static void deleteMessageByTime(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 设置消息已读
	* @param[in] targetid 传入会话对应的ID，群或者人
	* @param[in] msgs 传入要删除的消息ID集合
	*/
	static void setMessageRead(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取消息
	* @param[in] targetid 传入会话对应的ID，群或者人
	* @param[in] msgid 传入查询消息的起始ID，将从该消息的下一条消息开始查询
	* @param[in] count 传入查询消息总数
	* @param[in] flag  传入上一页还是下一页 向上偏移 0；向下偏移 1
	* @param[in] cb  传入接收结果回调 _1 错误信息 _2会话放ID， _3消息集合
	*/
	static void getMessages(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMessagesHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 会话置顶
	* @param[in] targetid 置顶的目标ID
	* @param[in] isTop  是否置顶
	* @param[in] cb  传入接收结果回调 _1 错误信息
	*/
	//static void chatTop(const v8::FunctionCallbackInfo<v8::Value> &args);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

