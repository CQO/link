#include "userService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> UserService::constructor;
UserService::UserService(SdkChannel *channel) :channel_(channel)
{
}

UserService::~UserService()
{
}

void UserService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "UserService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "regUserHeadImgUpdateCb", regUserHeadImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regOperateUserCb", regOperateUserCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGetMyselfCb", regGetMyselfCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "getAccountInfo", getAccountInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "updateAccountInfo", updateAccountInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "addLocalSetting", addLocalSetting);
	NODE_SET_PROTOTYPE_METHOD(tp, "getLocalSetting", getLocalSetting);
	NODE_SET_PROTOTYPE_METHOD(tp, "updateLocalSetting", updateLocalSetting);
	NODE_SET_PROTOTYPE_METHOD(tp, "deleteLocalSetting", deleteLocalSetting);
	NODE_SET_PROTOTYPE_METHOD(tp, "queryEmoticonPackageByLabel", queryEmoticonPackageByLabel);
	NODE_SET_PROTOTYPE_METHOD(tp, "queryEmoticon", queryEmoticon);
	NODE_SET_PROTOTYPE_METHOD(tp, "singleQueryEmoticonPackage", singleQueryEmoticonPackage);
	NODE_SET_PROTOTYPE_METHOD(tp, "pageQueryEmoticon", pageQueryEmoticon);
	NODE_SET_PROTOTYPE_METHOD(tp, "addOrDeleteCustomEmoticon", addOrDeleteCustomEmoticon);
	NODE_SET_PROTOTYPE_METHOD(tp, "queryEmotionInPackage", queryEmotionInPackage);
	NODE_SET_PROTOTYPE_METHOD(tp, "getAppInfo", getAppInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "queryMarketApplication", queryMarketApplication);
	NODE_SET_PROTOTYPE_METHOD(tp, "addOrDeleteApplication", addOrDeleteApplication);
	NODE_SET_PROTOTYPE_METHOD(tp, "getInstalledApplication", getInstalledApplication);
	NODE_SET_PROTOTYPE_METHOD(tp, "setPersonalData", setPersonalData);
	NODE_SET_PROTOTYPE_METHOD(tp, "getPersonalData", getPersonalData);
	NODE_SET_PROTOTYPE_METHOD(tp, "getGlobalNoDisturbMode", getGlobalNoDisturbMode);
	NODE_SET_PROTOTYPE_METHOD(tp, "setGolbalNoDisturbMode", setGolbalNoDisturbMode);
	NODE_SET_PROTOTYPE_METHOD(tp, "getUserNoDisturbMode", getUserNoDisturbMode);
	NODE_SET_PROTOTYPE_METHOD(tp, "setUserNoDisturbMode", setUserNoDisturbMode);

	constructor.Reset(isolate, tp->GetFunction());
}

void UserService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		UserService* client = new UserService((SdkChannel*)channel->Value());
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

void UserService::regUserHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	REG_NOTIFY(userHeadImgChange, args[0], &UserService::imageHandler, userService);
}

void UserService::imageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_UpdateHeadImg, data, len);
}

void UserService::regOperateUserCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	REG_NOTIFY(logic_cmd_operateUser, args[0], &UserService::operateUserHandler, userService);
}

void UserService::operateUserHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_operUser, data, len);
	//数据转换
	Local<Object> account = Object::New(isolate);
	Helper::toJsNumber(jsObject, "id", (double)rep->user.info.userid);
	Helper::toJsNumber(jsObject, "timeZone", (double)rep->user.info.timezone);
	Helper::toJsNumber(jsObject, "birthday", (double)rep->user.info.birthday);
	Helper::toJsNumber(jsObject, "sex", (double)rep->user.info.sex);
	Helper::toJsString(jsObject, "area", rep->user.info.area.c_str());
	Helper::toJsString(jsObject, "sign", rep->user.info.sign.c_str());
	Helper::toJsString(jsObject, "name", rep->user.info.name.c_str());
	if (!rep->user.info.emails.empty())
		Helper::toJsString(jsObject, "email", rep->user.info.emails[0].c_str());
	if (!rep->user.info.phones.empty())
		Helper::toJsString(jsObject, "phone", rep->user.info.phones[0].c_str());

	if (!rep->user.info.avatar.empty())
	{
		std::string avatar = channel_->headImgPath() + rep->user.info.avatar;
		Helper::toJsString(jsObject, "avatar", avatar.c_str());
	}
	if (!rep->user.info.srcAvatar.empty())
	{
		std::string srcAvatar = channel_->headImgPath() + rep->user.info.srcAvatar;
		Helper::toJsString(jsObject, "srcAvatar", srcAvatar.c_str());
	}
	Helper::callJsCallback(isolate, cb, jsObject);
}


void UserService::regGetMyselfCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	REG_NOTIFY(logic_cmd_getMyself, args[0], &UserService::getMyselfHandler, userService);
}

void UserService::getMyselfHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getMyself, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "id", (double)rep->user.info.userid);
	Helper::toJsNumber(jsObject, "timeZone", (double)rep->user.info.timezone);
	Helper::toJsNumber(jsObject, "birthday", (double)rep->user.info.birthday);
	Helper::toJsNumber(jsObject, "sex", (double)rep->user.info.sex);
	Helper::toJsString(jsObject, "area", rep->user.info.area.c_str());
	Helper::toJsString(jsObject, "sign", rep->user.info.sign.c_str());
	Helper::toJsString(jsObject, "name", rep->user.info.name.c_str());
	if (!rep->user.info.emails.empty())
		Helper::toJsString(jsObject, "email", rep->user.info.emails[0].c_str());
	if (!rep->user.info.phones.empty())
		Helper::toJsString(jsObject, "phone", rep->user.info.phones[0].c_str());

	if (!rep->user.info.avatar.empty())
	{
		std::string avatar = channel_->headImgPath() + rep->user.info.avatar;
		Helper::toJsString(jsObject, "avatar", avatar.c_str());
	}
	if (!rep->user.info.srcAvatar.empty())
	{
		std::string srcAvatar = channel_->headImgPath() + rep->user.info.srcAvatar;
		Helper::toJsString(jsObject, "srcAvatar", srcAvatar.c_str());
	}

	Helper::callJsCallback(isolate, cb, jsObject);
}


/**********************请求**********************/


//获取账户信息
void UserService::getAccountInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	
	ICacheMgr *cache = userService->channel_->getCacheMgr();
	imsdk::st_userBean user;;
	cache->getCacheData(ICacheMgr::data_user, &user);

	Isolate *isolate = args.GetIsolate();
	Local<Object> jsObject = Object::New(isolate);

	Helper::toJsNumber(jsObject, "userId", (double)user.info.userid);
	Helper::toJsNumber(jsObject, "sex", (double)user.info.sex);
	Helper::toJsNumber(jsObject, "birthday", (double)user.info.birthday);
	Helper::toJsString(jsObject, "name", user.info.name.c_str());
	Helper::toJsString(jsObject, "nickId", user.info.nickID.c_str());
	Helper::toJsString(jsObject, "area", user.info.area.c_str());
	Helper::toJsString(jsObject, "sign", user.info.sign.c_str());
	if (!user.info.phones.empty())
		Helper::toJsString(jsObject, "phone", user.info.phones[0].c_str());

	if (!user.info.emails.empty())
		Helper::toJsString(jsObject, "email", user.info.emails[0].c_str());

	if (!user.info.avatar.empty())
	{
		std::string avatar = userService->channel_->headImgPath() + user.info.avatar;
		Helper::toJsString(jsObject, "avatar", avatar.c_str());
	}
	if (!user.info.srcAvatar.empty())
	{
		std::string srcAvatar = userService->channel_->headImgPath() + user.info.srcAvatar;
		Helper::toJsString(jsObject, "srcAvatar", srcAvatar.c_str());
	}

	PersistentPtr cb = Helper::persistentFunc(args[0]);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//更新用户信息
void UserService::updateAccountInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_setMySelf *data = (req_setMySelf*)userService->channel_->getCmdData(logic_cmd_setMyself);
	if (data == nullptr) return;

	Local<Object> account = Local<Object>::Cast(args[0]);
	Local<Value> name = account->Get(String::NewFromUtf8(isolate, "name"));
	if (!name->IsUndefined())
	{
		auto strName =  Helper::toCppString(name);
		data->user.info._set_name(strName);
	}

	Local<Value> srcAvatar = account->Get(String::NewFromUtf8(isolate, "srcAvatar"));
	if (!srcAvatar->IsUndefined())
	{
		auto strSrcAvatar = Helper::toCppString(srcAvatar);
		data->user.info._set_srcAvatar(strSrcAvatar);
	}
	
	Local<Value> avatar = account->Get(String::NewFromUtf8(isolate, "avatar"));
	if (!avatar->IsUndefined())
	{
		auto strAvatar = Helper::toCppString(avatar);
		data->user.info._set_avatar(strAvatar);
	}

	Local<Value> sign = account->Get(String::NewFromUtf8(isolate, "sign"));
	if (!sign->IsUndefined())
	{
		auto srcSign = Helper::toCppString(sign);
		data->user.info._set_sign(srcSign);
	}

	Local<Value> sex = account->Get(String::NewFromUtf8(isolate, "sex"));
	if (!sex->IsUndefined())
	{
		auto iSex = (int)Helper::toCppNumber(sex);
		data->user.info._set_sex(iSex);
	}

	Local<Value> birthday = account->Get(String::NewFromUtf8(isolate, "birthday"));
	if (!birthday->IsUndefined())
	{
		auto iBirthday = (int64)Helper::toCppNumber(birthday);
		data->user.info._set_birthday(iBirthday);
	}
	
	Local<Value> nickId = account->Get(String::NewFromUtf8(isolate, "nickId"));
	if (!nickId->IsUndefined())
	{
		auto strNickId = Helper::toCppString(nickId);
		data->user.info._set_nickID(strNickId);
	}

	Local<Value> phone = account->Get(String::NewFromUtf8(isolate, "phone"));
	if (!phone->IsUndefined())
	{
		std::vector<std::string> phones(1, Helper::toCppString(phone));
		data->user.info._set_phones(phones);
	}

	Local<Value> email = account->Get(String::NewFromUtf8(isolate, "email"));
	if (!email->IsUndefined())
	{
		std::vector<std::string> emails(1, Helper::toCppString(email));
		data->user.info._set_emails(emails);
	}

	POST_REQUEST(logic_cmd_setMyself, data, args[1], &UserService::commonHandler, userService);
}

// 增加本地配置，以键值对方式保存
void UserService::addLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	imsdk::req_localSetting *data = (imsdk::req_localSetting*)userService->channel_->getCmdData(imsdk::logic_cmd_localSetting);
	if (data == nullptr) return;

	data->type = 0;
	Local<Array> items = Local<Array>::Cast(args[0]);
	for (uint32_t i = 0; i < items->Length(); ++i)
	{
		auto item = items->Get(i);
		Local<Object> obj = Local<Object>::Cast(item);
		auto key = obj->Get(String::NewFromUtf8(isolate, "key"));
		auto value = obj->Get(String::NewFromUtf8(isolate, "value"));
		data->items.push_back({ Helper::toCppString(key), Helper::toCppString(value) });
	}

	POST_REQUEST(logic_cmd_localSetting, data, args[1], &UserService::commonHandler, userService);
}

//获取本地配置
void UserService::getLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_localSetting *data = (req_localSetting*)userService->channel_->getCmdData(logic_cmd_localSetting);
	if (data == nullptr) return;

	auto keys = Helper::toCppStrVector(args[0]);
	data->type = 1;
	for (auto &i : keys)
		data->items.push_back({ i, "" });

	POST_REQUEST(logic_cmd_localSetting, data, args[1], &UserService::getLocalSettingHandler, userService);
}

void UserService::getLocalSettingHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_localSetting, data, len);
	//数据转换
	Local<Array> items = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->items)
	{
		Local<Object> jsObj = Object::New(isolate);
		Helper::toJsString(jsObj, "key", i.key.c_str());
		Helper::toJsString(jsObj, "value", i.val.c_str());
		items->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "items"), items);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//更新本地配置
void UserService::updateLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	imsdk::req_localSetting *data = (imsdk::req_localSetting*)userService->channel_->getCmdData(imsdk::logic_cmd_localSetting);
	if (data == nullptr) return;

	data->type = 2;
	Local<Array> items = Local<Array>::Cast(args[0]);
	for (uint32_t i = 0; i < items->Length(); ++i)
	{
		auto item = items->Get(i);
		Local<Object> obj = Local<Object>::Cast(item);
		auto key = obj->Get(String::NewFromUtf8(isolate, "key"));
		auto value = obj->Get(String::NewFromUtf8(isolate, "value"));
		data->items.push_back({ Helper::toCppString(key), Helper::toCppString(value) });
	}

	POST_REQUEST(logic_cmd_localSetting, data, args[1], &UserService::commonHandler, userService);
}

// 删除本地配置
void UserService::deleteLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_localSetting *data = (req_localSetting*)userService->channel_->getCmdData(logic_cmd_localSetting);
	if (data == nullptr) return;

	auto keys = Helper::toCppStrVector(args[0]);
	data->type = 3;
	for (auto &i : keys)
		data->items.push_back({ i, "" });

	POST_REQUEST(logic_cmd_localSetting, data, args[1], &UserService::commonHandler, userService);
}

// 通过label查询表情包
void UserService::queryEmoticonPackageByLabel(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_labelQueryEmoticonPackage *data = (req_labelQueryEmoticonPackage*)userService->channel_->getCmdData(logic_cmd_labelQueryEmoticonPackage);
	if (data == nullptr) return;

	data->label = Helper::toCppString(args[0]);

	POST_REQUEST(logic_cmd_labelQueryEmoticonPackage, data, args[1], &UserService::queryEmoticonPackageByLabelHandler, userService);
}

void UserService::queryEmoticonPackageByLabelHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_labelQueryEmoticonPackage, data, len);

	Local<Array> packages = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->emotPackageList)
	{
		Local<Object> package = Object::New(isolate);
		Helper::toJsString(package, "bannerUrl", i.bannerUrl.c_str());
		Helper::toJsNumber(package, "category", (double)i.category);
		Helper::toJsString(package, "coverUrl", i.coverUrl.c_str());
		Helper::toJsNumber(package, "createTime", (double)i.createTime);
		Helper::toJsString(package, "depict", i.depict.c_str());
		Helper::toJsString(package, "extendColumn", i.extendColumn.c_str());
		Helper::toJsString(package, "extendFiled", i.extendFiled.c_str());
		Helper::toJsString(package, "iconUrl", i.iconUrl.c_str());
		Helper::toJsString(package, "instruction", i.instruction.c_str());
		Helper::toJsString(package, "label", i.label.c_str());
		Helper::toJsString(package, "mainUrl", i.mainUrl.c_str());
		Helper::toJsString(package, "mdCode", i.mdCode.c_str());
		Helper::toJsString(package, "name", i.name.c_str());
		Helper::toJsString(package, "orderNum", i.orderNum.c_str());
		Helper::toJsNumber(package, "packStatus", (double)i.packStatus);
		Helper::toJsNumber(package, "property", (double)i.property);
		Helper::toJsNumber(package, "type", (double)i.type);
		Helper::toJsNumber(package, "userID", (double)i.userID);

		int index1 = 0;
		Local<Array> emoticonList = Array::New(isolate);
		for (auto &j : i.emoticonList)
		{
			Local<Object> emoticon = Object::New(isolate);
			Helper::toJsString(emoticon, "mdCode", j.mdCode.c_str());
			Helper::toJsString(emoticon, "thumbnailUrl", j.thumbnailUrl.c_str());
			Helper::toJsString(emoticon, "mainUrl", j.mainUrl.c_str());
			Helper::toJsString(emoticon, "meaning", j.meaning.c_str());
			emoticonList->Set(index1++, emoticon);
		}

		packages->Set(index++, package);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "packages"), packages);
	Helper::callJsCallback(isolate, cb, jsObject);
}

// 单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
void UserService::queryEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_queryEmoticon *data = (req_queryEmoticon*)userService->channel_->getCmdData(logic_cmd_queryEmoticon);
	if (data == nullptr) return;

	data->type = (int8)Helper::toCppNumber(args[0]);
	if (data->type != 7)
	{
		std::string md5 = Helper::toCppString(args[1]);
		data->_set_mdCode(md5);
	}

	POST_REQUEST(logic_cmd_queryEmoticon, data, args[2], &UserService::queryEmoticonHandler, userService);
}

void UserService::queryEmoticonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_queryEmoticon, data, len);

	Local<Array> emoticonList = Array::New(isolate);
	int index = 0;
	for (auto&i : rep->emotlist)
	{
		Local<Object> emoticon = Object::New(isolate);
		Helper::toJsNumber(emoticon, "category", (double)i.category);
		Helper::toJsNumber(emoticon, "createTime", (double)i.createTime);
		Helper::toJsString(emoticon, "depict", i.depict.c_str());
		Helper::toJsNumber(emoticon, "emoStatus", (double)i.emoStatus);
		Helper::toJsString(emoticon, "extendColumn", i.extendColumn.c_str());
		Helper::toJsString(emoticon, "extendFiled", i.extendFiled.c_str());
		Helper::toJsString(emoticon, "label", i.label.c_str());
		Helper::toJsString(emoticon, "mainUrl", i.mainUrl.c_str());
		Helper::toJsString(emoticon, "mdCode", i.mdCode.c_str());
		Helper::toJsString(emoticon, "meaning", i.meaning.c_str());
		Helper::toJsString(emoticon, "orderNum", i.orderNum.c_str());
		Helper::toJsString(emoticon, "packageCode", i.packageCode.c_str());
		Helper::toJsNumber(emoticon, "property", (double)i.property);
		Helper::toJsString(emoticon, "thumbnailUrl", i.thumbnailUrl.c_str());
		Helper::toJsNumber(emoticon, "type", (double)i.type);
		Helper::toJsNumber(emoticon, "userID", (double)i.userID);
		emoticonList->Set(index++, emoticon);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "emoticonList"), emoticonList);
	Helper::callJsCallback(isolate, cb, jsObject);
}

// 单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
void UserService::singleQueryEmoticonPackage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_singleQueryEmoticonPackage *data = (req_singleQueryEmoticonPackage*)userService->channel_->getCmdData(logic_cmd_singleQueryEmoticonPackage);
	if (data == nullptr) return;

	data->mdCode = Helper::toCppString(args[0]);

	POST_REQUEST(logic_cmd_singleQueryEmoticonPackage, data, args[1], &UserService::singleQueryEmoticonPackageHandler, userService);
}

void UserService::singleQueryEmoticonPackageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_singleQueryEmoticonPackage, data, len);

	Local<Object> emoticonPackage = Object::New(isolate);
	Helper::toJsString(emoticonPackage, "bannerUrl", rep->emotPackage.bannerUrl.c_str());
	Helper::toJsNumber(emoticonPackage, "category", (double)rep->emotPackage.category);
	Helper::toJsString(emoticonPackage, "coverUrl", rep->emotPackage.coverUrl.c_str());
	Helper::toJsNumber(emoticonPackage, "createTime", (double)rep->emotPackage.createTime);
	Helper::toJsString(emoticonPackage, "depict", rep->emotPackage.depict.c_str());

	Local<Array> emoticonList = Array::New(isolate);
	int index = 0;
	for (auto&i : rep->emotPackage.emoticonList)
	{
		Local<Object> emoticon = Object::New(isolate);
		Helper::toJsString(emoticon, "mdCode", i.mdCode.c_str());
		Helper::toJsString(emoticon, "thumbnailUrl", i.thumbnailUrl.c_str());
		Helper::toJsString(emoticon, "mainUrl", i.mainUrl.c_str());
		Helper::toJsString(emoticon, "meaning", i.meaning.c_str());
		emoticonList->Set(index++, emoticon);
	}
	emoticonPackage->Set(String::NewFromUtf8(isolate, "emoticonList"), emoticonList);

	Helper::toJsString(emoticonPackage, "extendColumn", rep->emotPackage.extendColumn.c_str());
	Helper::toJsString(emoticonPackage, "extendFiled", rep->emotPackage.extendFiled.c_str());
	Helper::toJsString(emoticonPackage, "iconUrl", rep->emotPackage.iconUrl.c_str());
	Helper::toJsString(emoticonPackage, "instruction", rep->emotPackage.instruction.c_str());
	Helper::toJsString(emoticonPackage, "label", rep->emotPackage.label.c_str());
	Helper::toJsString(emoticonPackage, "mainUrl", rep->emotPackage.mainUrl.c_str());
	Helper::toJsString(emoticonPackage, "mdCode", rep->emotPackage.mdCode.c_str());
	Helper::toJsString(emoticonPackage, "name", rep->emotPackage.name.c_str());
	Helper::toJsString(emoticonPackage, "orderNum", rep->emotPackage.orderNum.c_str());
	Helper::toJsNumber(emoticonPackage, "packStatus", (double)rep->emotPackage.packStatus);
	Helper::toJsNumber(emoticonPackage, "property", (double)rep->emotPackage.property);
	Helper::toJsNumber(emoticonPackage, "type", (double)rep->emotPackage.type);
	Helper::toJsNumber(emoticonPackage, "userID", (double)rep->emotPackage.userID);

	jsObject->Set(String::NewFromUtf8(isolate, "emoticonPackage"), emoticonPackage);
	Helper::callJsCallback(isolate, cb, jsObject);
}

// 分页查询表情包
void UserService::pageQueryEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_pageQueryEmoticon *data = (req_pageQueryEmoticon*)userService->channel_->getCmdData(logic_cmd_pageQueryEmoticon);
	if (data == nullptr) return;

	data->pageNum = (int32)Helper::toCppNumber(args[0]);
	data->pageSize = (int32)Helper::toCppNumber(args[1]);

	POST_REQUEST(logic_cmd_pageQueryEmoticon, data, args[2], &UserService::pageQueryEmoticonHandler, userService);
}

void UserService::pageQueryEmoticonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_pageQueryEmoticon, data, len);

	Helper::toJsNumber(jsObject, "pageNum", (double)rep->pageNum);
	Helper::toJsNumber(jsObject, "pageSize", (double)rep->pageSize);
	Helper::toJsNumber(jsObject, "totalPage", (double)rep->totalPage);
	Helper::toJsNumber(jsObject, "totalItems", (double)rep->totalItems);

	Local<Array> packageList = Array::New(isolate);
	int index = 0;
	for (auto&i : rep->emotItem)
	{
		Local<Object> package = Object::New(isolate);
		Helper::toJsString(package, "bannerUrl", i.bannerUrl.c_str());
		Helper::toJsNumber(package, "category", (double)i.category);
		Helper::toJsString(package, "coverUrl", i.coverUrl.c_str());
		Helper::toJsNumber(package, "createTime", (double)i.createTime);
		Helper::toJsString(package, "depict", i.depict.c_str());
		Helper::toJsString(package, "extendColumn", i.extendColumn.c_str());
		Helper::toJsString(package, "extendFiled", i.extendFiled.c_str());
		Helper::toJsString(package, "iconUrl", i.iconUrl.c_str());
		Helper::toJsString(package, "instruction", i.instruction.c_str());
		Helper::toJsString(package, "label", i.label.c_str());
		Helper::toJsString(package, "mainUrl", i.mainUrl.c_str());
		Helper::toJsString(package, "mdCode", i.mdCode.c_str());
		Helper::toJsString(package, "name", i.name.c_str());
		Helper::toJsString(package, "orderNum", i.orderNum.c_str());
		Helper::toJsNumber(package, "packStatus", (double)i.packStatus);
		Helper::toJsNumber(package, "property", (double)i.property);
		Helper::toJsNumber(package, "type", (double)i.type);
		Helper::toJsNumber(package, "userID", (double)i.userID);

		Local<Array> emoticonList = Array::New(isolate);
		int index1 = 0;
		for (auto &j : i.emoticonList)
		{
			Local<Object> emoticon = Object::New(isolate);
			Helper::toJsString(emoticon, "mdCode", j.mdCode.c_str());
			Helper::toJsString(emoticon, "thumbnailUrl", j.thumbnailUrl.c_str());
			Helper::toJsString(emoticon, "mainUrl", j.mainUrl.c_str());
			Helper::toJsString(emoticon, "meaning", j.meaning.c_str());
			emoticonList->Set(index1++, emoticon);
		}
		package->Set(String::NewFromUtf8(isolate, "emoticonList"), emoticonList);

		packageList->Set(index++, package);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "packageList"), packageList);

	Helper::callJsCallback(isolate, cb, jsObject);
}

// 增删自定义表情
void UserService::addOrDeleteCustomEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_addOrDeleteCustomEmoticon *data = (req_addOrDeleteCustomEmoticon*)userService->channel_->getCmdData(logic_cmd_addOrDeleteCustomEmoticon);
	if (data == nullptr) return;

	data->type = (int8)Helper::toCppNumber(args[0]);

	Local<Object> emot = Local<Object>::Cast(args[1]);
	Local<Value> category = emot->Get(String::NewFromUtf8(isolate, "category"));
	data->emot.category = (int8)Helper::toCppNumber(category);

	Local<Value> createTime = emot->Get(String::NewFromUtf8(isolate, "createTime"));
	data->emot.createTime = (int64)Helper::toCppNumber(createTime);

	Local<Value> depict = emot->Get(String::NewFromUtf8(isolate, "depict"));
	data->emot.depict = Helper::toCppString(depict);

	Local<Value> emoStatus = emot->Get(String::NewFromUtf8(isolate, "emoStatus"));
	data->emot.emoStatus = (int8)Helper::toCppNumber(emoStatus);

	Local<Value> extendColumn = emot->Get(String::NewFromUtf8(isolate, "extendColumn"));
	data->emot.extendColumn = Helper::toCppString(extendColumn);

	Local<Value> extendFiled = emot->Get(String::NewFromUtf8(isolate, "extendFiled"));
	data->emot.extendFiled = Helper::toCppString(extendFiled);

	Local<Value> label = emot->Get(String::NewFromUtf8(isolate, "label"));
	data->emot.label = Helper::toCppString(label);

	Local<Value> mainUrl = emot->Get(String::NewFromUtf8(isolate, "mainUrl"));
	data->emot.mainUrl = Helper::toCppString(mainUrl);

	Local<Value> mdCode = emot->Get(String::NewFromUtf8(isolate, "mdCode"));
	data->emot.mdCode = Helper::toCppString(mdCode);

	Local<Value> meaning = emot->Get(String::NewFromUtf8(isolate, "meaning"));
	data->emot.meaning = Helper::toCppString(meaning);

	Local<Value> orderNum = emot->Get(String::NewFromUtf8(isolate, "orderNum"));
	data->emot.orderNum = Helper::toCppString(orderNum);

	Local<Value> packageCode = emot->Get(String::NewFromUtf8(isolate, "packageCode"));
	data->emot.packageCode = Helper::toCppString(packageCode);

	Local<Value> property = emot->Get(String::NewFromUtf8(isolate, "property"));
	data->emot.property = (int8)Helper::toCppNumber(property);

	Local<Value> thumbnailUrl = emot->Get(String::NewFromUtf8(isolate, "thumbnailUrl"));
	data->emot.thumbnailUrl = Helper::toCppString(thumbnailUrl);

	Local<Value> type = emot->Get(String::NewFromUtf8(isolate, "type"));
	data->emot.type = (int8)Helper::toCppNumber(type);

	Local<Value> userID = emot->Get(String::NewFromUtf8(isolate, "userID"));
	data->emot.userID = (int8)Helper::toCppNumber(userID);

	POST_REQUEST(logic_cmd_addOrDeleteCustomEmoticon, data, args[2], &UserService::addOrDeleteCustomEmoticonHandler, userService);
}

void UserService::addOrDeleteCustomEmoticonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_addOrDeleteCustomEmoticon, data, len);

	Local<Array> successList = Array::New(isolate);
	Local<Array> failList = Array::New(isolate);

	int index = 0;
	for (auto&i : rep->successList)
	{
		Local<Object> succ = Object::New(isolate);
		Helper::toJsString(succ, "mdCode", i.mdCode.c_str());
		Helper::toJsNumber(succ, "orderNum", (double)i.orderNum);
		Helper::toJsNumber(succ, "resultCode", (double)i.resultCode);
		Helper::toJsString(succ, "resultMessage", i.resultMessage.c_str());
		successList->Set(index++, succ);
	}
	index = 0;
	for (auto&i : rep->failList)
	{
		Local<Object> fail = Object::New(isolate);
		Helper::toJsString(fail, "mdCode", i.mdCode.c_str());
		Helper::toJsNumber(fail, "orderNum", (double)i.orderNum);
		Helper::toJsNumber(fail, "resultCode", (double)i.resultCode);
		Helper::toJsString(fail, "resultMessage", i.resultMessage.c_str());
		failList->Set(index++, fail);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "successList"), successList);
	jsObject->Set(String::NewFromUtf8(isolate, "failList"), failList);

	Helper::callJsCallback(isolate, cb, jsObject);
}

// 单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
void UserService::queryEmotionInPackage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_querySingleEmoticon *data = (req_querySingleEmoticon*)userService->channel_->getCmdData(logic_cmd_querySingleEmoticon);
	if (data == nullptr) return;

	data->packMdCode = Helper::toCppString(args[0]);
	data->mdCode = Helper::toCppString(args[1]);

	POST_REQUEST(logic_cmd_querySingleEmoticon, data, args[2], &UserService::queryEmotionInPackageHandler, userService);
}

void UserService::queryEmotionInPackageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_querySingleEmoticon, data, len);

	Helper::toJsString(jsObject, "packMdCode", rep->packMdCode.c_str());
	Helper::toJsString(jsObject, "mainUrl", rep->mainUrl.c_str());
	Helper::toJsString(jsObject, "meaning", rep->meaning.c_str());
	Helper::toJsString(jsObject, "mdCode", rep->mdCode.c_str());
	Helper::toJsNumber(jsObject, "type", (double)rep->type);

	Helper::callJsCallback(isolate, cb, jsObject);
}

//通过应用ID获取应用信息
void UserService::getAppInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_getAppinfo *data = (req_getAppinfo*)userService->channel_->getCmdData(logic_cmd_getAppInfo);
	if (data == nullptr) return;

	data->appID = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getAppInfo, data, args[1], &UserService::getAppInfoHandler, userService);
}

void UserService::getAppInfoHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getAppInfo, data, len);

	Local<Object> appInfo = Object::New(isolate);
	if (!rep->info.appIcon.empty())
	{
		std::string appIcon = channel_->headImgPath() + rep->info.appIcon;
		Helper::toJsString(appInfo, "appInfo", appIcon.c_str());
	}
	Helper::toJsNumber(appInfo, "appID", rep->info.appID);
	Helper::toJsString(appInfo, "appMenus", rep->info.appMenus.c_str());
	Helper::toJsString(appInfo, "appName", rep->info.appName.c_str());
	Helper::toJsString(appInfo, "appSecret", rep->info.appSecret.c_str());
	Helper::toJsString(appInfo, "appSign", rep->info.appSign.c_str());
	Helper::toJsString(appInfo, "appToken", rep->info.appToken.c_str());

	Local<Object> configItems = Object::New(isolate);
	for (int i = 0; i < rep->info.configKey.size() && i < rep->info.configVal.size(); ++i)
		Helper::toJsString(configItems, rep->info.configKey[i].c_str(), rep->info.configVal[i].c_str());
	
	appInfo->Set(String::NewFromUtf8(isolate, "configItems"), configItems);
	Helper::toJsNumber(appInfo, "entID", (double)rep->info.entID);
	Helper::toJsString(appInfo, "entName", rep->info.entName.c_str());
	Helper::toJsNumber(appInfo, "openIDEncoded", (double)rep->info.openIDEncoded);
	Helper::toJsString(appInfo, "subAccount", rep->info.subAccount.c_str());
	Helper::toJsNumber(appInfo, "appSubType", (double)rep->info.appSubType);
	Helper::toJsNumber(appInfo, "recommend", (double)rep->info.recommend);
	Helper::toJsNumber(appInfo, "defaultInstall", (double)rep->info.defaultInstall);
	Helper::toJsString(appInfo, "extend", rep->info.extend.c_str());
	Helper::toJsString(appInfo, "deviceTypes", rep->info.deviceTypes.c_str());
	Helper::toJsString(appInfo, "messageHistoryUrl", rep->info.messageHistoryUrl.c_str());
	Helper::toJsString(appInfo, "menuOpen", rep->info.menuOpen.c_str());
	Helper::toJsString(appInfo, "replyMenuOpen", rep->info.replyMenuOpen.c_str());
	Helper::toJsNumber(appInfo, "appType", (double)rep->info.appType);
	Helper::toJsNumber(appInfo, "status", (double)rep->info.status);
	Helper::toJsNumber(appInfo, "subOrAppType", (double)rep->info.subOrAppType);
	Helper::toJsString(appInfo, "appFunctintroduce", rep->info.appFunctintroduce.c_str());
	Helper::toJsString(appInfo, "appDescription", rep->info.appDescription.c_str());

	jsObject->Set(String::NewFromUtf8(isolate, "appInfo"), appInfo);

	Helper::callJsCallback(isolate, cb, jsObject);
}

//查询应用市场应用信息
void UserService::queryMarketApplication(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_queryMarketApplication *data = (req_queryMarketApplication*)userService->channel_->getCmdData(logic_cmd_queryMarketApplication);
	if (data == nullptr) return;

	Local<Object> qData = Local<Object>::Cast(args[0]);
	Local<Value> pageNum = qData->Get(String::NewFromUtf8(isolate, "pageNum"));
	data->pageNum = (int32)Helper::toCppNumber(pageNum);

	Local<Value> pageSize = qData->Get(String::NewFromUtf8(isolate, "pageSize"));
	data->pageSize = (int32)Helper::toCppNumber(pageSize);

	Local<Value> userId = qData->Get(String::NewFromUtf8(isolate, "userId"));
	data->userID = (int64)Helper::toCppNumber(userId);

	Local<Value> deviceType = qData->Get(String::NewFromUtf8(isolate, "deviceType"));
	data->deviceType = (int32)Helper::toCppNumber(deviceType);

	Local<Value> appName = qData->Get(String::NewFromUtf8(isolate, "appName"));
	auto str = Helper::toCppString(appName);
	if (!str.empty())
		data->_set_appName(str);

	POST_REQUEST(logic_cmd_queryMarketApplication, data, args[1], &UserService::queryMarketApplicationHandler, userService);
}
void UserService::queryMarketApplicationHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_queryMarketApplication, data, len);
	Local<Object> marketAppPage = Object::New(isolate);
	Helper::toJsNumber(marketAppPage, "count", (double)rep->marketApp.count);
	Helper::toJsNumber(marketAppPage, "pageNum", (double)rep->marketApp.pageNum);
	Helper::toJsNumber(marketAppPage, "pageSize", (double)rep->marketApp.pageSize);
	Helper::toJsNumber(marketAppPage, "totalPage", (double)rep->marketApp.totalPage);

	Local<Array> appInfoList = Array::New(isolate);
	int index = 0;
	for (auto&i : rep->marketApp.appInfoList)
	{
		Local<Object> marketAppInfo = Object::New(isolate);
		Helper::toJsString(marketAppInfo, "activityName", i.activityName.c_str());
		Helper::toJsString(marketAppInfo, "adminUrl", i.adminUrl.c_str());
		Helper::toJsString(marketAppInfo, "appFunctintroduce", i.appFunctintroduce.c_str());
		Helper::toJsString(marketAppInfo, "appIcon", i.appIcon.c_str());
		Helper::toJsNumber(marketAppPage, "appID", (double)i.appID);
		Helper::toJsString(marketAppInfo, "appName", i.appName.c_str());
		Helper::toJsString(marketAppInfo, "appSign", i.appSign.c_str());
		Helper::toJsNumber(marketAppPage, "appType", (double)i.appType);
		Helper::toJsNumber(marketAppPage, "defaultInstall", (double)i.defaultInstall);
		Helper::toJsString(marketAppInfo, "downloadUrl", i.downloadUrl.c_str());
		Helper::toJsNumber(marketAppPage, "entID", (double)i.entID);
		Helper::toJsString(marketAppInfo, "extend", i.extend.c_str());
		Helper::toJsString(marketAppInfo, "mainUrl", i.mainUrl.c_str());
		Helper::toJsString(marketAppInfo, "myUrl", i.myUrl.c_str());
		Helper::toJsNumber(marketAppPage, "ownerId", (double)i.ownerId);
		Helper::toJsString(marketAppInfo, "packageName", i.packageName.c_str());
		Helper::toJsString(marketAppInfo, "quickUrl", i.quickUrl.c_str());
		Helper::toJsNumber(marketAppPage, "showInMy", (double)i.showInMy);
		Helper::toJsNumber(marketAppPage, "status", (double)i.status);
		Helper::toJsString(marketAppInfo, "urlScheme", i.urlScheme.c_str());
		Helper::toJsString(marketAppInfo, "version", i.version.c_str());
		appInfoList->Set(index++, marketAppInfo);
	}
	marketAppPage->Set(String::NewFromUtf8(isolate, "appInfoList"), appInfoList);
	jsObject->Set(String::NewFromUtf8(isolate, "marketAppPage"), marketAppPage);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//删除添加应用
void UserService::addOrDeleteApplication(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_addOrDeleteApplication *data = (req_addOrDeleteApplication*)userService->channel_->getCmdData(logic_cmd_addOrDeleteApplication);
	if (data == nullptr) return;

	data->appID = (int64)Helper::toCppNumber(args[0]);
	data->type = (int8)Helper::toCppNumber(args[1]);

	POST_REQUEST(logic_cmd_addOrDeleteApplication, data, args[2], &UserService::commonHandler, userService);
}

//获取已安装的应用
void UserService::getInstalledApplication(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_getInstalledApplication *data = (req_getInstalledApplication*)userService->channel_->getCmdData(logic_cmd_getInstalledApplication);
	if (data == nullptr) return;

	data->deviceType = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getInstalledApplication, data, args[1], &UserService::getInstalledApplicationHandler, userService);
}
void UserService::getInstalledApplicationHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getInstalledApplication, data, len);

	Local<Array> appInfoList = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->appInfoList)
	{
		Local<Object> appInfo = Object::New(isolate);
		Helper::toJsString(appInfo, "activityName", i.activityName.c_str());
		Helper::toJsString(appInfo, "adminUrl", i.adminUrl.c_str());
		Helper::toJsString(appInfo, "appFunctintroduce", i.appFunctintroduce.c_str());
		Helper::toJsString(appInfo, "appIcon", i.appIcon.c_str());
		Helper::toJsNumber(appInfo, "appID", (double)i.appID);
		Helper::toJsString(appInfo, "appName", i.appName.c_str());
		Helper::toJsString(appInfo, "appSign", i.appSign.c_str());
		Helper::toJsNumber(appInfo, "appType", (double)i.appType);
		Helper::toJsNumber(appInfo, "defaultInstall", (double)i.defaultInstall);
		Helper::toJsString(appInfo, "downloadUrl", i.downloadUrl.c_str());
		Helper::toJsNumber(appInfo, "entID", (double)i.entID);
		Helper::toJsString(appInfo, "extend", i.extend.c_str());
		Helper::toJsString(appInfo, "mainUrl", i.mainUrl.c_str());
		Helper::toJsString(appInfo, "myUrl", i.myUrl.c_str());
		Helper::toJsNumber(appInfo, "ownerId", (double)i.ownerId);
		Helper::toJsString(appInfo, "packageName", i.packageName.c_str());
		Helper::toJsString(appInfo, "quickUrl", i.quickUrl.c_str());
		Helper::toJsNumber(appInfo, "showInMy", (double)i.showInMy);
		Helper::toJsNumber(appInfo, "status", (double)i.status);
		Helper::toJsString(appInfo, "urlScheme", i.urlScheme.c_str());
		Helper::toJsString(appInfo, "version", i.version.c_str());
		appInfoList->Set(index++, appInfo);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "appInfoList"), appInfoList);

	Helper::callJsCallback(isolate, cb, jsObject);
}

void UserService::setPersonalData(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_PersonalData *data = (req_PersonalData*)userService->channel_->getCmdData(logic_cmd_setPersonalData);
	if (data == nullptr) return;

	Local<Array> items = Local<Array>::Cast(args[0]);
	for (uint32_t i = 0; i < items->Length(); ++i)
	{
		auto item = items->Get(i);
		Local<Object> obj = Local<Object>::Cast(item);
		auto key = obj->Get(String::NewFromUtf8(isolate, "type"));
		auto value = obj->Get(String::NewFromUtf8(isolate, "value"));
		data->items.push_back({ (int16)Helper::toCppNumber(key), (int16)Helper::toCppNumber(value) });
	}

	POST_REQUEST(logic_cmd_setPersonalData, data, args[1], &UserService::commonHandler, userService);
}

void UserService::getPersonalData(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_PersonalData *data = (req_PersonalData*)userService->channel_->getCmdData(logic_cmd_getPersonalData);
	if (data == nullptr) return;

	std::vector<int16> items;
	Helper::toCppNumVector(items, args[0]);
	for (auto &i : items)
		data->items.push_back({ i, 0 });

	POST_REQUEST(logic_cmd_getPersonalData, data, args[1], &UserService::getPersonalDataHandler, userService);
}

void UserService::getPersonalDataHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_PersonalData, data, len);
	//数据转换
	Local<Array> items = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->vt)
	{
		Local<Object> jsObj = Object::New(isolate);
		Helper::toJsNumber(jsObj, "key", (double)i.type);
		Helper::toJsNumber(jsObj, "value", (double)i.val);
		items->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "items"), items);
	Helper::callJsCallback(isolate, cb, jsObject);
}

void UserService::getGlobalNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	_sdkcmd_base *data = (_sdkcmd_base*)userService->channel_->getCmdData(logic_cmd_getNoDisturbMode);
	if (data == nullptr) return;

	POST_REQUEST(logic_cmd_getNoDisturbMode, data, args[0], &UserService::getGlobalNoDisturbModeHandler, userService);
}

void UserService::getGlobalNoDisturbModeHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getNoDisturbMode, data, len);
	bool flag = (rep->flag == 1) ? true : false;

	Helper::toJsNumber(jsObject, "startTime", (double)rep->startTime);
	Helper::toJsNumber(jsObject, "endTime", (double)rep->endTime);
	Helper::toJsBool(jsObject, "flag", rep->flag);

	Helper::callJsCallback(isolate, cb, jsObject);
}

void UserService::setGolbalNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_setNoDisturbMode *data = (req_setNoDisturbMode*)userService->channel_->getCmdData(logic_cmd_setNoDisturbMode);
	if (data == nullptr) return;

	data->startTime = (int64)Helper::toCppNumber(args[0]);
	data->endTime = (int64)Helper::toCppNumber(args[1]);	
	bool isOpen = Helper::toCppBool(args[2]);
	data->flag = (isOpen == true) ? 1 : 0;

	POST_REQUEST(logic_cmd_setNoDisturbMode, data, args[3], &UserService::commonHandler, userService);
}

void UserService::getUserNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_getUserToTargetSwitch *data = (req_getUserToTargetSwitch*)userService->channel_->getCmdData(logic_cmd_getUserToTargetSwitch);
	if (data == nullptr) return;

	data->type = 1;
	data->targetId = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getUserToTargetSwitch, data, args[1], &UserService::getUserNoDisturbModeHandler, userService);
}

void UserService::getUserNoDisturbModeHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getUserToTargetSwitch, data, len);

	Helper::toJsNumber(jsObject, "targetId", (double)rep->targetId);
	Helper::toJsNumber(jsObject, "value", (double)rep->value1);

	Helper::callJsCallback(isolate, cb, jsObject);
}

void UserService::setUserNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	UserService* userService = ObjectWrap::Unwrap<UserService>(args.Holder());
	req_setUserToTargetSwitch *data = (req_setUserToTargetSwitch*)userService->channel_->getCmdData(logic_cmd_setUserToTargetSwitch);
	if (data == nullptr) return;

	data->type = 1;
	data->targetId = (int64)Helper::toCppNumber(args[0]);
	data->value = (int64)Helper::toCppNumber(args[1]);

	POST_REQUEST(logic_cmd_setUserToTargetSwitch, data, args[2], &UserService::commonHandler, userService);
}

void UserService::commonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(_sdkrespcmd_base, data, 0);

	Helper::callJsCallback(isolate, cb, jsObject);
}

} /*namespace service*/

