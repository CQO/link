#ifndef NODE_USER_SERVICE_
#define NODE_USER_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class UserService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit UserService(SdkChannel *channel);
	~UserService();

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************注册推送********************/
	/**
	* \brief 监听个人头像更新
	* @param[in] _1 传入目标ID，人或者群 number
	* @param[in] _2	  传入头像 string
	*/
	static void regUserHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void imageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置监听用户信息更新的回调，服务器推送
	* @param[in] _1 用户信息
	*/
	static void regOperateUserCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void operateUserHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 监听用户信息修改，登录更新后会主动推送
	* @param[in] _1 用户信息
	*/
	static void regGetMyselfCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getMyselfHandler(void *data, int len, PersistentPtr cb);

	/**********************请求**********************/
	
	/**
	* \brief 获取账户信息，同步接口
	* @param[in] info 用户信息
	*/
	static void getAccountInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 更新用户信息
	* @param[in] user 传入用户信息
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void updateAccountInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 增加本地配置，以键值对方式保存
	* @param[in] items  配置信息集合
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void addLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 获取本地配置
	* @param[in] keys 获取的配置信息的键集合
	* @param[in] cb 传入接收结果回调  _1错误信息  _2获取的配置信息集合
	*/
	static void getLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getLocalSettingHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 更新本地配置
	* @param[in] newItems 插入的新的配置信息集合
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void updateLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 删除本地配置
	* @param[in] keys 删除的配置信息集合
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void deleteLocalSetting(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief 通过label查询表情包
	* @param[in] label表情包标签
	* @param[in] cb 传入接收结果回调  _1错误信息 _2返回的查询信息
	*/
	static void queryEmoticonPackageByLabel(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryEmoticonPackageByLabelHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
	* @param[in] type  1, 单个查询表情 md5Code表示单个表情的md5, 6,根据表情包标识查询所有表情 md5Code表示表情包的md5, 7,查询用户所有自定义表情 这里md5Code不传值
	* @param[in] md5   md5值
	* @param[in] cb 传入接收结果回调  _1错误信息 _2返回的查询信息
	*/
	static void queryEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryEmoticonHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 单个查询表情包
	* @param[in] md5   md5值
	* @param[in] cb 传入接收结果回调  _1错误信息 _2返回的查询信息
	*/
	static void singleQueryEmoticonPackage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void singleQueryEmoticonPackageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 分页查询表情包
	* @param[in] pageNum   页码
	* @param[in] pageSize   页长
	* @param[in] cb 传入接收结果回调  _1错误信息 _2返回的查询信息
	*/
	static void pageQueryEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args);
	void pageQueryEmoticonHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 增删自定义表情
	* @param[in] type   类型  4 增加， 5删除.
	* @param[in] emot   表情
	* @param[in] cb 传入接收结果回调  _1错误信息 _2resultCode _3resultMsg _4successList _5failedList
	*/
	static void addOrDeleteCustomEmoticon(const v8::FunctionCallbackInfo<v8::Value> &args);
	void addOrDeleteCustomEmoticonHandler(void *data, int len, PersistentPtr cb);

	/**
	*\brief 查选单个表情包内的单个表情
	*@param[in] packageCode 表情包md5
	*@param[in] emotCode  表情md5
	*/
	static void queryEmotionInPackage(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryEmotionInPackageHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 通过应用ID获取应用信息
	* @param[in] appId 应用程序ID
	* @param[in] cb 传入接收结果回调  _1错误信息  _2返回的值
	*/
	static void getAppInfo(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getAppInfoHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 查询应用市场应用信息
	* @param[in] qData 传入的查询信息
	* @param[in] cb 传入接收结果回调  _1错误信息  _2返回的应用信息
	*/
	static void queryMarketApplication(const v8::FunctionCallbackInfo<v8::Value> &args);
	void queryMarketApplicationHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 删除添加应用
	* @param[in] type 传入的查询类型
	* @param[in] appID 操作的appID
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void addOrDeleteApplication(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取已安装的应用
	* @param[in] deviceType 安卓orIOS
	* @param[in] cb 传入接收结果回调  _1错误信息 _2返回的查询信息
	*/
	static void getInstalledApplication(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getInstalledApplicationHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置个人设置项\n
	*		type: 1 (生日)，２（电话），３（邮件）\n
	* 　　　　　　value:  1：所有人可见 2：仅好友可见 3：仅自己可见，默认1\n
	*		type: 4 (好友验证方式)\n
	* 　　　　　　value:   1：需要验证信息,2:不允许任何人添加,3:允许任何人添加，默认1\n
	*		type: 5 V标\n
	*			value: 1:表示始终有声音提醒，2：表示始终无声音提醒 3:不始终提醒，默认1\n
	*		type: 6 @相关人提醒模式\n
	*			value: 1:表示始终有声音提醒，2：表示始终无声音提醒 3:不始终提醒，默认1\n
	*		type: 7 全局通知消息内容展现模式\n
	*         value: 1:通知详情，2：通知源，隐藏内容 3:完全隐藏，默认2\n
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void setPersonalData(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取个人设置项
	* @param[in] cb 传入接收结果回调  _1错误信息  _2返回的值
	*/
	static void getPersonalData(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getPersonalDataHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 获取全局勿扰模式
	* @param[in] cb 传入接收结果回调  _1错误信息 _2起始时间  _3结束时间  _4是否打开
	*/
	static void getGlobalNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getGlobalNoDisturbModeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置全局勿扰模式
	* @param[in] startTime  起始时间
	* @param[in] endTime 结束时间
	* @param[in] isOpen 是否打开
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void setGolbalNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 获取针对目标用户的勿扰模式
	* @param[in] targetId  用户ID
	* @param[in] cb 传入接收结果回调  _1错误信息  _2用户ID  _3设置的值:  1为接收提醒 2表示不提醒仅显示数字 3:为免打扰
	*/
	static void getUserNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getUserNoDisturbModeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief 设置针对目标用户的勿扰模式
	* @param[in] targetId  用户ID
	* @param[in] value 设置的值:  1为接收提醒 2表示不提醒仅显示数字 3:为免打扰, 默认1
	* @param[in] cb 传入接收结果回调  _1错误信息
	*/
	static void setUserNoDisturbMode(const v8::FunctionCallbackInfo<v8::Value> &args);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

