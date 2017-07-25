#ifndef NODE_SEARCH_SERVICE_
#define NODE_SEARCH_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class SearchService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit SearchService(SdkChannel *channel);
	~SearchService();

	//创建实例
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**********************请求**********************/
	/**
	* \brief 从网络进行查找
	* @param[in] key 传入关键字
	* @param[in] cb 传入接收结果回调  _1错误信息  _2搜索结果
	*/
	static void searchFromNet(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief 从本地数据库进行查找
	* @param[in] key 传入关键字
	* @param[in] cb 传入接收结果回调  _1错误信息  _2搜索结果
	*/
	static void searchFromLocal(const v8::FunctionCallbackInfo<v8::Value> &args);

	void searchHandler(void *data, int len, PersistentPtr cb);

    /**
    * \brief 全局查找消息
    * @param[in] key	   传入查找关键字
    * @param[in] msgProperty  传入查找的附加属性
    * @param[in] cb     传入接收结果回调  _1错误信息  _2消息搜索结果集合
    */
    static void searchGlobalMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
    void searchGMessageHandler(void *data, int len, PersistentPtr cb);

    /**
    * \brief 查找相应targetID的详细消息
    * @param[in] key	   传入查找关键字
    * @param[in] msgDetailProperty  传入查找的附加属性
    * @param[in] cb     传入接收结果回调  _1错误信息  _2消息搜索结果集合
    */
    static void searchDetailMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
    void searchDMessageHandler(void *data, int len, PersistentPtr cb);


	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

