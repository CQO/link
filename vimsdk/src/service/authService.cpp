#include "authService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> AuthService::constructor;
AuthService::AuthService(SdkChannel *channel) :channel_(channel)
{
}

AuthService::~AuthService()
{
}

void AuthService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "AuthService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "regNetChangedCb", regNetChangedCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regDbUpdatedCb", regDbUpdatedCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regReLoginCb", regReLoginCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regUpgrageCb", regUpgrageCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "quickRegister", quickRegister);
	NODE_SET_PROTOTYPE_METHOD(tp, "getRegCode", getRegCode);
	NODE_SET_PROTOTYPE_METHOD(tp, "signup", signup);
	NODE_SET_PROTOTYPE_METHOD(tp, "getResetPwCode", getResetPwCode);
	NODE_SET_PROTOTYPE_METHOD(tp, "resetPasswd", resetPasswd);
	NODE_SET_PROTOTYPE_METHOD(tp, "getPasswordRule", getPasswordRule);
	NODE_SET_PROTOTYPE_METHOD(tp, "login", login);
	NODE_SET_PROTOTYPE_METHOD(tp, "autoLogin", autoLogin);
	NODE_SET_PROTOTYPE_METHOD(tp, "offlineLogin", offlineLogin);
	NODE_SET_PROTOTYPE_METHOD(tp, "logout", logout);
	NODE_SET_PROTOTYPE_METHOD(tp, "verifyImgCode", verifyImgCode);

	constructor.Reset(isolate, tp->GetFunction());
}

void AuthService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		AuthService* client = new AuthService((SdkChannel*)channel->Value());
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

//注册网络变化通知
void AuthService::regNetChangedCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	REG_NOTIFY(logic_cmd_netNotifyer, args[0], &AuthService::netChangedHandler, authService);
}

void AuthService::netChangedHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_netNotifyer, data, len);
	//数据转换
	Helper::toJsBool(jsObject, "netIsOk", rep->type == 0);		//网络是否正常
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册数据库升级完成通知
void AuthService::regDbUpdatedCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	REG_NOTIFY(logic_cmd_dbUpdateNotifyer, args[0], &AuthService::dbUpdatedHandler, authService);
}

void AuthService::dbUpdatedHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_commonResult, data, 0);
	//数据转换
	Helper::toJsBool(jsObject, "isFinished", true);	//数据库升级是否完成
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册重新登录通知
void AuthService::regReLoginCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	REG_NOTIFY(logic_cmd_login, args[0], &AuthService::reLoginHandler, authService);
}

void AuthService::reLoginHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_LoginResultBean, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "userId", (double)rep->userid);		//登录的用户ID
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册版本升级通知
void AuthService::regUpgrageCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	REG_NOTIFY(logic_cmd_upgrade, args[0], &AuthService::upgrageHandler, authService);
}

void AuthService::upgrageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_upgrade, data, len);
	//数据转换
	Helper::toJsString(jsObject, "hashCode", rep->hasCode.c_str());			//升级包的HAXI值.
	Helper::toJsBool(jsObject, "forceUpdate", rep->gradeType == 1);			//是否强制升级
	Helper::toJsString(jsObject, "verson", rep->version.c_str());			//服务器版本
	Helper::toJsString(jsObject, "updateAddr", rep->updateAddr.c_str());	//升级的地址
	Helper::toJsString(jsObject, "description", rep->desc.c_str());			//升级描述
	Helper::callJsCallback(isolate, cb, jsObject);
}

/**********************请求**********************/

void AuthService::setLoginParam(imsdk::req_LoginParamBean &param, const std::string &user, const std::string &pwd, int8_t userType, const std::string &server)
{
	std::string username = user;
	param.userType = userType;									/* 用户类型 手机 1，  qq 2 ， 邮箱 3 ，豆豆号 4， 身份证 5 , 6 豆豆账号 ，   */
	param.pwd = pwd;
	param.user = username;
	param.tick = 1;                								/* 踢人策略 1: 强制踢(登录) 2: 不踢人(重连) */
	param.area = server;
	param.mark = server;										/* 版本标识 */

	param.usrid = 0;

	if (param.userType == 1)
		param.nationalCode = user.substr(0, 4);

	auto initInfo = channel_->getInitInfo();
	param.version = initInfo->version;
	param.device_type = initInfo->deviceType;
	param.device_info = initInfo->deviceInfo;
	param.net_type = initInfo->netType;
	param.mac = initInfo->mac;
	param.local = initInfo->local;
}

//快速注册
void AuthService::quickRegister(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_quickRegister *data = (imsdk::req_quickRegister*)authService->channel_->getCmdData(imsdk::logic_cmd_quickRegister);
	if (data == nullptr) return;

	data->type = 7;
	data->login.area = Helper::toCppString(args[0]);
	data->name = Helper::toCppString(args[1]);
	data->account = Helper::toCppString(args[2]);
	data->password = Helper::toCppString(args[3]);
	data->login.usrid = 0;

	POST_REQUEST(logic_cmd_quickRegister, data, args[4], &AuthService::commonHandler, authService);
}

//获取注册验证码
void AuthService::getRegCode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_regParam1 *data = (imsdk::req_regParam1*)authService->channel_->getCmdData(imsdk::logic_cmd_reg1);
	if (data == nullptr) return;

	std::string account = Helper::toCppString(args[0]);
	int8_t userType = (int8_t)Helper::toCppNumber(args[1]);
	std::string server = Helper::toCppString(args[2]);
	imsdk::req_LoginParamBean param;
	authService->setLoginParam(param, account, "", userType, server);

	data->login = param;
	data->number = param.user;
	data->domain = server;
	data->usertype = data->login.userType;

	POST_REQUEST(logic_cmd_reg1, data, args[3], &AuthService::getCodeHandler, authService);
}

void AuthService::getCodeHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_regResult1, data, len);
	//数据转换
	registryId_ = rep->registryID;
	Helper::toJsNumber(jsObject, "timeOut", (double)rep->reregistryTimeout);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册
void AuthService::signup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_regParam2 *data = (imsdk::req_regParam2*)authService->channel_->getCmdData(imsdk::logic_cmd_reg2);
	if (data == nullptr) return;

	//数据转换
	data->code = Helper::toCppString(args[0]);
	data->name = Helper::toCppString(args[1]);
	data->pwd = Helper::toCppString(args[2]);
	data->sdkid = 0;
	data->registryID = authService->registryId_;

	POST_REQUEST(logic_cmd_reg2, data, args[3], &AuthService::commonHandler, authService);
}

//获取重置密码验证码
void AuthService::getResetPwCode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_regParam2 *data = (imsdk::req_regParam2*)authService->channel_->getCmdData(imsdk::logic_cmd_resetPw1);
	if (data == nullptr) return;

	std::string server = Helper::toCppString(args[0]);
	std::string account = Helper::toCppString(args[1]);
	int8_t userType = (int8_t)Helper::toCppNumber(args[2]);
	imsdk::req_LoginParamBean param;
	authService->setLoginParam(param, account, "", userType, server);

	POST_REQUEST(logic_cmd_resetPw1, data, args[3], &AuthService::getCodeHandler, authService);
}

//重置密码
void AuthService::resetPasswd(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_regParam2 *data = (imsdk::req_regParam2*)authService->channel_->getCmdData(imsdk::logic_cmd_resetPw2);
	if (data == nullptr) return;

	//数据转换
	data->code = Helper::toCppString(args[0]);
	data->name = Helper::toCppString(args[1]);
	data->pwd = Helper::toCppString(args[2]);
	data->sdkid = 0;
	data->registryID = authService->registryId_;

	POST_REQUEST(logic_cmd_resetPw2, data, args[3], &AuthService::commonHandler, authService);
}

//获取密码复杂度
void AuthService::getPasswordRule(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_regParam2 *data = (imsdk::req_regParam2*)authService->channel_->getCmdData(imsdk::logic_cmd_getPwRule);
	if (data == nullptr) return;

	POST_REQUEST(logic_cmd_getPwRule, data, args[0], &AuthService::getPasswordRuleHandler, authService);
}

void AuthService::getPasswordRuleHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_pwRuleResult, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "ruleType", (double)rep->stipulate);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//登录
void AuthService::login(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate* isolate = args.GetIsolate();
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	
	imsdk::req_LoginParamBean *data = (imsdk::req_LoginParamBean*)authService->channel_->getCmdData(imsdk::logic_cmd_login);
	if (data == nullptr) return;

	//数据转换
	std::string user = Helper::toCppString(args[0]);
	std::string pwd = Helper::toCppString(args[1]);
	int8_t userType = (int8_t)Helper::toCppNumber(args[2]);
	std::string area = Helper::toCppString(args[3]);
	authService->setLoginParam(*data, user, pwd, userType, area);
	
	POST_REQUEST(logic_cmd_login, data, args[4], &AuthService::loginHandler, authService);

	args.GetReturnValue().Set(Number::New(isolate, (double)data->localID));
}

void AuthService::loginHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_LoginResultBean, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "userId", (double)rep->userid);		//登录的用户ID
	Helper::callJsCallback(isolate, cb, jsObject);
}

//自动登录
void AuthService::autoLogin(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate* isolate = args.GetIsolate();
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());

	imsdk::req_LoginParamBean *data = (imsdk::req_LoginParamBean*)authService->channel_->getCmdData(logic_cmd_loginAuto);
	if (data == nullptr) return;

	data->usrid = (int64_t)Helper::toCppNumber(args[0]);
	int8_t userType = (int8_t)Helper::toCppNumber(args[1]);
	std::string server = Helper::toCppString(args[2]);
	authService->setLoginParam(*data, "", "", userType, server);
	
	POST_REQUEST(logic_cmd_loginAuto, data, args[3], &AuthService::loginHandler, authService);

	args.GetReturnValue().Set(Number::New(isolate, (double)data->localID));
}

//离线登录
void AuthService::offlineLogin(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_offlineLogin *data = (imsdk::req_offlineLogin*)authService->channel_->getCmdData(logic_cmd_offlineLogin);
	if (data == nullptr) return;

	int64_t userId = (int64_t)Helper::toCppNumber(args[0]);
	std::string pwd = Helper::toCppString(args[1]);

	POST_REQUEST(logic_cmd_offlineLogin, data, args[2], &AuthService::loginHandler, authService);
}

//登出
void AuthService::logout(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::_sdkcmd_base *data = (imsdk::_sdkcmd_base*)authService->channel_->getCmdData(logic_cmd_logout);
	if (data == nullptr) return;

	POST_REQUEST(logic_cmd_logout, data, args[0], &AuthService::commonHandler, authService);
}

//验证登录验证码或者获取下一张验证码
void AuthService::verifyImgCode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	AuthService* authService = ObjectWrap::Unwrap<AuthService>(args.Holder());
	imsdk::req_verifyImg *data = (imsdk::req_verifyImg*)authService->channel_->getCmdData(logic_cmd_verifyImg);
	if (data == nullptr) return;

	data->usrName = Helper::toCppString(args[0]);
	data->info = Helper::toCppString(args[1]);
	data->next = (data->info.empty()) ? 1 : 2;
	data->type = 1;
	auto initInfo = authService->channel_->getInitInfo();
	data->macAddr = initInfo->mac;

	POST_REQUEST(logic_cmd_verifyImg, data, args[2], &AuthService::verifyImgCodeHandler, authService);
}

void AuthService::verifyImgCodeHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_verifyImg, data, len);
	//数据转换
	Helper::toJsString(jsObject, "img", rep->img.c_str());	
	Helper::callJsCallback(isolate, cb, jsObject);
}

void AuthService::commonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_commonResult, data, 0);

	Helper::callJsCallback(isolate, cb, jsObject);
}

} /*namespace service*/

