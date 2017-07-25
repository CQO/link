#include "searchService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> SearchService::constructor;
extern void toJsMessage(Local<Object> &jsObject, const st_msgBean& msg);

SearchService::SearchService(SdkChannel *channel) :channel_(channel)
{
}

SearchService::~SearchService()
{
}

void SearchService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "SearchService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "searchFromNet", searchFromNet);
	NODE_SET_PROTOTYPE_METHOD(tp, "searchFromLocal", searchFromLocal);
    NODE_SET_PROTOTYPE_METHOD(tp, "searchGlobalMessage", searchGlobalMessage);
    NODE_SET_PROTOTYPE_METHOD(tp, "searchDetailMessage", searchDetailMessage);

	constructor.Reset(isolate, tp->GetFunction());
}

void SearchService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		SearchService* client = new SearchService((SdkChannel*)channel->Value());
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

void SearchService::searchFromNet(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	SearchService* searchService = ObjectWrap::Unwrap<SearchService>(args.Holder());
	req_simpleSearch *data = (req_simpleSearch*)searchService->channel_->getCmdData(logic_cmd_simpleSearch);
	if (data == nullptr) return;

	data->keyword = Helper::toCppString(args[0]);
	data->type = 0;

	POST_REQUEST(logic_cmd_simpleSearch, data, args[1], &SearchService::searchHandler, searchService);
}

void SearchService::searchFromLocal(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	SearchService* searchService = ObjectWrap::Unwrap<SearchService>(args.Holder());
    req_simpleSearch *data = (req_simpleSearch*)searchService->channel_->getCmdData(logic_cmd_simpleSearch);
	if (data == nullptr) return;

    data->keyword = Helper::toCppString(args[0]);
	data->type = 0x10;

	POST_REQUEST(logic_cmd_simpleSearch, data, args[1], &SearchService::searchHandler, searchService);
}

void SearchService::searchHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_SearchResult, data, len);
	//数据转换

	if (!rep->vtub.empty())
	{
		Local<Array> buddys = Array::New(isolate);
		int index = 0;
		for (auto &i : rep->vtub)
		{
			Local<Object> buddy = Object::New(isolate);
			Helper::toJsString(buddy, "name", i.name.c_str());
			Helper::toJsNumber(buddy, "userId", (double)i.userID);
			if (!i.oriPortraitURL.empty())
			{
				std::string srcAvatar = channel_->headImgPath() + i.oriPortraitURL;
				Helper::toJsString(buddy, "srcAvatar", srcAvatar.c_str());
			}
			if (!i.portraitURL.empty())
			{
				std::string avatar = channel_->headImgPath() + i.portraitURL;
				Helper::toJsString(buddy, "avatar", avatar.c_str());
			}
			buddys->Set(index++, buddy);
		}
		jsObject->Set(String::NewFromUtf8(isolate, "buddys"), buddys);
	}
	
	if (!rep->vtgroup.empty())
	{
		Local<Array> groups = Array::New(isolate);
		int index = 0;
		for (auto &i : rep->vtgroup)
		{
			Local<Object> group = Object::New(isolate);
			Helper::toJsString(group, "name", i.info.name.c_str());
			Helper::toJsNumber(group, "groupId", (double)i.info.groupID);
			Helper::toJsNumber(group, "level", (double)i.info.level);
			if (!i.info.icon.empty())
			{
				std::string icon = channel_->headImgPath() + i.info.icon;
				Helper::toJsString(group, "icon", icon.c_str());
			}
			groups->Set(index++, group);
		}
		jsObject->Set(String::NewFromUtf8(isolate, "buddys"), groups);
	}

	Helper::callJsCallback(isolate, cb, jsObject);
}

void SearchService::searchGlobalMessage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    SearchService* searchService = ObjectWrap::Unwrap<SearchService>(args.Holder());
	req_msgGlobalSearch *data = (req_msgGlobalSearch*)searchService->channel_->getCmdData(logic_cmd_msgGlobalSearch);
    if (data == nullptr) return;

    data->key = Helper::toCppString(args[0]);
    Local<Object> jsObj = Local<Object>::Cast(args[1]);

    Isolate *is = jsObj->GetIsolate();
    data->count = (int)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "count")));
    data->endTime = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "end_time")));
    data->msgType = (int16)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "msgtype")));
    data->targetId = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "targetid")));
    data->startTime = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "start_time")));

	POST_REQUEST(logic_cmd_msgGlobalSearch, data, args[2], &SearchService::searchGMessageHandler, searchService);
}
void SearchService::searchGMessageHandler(void *data, int len, PersistentPtr cb)
{
    MAKE_DATA(resp_msgGlobalSearch, data, len);
    std::string headpath(channel_->headImgPath());

    int index = 0;
    Local<Array> jsArray = Array::New(isolate);
    Helper::toJsNumber(jsObject, "nextTargetId", (double)rep->nextTargetId);
    for (auto &i : rep->results)
    {
        Local<Object> jsObj = Object::New(isolate);
        Helper::toJsNumber(jsObj, "count", (double)i.amount);
        Helper::toJsNumber(jsObj, "targetId", (double)i.targetID);
        Local<Object> jsSimpleObj = Object::New(isolate);
        Helper::toJsString(jsSimpleObj, "name", i.info.name.c_str());
        Helper::toJsNumber(jsSimpleObj, "flag", (double)i.info.targetFlag);
        if (!i.info.avatr.empty())
        {
            std::string avatr = headpath + i.info.avatr;
            //if (utils::isFileExist(avatr))
            Helper::toJsString(jsSimpleObj, "avatar", avatr.c_str());
        }
        Helper::toJsString(jsSimpleObj, "avatarUrl", i.info.avatrUrl.c_str());
        jsObj->Set(String::NewFromUtf8(isolate, "simpleInfo"), jsSimpleObj);
        jsArray->Set(index++, jsObj);
    }
    jsObject->Set(String::NewFromUtf8(isolate, "infos"), jsArray);
    Helper::callJsCallback(isolate, cb, jsObject);
}

void SearchService::searchDetailMessage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    SearchService* searchService = ObjectWrap::Unwrap<SearchService>(args.Holder());
	req_msgDetailSearch *data = (req_msgDetailSearch*)searchService->channel_->getCmdData(logic_cmd_msgDetailSearch);
    if (data == nullptr) return;

    data->key = Helper::toCppString(args[0]);
    Local<Object> jsObj = Local<Object>::Cast(args[1]);

    Isolate *is = jsObj->GetIsolate();
    data->count = (int)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "count")));
    data->endTime = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "end_time")));
    data->msgType = (int16)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "msgtype")));
    data->targetId = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "targetid")));
    data->startTime = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "start_time")));
    data->msgId = (int64)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(is, "messageId")));

	POST_REQUEST(logic_cmd_msgDetailSearch, data, args[1], &SearchService::searchDMessageHandler, searchService);
}
void SearchService::searchDMessageHandler(void *data, int len, PersistentPtr cb)
{
    MAKE_DATA(resp_msgDetailSearch, data, len);
   
    Helper::toJsNumber(jsObject, "targetId", (double)rep->targetId);
    Helper::toJsNumber(jsObject, "nextMsgId", (double)rep->nextMsgId);
    Helper::toJsString(jsObject, "key", rep->key.c_str());
  
    std::string headpath(channel_->headImgPath());
    Local<Object> senderInfos = Object::New(isolate);
    for (auto &i : rep->senderInfos)
    {
        Local<Object> jsObj = Object::New(isolate);
        Helper::toJsNumber(jsObj, "flag", (double)i.second.targetFlag);
        Helper::toJsString(jsObj, "name", i.second.name.c_str());
        if (!i.second.avatr.empty())
        {
            std::string avatr = headpath + i.second.avatr;
            Helper::toJsString(jsObj, "avatar", avatr.c_str());
        }
        //Helper::toJsString(jsObj, "avatarUrl", i.second.avatrUrl.c_str());
        std::string senderId = std::to_string(i.first);
        senderInfos->Set(String::NewFromUtf8(isolate, senderId.c_str()), jsObj);
    }
    jsObject->Set(String::NewFromUtf8(isolate, "senderInfos"), senderInfos);

    int index = 0;
    Local<Array> messageList = Array::New(isolate);
    for (auto &i : rep->msgs)
    {
        Local<Object> message = Object::New(isolate);
        service::toJsMessage(message, i);

        messageList->Set(index++, message);
    }
    jsObject->Set(String::NewFromUtf8(isolate, "msgs"), messageList);
   
    Helper::callJsCallback(isolate, cb, jsObject);
}

} /*namespace service*/

