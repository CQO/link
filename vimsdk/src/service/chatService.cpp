#include "chatService.h"
#include "../utils/utils.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

void toJsMessage(Local<Object> &jsObject, const st_msgBean& msg);
void toCppMessage(req_sendMessage *msg, const Local<Value> &jsObject);

Persistent<Function> ChatService::constructor;
ChatService::ChatService(SdkChannel *channel) :channel_(channel)
{
}

ChatService::~ChatService()
{
}

void ChatService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "ChatService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "regChatHeadImgUpdateCb", regChatHeadImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regMsgNoticeCb", regMsgNoticeCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regOfflineMsgCb", regOfflineMsgCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regRecontactCb", regRecontactCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "getChatList", getChatList);
	NODE_SET_PROTOTYPE_METHOD(tp, "removeChat", removeChat);
	NODE_SET_PROTOTYPE_METHOD(tp, "sendMessage", sendMessage);
	NODE_SET_PROTOTYPE_METHOD(tp, "deleteAllMessage", deleteAllMessage);
	NODE_SET_PROTOTYPE_METHOD(tp, "deleteMessageById", deleteMessageById);
	NODE_SET_PROTOTYPE_METHOD(tp, "deleteMessageByTime", deleteMessageByTime);
	NODE_SET_PROTOTYPE_METHOD(tp, "setMessageRead", setMessageRead);
	NODE_SET_PROTOTYPE_METHOD(tp, "getMessages", getMessages);
	//NODE_SET_PROTOTYPE_METHOD(tp, "chatTop", chatTop);

	constructor.Reset(isolate, tp->GetFunction());
}

void ChatService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		ChatService* client = new ChatService((SdkChannel*)channel->Value());
		client->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	else 
	{
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}



/********************注册推送********************/

//注册最近回话头像更新
void ChatService::regChatHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	REG_NOTIFY(chatHeadImgChange, args[0], &ChatService::imageHandler, chatService);
}

void ChatService::imageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_UpdateHeadImg, data, len);
}

//注册新聊天消息的回调
void ChatService::regMsgNoticeCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	REG_NOTIFY(logic_cmd_msgStore, args[0], &ChatService::msgNoticeHandler, chatService);
}

void ChatService::msgNoticeHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_msgBean, data, len);
	printf("msgNoticeHandler\n");
	//数据转换
	toJsMessage(jsObject, rep->msg);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册群主更新的回调
void ChatService::regOfflineMsgCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	REG_NOTIFY(logic_cmd_unReadmsgList, args[0], &ChatService::offlineMsgHandler, chatService);
}

void ChatService::offlineMsgHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_msgList, data, len);
	//数据转换
	if (!rep->msglist.empty())
	{
		Local<Object> jsObj = Object::New(isolate);
		toJsMessage(jsObj, rep->msglist.at(0).msg);
		jsObject->Set(String::NewFromUtf8(isolate, "lastMessage"), jsObj);
		Helper::toJsNumber(jsObject, "unReadCount", rep->msglist.at(0).unReadCnt);
		Helper::toJsNumber(jsObject, "weakTipCount", rep->msglist.at(0).weakTipCnt);
	}

	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册会话列表更新通知
void ChatService::regRecontactCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	REG_NOTIFY(logic_cmd_reContactList, args[0], &ChatService::recontactHandler, chatService);
}

void ChatService::recontactHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_reContactList, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "flag", (double)rep->flag);
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->_vt)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "avatar", avatar.c_str());
		}
		if (utils::isGroup(i.targetID))
			Helper::toJsNumber(jsObj, "chatType", 2);
		else if (utils::isApp(i.targetID))
			Helper::toJsNumber(jsObj, "chatType", 3);
		else
			Helper::toJsNumber(jsObj, "chatType", 1);

		Helper::toJsNumber(jsObj, "id", (double)i.targetID);
		Helper::toJsString(jsObj, "name", i.nickName.c_str());
		Helper::toJsNumber(jsObj, "lastMsgId", (double)i.lastMsgid);
		Helper::toJsString(jsObj, "lastMsg", i.lastMsg.c_str());
		Helper::toJsString(jsObj, "msgProp", i.msgProp.c_str());
		Helper::toJsString(jsObj, "whereFrom", i.whereFrom.c_str());
		Helper::toJsNumber(jsObj, "lastAtMsgID", (double)i.lastAtMsgID);
		Helper::toJsNumber(jsObj, "time", (double)i.time);
		Helper::toJsNumber(jsObj, "unreadCount", (double)i.unreadCnt);
		Helper::toJsNumber(jsObj, "msgType", (double)i.msgType);
		Helper::toJsBool(jsObj, "isTop", (i.status & 0x04) == 0x04);
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "chats"), jsArray);
	Helper::callJsCallback(isolate, cb, jsObject);
}

/**********************请求**********************/


//获取会话列表
void ChatService::getChatList(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	ICacheMgr *cache = chatService->channel_->getCacheMgr();
	std::vector<st_reContact> st_reContacts;
	cache->getCacheData(ICacheMgr::data_reContact, &st_reContacts);

	Isolate *isolate = args.GetIsolate();
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : st_reContacts)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = chatService->channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "avatar", avatar.c_str());
		}

		//获取最近联系人类型 1.好友  2.群组  3.机器人
		if (utils::isGroup(i.targetID))
			Helper::toJsNumber(jsObj, "chatType", 2);
		else if (utils::isApp(i.targetID))
			Helper::toJsNumber(jsObj, "chatType", 3);
		else
			Helper::toJsNumber(jsObj, "chatType", 1);

		Helper::toJsNumber(jsObj, "id", (double)i.targetID);
		Helper::toJsString(jsObj, "name", i.nickName.c_str());
		Helper::toJsNumber(jsObj, "lastMsgId", (double)i.lastMsgid);
		Helper::toJsString(jsObj, "lastMsg", i.lastMsg.c_str());
		Helper::toJsString(jsObj, "msgProp", i.msgProp.c_str());
		Helper::toJsString(jsObj, "whereFrom", i.whereFrom.c_str());
		Helper::toJsNumber(jsObj, "lastAtMsgID", (double)i.lastAtMsgID);
		Helper::toJsNumber(jsObj, "time", (double)i.time);
		Helper::toJsNumber(jsObj, "unreadCount", (double)i.unreadCnt);
		Helper::toJsNumber(jsObj, "msgType", (double)i.msgType);
		Helper::toJsBool(jsObj, "isTop", (i.status & 0x04) == 0x04);
		jsArray->Set(index++, jsObj);
	}
	Local<Object> jsObject = Object::New(isolate);
	jsObject->Set(String::NewFromUtf8(isolate, "chats"), jsArray);
	PersistentPtr cb = Helper::persistentFunc(args[0]);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//移除会话
void ChatService::removeChat(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_delReContact *data = (req_delReContact*)chatService->channel_->getCmdData(logic_cmd_delReContact);
	if (data == nullptr) return;

	data->targetID = (int64)Helper::toCppNumber(args[0]);
	POST_REQUEST(logic_cmd_delReContact, data, args[1], &ChatService::commonHandler, chatService);
}

//发送消息
void ChatService::sendMessage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_sendMessage *data = (req_sendMessage*)chatService->channel_->getCmdData(logic_cmd_sendMessage);
	if (data == nullptr) return;

	toCppMessage(data, args[0]);
	POST_REQUEST(logic_cmd_sendMessage, data, args[1], &ChatService::sendMessageHandler, chatService);
}

void ChatService::sendMessageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_sendMessage, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "sendTime", (double)rep->sendTime);
	Helper::toJsNumber(jsObject, "messageId", (double)rep->messageID);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//删除所有消息
void ChatService::deleteAllMessage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_delMsg *data = (req_delMsg*)chatService->channel_->getCmdData(logic_cmd_delMessage);
	if (data == nullptr) return;

	data->targetID = 0;
	data->bDelContact = Helper::toCppBool(args[0]);
	data->bSyncSrv = true;
	
	POST_REQUEST(logic_cmd_delMessage, data, args[1], &ChatService::commonHandler, chatService);
}

//通过msgId删除消息
void ChatService::deleteMessageById(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_delMsg *data = (req_delMsg*)chatService->channel_->getCmdData(logic_cmd_delMessage);
	if (data == nullptr) return;

	data->targetID = (int64)Helper::toCppNumber(args[0]);
	Helper::toCppNumVector(data->idVt, args[1]);
	data->bSyncSrv = true;

	POST_REQUEST(logic_cmd_delMessage, data, args[2], &ChatService::commonHandler, chatService);
}

// 删除时间段消息
void ChatService::deleteMessageByTime(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_delMsg *data = (req_delMsg*)chatService->channel_->getCmdData(logic_cmd_delMessage);
	if (data == nullptr) return;

	data->targetID = (int64)Helper::toCppNumber(args[0]);
	data->startTime = (int64)Helper::toCppNumber(args[1]);
	data->endTime = (int64)Helper::toCppNumber(args[2]);
	data->bSyncSrv = true;

	POST_REQUEST(logic_cmd_delMessage, data, args[3], &ChatService::commonHandler, chatService);
}

// 设置消息已读
void ChatService::setMessageRead(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_setMsgReaded *data = (req_setMsgReaded*)chatService->channel_->getCmdData(logic_cmd_setMsgReaded);
	if (data == nullptr) return;

	data->targetID = (int64)Helper::toCppNumber(args[0]);
	data->msgID = (int64)Helper::toCppNumber(args[1]);

	chatService->channel_->postRequest(logic_cmd_setMsgReaded, data, sizeof(*data));
}

// 获取消息
void ChatService::getMessages(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ChatService* chatService = ObjectWrap::Unwrap<ChatService>(args.Holder());
	req_getHistoryMsg *data = (req_getHistoryMsg*)chatService->channel_->getCmdData(logic_cmd_getHistoryMsg);
	if (data == nullptr) return;

	data->targetID = (int64)Helper::toCppNumber(args[0]);
	data->msgBeginID = (int64)Helper::toCppNumber(args[1]);
	data->msgOffset = (int32)Helper::toCppNumber(args[2]);
	data->offsetFlag = (int8)Helper::toCppNumber(args[3]);

	POST_REQUEST(logic_cmd_getHistoryMsg, data, args[4], &ChatService::getMessagesHandler, chatService);
}

void ChatService::getMessagesHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getHistoryMsg, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->_vt)
	{
		Local<Object> jsObj = Object::New(isolate);
		toJsMessage(jsObj, i);
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "messages"), jsArray);
	Helper::toJsNumber(jsObject, "targetId", (double)rep->targetID);
	Helper::callJsCallback(isolate, cb, jsObject);
}

void ChatService::commonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(_sdkrespcmd_base, data, 0);

	Helper::callJsCallback(isolate, cb, jsObject);
}

void toJsMessage(Local<Object> &jsObject, const st_msgBean& msg)
{
	Helper::toJsNumber(jsObject, "messageId", (double)msg.messageID);
	Helper::toJsNumber(jsObject, "localId", (double)msg.localID);
	Helper::toJsNumber(jsObject, "messageType", (double)msg.messageType);
	Helper::toJsNumber(jsObject, "senderId", (double)msg.sendUserID);
	Helper::toJsString(jsObject, "format", msg.format.c_str());
	Helper::toJsNumber(jsObject, "lastMessageId", (double)msg.lastMessageID);
	Helper::toJsNumber(jsObject, "targetId", (double)msg.targetID);
	Helper::toJsNumber(jsObject, "sendTime", (double)msg.sendTime);
	Helper::toJsNumVector(jsObject, "limitRange", msg.limitRange);
	Helper::toJsNumber(jsObject, "relatedMsgId", (double)msg.relatedMsgID);
	Helper::toJsNumVector(jsObject, "relatedUsers", msg.relatedUsers);
	Helper::toJsString(jsObject, "message", msg.message.c_str());
	Helper::toJsString(jsObject, "msgProperties", msg.msgProperties.c_str());
	Helper::toJsBool(jsObject, "isBurn", msg.isBurn == 1);
}

void toCppMessage(req_sendMessage *msg, const Local<Value> &jsObject)
{
	if (jsObject->IsUndefined() || !jsObject->IsObject())
		return;

	Local<Object> jsObj = Local<Object>::Cast(jsObject);

	Isolate *is = jsObj->GetIsolate();
	msg->targetID = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "targetId")));
	msg->userID = 0;
	msg->message = Helper::toCppString(jsObj->Get(String::NewFromUtf8(is, "message")));
	msg->msgProperties = Helper::toCppString(jsObj->Get(String::NewFromUtf8(is, "msgProperty")));
	msg->isBurn = Helper::toCppBool(jsObj->Get(String::NewFromUtf8(is, "isBurn"))) ? 1 : 0;
	msg->messageType = (int16)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "messageType")));
	Helper::toCppNumVector(msg->limitRange, jsObj->Get(String::NewFromUtf8(is, "limitRange")));
	Helper::toCppNumVector(msg->relatedUsers, jsObj->Get(String::NewFromUtf8(is, "relatedUsers")));
}

} /*namespace service*/

