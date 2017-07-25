#ifndef NODE_SERVICE_CLIENT_
#define NODE_SERVICE_CLIENT_

#include <node.h>
#include <node_object_wrap.h>

namespace service
{

class SdkChannel;

class Client : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);

	SdkChannel *channel() { return channel_; }

private:
	Client();
	~Client();

	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	static void startup(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void shutdown(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void authService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void groupService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void contactService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void chatService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void fileService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void userService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void searchService(const v8::FunctionCallbackInfo<v8::Value> &args);
	static void configService(const v8::FunctionCallbackInfo<v8::Value> &args);

	static v8::Persistent<v8::Function> constructor;
	SdkChannel *channel_;
};

} /*namespace service*/

#endif

