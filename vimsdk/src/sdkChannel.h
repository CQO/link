#ifndef NODE_SDK_CORE_
#define NODE_SDK_CORE_
/*
*  SdkChannel.h
*  desc: 封装SDKAPI调用
*  Created on: 2016年7月27日
*  Author: huhong
*/

#include <functional>
#include <memory>
#include <stdint.h>
#include <unordered_map>
#include <v8.h>
#include <uv.h>
#include "imsdk/imsdk_api_for_pc.h"
#include "imsdk/interfaceExport.h"
#include "imsdk/cmdExport.h"
#include "utils/queue.h"

namespace service
{

using PersistentPtr = std::shared_ptr<v8::Global<v8::Function> >;
struct InitInfo
{
	std::string version;		//应用版本号，如"1.5.0"  *必须设置
	int deviceType;				//设备类型 1：PC；2：手机；3：pad；4：网页  *必须设置
	std::string deviceInfo;		//设备信息 如"pc-windows"  *必须设置
	std::string appName;		//应用信息 需与应用名保持一致 *必须设置
	std::string netType;		//网络信息 如"wifi"  可选
	std::string mac;			//mac地址  windows,mac,linux平台会自动获取，其它平台需手动设置
	std::string local;			//地区
};

enum ImgNotifyType
{
	userHeadImgChange = 1000,
	chatHeadImgChange,
	contactHeadImgChange,
	groupHeadImgChange,
	groupMemHeadImgChange,
	userBackImgChange,
	contactBackImgChange,
	groupBackImgChange
};

class SdkChannel : public imMsgCallBack
{
public:
	using  ResponseCb = std::function<void(void* data, int len)>;

	SdkChannel();
	SdkChannel(const SdkChannel&) = delete;
	SdkChannel& operator = (const SdkChannel&) = delete;

	//初始化客户端
	int init(const std::string &root_path, const std::string &certPath, std::shared_ptr<InitInfo> info);
	//反初始化客户端
    void unInit();

	//生成唯一localId
	int64 nextLocalId() { return ++nextId_; }

	//获取用户数据目录
	std::string rootPath() { return rootPath_; }

	//获取初始化信息
	std::shared_ptr<InitInfo> getInitInfo()
	{
		return initInfo_;
	}

	//解密文件
	bool decryptFile(const char * pKey, const char  * pSrcPath, const char * pDestPath);

	//打印日志
	void loglog(enlogLvl lvl, enLogModel model, const char * tag, const char * fmt, ...);

	//获取请求或响应结构的内存
	void *getCmdData(uint16_t msg, bool bisReq = true);

	//向SDK发送异步请求
	int postRequest(unsigned short msg, void *pData, int len, ResponseCb respCb = nullptr, ResponseCb processCb = nullptr);

	//获取配置
	bool getConfig(int type, void * pOut);

	//设置配置
	bool setConfig(int type, void * pIn);

    //实现sdk::imMsgCallBack，sdk有数据返回时，会调用此函数
	virtual bool onMsgcallBack(uint16_t type, void * pBuffer, int len);

	//注册通知事件
	void regNotify(uint16_t type, ResponseCb cb);

	//反注册通知事件
	void unRegNotify(uint16_t type);

	//获取本地头像路径
	std::string headImgPath()
	{ 
		if (headImgPath_.empty())
			getConfig(cfg_headImgpath, &headImgPath_);
		return headImgPath_; 
	}

	//获取缓存
	ICacheMgr* getCacheMgr()
	{
		ICacheMgr *cache = nullptr;
		getConfig(cfg_getCache, &cache);
		return cache;
	}

	//获取clientId
	int64_t clientId() { return clientId_; }
private:
	/*
	* 此方法会在node主线程调用，以读取responseDatas_中的所有数据
	* 注：uv_async_t有可能连续发送多次通知，而此回调函数只调用一次，
	* 因此每次调用时，需要读取responseDatas_里的所有数据。
	*/
	static void asyncCb(uv_async_t* handle);
	static void closeCb(uv_handle_t* handle);		//关闭uv_async_t时调用

	//申请保存返回的resp结构的空间
	imsdk::_sdkrespcmd_base *newResp(void *data, unsigned short type);

	/*
	*   服务器返回数据不在node主线程，需要封装成此结构放入队列中，
	*	然后通过uv_async_t通知node主线程取得此数据。
	*/
	struct ResponseData
	{
		~ResponseData()
		{
			delete data;
		}

		unsigned short type;
		int len;
		imsdk::_sdkrespcmd_base *data;
	};

	//存放请求的回调数据
	struct callbackItem
	{
		uint16_t msgType;
		ResponseCb respCb;
		ResponseCb processCb;
	};

	//存放推送的回调数据
	struct notifyItem
	{
		uint16_t msgType;
		ResponseCb respCb;
	};

	std::shared_ptr<InitInfo> initInfo_;
	std::string rootPath_;
	int clientId_;
	int64_t nextId_;

	std::unordered_map<int64_t, callbackItem > callbacks_;	//存放请求回调
	std::unordered_map<uint16_t, notifyItem > notifies_;    //存放推送回调
	std::vector<notifyItem> headImgNotifies_;			   //存放头像推送的回调
	std::vector<notifyItem> backImgNotifies_;			   //存放背景推送的回调

	Queue responseDatas_;
	uv_async_t async_;

	std::string headImgPath_;
};

} /*namespace service*/

#endif