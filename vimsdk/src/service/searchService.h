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

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**********************����**********************/
	/**
	* \brief ��������в���
	* @param[in] key ����ؼ���
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2�������
	*/
	static void searchFromNet(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief �ӱ������ݿ���в���
	* @param[in] key ����ؼ���
	* @param[in] cb ������ս���ص�  _1������Ϣ  _2�������
	*/
	static void searchFromLocal(const v8::FunctionCallbackInfo<v8::Value> &args);

	void searchHandler(void *data, int len, PersistentPtr cb);

    /**
    * \brief ȫ�ֲ�����Ϣ
    * @param[in] key	   ������ҹؼ���
    * @param[in] msgProperty  ������ҵĸ�������
    * @param[in] cb     ������ս���ص�  _1������Ϣ  _2��Ϣ�����������
    */
    static void searchGlobalMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
    void searchGMessageHandler(void *data, int len, PersistentPtr cb);

    /**
    * \brief ������ӦtargetID����ϸ��Ϣ
    * @param[in] key	   ������ҹؼ���
    * @param[in] msgDetailProperty  ������ҵĸ�������
    * @param[in] cb     ������ս���ص�  _1������Ϣ  _2��Ϣ�����������
    */
    static void searchDetailMessage(const v8::FunctionCallbackInfo<v8::Value> &args);
    void searchDMessageHandler(void *data, int len, PersistentPtr cb);


	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

