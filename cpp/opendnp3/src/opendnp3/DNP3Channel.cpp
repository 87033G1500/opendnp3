#include "DNP3Channel.h"

#ifndef OPENDNP3_NO_MASTER
#include "MasterStackImpl.h"
#endif

#include <openpal/IPhysicalLayerAsync.h>

#include "OutstationStackImpl.h"
#include "ExecutorPause.h"

using namespace openpal;

namespace opendnp3
{

DNP3Channel::DNP3Channel(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, IPhysicalLayerAsync* apPhys, std::function<void (DNP3Channel*)> aOnShutdown) :
	Loggable(aLogger),
	mpPhys(apPhys),
	mOnShutdown(aOnShutdown),
	mRouter(aLogger.GetSubLogger("Router"), mpPhys.get(), aOpenRetry)
#ifndef OPENDNP3_NO_MASTER
	, mGroup(apPhys->GetExecutor())
#endif
{

}

DNP3Channel::~DNP3Channel()
{
	this->Cleanup();
}

void DNP3Channel::Shutdown()
{
	this->Cleanup();
	mOnShutdown(this);
}

void DNP3Channel::AddStateListener(std::function<void (ChannelState)> aListener)
{
	mRouter.AddStateListener(aListener);
}

openpal::IExecutor* DNP3Channel::GetExecutor()
{
	return mpPhys->GetExecutor();
}

void DNP3Channel::Cleanup()
{
	std::set<IStack*> copy(mStacks);
	for(auto pStack: copy) pStack->Shutdown();
	{
		ExecutorPause p(mpPhys->GetExecutor());
#ifndef OPENDNP3_NO_MASTER
		this->mGroup.Shutdown();	// no more task callbacks
#endif
		this->mRouter.Shutdown();	// start shutting down the router
	}
	mRouter.WaitForShutdown();
}

#ifndef OPENDNP3_NO_MASTER
IMaster* DNP3Channel::AddMaster(const std::string& arLoggerId, LogLevel aLevel, IMeasurementHandler* apPublisher, IUTCTimeSource* apTimeSource, const MasterStackConfig& arCfg)
{
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	ExecutorPause p(mpPhys->GetExecutor());
	if(mRouter.IsRouteInUse(route)) {
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Route already in use: " << route.ToString());
	}
	else {
		auto logger = mLogger.GetSubLogger(arLoggerId, aLevel);		
		auto routeFunc = GetEnableDisableRoute(mpPhys->GetExecutor(), &mRouter, route);
		auto shutdownFunc = [this, route](IStack * apStack) { this->OnStackShutdown(apStack, route); };
		auto pMaster = new MasterStackImpl(logger, mpPhys->GetExecutor(), apPublisher, apTimeSource, &mGroup, arCfg, routeFunc, shutdownFunc);
		pMaster->SetLinkRouter(&mRouter);
		mStacks.insert(pMaster);
		mRouter.AddContext(pMaster->GetLinkContext(), route);		
		return pMaster;
	}
}
#endif

IOutstation* DNP3Channel::AddOutstation(const std::string& arLoggerId, LogLevel aLevel, ICommandHandler* apCmdHandler, ITimeWriteHandler* apTimeWriteHandler, const SlaveStackConfig& arCfg)
{
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	ExecutorPause p(mpPhys->GetExecutor());
	if(mRouter.IsRouteInUse(route)) {
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Route already in use: " << route.ToString());
	}
	else {
		auto logger = mLogger.GetSubLogger(arLoggerId, aLevel);
		auto routeFunc = GetEnableDisableRoute(mpPhys->GetExecutor(), &mRouter, route);
		auto shutdownFunc = [this, route](IStack * apStack) { this->OnStackShutdown(apStack, route); };
		auto pOutstation = new OutstationStackImpl(logger, mpPhys->GetExecutor(), apTimeWriteHandler, apCmdHandler, arCfg, routeFunc, shutdownFunc);
		pOutstation->SetLinkRouter(&mRouter);
		mStacks.insert(pOutstation);
		mRouter.AddContext(pOutstation->GetLinkContext(), route);		
		return pOutstation;
	}
}

std::function<void (bool)> DNP3Channel::GetEnableDisableRoute(IExecutor* apExecutor,  LinkLayerRouter* apRouter, LinkRoute aRoute)
{
	return [apExecutor, apRouter, aRoute](bool enable) {
		apExecutor->Post([=](){
			if(enable) apRouter->EnableRoute(aRoute);
			else apRouter->DisableRoute(aRoute);
		});
	};
}

void DNP3Channel::OnStackShutdown(IStack* apStack, LinkRoute route)
{
	mStacks.erase(apStack);
	{
		ExecutorPause p(mpPhys->GetExecutor());
		mRouter.RemoveContext(route);
	}
	delete apStack;
}

}

