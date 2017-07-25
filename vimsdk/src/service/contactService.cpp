#include "contactService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> ContactService::constructor;
ContactService::ContactService(SdkChannel *channel) :channel_(channel)
{
}

ContactService::~ContactService()
{
}

void ContactService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "ContactService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "regContactHeadImgUpdateCb", regContactHeadImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regContactBackImgUpdateCb", regContactBackImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regPresentCb", regPresentCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regOperateContactCb", regOperateContactCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regContactListCb", regContactListCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "addContact", addContact);
	NODE_SET_PROTOTYPE_METHOD(tp, "getVerifyType", getVerifyType);
	NODE_SET_PROTOTYPE_METHOD(tp, "removeContact", removeContact);
	NODE_SET_PROTOTYPE_METHOD(tp, "updateContactInfo", updateContactInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "getContactList", getContactList);
	NODE_SET_PROTOTYPE_METHOD(tp, "getContactOnline", getContactOnline);
	NODE_SET_PROTOTYPE_METHOD(tp, "getContactInfo", getContactInfo);

	constructor.Reset(isolate, tp->GetFunction());
}

void ContactService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		ContactService* client = new ContactService((SdkChannel*)channel->Value());
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

//注册联系人头像更新
void ContactService::regContactHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	REG_NOTIFY(contactHeadImgChange, args[0], &ContactService::imageHandler, contactService);
}

//注册联系人聊天背景更新
void ContactService::regContactBackImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	REG_NOTIFY(contactBackImgChange, args[0], &ContactService::imageHandler, contactService);
}

void ContactService::imageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_UpdateHeadImg, data, len);
}

//注册在线状态更新的回调
void ContactService::regPresentCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	REG_NOTIFY(logic_cmd_present, args[0], &ContactService::presentHandler, contactService);
}	

void ContactService::presentHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_present, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "flag", (double)rep->flag);
	Helper::toJsNumber(jsObject, "userId", (double)rep->userID);
	Helper::toJsNumber(jsObject, "deviceType", (double)rep->deviceType);
	Helper::toJsString(jsObject, "deviceInfo", rep->deviceInfo.c_str());
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册好友信息更新的回调
void ContactService::regOperateContactCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	REG_NOTIFY(logic_cmd_operateBuddy, args[0], &ContactService::operateContactHandler, contactService);
}

void ContactService::operateContactHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(_sdkrespcmd_base, data, 0);
	//数据转换
	Helper::toJsBool(jsObject, "isFinished", true);	//数据库升级是否完成
	Helper::callJsCallback(isolate, cb, jsObject);
}

//监听好友刷新
void ContactService::regContactListCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	REG_NOTIFY(logic_cmd_buddylist, args[0], &ContactService::contactListHandler, contactService);
}

void ContactService::contactListHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_buddyList, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->tub)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "avatar", avatar.c_str());
		}
		Helper::toJsBool(jsObj, "isHidden", 0x2000 == (i.flag & 0x2000));
		Helper::toJsBool(jsObj, "isStar", (i.flag & 1) == 1);
		Helper::toJsNumber(jsObj, "sex", (double)i.sex);
		Helper::toJsNumber(jsObj, "userId", (double)i.userid);
		Helper::toJsString(jsObj, "sign", i.sign.c_str());
		Helper::toJsString(jsObj, "server", i.srvName.c_str());
		if (!i.remark.empty())
			Helper::toJsString(jsObj, "remark", i.remark.c_str());

		if (!i.name.empty())
		{
			Helper::toJsString(jsObj, "name", i.name.c_str());
			Helper::toJsString(jsObj, "pinyin", i.pinyin.c_str());
		}
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "contacts"), jsArray);
	Helper::toJsNumber(jsObject, "flag", (double)rep->flag);
	Helper::callJsCallback(isolate, cb, jsObject);
}

/**********************请求**********************/


//添加联系人
void ContactService::addContact(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	req_addBuddyRequest *data = (req_addBuddyRequest*)contactService->channel_->getCmdData(logic_cmd_addBuddyRequest);
	if (data == nullptr) return;
	//数据转换
	data->requestType = 1;
	data->userID = (int64)Helper::toCppNumber(args[0]);
	data->remark = Helper::toCppString(args[1]);
	data->verifyInfo.push_back(Helper::toCppString(args[2]));

	POST_REQUEST(logic_cmd_addBuddyRequest, data, args[3], &ContactService::commonHandler, contactService);
}

//获取联系人验证方式
void ContactService::getVerifyType(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	req_getBuddyVerifyType *data = (req_getBuddyVerifyType*)contactService->channel_->getCmdData(logic_cmd_getBuddyVerifyType);
	if (data == nullptr) return;
	//数据转换
	data->userid = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getBuddyVerifyType, data, args[1], &ContactService::getVerifyTypeHandler, contactService);
}

void ContactService::getVerifyTypeHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getBuddyVerifyType, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "userId", (double)rep->userid);
	Helper::toJsNumber(jsObject, "type", (double)rep->type);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//删除联系人
void ContactService::removeContact(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	req_opUser *data = (req_opUser*)contactService->channel_->getCmdData(logic_cmd_opuser);
	if (data == nullptr) return;
	//数据转换
	int8 oper = 1;
	data->_set_isDeleteOtherSide(oper);
	data->operType = 3;
	data->userID = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_opuser, data, args[1], &ContactService::commonHandler, contactService);
}

//更新联系人信息（星标&V标&备注）
void ContactService::updateContactInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	req_opUser *data = (req_opUser*)contactService->channel_->getCmdData(logic_cmd_opuser);
	if (data == nullptr) return;
	//数据转换
	Local<Object> updateInfo = Local<Object>::Cast(args[0]);

	Local<Value> remark = updateInfo->Get(String::NewFromUtf8(isolate, "remark"));
	if (!remark->IsUndefined() || !remark->IsSet())
	{
		auto str = Helper::toCppString(remark);
		data->_set_remark(str);
	}
		

	Local<Value> isStar = updateInfo->Get(String::NewFromUtf8(isolate, "isStar"));
	if (!isStar->IsUndefined() || !isStar->IsSet())
	{
		int8 tmp = Helper::toCppBool(isStar) ? 1 : 2;
		data->_set_starBuddy(tmp);
	}
		
	Local<Value> chatImg = updateInfo->Get(String::NewFromUtf8(isolate, "chatImg"));
	if (!chatImg->IsUndefined() || !chatImg->IsSet())
	{
		auto str = Helper::toCppString(chatImg);
		data->_set_backgroundURL(str);
	}

	data->userID = (int64)Helper::toCppNumber(updateInfo->Get(String::NewFromUtf8(isolate, "userId")));
	data->operType = 2;

	POST_REQUEST(logic_cmd_opuser, data, args[1], &ContactService::commonHandler, contactService);
}

//获取联系人列表
void ContactService::getContactList(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	ICacheMgr *cache = contactService->channel_->getCacheMgr();
	std::vector<st_tinyBuddyBean> st_buddys;
	cache->getCacheData(ICacheMgr::data_buddys, &st_buddys);

	Isolate *isolate = args.GetIsolate();
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : st_buddys)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = contactService->channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "avatar", avatar.c_str());
		}
		Helper::toJsBool(jsObj, "isHidden", 0x2000 == (i.flag & 0x2000));
		Helper::toJsBool(jsObj, "isStar", (i.flag & 1) == 1);
		Helper::toJsNumber(jsObj, "sex", (double)i.sex);
		Helper::toJsNumber(jsObj, "userId", (double)i.userid);
		Helper::toJsString(jsObj, "sign", i.sign.c_str());
		Helper::toJsString(jsObj, "server", i.srvName.c_str());
		if (!i.remark.empty())
			Helper::toJsString(jsObj, "remark", i.remark.c_str());

		if (!i.name.empty())
		{
			Helper::toJsString(jsObj, "name", i.name.c_str());
			Helper::toJsString(jsObj, "pinyin", i.pinyin.c_str());
		}
		jsArray->Set(index++, jsObj);
	}
	Local<Object> jsObject = Object::New(isolate);
	jsObject->Set(String::NewFromUtf8(isolate, "contacts"), jsArray);
	PersistentPtr cb = Helper::persistentFunc(args[0]);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//获取联系人在线状态
void ContactService::getContactOnline(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	req_getBuddyOnline *data = (req_getBuddyOnline*)contactService->channel_->getCmdData(logic_cmd_getBuddyOnline);
	if (data == nullptr) return;
	//数据转换
	std::vector<int64> members;
	Helper::toCppNumVector(members, args[0]);

	POST_REQUEST(logic_cmd_getBuddyOnline, data, args[1], &ContactService::getContactOnlineHandler, contactService);
}

void ContactService::getContactOnlineHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getBuddyOnlineResult, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->obs)
	{
		Local<Object> jsObj = Object::New(isolate);
		Helper::toJsNumber(jsObj, "userId", (double)i.userID);
		Helper::toJsNumber(jsObj, "connectId", (double)i.connectID);
		Helper::toJsNumber(jsObj, "deviceType", (double)i.deviceType);
		Helper::toJsNumber(jsObj, "flag", (double)i.flag);
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "contacts"), jsArray);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//获取联系人信息 ( 同步接口 )
void ContactService::getContactInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ContactService* contactService = ObjectWrap::Unwrap<ContactService>(args.Holder());
	ICacheMgr *cache = contactService->channel_->getCacheMgr();
	st_buddyBean st_buddy;
	cache->getCacheData(ICacheMgr::data_buddy, &st_buddy);

	Local<Object> jsObject = Object::New(isolate);
	Helper::toJsNumber(jsObject, "userId", (double)st_buddy.info.userid);
	Helper::toJsString(jsObject, "name", st_buddy.info.name.c_str());
	Helper::toJsString(jsObject, "pinyin", st_buddy.info.pinyin.c_str());
	Helper::toJsString(jsObject, "remark", st_buddy.remark.c_str());
	Helper::toJsBool(jsObject, "isStar", (st_buddy.isstar & 1) == 1);
	Helper::toJsBool(jsObject, "isStar", (st_buddy.isstar & 0x4000) != 0);
	Helper::toJsBool(jsObject, "isApp", (st_buddy.isstar & 0x1000) != 0);
	Helper::toJsBool(jsObject, "isHidden", (st_buddy.isstar & 0x2000) == 0x2000);
	Helper::toJsNumber(jsObject, "birthday", (double)st_buddy.info.birthday);
	Helper::toJsString(jsObject, "accountName", st_buddy.info.nickID.c_str());
	Helper::toJsNumber(jsObject, "sex", (double)st_buddy.info.sex);
	Helper::toJsString(jsObject, "server", st_buddy.srvName.c_str());
	Helper::toJsString(jsObject, "privateMsgPwd", st_buddy.privateMsgPwd.c_str());
	Helper::toJsString(jsObject, "area", st_buddy.info.area.c_str());
	Helper::toJsString(jsObject, "sign", st_buddy.info.sign.c_str());
	if (!st_buddy.chatImage.empty())
	{
		std::string chatImage = contactService->channel_->headImgPath() + st_buddy.chatImage;
		Helper::toJsString(jsObject, "chatImage", chatImage.c_str());
	}
	if (!st_buddy.info.avatar.empty())
	{
		std::string avatar = contactService->channel_->headImgPath() + st_buddy.info.avatar;
		Helper::toJsString(jsObject, "avatar", avatar.c_str());
	}
	if (!st_buddy.info.srcAvatar.empty())
	{
		std::string srcAvatar = contactService->channel_->headImgPath() + st_buddy.info.srcAvatar;
		Helper::toJsString(jsObject, "srcAvatar", srcAvatar.c_str());
	}
	PersistentPtr cb = Helper::persistentFunc(args[0]);
	Helper::callJsCallback(isolate, cb, jsObject);
}

void ContactService::commonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(_sdkrespcmd_base, data, 0);

	Helper::callJsCallback(isolate, cb, jsObject);
}

} /*namespace service*/

