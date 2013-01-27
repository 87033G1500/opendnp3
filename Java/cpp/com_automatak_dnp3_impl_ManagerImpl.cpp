#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include <DNP3/DNP3Manager.h>
#include <APL/LogToStdio.h>

#include "JNIHelpers.h"

using namespace apl;
using namespace apl::dnp;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager
  (JNIEnv *, jobject, jint concurrency)
{
	auto pManager = new DNP3Manager(concurrency);
	pManager->AddLogSubscriber(LogToStdio::Inst());
	return (jlong) pManager;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_destroy_1native_1manager
  (JNIEnv *, jobject, jlong ptrManager)
{
	delete (DNP3Manager*) ptrManager;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1client
  (JNIEnv * pEnv, jobject, jlong ptrManager, jstring jloggerId, jobject logLevel, jlong timeoutMs, jstring jhost, jint port)
{
	auto pMgr = (DNP3Manager*) ptrManager;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	std::string host = JNIHelpers::GetString(jhost, pEnv);
	return (jlong) pMgr->AddTCPClient(loggerId, LEV_INFO, timeoutMs, host, port);
}


