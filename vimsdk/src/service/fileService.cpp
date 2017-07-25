#include "fileService.h"

namespace service
{

using namespace v8;
using namespace imsdk;
using std::string;

Persistent<Function> FileService::constructor;
FileService::FileService(SdkChannel *channel) :channel_(channel)
{
}

FileService::~FileService()
{
}

void FileService::init(Local<Object> exports)
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "FileService"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tp, "uploadAvatar", uploadAvatar);
	NODE_SET_PROTOTYPE_METHOD(tp, "uploadFile", uploadFile);
	NODE_SET_PROTOTYPE_METHOD(tp, "downloadFile", downloadFile);
	NODE_SET_PROTOTYPE_METHOD(tp, "uploadImage", uploadImage);
	NODE_SET_PROTOTYPE_METHOD(tp, "downloadImage", downloadImage);
	NODE_SET_PROTOTYPE_METHOD(tp, "decryptFile", decryptFile);

	constructor.Reset(isolate, tp->GetFunction());
}

void FileService::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		Local<External> channel = Local<External>::Cast(args[0]);
		FileService* client = new FileService((SdkChannel*)channel->Value());
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



/**********************请求**********************/


//上传头像
void FileService::uploadAvatar(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	FileService* fileService = ObjectWrap::Unwrap<FileService>(args.Holder());
	imsdk::req_setHeadImg *data = (imsdk::req_setHeadImg*)fileService->channel_->getCmdData(imsdk::logic_cmd_setHeadImg);
	if (data == nullptr) return;

	
	data->orgi_path = Helper::toCppString(args[0]);
	data->thumb_path = Helper::toCppString(args[1]);

	POST_REQUEST(logic_cmd_setHeadImg, data, args[2], &FileService::uploadAvatarHandler, fileService);
}

void FileService::uploadAvatarHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_setHeadImg, data, len);
	//数据转换
	Helper::toJsString(jsObject, "srcJson", rep->orgi_json.c_str());		//原图信息json
	Helper::toJsString(jsObject, "thumbJson", rep->thumb_json.c_str());     //缩略图信息json
	Helper::callJsCallback(isolate, cb, jsObject);
}

//上传文件
void FileService::uploadFile(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	FileService* fileService = ObjectWrap::Unwrap<FileService>(args.Holder());
	imsdk::req_sendFiles *data = (imsdk::req_sendFiles*)fileService->channel_->getCmdData(imsdk::logic_cmd_sendFiles);
	if (data == nullptr) return;

	Local<Object> fileProperty = Local<Object>::Cast(args[0]);
	Local<Value> message = fileProperty->Get(String::NewFromUtf8(isolate, "message"));
	data->message = Helper::toCppString(message);
	
	Local<Value> type = fileProperty->Get(String::NewFromUtf8(isolate, "type"));
	data->type = (int16)Helper::toCppNumber(type);

	Local<Value> targetId = fileProperty->Get(String::NewFromUtf8(isolate, "targetId"));
	data->targetID = (int64)Helper::toCppNumber(targetId);

	Local<Value> encryptKey = fileProperty->Get(String::NewFromUtf8(isolate, "encryptKey"));
	data->encryptKey = Helper::toCppString(encryptKey);

	Local<Value> isP2P = fileProperty->Get(String::NewFromUtf8(isolate, "isP2P"));
	data->isP2p = Helper::toCppBool(isP2P)?1:0;

	Local<Value> localId = fileProperty->Get(String::NewFromUtf8(isolate, "localId"));
	data->localID = (int64)Helper::toCppNumber(localId);
	if (data->localID == 0)
		data->localID = fileService->channel_->nextLocalId();

	imsdk::st_sendFile file;
	file.fileSign = (int32)time(NULL) + 1;
	Local<Value> localPath = fileProperty->Get(String::NewFromUtf8(isolate, "localPath"));
	file.localPath = Helper::toCppString(localPath);

	Local<Value> isResume = fileProperty->Get(String::NewFromUtf8(isolate, "isResume"));
	file.localPath = Helper::toCppBool(isResume);
	data->_vt.push_back(file);

	POST_REQUEST_WITH_PROCESS(logic_cmd_sendFiles, data, args[1], &FileService::uploadFileHandler, args[2], &FileService::progressValueHandler, fileService);
	args.GetReturnValue().Set(Number::New(isolate, (double)data->localID));
}

void FileService::uploadFileHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_sendFiles, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "targetId", (double)rep->targetID);		//创建的群ID
	if (!rep->jsonVt.empty())
	{
		Helper::toJsString(jsObject, "jsonInfo", rep->jsonVt[0].c_str());
	}
	Helper::callJsCallback(isolate, cb, jsObject);
}

//下载文件
void FileService::downloadFile(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	FileService* fileService = ObjectWrap::Unwrap<FileService>(args.Holder());
	imsdk::req_recvFiles *data = (imsdk::req_recvFiles*)fileService->channel_->getCmdData(imsdk::logic_cmd_recvFiles);
	if (data == nullptr) return;

	Local<Object> fileProperty = Local<Object>::Cast(args[0]);

	Local<Value> targetId = fileProperty->Get(String::NewFromUtf8(isolate, "targetId"));
	data->targetID = (int64)Helper::toCppNumber(targetId);

	Local<Value> messageId = fileProperty->Get(String::NewFromUtf8(isolate, "messageId"));
	data->msgID = (int64)Helper::toCppNumber(messageId);

	Local<Value> localPath = fileProperty->Get(String::NewFromUtf8(isolate, "localPath"));
	data->localPath = Helper::toCppString(localPath);

	Local<Value> remotePath = fileProperty->Get(String::NewFromUtf8(isolate, "remotePath"));
	data->remotePath = Helper::toCppString(remotePath);

	Local<Value> fromId = fileProperty->Get(String::NewFromUtf8(isolate, "fromId"));
	data->fromID = (int64)Helper::toCppNumber(fromId);

	Local<Value> fileName = fileProperty->Get(String::NewFromUtf8(isolate, "fileName"));
	data->fileName = Helper::toCppString(fileName);

	Local<Value> encryptKey = fileProperty->Get(String::NewFromUtf8(isolate, "encryptKey"));
	data->encryptKey = Helper::toCppString(encryptKey);

	Local<Value> isP2P = fileProperty->Get(String::NewFromUtf8(isolate, "isP2P"));
	data->bisP2P = Helper::toCppBool(isP2P);

	Local<Value> isResume = fileProperty->Get(String::NewFromUtf8(isolate, "isResume"));
	data->bisResume = Helper::toCppBool(isResume);

	Local<Value> localId = fileProperty->Get(String::NewFromUtf8(isolate, "localId"));
	data->localID = (int64)Helper::toCppNumber(localId);
	if (data->localID == 0)
		data->localID = fileService->channel_->nextLocalId();

	POST_REQUEST_WITH_PROCESS(logic_cmd_recvFiles, data, args[1], &FileService::downloadFileHandler, args[2], &FileService::progressValueHandler, fileService);

	args.GetReturnValue().Set(Number::New(isolate, (double)data->localID));
}

void FileService::downloadFileHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_recvFiles, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "targetId", (double)rep->targetID);		//下载文件的targetId
	Helper::toJsString(jsObject, "localPath", rep->localPath.c_str());		//下载的本地路径
	Helper::callJsCallback(isolate, cb, jsObject);
}

//上传图片
void FileService::uploadImage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	FileService* fileService = ObjectWrap::Unwrap<FileService>(args.Holder());
	req_sendImg *data = (req_sendImg*)fileService->channel_->getCmdData(logic_cmd_sendImg);
	if (data == nullptr) return;

	data->targetID = (int64)Helper::toCppNumber(args[0]);
	data->encryptKey = Helper::toCppString(args[1]);
	data->orgi_path = Helper::toCppString(args[2]);
	data->thumb_path = Helper::toCppString(args[3]);
	data->localID = (int64)Helper::toCppNumber(args[4]);

	POST_REQUEST_WITH_PROCESS(logic_cmd_sendImg, data, args[5], &FileService::uploadImageHandler, args[6], &FileService::progressValueHandler, fileService);
}

void FileService::uploadImageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_sendImg, data, len);
	//数据转换
	Helper::toJsNumber(jsObject, "targetId", (double)rep->targetID);		
	Helper::toJsString(jsObject, "srcJson", rep->orgi_json.c_str());
	Helper::toJsString(jsObject, "thumbJson", rep->thumb_Json.c_str());
	Helper::callJsCallback(isolate, cb, jsObject);
}

//下载图片
void FileService::downloadImage(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	FileService* fileService = ObjectWrap::Unwrap<FileService>(args.Holder());
	req_recvImg *data = (req_recvImg*)fileService->channel_->getCmdData(logic_cmd_recvImg);
	if (data == nullptr) return;

	//数据转换
	data->targetID = (int64)Helper::toCppNumber(args[0]);
	data->rPath = Helper::toCppString(args[1]);

	POST_REQUEST_WITH_PROCESS(logic_cmd_recvImg, data, args[2], &FileService::downloadImageHandler, args[3], &FileService::progressValueHandler, fileService);
}

void FileService::downloadImageHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(resp_recvImg, data, len);
	//数据转换
	Helper::toJsString(jsObject, "imgName", rep->imgName.c_str());	
	Helper::toJsNumber(jsObject, "targetId", (double)rep->targetID);
	Helper::callJsCallback(isolate, cb, jsObject);
}

// 解密
void FileService::decryptFile(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate* isolate = args.GetIsolate();
	FileService* fileService = ObjectWrap::Unwrap<FileService>(args.Holder());

	//数据转换
	std::string key = Helper::toCppString(args[0]);
	std::string srcPath = Helper::toCppString(args[1]);
	std::string destPath = Helper::toCppString(args[2]);
	bool ret = fileService->channel_->decryptFile(key.c_str(), srcPath.c_str(), destPath.c_str());
	args.GetReturnValue().Set(Boolean::New(isolate, ret));
}

void FileService::progressValueHandler(void *data, int len, PersistentPtr cb)
{
	resp_Progress *rep = (resp_Progress*)data;
	auto isolate = v8::Isolate::GetCurrent();					
	v8::HandleScope scope(isolate);								
	Local<Object> jsObject = Object::New(isolate);
	Helper::toJsNumber(jsObject, "extra", (double)rep->extra_req); //额外参数
	Helper::toJsNumber(jsObject, "progess", (double)rep->progess); //进度
	Helper::toJsString(jsObject, "info", rep->info.c_str());    //描述信息
	Helper::callJsCallback(isolate, cb, jsObject);
}

void FileService::commonHandler(void *data, int len, PersistentPtr cb)
{
	MAKE_DATA(_sdkrespcmd_base, data, 0);

	Helper::callJsCallback(isolate, cb, jsObject);
}

} /*namespace service*/

