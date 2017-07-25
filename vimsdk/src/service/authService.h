#ifndef NODE_AUTH_SERVICE_
#define NODE_AUTH_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class AuthService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit AuthService(SdkChannel *channel);
	~AuthService();

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************注册推送********************/

	//注册网络变化通知
	static void regNetChangedCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void netChangedHandler(void *data, int len, PersistentPtr cb);

	//注册数据库升级完成通知
	static void regDbUpdatedCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void dbUpdatedHandler(void *data, int len, PersistentPtr cb);

	//注册重新登录通知
	static void regReLoginCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void reLoginHandler(void *data, int len, PersistentPtr cb);

	//注册版本升级通知
	static void regUpgrageCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void upgrageHandler(void *data, int len, PersistentPtr cb);

	/**********************请求**********************/

	void setLoginParam(imsdk::req_LoginParamBean &param, const std::string &user, const std::string &pwd, int8_t userType, const std::string &server);
	
	/**
	* \brief 快速注册
	* @param[in] server:string 传入服务器地址，域名或IP均可
	* @param[in] name:string 用户昵称
	* @param[in] account:string 传入注册账号 如果是手机账户格式为“0086158********”
	* @param[in] passwd:string 密码
	* @param[in] cb 传入接收结果回调 _1错误信息 
	*/
	static void quickRegister(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 获取注册验证码
	* @param[in] account:string 传入注册账号 如果是手机账户格式为“0086158********”
	* @param[in] userType:number 账号类型
	* @param[in] server:string 服务器
	* @param[in] cb 传入接收结果回调 _1错误信息  _2超时
	*/
	static void getRegCode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getCodeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 注册用户
	* @param[in] regCode 验证码
	* @param[in] user    传入用户名，建议使用真实姓名
	* @param[in] pwd	 传入注册密码
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void signup(const v8::FunctionCallbackInfo<v8::Value> &args);
	void signupHandler(void *data, int len, PersistentPtr cb);
	
	/**
	* \brief 获取重置密码验证码
	* @param[in] server:string 服务器
	* @param[in] account:string 传入注册账号 如果是手机账户格式为“0086158********”
	* @param[in] cb 传入接收结果回调 _1错误信息  _2超时
	*/
	static void getResetPwCode(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 重置密码
	* @param[in] regCode 验证码
	* @param[in] user    传入用户名，为空代表不改名
	* @param[in] pwd	 传入新密码
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void resetPasswd(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取密码复杂度
	* @param[in] cb 传入接收结果回调 _1错误信息
	*  _2密码规则:
	*	高8位代表最小长度
	*	低8位，按照最低位开始，依次代表:(1代表必须，0 表示可选)
	*	1. 是否必须有数字
	*	2. 是否必须有小写字母
	*	3. 是否必须有大写字母
	*	4. 是否必须有英文字母
	*	5. 是否必须有字符(特殊字符)
	*	6. 是否允许注册(1允许，0不允许)
	*/
	static void getPasswordRule(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getPasswordRuleHandler(void *data, int len, PersistentPtr cb);

	/**
	*  \brief 登录
	* @param[in] user:string 传入用户名 如果是手机账户格式为“0086158********”
	* @param[in] pwd:string 传入密码
	* @param[in] userType:number 账号类型
	* @param[in] server:string 传入服务器地址，域名或IP均可
	* @param[in] cb 传入接收结果回调 _1错误信息  _2用户Id
	* @return	int64 返回当前执行的操作ID，用于取消该次执行
	*/
	static void login(const v8::FunctionCallbackInfo<v8::Value> &args);
	void loginHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 自动登录（不用填密码）
	* @param[in] userid:number 传入用户ID
	* @param[in] userType:number 账号类型
	* @param[in] server:string 传入服务器地址，域名或IP均可
	* @param[in] cb 传入接收结果回调 _1错误信息  _2用户Id
	* @return	int64 返回当前执行的操作ID，用于取消该次执行
	*/
	static void autoLogin(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 离线登录
	* @param[in] userId:number 用户id如果为零，获取最近一次登录成功的用户id
	* @param[in] pwd:string 密码
	* @param[in] cb 传入接收结果回调 _1错误信息
	*/
	static void offlineLogin(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 登出
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void logout(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 验证登录验证码或者获取下一张验证码
	* @param[[in] account:string 传入登录账号
	* @param[[in] code:string 传入验证码内容 code为空则获取下一张验证码
	* @param[[in] cb 传入接收结果回调 _1错误信息  _2下一张验证图片
	*/
	static void verifyImgCode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void verifyImgCodeHandler(void *data, int len, PersistentPtr cb);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

