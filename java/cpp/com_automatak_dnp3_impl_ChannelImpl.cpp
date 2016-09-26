
#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include <asiodnp3/IChannel.h>

#include "adapters/ConfigReader.h"
#include "adapters/SOEHandlerAdapter.h"

#include <memory>

using namespace asiodnp3;
using namespace opendnp3;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
(JNIEnv* env, jobject, jlong native)
{
	const auto channel = (IChannel*) native;
	channel->Shutdown();
}


JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
(JNIEnv* env, jobject, jlong native, jstring id, jobject handler, jobject application, jobject jconfig)
{
	const auto channel = (IChannel*)native;

	auto config = ConfigReader::ConvertMasterStackConfig(env, jconfig);

	auto hadapter = std::make_shared<SOEHandlerAdapter>(handler);
	//auto appadapter


	return 0;
}

