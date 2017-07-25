#include "SdkChannel.h"
#include <time.h>
#include "utils/helper.h"

using namespace imsdk;
using namespace v8;
namespace service
{

SdkChannel::SdkChannel() : clientId_(0)
{
	nextId_ = time(NULL);
}

int SdkChannel::init(const std::string &rootPath, const std::string &certPath, std::shared_ptr<InitInfo> info)
{
	rootPath_ = rootPath;
	initInfo_ = info;

	std::string cert = certPath;
	setConfig(cfg_certificatePath, &cert);

	std::string appName = info->appName;
	setConfig(cfg_appName, &appName);

	clientId_ = initClient(rootPath.c_str(), this);

	if (clientId_ > 0)
	{
		async_.data = this;
		uv_async_init(uv_default_loop(), &async_, asyncCb);
	}

	return clientId_;
}

void SdkChannel::unInit()
{
	if (clientId_ > 0)
	{
		uv_close((uv_handle_t*)&async_, closeCb);
		uninitClient(clientId_);
		callbacks_.clear();
		notifies_.clear();
		headImgNotifies_.clear();
		backImgNotifies_.clear();
	}
		
	clientId_ = 0;
}

bool SdkChannel::getConfig(int type, void * pOut)
{
	return ::getConfig(clientId_, type, pOut);
}

bool SdkChannel::setConfig(int type, void * pIn)
{
	return ::setConfig(clientId_, type, pIn);
}

bool SdkChannel::decryptFile(const char * pKey, const char  * pSrcPath, const char * pDestPath)
{
	return ::decryptFile(pKey, pSrcPath, pDestPath);
}

void SdkChannel::loglog(enlogLvl lvl, enLogModel model, const char * tag, const char * fmt, ...)
{
	::loglog(clientId_, lvl, model, tag, fmt);
}

void* SdkChannel::getCmdData(uint16 msg, bool bisReq)
{
	_sdkcmd_base *data = (_sdkcmd_base*)::getCmdData(clientId_, msg, bisReq);
	data->localID = 0;
	return data;
}

int SdkChannel::postRequest(uint16_t type, void *pData, int len, ResponseCb respCb, ResponseCb processCb)
{
	_sdkcmd_base *req = (_sdkcmd_base*)pData;
	if (req->localID == 0)
	{
		req->localID = nextLocalId();
	}

	if (type == imsdk::logic_cmd_loginAuto)
		type = imsdk::logic_cmd_login;
	
	if (respCb != nullptr)
	{
		callbackItem item = { type, respCb, processCb };
		callbacks_[req->localID] = item;
	}
	
	return ::sendRequestEx(clientId_, type, pData, len);
}

void SdkChannel::regNotify(uint16_t type, ResponseCb cb)
{
	if (cb == nullptr)
		return;

	notifyItem item = { type, cb };

	if (type >= userHeadImgChange && type <= groupMemHeadImgChange)
		headImgNotifies_.push_back(item);
	else if(type >= userBackImgChange && type <= groupBackImgChange)
		backImgNotifies_.push_back(item);
	else
		notifies_[type] = item;
}

void SdkChannel::unRegNotify(uint16_t type)
{
	auto iter = notifies_.find(type);
	notifies_.erase(iter);
}

bool SdkChannel::onMsgcallBack(unsigned short type, void * pBuffer, int len)
{
	_sdkrespcmd_base *data = newResp(pBuffer, type);
	if (data == nullptr)
		return false;	

	ResponseData *resp = new ResponseData;
	resp->data = data;
	resp->len = len;
	resp->type = type;

	responseDatas_.push(resp);
	uv_async_send(&async_);

	return true;
}

void SdkChannel::asyncCb(uv_async_t* handle)
{
	SdkChannel* channel = (SdkChannel*)handle->data;
	Queue &responseDatas = channel->responseDatas_;

	void *data = nullptr;
	while ((data = responseDatas.tryPop()) != nullptr)
	{
		ResponseData *resp = (ResponseData*)data;
		_sdkrespcmd_base* respData = (_sdkrespcmd_base*)resp->data;

		/************查找注册的通知和请求***********/
		//首先在请求回调中查找
		auto iter = channel->callbacks_.find(respData->localID);
		if (iter != channel->callbacks_.end())
		{
			//调用回调函数
			if (resp->type == iter->second.msgType)
			{
				if (iter->second.respCb)
					iter->second.respCb(resp->data, resp->len);
				channel->callbacks_.erase(iter);
			}
			//调用进度回调
			else if (resp->type == 0 && iter->second.processCb != nullptr)
				iter->second.processCb(resp->data, resp->len);
		}
		//在注册的通知回调中查找
		else
		{
			auto iter1 = channel->notifies_.find(resp->type);
			if (iter1 != channel->notifies_.end())
			{
				iter1->second.respCb(resp->data, resp->len);
			}
		}
		delete resp;
	}
}

void SdkChannel::closeCb(uv_handle_t* handle)
{
	printf("close the async handle!\n");
}



/********************************内存分配**********************************/
#define NEW_RESP(T, data) {\
	T *resp = (T*)data;  \
	T *i = new T(*resp);		\
	return i; }

_sdkrespcmd_base *SdkChannel::newResp(void *data, unsigned short type)
{
	switch (type)
	{
	case logic_cmd_dbUpdateNotifyer:
	case logic_cmd_quickRegister:
	case logic_cmd_reg2:
	case logic_cmd_resetPw2:
	case logic_cmd_delReContact:
	case logic_cmd_delMessage:
	case logic_cmd_operateBuddy:
	case logic_cmd_addBuddyRequest:
	case logic_cmd_opuser:
	case logic_cmd_groupVerifyReq:
	case logic_cmd_groupDelete:
	case logic_cmd_groupTrans:
	case logic_cmd_setGroupSet:
	case logic_cmd_groupUpdateInfo:
	case logic_cmd_groupInviteReq:
	case logic_cmd_removeGroupMember:
	case logic_cmd_setGroupMemberInfo:
	case logic_cmd_setMyself:
	case logic_cmd_addOrDeleteApplication:
	case logic_cmd_setPersonalData:
		NEW_RESP(resp_commonResult, data)
	case logic_cmd_netNotifyer:
		NEW_RESP(resp_netNotifyer, data)
	case logic_cmd_login:
	case logic_cmd_loginAuto:
	case logic_cmd_offlineLogin:
		NEW_RESP(resp_LoginResultBean, data)
	case logic_cmd_upgrade:
		NEW_RESP(resp_upgrade, data)
	case logic_cmd_reg1:
	case logic_cmd_resetPw1:
		NEW_RESP(resp_regResult1, data)
	case logic_cmd_getPwRule:
		NEW_RESP(resp_pwRuleResult, data)
	case logic_cmd_verifyImg:
		NEW_RESP(resp_verifyImg, data)
	case logic_cmd_msgStore:
		NEW_RESP(resp_msgBean, data)
	case logic_cmd_unReadmsgList:
		NEW_RESP(resp_msgList, data)
	case logic_cmd_reContactList:
		NEW_RESP(resp_reContactList, data)
	case logic_cmd_sendMessage:
		NEW_RESP(resp_sendMessage, data)
	case logic_cmd_getHistoryMsg:
		NEW_RESP(resp_getHistoryMsg, data)
	case logic_cmd_present:
		NEW_RESP(resp_present, data)
	case logic_cmd_buddylist:
		NEW_RESP(resp_buddyList, data)
	case logic_cmd_getBuddyVerifyType:
		NEW_RESP(resp_getBuddyVerifyType, data)
	case logic_cmd_getBuddyOnline:
		NEW_RESP(resp_getBuddyOnlineResult, data)
	case logic_cmd_setHeadImg:
		NEW_RESP(resp_setHeadImg, data)
	case logic_cmd_sendFiles:
		NEW_RESP(resp_sendFiles, data)
	case logic_cmd_recvFiles:
		NEW_RESP(resp_recvFiles, data)
	case logic_cmd_sendImg:
		NEW_RESP(resp_sendImg, data)
	case logic_cmd_recvImg:
		NEW_RESP(resp_recvImg, data)
	case logic_cmd_progress:
		NEW_RESP(resp_Progress, data)
	case logic_cmd_groupRefresh:
		NEW_RESP(resp_groupNotify, data)
	case logic_cmd_transferGroup:
		NEW_RESP(resp_notifyTransGroup, data)
	case logic_cmd_groupMemberInfoRefresh:
		NEW_RESP(resp_groupMemberInfoRefresh, data)
	case logic_cmd_groupMembersRefresh:
		NEW_RESP(resp_groupMembersRefresh, data)
	case logic_cmd_getGroupMemList:
		NEW_RESP(resp_getGroupMemList, data)
	case logic_cmd_getGroups:
		NEW_RESP(resp_getGroupList, data)
	case logic_cmd_createGroup:
		NEW_RESP(resp_createGroup, data)
	case logic_cmd_getGroupSet:
		NEW_RESP(resp_getGroupSet, data)
	case logic_cmd_getGroupInfo:
		NEW_RESP(resp_getGroupInfo, data)
	case logic_cmd_getGroupMemberInfo:
		NEW_RESP(resp_getGroupMemberInfo, data)
	case logic_cmd_getMyself:
		NEW_RESP(resp_getMyself, data)
	case logic_cmd_operateUser:
		NEW_RESP(resp_operUser, data)
	case logic_cmd_localSetting:
		NEW_RESP(resp_localSetting, data)
	case logic_cmd_labelQueryEmoticonPackage:
		NEW_RESP(resp_labelQueryEmoticonPackage, data)
	case logic_cmd_queryEmoticon:
		NEW_RESP(resp_queryEmoticon, data)
	case logic_cmd_singleQueryEmoticonPackage:
		NEW_RESP(resp_singleQueryEmoticonPackage, data)
	case logic_cmd_pageQueryEmoticon:
		NEW_RESP(resp_pageQueryEmoticon, data)
	case logic_cmd_addOrDeleteCustomEmoticon:
		NEW_RESP(resp_addOrDeleteCustomEmoticon, data)
	case logic_cmd_querySingleEmoticon:
		NEW_RESP(resp_querySingleEmoticon, data)
	case logic_cmd_getAppInfo:
		NEW_RESP(resp_getAppInfo, data)
	case logic_cmd_queryMarketApplication:
		NEW_RESP(resp_queryMarketApplication, data)
	case logic_cmd_getInstalledApplication:
		NEW_RESP(resp_getInstalledApplication, data)
	case logic_cmd_getPersonalData:
		NEW_RESP(resp_PersonalData, data)
	case logic_cmd_simpleSearch:
		NEW_RESP(resp_SearchResult, data)
	case logic_cmd_msgGlobalSearch:
		NEW_RESP(resp_msgGlobalSearch, data)
	case logic_cmd_msgDetailSearch:
		NEW_RESP(resp_msgDetailSearch, data)
			
	default:
		return nullptr;
	}
}

} /*namespace service*/
