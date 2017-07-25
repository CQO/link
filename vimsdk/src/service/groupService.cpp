#include "groupService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> GroupService::constructor;
GroupService::GroupService(SdkChannel *channel) :channel_(channel)
{
}

GroupService::~GroupService()
{
}

void GroupService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "GroupService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "regGroupHeadImgUpdateCb", regGroupHeadImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regMemberHeadImgUpdateCb", regMemberHeadImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGroupBackImgUpdateCb", regGroupBackImgUpdateCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGroupRefreshCb", regGroupRefreshCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regTransferGroupCb", regTransferGroupCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGrpMemInfoRefreshCb", regGrpMemInfoRefreshCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGrpMemRefreshCb", regGrpMemRefreshCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGetGrpMemListCb", regGetGrpMemListCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "regGroupListCb", regGroupListCb);
	NODE_SET_PROTOTYPE_METHOD(tp, "createGroup", createGroup);
	NODE_SET_PROTOTYPE_METHOD(tp, "addGroup", addGroup);
	NODE_SET_PROTOTYPE_METHOD(tp, "removeGroup", removeGroup);
	NODE_SET_PROTOTYPE_METHOD(tp, "transferGroup", transferGroup);
	NODE_SET_PROTOTYPE_METHOD(tp, "getGroupSet", getGroupSet);
	NODE_SET_PROTOTYPE_METHOD(tp, "setGroupSet", setGroupSet);
	NODE_SET_PROTOTYPE_METHOD(tp, "getGroupInfo", getGroupInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "setGroupInfo", setGroupInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "getGroupList", getGroupList);
	NODE_SET_PROTOTYPE_METHOD(tp, "getMemberList", getMemberList);
	NODE_SET_PROTOTYPE_METHOD(tp, "inviteMember", inviteMember);
	NODE_SET_PROTOTYPE_METHOD(tp, "removeMember", removeMember);
	NODE_SET_PROTOTYPE_METHOD(tp, "getMemberInfo", getMemberInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "setMemberInfo", setMemberInfo);
	NODE_SET_PROTOTYPE_METHOD(tp, "isInGroup", isInGroup);

	constructor.Reset(isolate, tp->GetFunction());
}

void GroupService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		GroupService* client = new GroupService((SdkChannel*)channel->Value());
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

//注册监听群头像更新
void GroupService::regGroupHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(groupHeadImgChange, args[0], &GroupService::imageHandler, groupService);
}

//注册群成员头像更新
void GroupService::regMemberHeadImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(groupMemHeadImgChange, args[0], &GroupService::imageHandler, groupService);
}

//注册群背景更新
void GroupService::regGroupBackImgUpdateCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(groupBackImgChange, args[0], &GroupService::imageHandler, groupService);
}

void GroupService::imageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_UpdateHeadImg, data, len);
}

//注册群信息更新的回调
void GroupService::regGroupRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(logic_cmd_groupRefresh, args[0], &GroupService::groupRefreshHandler, groupService);
}


void GroupService::groupRefreshHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_groupNotify, data, len);
	//数据转换
	Local<Object> tinyGroup = Object::New(isolate);
	Helper::toJsNumber(tinyGroup, "groupId", (double)rep->group.info.groupID);
	Helper::toJsNumber(tinyGroup, "createrId", (double)rep->group.info.createrID);
	Helper::toJsString(tinyGroup, "groupName", rep->group.info.name.c_str());
	Helper::toJsString(tinyGroup, "pinyin", rep->group.info.pinyin.c_str());
	Helper::toJsNumber(tinyGroup, "level", rep->group.info.level & 0xF);
	Helper::toJsBool(tinyGroup, "cantExit", ((rep->group.info.level >> 16) & 1) == 0);
	Helper::toJsBool(tinyGroup, "isVSign", ((rep->group.info.level >> 6) & 1) == 1);
	Helper::toJsBool(tinyGroup, "isTop", ((rep->group.info.level >> 8) & 1) == 1);
	if (rep->group.info.icon.empty())
	{
		std::string avatar = channel_->headImgPath() + rep->group.info.icon;
		Helper::toJsString(tinyGroup, "avatar", avatar.c_str());
	}
	jsObject->Set(v8::String::NewFromUtf8(isolate, "info"), tinyGroup);

	Helper::toJsNumber(jsObject, "createTime", (double)rep->group.info.createTime);
	Helper::toJsString(jsObject, "createTime", rep->group.brief.c_str());
	Helper::toJsString(jsObject, "bulletin", rep->group.bulletin.c_str());
	Helper::toJsString(jsObject, "extend", rep->group.extend.c_str());
	Helper::toJsNumber(jsObject, "isSearch", rep->group.isSearch == 1);

	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册群主更新的回调
void GroupService::regTransferGroupCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(logic_cmd_transferGroup, args[0], &GroupService::transferGroupHandler, groupService);
}

void GroupService::transferGroupHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_notifyTransGroup, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "newOwnerId", (double)rep->userID);
	Helper::toJsString(jsObject, "newOwnerName", rep->userName.c_str());			
	Helper::toJsNumber(jsObject, "groupId", (double)rep->groupID);
	Helper::toJsString(jsObject, "groupName", rep->groupName.c_str());		
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册群成员信息更新的回调
void GroupService::regGrpMemInfoRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(logic_cmd_groupMemberInfoRefresh, args[0], &GroupService::grpMemInfoRefreshHandler, groupService);
}

void GroupService::grpMemInfoRefreshHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_groupMemberInfoRefresh, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "groupId", (double)rep->groupID);
	Helper::toJsNumber(jsObject, "userId", (double)rep->info.userid);
	uint8 userType = (uint8)rep->info.userType;
	Helper::toJsNumber(jsObject, "memberType", userType & 0xF);
	Helper::toJsBool(jsObject, "isRobot", (userType & 0xF0) != 0);
	if (!rep->info.avatar.empty())
	{
		std::string avatar = channel_->headImgPath() + rep->info.avatar;
		Helper::toJsString(jsObject, "thumbAvatar", avatar.c_str());
	}
	Helper::toJsString(jsObject, "remark", rep->info.memberName.c_str());		
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册成员列表更新的回调
void GroupService::regGrpMemRefreshCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(logic_cmd_groupMembersRefresh, args[0], &GroupService::grpMemRefreshHandler, groupService);
}

void GroupService::grpMemRefreshHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_groupMembersRefresh, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->infos)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "avatar", avatar.c_str());
		}
		Helper::toJsString(jsObj, "remark", i.memberName.c_str());
		Helper::toJsNumber(jsObj, "sex", i.sex);
		Helper::toJsNumber(jsObj, "groupId", (double)rep->groupID);
		uint8 userType = (uint8)i.userType;
		Helper::toJsNumber(jsObj, "memberType", userType & 0xF);
		Helper::toJsBool(jsObj, "isRobot", (userType & 0xF0) != 0);
		Helper::toJsNumber(jsObj, "userId", (double)i.userid);
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "members"), jsArray);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册群成员列表的回调
void GroupService::regGetGrpMemListCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(logic_cmd_getGroupMemList, args[0], &GroupService::getGrpMemListHandler, groupService);
}

void GroupService::getGrpMemListHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getGroupMemList, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->_vt)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "thumbAvatar", avatar.c_str());
		}
		Helper::toJsString(jsObj, "remark", i.memberName.c_str());
		Helper::toJsNumber(jsObj, "sex", (double)i.sex);
		Helper::toJsNumber(jsObj, "groupId", (double)rep->groupID);
		uint8 userType = (uint8)i.userType;
		Helper::toJsNumber(jsObj, "memberType", userType & 0xF);
		Helper::toJsNumber(jsObj, "isRobot", (userType & 0xF0) != 0);
		Helper::toJsNumber(jsObj, "userId", (double)i.userid);
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "members"), jsArray);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//注册群列表刷新
void GroupService::regGroupListCb(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	REG_NOTIFY(logic_cmd_getGroups, args[0], &GroupService::groupListHandler, groupService);
}

void GroupService::groupListHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getGroupList, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->groups)
	{
		Local<Object> jsObj = Object::New(isolate);
		Helper::toJsNumber(jsObj, "createrId", (double)i.createrID);
		Helper::toJsBool(jsObj, "groupId", i.groupID);
		if (!i.icon.empty())
		{
			std::string icon = channel_->headImgPath() + i.icon;
			Helper::toJsString(jsObj, "icon", icon.c_str());
		}
		Helper::toJsNumber(jsObj, "level", i.level & 0xF);
		Helper::toJsNumber(jsObj, "canExit", ((i.level >> 16) & 1) == 0);
		Helper::toJsString(jsObj, "groupName", i.name.c_str());
		Helper::toJsString(jsObj, "pinyin", i.pinyin.c_str());
		Helper::toJsString(jsObj, "server", i.srvName.c_str());
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "groups"), jsArray);
	Helper::callJsCallback(isolate, cb, jsObject);
}

/**********************请求**********************/


//创建群
void GroupService::createGroup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	imsdk::req_createGroup *data = (imsdk::req_createGroup*)groupService->channel_->getCmdData(imsdk::logic_cmd_createGroup);
	if (data == nullptr) return;

	data->level = (int8)Helper::toCppNumber(args[0]);
	std::vector<int64> members;
	Helper::toCppNumVector(members, args[1]);
	data->inviteMembers = members;
	data->name = Helper::toCppString(args[2]);

	POST_REQUEST(logic_cmd_createGroup, data, args[3], &GroupService::commonHandler, groupService);
}

void GroupService::createGroupHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_createGroup, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "groupId", (double)rep->groupid);		//创建的群ID
	Helper::callJsCallback(isolate, cb, jsObject);
}

//添加群
void GroupService::addGroup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupVerifyRequest *data = (req_groupVerifyRequest*)groupService->channel_->getCmdData(logic_cmd_groupVerifyReq);
	if (data == nullptr) return;

	data->groupID = (int64)Helper::toCppNumber(args[0]);
	data->verifyInfo = Helper::toCppString(args[1]);

	POST_REQUEST(logic_cmd_groupVerifyReq, data, args[2], &GroupService::commonHandler, groupService);
}

//解散群
void GroupService::removeGroup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupDelete *data = (req_groupDelete*)groupService->channel_->getCmdData(logic_cmd_groupDelete);
	if (data == nullptr) return;

	//数据转换
	data->operType = (int8)Helper::toCppNumber(args[0]);
	data->groupid = (int64)Helper::toCppNumber(args[1]);

	POST_REQUEST(logic_cmd_groupDelete, data, args[2], &GroupService::commonHandler, groupService);
}

//转让群
void GroupService::transferGroup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupTrans *data = (req_groupTrans*)groupService->channel_->getCmdData(logic_cmd_groupTrans);
	if (data == nullptr) return;

	data->groupid = (int64)Helper::toCppNumber(args[0]);
	data->targetUserid = (int64)Helper::toCppNumber(args[1]);

	POST_REQUEST(logic_cmd_groupTrans, data, args[2], &GroupService::commonHandler, groupService);
}

// 获取群设置
void GroupService::getGroupSet(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupCommonData *data = (req_groupCommonData*)groupService->channel_->getCmdData(logic_cmd_getGroupSet);
	if (data == nullptr) return;

	//数据转换
	data->groupid = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getGroupSet, data, args[1], &GroupService::getGroupSetHandler, groupService);
}

void GroupService::getGroupSetHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getGroupSet, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "verifyType", (double)rep->gs.verifyType);		//验证方式
	Helper::toJsBool(jsObject, "isAllow", rep->gs.isAllow == 1);	//是否允许进群
	Helper::callJsCallback(isolate, cb, jsObject);
}

// 设置群设置
void GroupService::setGroupSet(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_setGroupSet *data = (req_setGroupSet*)groupService->channel_->getCmdData(logic_cmd_setGroupSet);
	if (data == nullptr) return;

	//数据转换
	data->groupID = (int64)Helper::toCppNumber(args[0]);
	data->gs.verifyType = (int8)Helper::toCppNumber(args[1]);
	int8 isAllow = Helper::toCppBool(args[2])? 1 : 2;
	data->gs.isAllow = isAllow;

	POST_REQUEST(logic_cmd_setGroupSet, data, args[3], &GroupService::commonHandler, groupService);
}

//获取群信息
void GroupService::getGroupInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupCommonData *data = (req_groupCommonData*)groupService->channel_->getCmdData(logic_cmd_getGroupInfo);
	if (data == nullptr) return;

	//数据转换
	data->groupid = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getGroupInfo, data, args[1], &GroupService::getGroupInfoHandler, groupService);
}

void GroupService::getGroupInfoHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getGroupInfo, data, len);
	//数据转换
	Local<Object> tinyGroup = Object::New(isolate);
	Helper::toJsNumber(tinyGroup, "groupId", (double)rep->group.info.groupID);
	Helper::toJsNumber(tinyGroup, "createrId", (double)rep->group.info.createrID);
	Helper::toJsString(tinyGroup, "groupName", rep->group.info.name.c_str());
	Helper::toJsString(tinyGroup, "pinyin", rep->group.info.pinyin.c_str());
	Helper::toJsNumber(tinyGroup, "level", rep->group.info.level & 0xF);
	Helper::toJsBool(tinyGroup, "cantExit", ((rep->group.info.level >> 16) & 1) == 0);
	Helper::toJsBool(tinyGroup, "isVSign", ((rep->group.info.level >> 6) & 1) == 1);
	Helper::toJsBool(tinyGroup, "isTop", ((rep->group.info.level >> 8) & 1) == 1);
	if (rep->group.info.icon.empty())
	{
		std::string avatar = channel_->headImgPath() + rep->group.info.icon;
		Helper::toJsString(tinyGroup, "avatar", avatar.c_str());
	}
	jsObject->Set(v8::String::NewFromUtf8(isolate, "info"), tinyGroup);

	Helper::toJsNumber(jsObject, "createTime", (double)rep->group.info.createTime);
	Helper::toJsString(jsObject, "createTime", rep->group.brief.c_str());
	Helper::toJsString(jsObject, "bulletin", rep->group.bulletin.c_str());
	Helper::toJsString(jsObject, "extend", rep->group.extend.c_str());
	Helper::toJsNumber(jsObject, "isSearch", rep->group.isSearch == 1);

	Helper::callJsCallback(isolate, cb, jsObject);
}

//设置群信息
void GroupService::setGroupInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupInfoUpdate *data = (req_groupInfoUpdate*)groupService->channel_->getCmdData(logic_cmd_groupUpdateInfo);
	if (data == nullptr) return;

	data->groupID = (int64)Helper::toCppNumber(args[0]);

	Local<Object> updateInfo = Local<Object>::Cast(args[1]);

	Local<Value> groupBrief = updateInfo->Get(String::NewFromUtf8(isolate, "groupBrief"));
	if (!groupBrief->IsUndefined() || !groupBrief->IsSet())
	{
		auto str = Helper::toCppString(groupBrief);
		data->_set_groupBrief(str);
	}

	Local<Value> groupBulletin = updateInfo->Get(String::NewFromUtf8(isolate, "groupBulletin"));
	if (!groupBulletin->IsUndefined() || !groupBulletin->IsSet())
	{
		auto str = Helper::toCppString(groupBulletin);
		data->_set_groupBulletin(str);
	}

	Local<Value> groupName = updateInfo->Get(String::NewFromUtf8(isolate, "groupName"));
	if (!groupName->IsUndefined() || !groupName->IsSet())
	{
		auto str = Helper::toCppString(groupName);
		data->_set_groupName(str);
	}	

	Local<Value> groupIcon = updateInfo->Get(String::NewFromUtf8(isolate, "groupIcon"));
	if (!groupIcon->IsUndefined() || !groupIcon->IsSet())
	{
		auto str = Helper::toCppString(groupIcon);
		data->_set_groupIcon(str);
	}	

	Local<Value> groupBackgroundURL = updateInfo->Get(String::NewFromUtf8(isolate, "groupBackgroundURL"));
	if (!groupBackgroundURL->IsUndefined() || !groupBackgroundURL->IsSet())
	{
		auto str = Helper::toCppString(groupBackgroundURL);
		data->_set_groupBackgroundURL(str);
	}		

	Local<Value> groupSearch = updateInfo->Get(String::NewFromUtf8(isolate, "groupSearch"));
	if (!groupSearch->IsUndefined() || !groupSearch->IsSet())
	{
		int8 search = Helper::toCppBool(groupSearch) ? 1 : 2;
		data->_set_groupSearch(search);
	}

	Local<Value> groupMessageContentMode = updateInfo->Get(String::NewFromUtf8(isolate, "groupMessageContentMode"));
	if (!groupMessageContentMode->IsUndefined())
	{
		int8 mode = (int8)Helper::toCppNumber(groupMessageContentMode);
		data->_set_groupMessageContentMode(mode);
	}

	Local<Value> extendProperties = updateInfo->Get(String::NewFromUtf8(isolate, "extendProperties"));
	if (!extendProperties->IsUndefined())
	{
		auto str = Helper::toCppString(extendProperties);
		data->_set_extendProperties(str);
	}

	POST_REQUEST(logic_cmd_groupUpdateInfo, data, args[2], &GroupService::commonHandler, groupService);
}

// 获取群列表 
void GroupService::getGroupList(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	ICacheMgr *cache = groupService->channel_->getCacheMgr();
	std::vector<st_tinyGroupBean> st_groups;
	cache->getCacheData(ICacheMgr::data_groups, &st_groups);

	Isolate *isolate = args.GetIsolate();
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : st_groups)
	{
		Local<Object> jsObj = Object::New(isolate);
		Helper::toJsNumber(jsObj, "createrId", (double)i.createrID);
		Helper::toJsBool(jsObj, "groupId", i.groupID);
		if (!i.icon.empty())
		{
			std::string icon = groupService->channel_->headImgPath() + i.icon;
			Helper::toJsString(jsObj, "icon", icon.c_str());
		}
		Helper::toJsNumber(jsObj, "level", i.level & 0xF);
		Helper::toJsNumber(jsObj, "canExit", ((i.level >> 16) & 1) == 0);
		Helper::toJsString(jsObj, "groupName", i.name.c_str());
		Helper::toJsString(jsObj, "pinyin", i.pinyin.c_str());
		Helper::toJsString(jsObj, "server", i.srvName.c_str());
		jsArray->Set(index++, jsObj);
	}
	Local<Object> jsObject = Object::New(isolate);
	jsObject->Set(String::NewFromUtf8(isolate, "members"), jsArray);
	PersistentPtr cb = Helper::persistentFunc(args[0]);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//获取群成员列表
void GroupService::getMemberList(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupCommonData *data = (req_groupCommonData*)groupService->channel_->getCmdData(logic_cmd_getGroupMemList);
	if (data == nullptr) return;

	//数据转换
	data->groupid = (int64)Helper::toCppNumber(args[0]);

	POST_REQUEST(logic_cmd_getGroupMemList, data, args[1], &GroupService::getMemberListHandler, groupService);
}

void GroupService::getMemberListHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getGroupMemList, data, len);
	//数据转换
	Local<Array> jsArray = Array::New(isolate);
	int index = 0;
	for (auto &i : rep->_vt)
	{
		Local<Object> jsObj = Object::New(isolate);
		if (!i.avatar.empty())
		{
			std::string avatar = channel_->headImgPath() + i.avatar;
			Helper::toJsString(jsObj, "avatar", avatar.c_str());
		}
		Helper::toJsString(jsObj, "remark", i.memberName.c_str());
		Helper::toJsNumber(jsObj, "sex", i.sex);
		Helper::toJsNumber(jsObj, "groupId", (double)rep->groupID);
		uint8 userType = (uint8)i.userType;
		Helper::toJsNumber(jsObj, "memberType", userType & 0xF);
		Helper::toJsBool(jsObj, "isRobot", (userType & 0xF0) != 0);
		Helper::toJsNumber(jsObj, "userId", (double)i.userid);
		jsArray->Set(index++, jsObj);
	}
	jsObject->Set(String::NewFromUtf8(isolate, "members"), jsArray);
	Helper::callJsCallback(isolate, cb, jsObject);
}

//邀请群成员
void GroupService::inviteMember(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupInviteRequest *data = (req_groupInviteRequest*)groupService->channel_->getCmdData(logic_cmd_groupInviteReq);
	if (data == nullptr) return;

	//数据转换
	data->groupID = (int64)Helper::toCppNumber(args[0]);
	std::vector<int64> users;
	Helper::toCppNumVector(users, args[1]);
	data->users = users;

	POST_REQUEST(logic_cmd_groupInviteReq, data, args[2], &GroupService::commonHandler, groupService);
}

//移除群成员
void GroupService::removeMember(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_removeGroupMember *data = (req_removeGroupMember*)groupService->channel_->getCmdData(logic_cmd_removeGroupMember);
	if (data == nullptr) return;

	//数据转换
	data->groupID = (int64)Helper::toCppNumber(args[0]);
	std::vector<int64> users;
	Helper::toCppNumVector(users, args[1]);
	data->rmIDs = users;

	POST_REQUEST(logic_cmd_removeGroupMember, data, args[2], &GroupService::commonHandler, groupService);
}

//获取群成员信息
void GroupService::getMemberInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_getMemberInfo *data = (req_getMemberInfo*)groupService->channel_->getCmdData(logic_cmd_getGroupMemberInfo);
	if (data == nullptr) return;

	//数据转换
	data->groupid = (int64)Helper::toCppNumber(args[0]);
	data->memberid = (int64)Helper::toCppNumber(args[1]);

	POST_REQUEST(logic_cmd_getGroupMemberInfo, data, args[2], &GroupService::getMemberInfoHandler, groupService);
}

void GroupService::getMemberInfoHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_getGroupMemberInfo, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "groupId", (double)rep->member.shield.groupid);
	Helper::toJsNumber(jsObject, "userId", (double)rep->member.info.userid);
	uint8 userType = (uint8)rep->member.info.userType;
	Helper::toJsNumber(jsObject, "memberType", userType & 0xF);
	Helper::toJsBool(jsObject, "isRobot", (userType & 0xF0) != 0);
	Helper::toJsString(jsObject, "remark", rep->member.info.memberName.c_str());
	Helper::toJsString(jsObject, "name", rep->member.name.c_str());
	if (!rep->member.info.avatar.empty())
	{
		std::string avatar = channel_->headImgPath() + rep->member.info.avatar;
		Helper::toJsString(jsObject, "avatar", avatar.c_str());
	}
	Helper::callJsCallback(isolate, cb, jsObject);
}

//设置群成员信息
void GroupService::setMemberInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_setGroupMemberInfo *data = (req_setGroupMemberInfo*)groupService->channel_->getCmdData(logic_cmd_setGroupMemberInfo);
	if (data == nullptr) return;

	//数据转换
	Local<Object> updateInfo = Local<Object>::Cast(args[0]);

	Local<Value> memberType = updateInfo->Get(String::NewFromUtf8(isolate, "memberType"));
	if (!memberType->IsUndefined()){
		int8 memType = (int8)Helper::toCppNumber(memberType);
		data->_set_memType(memType);
	}
	Local<Value> remark = updateInfo->Get(String::NewFromUtf8(isolate, "remark"));
	if (!remark->IsUndefined()){
		std::string name = Helper::toCppString(remark);
		data->_set_memberName(name);
	}

	POST_REQUEST(logic_cmd_setGroupMemberInfo, data, args[1], &GroupService::commonHandler, groupService);
}


void GroupService::isInGroup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}


//获取群文件列表
void GroupService::getGroupFileList(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	GroupService* groupService = ObjectWrap::Unwrap<GroupService>(args.Holder());
	req_groupInviteRequest *data = (req_groupInviteRequest*)groupService->channel_->getCmdData(logic_cmd_getFileList);
	if (data == nullptr) return;

	//数据转换
	data->groupID = (int64)Helper::toCppNumber(args[0]);
	std::vector<int64> users;
	Helper::toCppNumVector(users, args[1]);
	data->users = users;

	POST_REQUEST(logic_cmd_groupInviteReq, data, args[2], &GroupService::commonHandler, groupService);
}

void GroupService::getGroupFileListHandler(void *data, int len, PersistentPtr cb)
{

}

void GroupService::commonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(_sdkrespcmd_base, data, 0);

	Helper::callJsCallback(isolate, cb, jsObject);
}

} /*namespace service*/

