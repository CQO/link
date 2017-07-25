#ifndef HELPER_H_
#define HELPER_H_

#include <v8.h>
#include <memory>
#include <node_object_wrap.h>
#include "../client.h"

namespace service
{
class Client;
}

//注册回调
#define REG_NOTIFY(msgType, cb, cbHandler, objPtr)      \
	PersistentPtr pCb = Helper::persistentFunc(cb);		\
	auto bFunc = std::bind(cbHandler, objPtr, std::placeholders::_1, std::placeholders::_2, pCb); \
	objPtr->channel_->regNotify(msgType, bFunc)

//绑定请求回调并发送消息
#define POST_REQUEST(msgType, data, cb, cbHandler, objPtr) \
	PersistentPtr pCb = Helper::persistentFunc(cb);		  \
	auto bFunc = std::bind(cbHandler, objPtr, std::placeholders::_1, std::placeholders::_2, pCb); \
	objPtr->channel_->postRequest(msgType, data, sizeof(*data), bFunc)

//绑定请求回调和进度回调并发送消息
#define POST_REQUEST_WITH_PROCESS(msgType, data, cb, cbHandler, processCb, processCbHandler, objPtr) \
	PersistentPtr pCb = Helper::persistentFunc(cb);		  \
	auto bFunc = std::bind(cbHandler, objPtr, std::placeholders::_1, std::placeholders::_2, pCb); \
	PersistentPtr pProcessCb = Helper::persistentFunc(processCb);			\
	auto processFunc = std::bind(processCbHandler, objPtr, std::placeholders::_1, std::placeholders::_2, pProcessCb); \
	objPtr->channel_->postRequest(msgType, data, sizeof(*data), bFunc, processFunc)

//生成回调数据 rep和jsObject  设置通用返回数据code len = 0为不进行数据校验
#define MAKE_DATA(respType, data, len)							\
	if (len != 0 && sizeof(respType) != len)					\
		printf("数据长度不一致\n");								\
	respType *rep = (respType*)data;							\
	auto isolate = v8::Isolate::GetCurrent();					\
	v8::HandleScope scope(isolate);								\
	Local<Object> jsObject = Object::New(isolate);				\
	Helper::toJsNumber(jsObject, "code", (double)rep->code)		

//数据转换帮助类
class Helper
{
public:
	/*************************通知处理函数*******************************/

	/********通过local<Function>获取persistent<Function>对象**********/
	static std::shared_ptr<v8::Global<v8::Function> > persistentFunc(v8::Local<v8::Value> local)
	{
		if (local->IsUndefined() || !local->IsFunction())
			return nullptr;

		v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(local);
		return std::make_shared< v8::Global<v8::Function> >(cb->GetIsolate(), cb);
	}

	/********把std::shared_ptr<v8::Global<v8::Function> >转换为local<Function>,并调用**********/
	static void callJsCallback(v8::Isolate *isolate, std::shared_ptr<v8::Global<v8::Function> > persistent, v8::Local<v8::Object> &jsObject)
	{
		if (persistent == nullptr)
			return;

		const int argc = 1;
		v8::Handle<v8::Value> argv[argc];
		argv[0] = jsObject;
		auto cb = v8::Local<v8::Function>::New(isolate, *persistent.get());
		cb->Call(Null(isolate), argc, argv);
	}

	/********生成javascript service对象实例，作为返回值返回给js调用者**********/
	template<typename T>
	static void buildService(const v8::FunctionCallbackInfo<v8::Value> &args, T &constructor)
	{
		v8::Isolate* isolate = args.GetIsolate();

		service::Client* client = node::ObjectWrap::Unwrap<service::Client>(args.Holder());

		const unsigned argc = 1;
		v8::Local<v8::Value> argv[argc] = { v8::External::New(isolate, client->channel()) };

		v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();

		args.GetReturnValue().Set(instance);
	}

	/*********************把c++数据转换为相应的js类型*******************************/
	static void toJsBool(v8::Local<v8::Object> &jsObj, const char *key, bool value)
	{
		v8::Isolate *isolate = jsObj->GetIsolate();
		jsObj->Set(v8::String::NewFromUtf8(isolate, key), v8::Boolean::New(isolate, value));
	}

	static void toJsNumber(v8::Local<v8::Object> &jsObj, const char *key, double value)
	{
		v8::Isolate *isolate = jsObj->GetIsolate();
		jsObj->Set(v8::String::NewFromUtf8(isolate, key), v8::Number::New(isolate, value));
	}

	static void toJsString(v8::Local<v8::Object> &jsObj, const char *key, const char *value)
	{
		v8::Isolate *isolate = jsObj->GetIsolate();
		jsObj->Set(v8::String::NewFromUtf8(isolate, key), v8::String::NewFromUtf8(isolate, value));
	}

	static void toJsStrArray(v8::Local<v8::Object> &jsObj, const char *key, const std::vector<std::string> &vec)
	{
		v8::Isolate *isolate = jsObj->GetIsolate();
		v8::Local<v8::Array> arr = v8::Array::New(isolate);
		for (uint32_t i = 0; i < vec.size(); ++i)
		{
			arr->Set(i, v8::String::NewFromUtf8(isolate, vec[i].c_str()));
		}
		jsObj->Set(v8::String::NewFromUtf8(isolate, key), arr);
	}

	template<typename T>
	static void toJsNumVector(v8::Local<v8::Object> &jsObj, const char *key, const std::vector<T> &vec)
	{
		v8::Isolate *isolate = jsObj->GetIsolate();
		v8::Local<v8::Array> arr = v8::Array::New(isolate);
		for (uint32_t i = 0; i < vec.size(); ++i)
		{
			arr->Set(i, v8::Number::New(isolate, (double)vec[i]));
		}
		jsObj->Set(v8::String::NewFromUtf8(isolate, key), arr);
	}
	
	/*********************把js数据转换为相应的c++类型*******************************/


	static std::string toCppString(const v8::Local<v8::Value> &jsValue)
	{
		if (jsValue->IsUndefined() || !jsValue->IsString())
			return "";

		v8::Local<v8::String> lStr = v8::Local<v8::String>::Cast(jsValue);
		v8::String::Utf8Value utfValue(lStr);
		return std::string(*utfValue);
	}

	static bool toCppBool(const v8::Local<v8::Value> &jsValue)
	{
		if (jsValue->IsUndefined() || !jsValue->IsBoolean())
			return false;

		v8::Local<v8::Boolean> b = v8::Local<v8::Boolean>::Cast(jsValue);
		return b->BooleanValue();
	}

	static double toCppNumber(const v8::Local<v8::Value> &jsValue)
	{
		if (jsValue->IsUndefined() || !jsValue->IsNumber())
			return 0;

		v8::Local<v8::Number> jsNum = v8::Local<v8::Number>::Cast(jsValue);
		return jsNum->NumberValue();
	}

	static std::string toCppStringFromObj(const v8::Local<v8::Object> &obj, const char *key)
	{
		v8::Isolate *isolate = obj->GetIsolate();
		auto value = obj->Get(v8::String::NewFromUtf8(isolate, key));
		return toCppString(value);
	}

	static double toCppNumberFromObj(const v8::Local<v8::Object> &obj, const char *key)
	{
		v8::Isolate *isolate = obj->GetIsolate();
		auto value = obj->Get(v8::String::NewFromUtf8(isolate, key));
		return toCppNumber(value);
	}

	static bool toCppBooleanFromObj(const v8::Local<v8::Object> &obj, const char *key)
	{
		v8::Isolate *isolate = obj->GetIsolate();
		auto value = obj->Get(v8::String::NewFromUtf8(isolate, key));
		return toCppBool(value);
	}

	template<typename T>
	static bool toCppNumVector(std::vector<T> &vec, const v8::Local<v8::Value> &jsValue)
	{
		if (jsValue->IsUndefined() || !jsValue->IsArray())
			return false;

		v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(jsValue);

		for (uint32_t i = 0; i < jsArray->Length(); ++i)
		{
			vec.push_back((T)jsArray->Get(i)->NumberValue());
		}
		return true;
	}

	static std::vector<std::string> toCppStrVector(const v8::Local<v8::Value> &jsValue)
	{
		std::vector<std::string> vec;
		if (jsValue->IsUndefined() || !jsValue->IsArray())
			return vec;

		v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(jsValue);

		for (uint32_t i = 0; i < jsArray->Length(); ++i)
		{
			std::string str = toCppString(jsArray->Get(i));
			vec.push_back(std::move(str));
		}
		return vec;
	}
};

#endif