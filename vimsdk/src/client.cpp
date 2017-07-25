#include "client.h"
#include "sdkChannel.h"
#include "utils/helper.h"
#include "service/authService.h"
#include "service/groupService.h"
#include "service/contactService.h"
#include "service/configService.h"
#include "service/chatService.h"
#include "service/fileService.h"
#include "service/searchService.h"
#include "service/userService.h"

namespace service
{

using namespace v8;
using namespace node;

Persistent<Function> Client::constructor;
Client::Client() 
{
	channel_ = new SdkChannel;
}

Client::~Client() 
{
	delete channel_;
}

void Client::init(Local<Object> exports) 
{
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tp = FunctionTemplate::New(isolate, instance);
	tp->SetClassName(String::NewFromUtf8(isolate, "client"));
	tp->InstanceTemplate()->SetInternalFieldCount(1);


	NODE_SET_PROTOTYPE_METHOD(tp, "startup", startup);
	NODE_SET_PROTOTYPE_METHOD(tp, "shutdown", shutdown);

	AuthService::init(exports);
	GroupService::init(exports);
	ContactService::init(exports);
	ChatService::init(exports);
	FileService::init(exports);
	UserService::init(exports);
	SearchService::init(exports);
	ConfigService::init(exports);

	NODE_SET_PROTOTYPE_METHOD(tp, "authService", authService);
	NODE_SET_PROTOTYPE_METHOD(tp, "groupService", groupService);
	NODE_SET_PROTOTYPE_METHOD(tp, "contactService", contactService);
	NODE_SET_PROTOTYPE_METHOD(tp, "chatService", chatService);
	NODE_SET_PROTOTYPE_METHOD(tp, "fileService", fileService);
	NODE_SET_PROTOTYPE_METHOD(tp, "userService", userService);
	NODE_SET_PROTOTYPE_METHOD(tp, "searchService", searchService);
	NODE_SET_PROTOTYPE_METHOD(tp, "configService", configService);

	exports->Set(String::NewFromUtf8(isolate, "client"), tp->GetFunction());
	constructor.Reset(isolate, tp->GetFunction());
}

void Client::instance(const FunctionCallbackInfo<Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) 
	{
		// Invoked as constructor: `new MyObject(...)`
		double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
		Client* client = new Client;
		client->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	else 
	{
		// Invoked as plain function `MyObject(...)`, turn into construct call.
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void Client::startup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	Client* client = ObjectWrap::Unwrap<Client>(args.Holder());

	std::string rootPath = Helper::toCppString(args[0]);
	std::string certificatePath = Helper::toCppString(args[1]);
	int ret = 0;
	if (!args[2]->IsObject() || args[2]->IsUndefined())
		ret = -1;

	Local<Object> jsObj = Local<Object> ::Cast(args[2]);
	auto initInfo = std::make_shared<InitInfo>();
	initInfo->version = Helper::toCppString(jsObj->Get(String::NewFromUtf8(isolate, "version")));
	initInfo->deviceType = (int)Helper::toCppNumber(jsObj->Get(String::NewFromUtf8(isolate, "deviceType")));
	initInfo->deviceInfo = Helper::toCppString(jsObj->Get(String::NewFromUtf8(isolate, "deviceInfo")));
	initInfo->appName = Helper::toCppString(jsObj->Get(String::NewFromUtf8(isolate, "appName")));
	initInfo->netType = Helper::toCppString(jsObj->Get(String::NewFromUtf8(isolate, "netType")));
	initInfo->mac = Helper::toCppString(jsObj->Get(String::NewFromUtf8(isolate, "mac")));
	initInfo->local = Helper::toCppString(jsObj->Get(String::NewFromUtf8(isolate, "local")));
	ret = client->channel_->init(rootPath, certificatePath, initInfo);

	args.GetReturnValue().Set(Number::New(isolate, ret));
}

void Client::shutdown(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Isolate* isolate = args.GetIsolate();

	Client* client = ObjectWrap::Unwrap<Client>(args.Holder());
	client->channel_->unInit();
}

void Client::authService(const FunctionCallbackInfo<Value> &args) 
{
	Helper::buildService(args, AuthService::constructor);
}

void Client::groupService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, GroupService::constructor);
}

void Client::contactService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, ContactService::constructor);
}

void Client::chatService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, ChatService::constructor);
}

void Client::fileService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, FileService::constructor);
}

void Client::userService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, UserService::constructor);
}

void Client::searchService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, SearchService::constructor);
}

void Client::configService(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	Helper::buildService(args, ConfigService::constructor);
}

} /*namespace service*/

