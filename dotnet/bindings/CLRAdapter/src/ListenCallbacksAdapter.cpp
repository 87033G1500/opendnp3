
#include "ListenCallbacksAdapter.h"

#include "Conversions.h"
#include "SessionAcceptorAdapter.h"
#include "MasterSessionAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {

	ListenCallbacksAdapter::ListenCallbacksAdapter(Interface::IListenCallbacks^ proxy) : 
		m_proxy(proxy)		
	{}

	bool ListenCallbacksAdapter::AcceptConnection(uint64_t sessionid, const std::string& ipaddress)
	{		
		return m_proxy->AcceptConnection(sessionid, Conversions::ConvertString(ipaddress));
	}

	openpal::TimeDuration ListenCallbacksAdapter::GetFirstFrameTimeout()
	{
		return Conversions::ConvertTimespan(m_proxy->GetFirstFrameTimeout());
	}

	void ListenCallbacksAdapter::OnFirstFrame(uint64_t sessionid, const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor)
	{
		auto linkheader = Conversions::Convert(header);
		auto adapter = gcnew SessionAcceptorAdapter(acceptor);
		m_proxy->OnFirstFrame(sessionid, linkheader, adapter);
	}

	void ListenCallbacksAdapter::OnSessionClose(uint64_t sessionid, std::shared_ptr<asiodnp3::IMasterSession> session)
	{		
		auto stats = Conversions::ConvertStackStats(session->GetStackStatistics());
		m_proxy->OnSessionClose(sessionid, stats);	
	}

}}}


