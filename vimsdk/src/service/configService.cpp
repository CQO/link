#include "configService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> ConfigService::constructor;
ConfigService::ConfigService(SdkChannel *channel) :channel_(channel)
{
}

ConfigService::~ConfigService()
{
}

void ConfigService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "AuthService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "getLoginInfos", getLoginInfos);
	NODE_SET_PROTOTYPE_METHOD(tp, "setLoginInfo", setLoginInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "userImgPath", userImgPath);
	NODE_SET_PROTOTYPE_METHOD(tp, "userAudioPath", userAudioPath);
	NODE_SET_PROTOTYPE_METHOD(tp, "userVideoPath", userVideoPath);
	NODE_SET_PROTOTYPE_METHOD(tp, "headImgPath", headImgPath);
	NODE_SET_PROTOTYPE_METHOD(tp, "userFilePath", userFilePath);

	constructor.Reset(isolate, tp->GetFunction());
}

void ConfigService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		ConfigService* client = new ConfigService((SdkChannel*)channel->Value());
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

/**********************请求**********************/


void ConfigService::getLoginInfos(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());
	st_loginfoArray arr;
	getConfig(configService->channel_->clientId(), cfg_lastLogininfo, &arr);
	
	Local<Array> infos = Array::New(isolate);
	int index = 0;
	for (auto &i : arr._vt)
	{
		Local<Object> info = Object::New(isolate);
		Helper::toJsNumber(info, "status", (double)i.status);   ///< 在线状态-1：离线，1:在线；2:隐身,3:忙碌,4:离开,5:请勿打扰,6:想聊天 移动端用前两个
		Helper::toJsBool(info, "keepPwd", i.keepPwd == 1);    ///< 是否记录密码
		Helper::toJsBool(info, "autoLogin", i.autoLogin == 1);    ///< 是否自动登录
		Helper::toJsNumber(info, "userId", (double)i.userid);	///< 用户ID
		Helper::toJsNumber(info, "userType", (double)i.userType);  ///< 登录账户类型，1：手机，2：QQ，3：邮箱，4：UserID
		Helper::toJsNumber(info, "time", (double)i.time);			///< 上次登录时间
		Helper::toJsString(info, "account", i.account.c_str());		///< 帐号
		Helper::toJsString(info, "pwd", i.pwd.c_str());				///< 密码 未加密
		Helper::toJsString(info, "avatar", i.avatar.c_str());		///< 头像
		Helper::toJsString(info, "nationalCode", i.nationalCode.c_str());   ///< 国家代码
		Helper::toJsString(info, "server", i.entArea.c_str());		///< 企业域
		infos->Set(index++, info);
	}
	args.GetReturnValue().Set(infos);
}

void ConfigService::setLoginInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());
	Local<Object> info = Local<Object>::Cast(args[0]);

	st_LoginInfoBean bean;
	bean.status = (int32)Helper::toCppNumberFromObj(info, "status");
	bean.keepPwd = Helper::toCppBooleanFromObj(info, "keepPwd") ? 1 : 0;
	bean.autoLogin = Helper::toCppBooleanFromObj(info, "autoLogin") ? 1 : 0;
	bean.userid = (int64)Helper::toCppNumberFromObj(info, "userId");
	bean.time = (int64)Helper::toCppNumberFromObj(info, "time");
	bean.account = Helper::toCppStringFromObj(info, "account");
	bean.pwd = Helper::toCppStringFromObj(info, "pwd");
	bean.avatar = Helper::toCppStringFromObj(info, "avatar");
	bean.entArea = Helper::toCppStringFromObj(info, "server");
	bean.nationalCode = Helper::toCppStringFromObj(info, "nationalCode");
	bean.userType = (int32)Helper::toCppNumberFromObj(info, "userType");

	bool ret = setConfig(configService->channel_->clientId(), cfg_lastLogininfo, &bean);
	args.GetReturnValue().Set(Boolean::New(isolate, ret));
}

void ConfigService::userImgPath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());

	if (configService->userImgPath_.empty())
		getConfig(configService->channel_->clientId(), cfg_userImgPath, &configService->userImgPath_);

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, configService->userImgPath_.c_str()));
}

void ConfigService::userAudioPath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());

	if (configService->userAudioPath_.empty())
		getConfig(configService->channel_->clientId(), cfg_userAudioPath, &configService->userAudioPath_);

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, configService->userAudioPath_.c_str()));
}

void ConfigService::userVideoPath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());

	if (configService->userVideoPath_.empty())
		getConfig(configService->channel_->clientId(), cfg_userVideoPath, &configService->userVideoPath_);

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, configService->userVideoPath_.c_str()));
}

void ConfigService::userFilePath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());

	if (configService->userFilePath_.empty())
		getConfig(configService->channel_->clientId(), cfg_userFilePath, &configService->userFilePath_);

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, configService->userFilePath_.c_str()));
}

void ConfigService::headImgPath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	ConfigService* configService = ObjectWrap::Unwrap<ConfigService>(args.Holder());

	if (configService->headImgPath_.empty())
		getConfig(configService->channel_->clientId(), cfg_headImgpath, &configService->headImgPath_);

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, configService->headImgPath_.c_str()));
}

} /*namespace service*/

