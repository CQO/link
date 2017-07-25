#include <v8.h>
#include "client.h"
#include "imsdk/cmdExport.h"
#include "imsdk/imsdk_api_for_pc.h"

namespace service
{

using namespace v8;
using namespace imsdk;


void initAll(Local<Object> exports) 
{
	Client::init(exports);
}

NODE_MODULE(service, initAll)

} /*namespace service*/