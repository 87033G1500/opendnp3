
#ifndef OPENDNP3CLR_CALLBACK_ADAPTER_H
#define OPENDNP3CLR_CALLBACK_ADAPTER_H

using namespace System::Collections::ObjectModel;
using namespace System::Threading::Tasks;

#include <openpal/util/Uncopyable.h>

#include <opendnp3/master/ICommandCallback.h>
#include <secauth/master/BeginUpdateKeyChangeCallbackT.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{						
			class CallbackAdapters : private openpal::StaticOnly
			{

				public:
													
				static opendnp3::CommandCallbackT Get(TaskCompletionSource<CommandResponse>^ tcs);

				static secauth::BeginUpdateKeyChangeCallbackT Get(TaskCompletionSource<BeginUpdateKeyChangeResult^>^ tcs);

			};			

		}
	}
}

#endif
