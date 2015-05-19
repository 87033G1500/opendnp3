
#include "Stdafx.h"
#include <msclr\marshal_cppstd.h>
#include "Conversions.h"

using namespace Automatak::DNP3::Interface;

#ifdef ERROR
#undef ERROR
#endif

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			std::string Conversions::ConvertString(System::String^ s)
			{
				return msclr::interop::marshal_as<std::string>(s);
			}

			System::String^ Conversions::ConvertString(const std::string& s)
			{
				return gcnew System::String(s.c_str());
			}

			ChannelState Conversions::ConvertChannelState(opendnp3::ChannelState aState)
			{
				return (ChannelState)aState;
			}

			IChannelStatistics^ Conversions::ConvertChannelStats(const opendnp3::LinkChannelStatistics& stats)
			{
				ChannelStatistics^ ret = gcnew ChannelStatistics();
				ret->numBytesRx = stats.numBytesRx;
				ret->numBytesTx = stats.numBytesTx;
				ret->numOpen = stats.numOpen;
				ret->numOpenFail = stats.numOpenFail;
				ret->numClose = stats.numClose;
				ret->numLinkFrameRx = stats.numLinkFrameRx;
				ret->numLinkFrameTx = stats.numLinkFrameTx;
				ret->numBadLinkFrameRx = stats.numBadLinkFrameRx;
				ret->numCrcError = stats.numCrcError;
				return ret;
			}

			IStackStatistics^ Conversions::ConvertStackStats(const opendnp3::StackStatistics& statistics)
			{
				StackStatistics^ ret = gcnew StackStatistics();
				ret->numTransportErrorRx = statistics.numTransportErrorRx;
				ret->numTransportRx = statistics.numTransportRx;
				ret->numTransportTx = statistics.numTransportTx;
				return ret;
			}

			CommandResponse Conversions::ConvertCommandResponse(opendnp3::CommandResponse response)
			{
				auto result = (TaskCompletion) response.GetResult();
				auto status = ConvertCommandStatus(response.GetStatus());
				return CommandResponse(result, status);
			}			

			CommandStatus Conversions::ConvertCommandStatus(opendnp3::CommandStatus status)
			{
				return (CommandStatus)status;
			}

			opendnp3::CommandStatus Conversions::ConvertCommandStatus(CommandStatus status)
			{
				return (opendnp3::CommandStatus) status;
			}

			ControlCode Conversions::ConvertControlCode(opendnp3::ControlCode code)
			{
				return (ControlCode)ControlCodeToType(code);
			}

			opendnp3::ControlCode Conversions::ConvertControlCode(ControlCode code)
			{
				return (opendnp3::ControlCode) code;
			}

			ControlRelayOutputBlock^ Conversions::ConvertCommand(const opendnp3::ControlRelayOutputBlock& bo)
			{
				return gcnew ControlRelayOutputBlock(ConvertControlCode(bo.functionCode), bo.count, bo.onTimeMS, bo.offTimeMS);
			}

			opendnp3::ControlRelayOutputBlock Conversions::ConvertCommand(ControlRelayOutputBlock^ bo)
			{
				return opendnp3::ControlRelayOutputBlock(ConvertControlCode(bo->code), bo->count, bo->onTime, bo->offTime);
			}

			opendnp3::AnalogOutputInt32 Conversions::ConvertCommand(AnalogOutputInt32^ sp)
			{
				return opendnp3::AnalogOutputInt32(sp->value);
			}

			AnalogOutputInt32^ Conversions::ConvertCommand(const opendnp3::AnalogOutputInt32& sp)
			{
				return gcnew AnalogOutputInt32(sp.value);
			}

			opendnp3::AnalogOutputInt16 Conversions::ConvertCommand(AnalogOutputInt16^ sp)
			{
				return opendnp3::AnalogOutputInt16(sp->value);
			}

			AnalogOutputInt16^ Conversions::ConvertCommand(const opendnp3::AnalogOutputInt16& sp)
			{
				return gcnew AnalogOutputInt16(sp.value);
			}

			opendnp3::AnalogOutputFloat32 Conversions::ConvertCommand(AnalogOutputFloat32^ sp)
			{
				return opendnp3::AnalogOutputFloat32(sp->value);
			}

			AnalogOutputFloat32^ Conversions::ConvertCommand(const opendnp3::AnalogOutputFloat32& sp)
			{
				return gcnew AnalogOutputFloat32(sp.value);
			}

			opendnp3::AnalogOutputDouble64 Conversions::ConvertCommand(AnalogOutputDouble64^ sp)
			{
				return opendnp3::AnalogOutputDouble64(sp->value);
			}

			AnalogOutputDouble64^ Conversions::ConvertCommand(const opendnp3::AnalogOutputDouble64& sp)
			{
				return gcnew AnalogOutputDouble64(sp.value);
			}

			Binary^ Conversions::ConvertMeas(opendnp3::Binary meas)
			{				
				return gcnew Binary(meas.value, meas.quality, TimeStamp::Convert(meas.time));				
			}

			DoubleBitBinary^ Conversions::ConvertMeas(opendnp3::DoubleBitBinary meas)
			{
				return gcnew DoubleBitBinary(static_cast<DoubleBit>(meas.value), meas.quality, TimeStamp::Convert(meas.time));				
			}

			Analog^ Conversions::ConvertMeas(opendnp3::Analog meas)
			{				
				return gcnew Analog(meas.value, meas.quality, TimeStamp::Convert(meas.time));				
			}

			Counter^ Conversions::ConvertMeas(opendnp3::Counter meas)
			{
				return gcnew Counter(meas.value, meas.quality, TimeStamp::Convert(meas.time));				
			}

			FrozenCounter^ Conversions::ConvertMeas(opendnp3::FrozenCounter meas)
			{
				return gcnew FrozenCounter(meas.value, meas.quality, TimeStamp::Convert(meas.time));				
			}

			AnalogOutputStatus^ Conversions::ConvertMeas(opendnp3::AnalogOutputStatus meas)
			{
				return gcnew AnalogOutputStatus(meas.value, meas.quality, TimeStamp::Convert(meas.time));				
			}

			BinaryOutputStatus^ Conversions::ConvertMeas(opendnp3::BinaryOutputStatus meas)
			{				
				return gcnew BinaryOutputStatus(meas.value, meas.quality, TimeStamp::Convert(meas.time));				
			}

			OctetString^ Conversions::ConvertMeas(const opendnp3::OctetString& arMeas)
			{
				auto buffer = arMeas.ToReadOnly();
				array<System::Byte>^ bytes = gcnew array<System::Byte>(buffer.Size());
				for (uint32_t i = 0; i < buffer.Size(); ++i) bytes[i] = buffer[i];
				return gcnew OctetString(bytes);
			}

			TimeAndInterval^ Conversions::ConvertMeas(const opendnp3::TimeAndInterval& meas)
			{
				return gcnew TimeAndInterval(meas.time, meas.interval, meas.units);
			}

			BinaryCommandEvent^ Conversions::ConvertMeas(const opendnp3::BinaryCommandEvent& meas)
			{
				return gcnew BinaryCommandEvent(meas.value, ConvertCommandStatus(meas.status), TimeStamp::Convert(meas.time));
			}

			AnalogCommandEvent^ Conversions::ConvertMeas(const opendnp3::AnalogCommandEvent& meas)
			{
				return gcnew AnalogCommandEvent(meas.value, ConvertCommandStatus(meas.status), TimeStamp::Convert(meas.time));
			}

			SecurityStat^ Conversions::ConvertMeas(const opendnp3::SecurityStat& meas)
			{
				return gcnew SecurityStat(meas.count, meas.assocId, meas.quality, TimeStamp::Convert(meas.time));
			}

			opendnp3::Binary Conversions::ConvertMeas(Binary^ meas)
			{
				return opendnp3::Binary(meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::DoubleBitBinary Conversions::ConvertMeas(DoubleBitBinary^ meas)
			{
				return opendnp3::DoubleBitBinary((opendnp3::DoubleBit) meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::Analog Conversions::ConvertMeas(Analog^ meas)
			{
				return opendnp3::Analog(meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::Counter Conversions::ConvertMeas(Counter^ meas)
			{
				return opendnp3::Counter(meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::FrozenCounter Conversions::ConvertMeas(FrozenCounter^ meas)
			{
				return opendnp3::FrozenCounter(meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::AnalogOutputStatus Conversions::ConvertMeas(AnalogOutputStatus^ meas)
			{
				return opendnp3::AnalogOutputStatus(meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::TimeAndInterval Conversions::ConvertMeas(TimeAndInterval^ meas)
			{
				return opendnp3::TimeAndInterval(opendnp3::DNPTime(meas->time), meas->interval, meas->units);
			}

			opendnp3::BinaryCommandEvent Conversions::ConvertMeas(BinaryCommandEvent^ meas)
			{
				return opendnp3::BinaryCommandEvent(meas->Value, ConvertCommandStatus(meas->Status), opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::AnalogCommandEvent Conversions::ConvertMeas(AnalogCommandEvent^ meas)
			{
				return opendnp3::AnalogCommandEvent(meas->Value, ConvertCommandStatus(meas->Status), opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			opendnp3::BinaryOutputStatus Conversions::ConvertMeas(BinaryOutputStatus^ meas)
			{
				return opendnp3::BinaryOutputStatus(meas->Value, meas->Quality, opendnp3::DNPTime(TimeStamp::Convert(meas->Timestamp)));
			}

			asiopal::SerialSettings Conversions::ConvertSerialSettings(SerialSettings^ settings)
			{
				asiopal::SerialSettings s;
				s.deviceName = ConvertString(settings->port);
				s.baud = settings->baud;
				s.dataBits = settings->dataBits;
				s.stopBits = (asiopal::StopBits) settings->stopBits;
				s.parity = (asiopal::ParityType) settings->parity;
				s.flowType = (asiopal::FlowType) settings->flowControl;
				return s;
			}

			openpal::TimeDuration Conversions::ConvertMilliseconds(System::UInt64 ms)
			{
				return openpal::TimeDuration::Milliseconds(ms);
			}

			openpal::TimeDuration Conversions::ConvertTimespan(System::TimeSpan ts)
			{
				return ConvertMilliseconds(ts.Ticks / System::TimeSpan::TicksPerMillisecond);
			}

			opendnp3::ClassField Conversions::ConvertClassField(ClassField classField)
			{
				return opendnp3::ClassField(classField.ClassMask);
			}

			opendnp3::LinkConfig Conversions::ConvertConfig(LinkConfig^ config)
			{
				return opendnp3::LinkConfig(config->isMaster, config->useConfirms, config->numRetry, config->localAddr, config->remoteAddr, ConvertMilliseconds(config->timeoutMs));
			}

			opendnp3::EventBufferConfig Conversions::ConvertConfig(EventBufferConfig^ cm)
			{
				opendnp3::EventBufferConfig config;
				config.maxBinaryEvents = cm->maxBinaryEvents;
				config.maxDoubleBinaryEvents = cm->maxDoubleBinaryEvents;
				config.maxCounterEvents = cm->maxCounterEvents;
				config.maxFrozenCounterEvents = cm->maxFrozenCounterEvents;
				config.maxAnalogEvents = cm->maxAnalogEvents;
				config.maxBinaryOutputStatusEvents = cm->maxBinaryOutputStatusEvents;
				config.maxAnalogOutputStatusEvents = cm->maxAnalogOutputStatusEvents;
				return config;
			}

			opendnp3::OutstationConfig Conversions::ConvertConfig(OutstationConfig^ config, opendnp3::IndexMode indexMode)
			{
				opendnp3::OutstationConfig oc;				

				oc.params = ConvertConfig(config->config, indexMode);
				oc.eventBufferConfig = ConvertConfig(config->buffer);				

				return oc;
			}			

			opendnp3::OutstationParams Conversions::ConvertConfig(OutstationParams^ config, opendnp3::IndexMode indexMode)
			{
				opendnp3::OutstationParams params;

				params.indexMode = indexMode;
				params.allowUnsolicited = config->allowUnsolicited;
				params.typesAllowedInClass0 = opendnp3::StaticTypeBitField(config->typesAllowedInClass0.mask);
				params.maxControlsPerRequest = config->maxControlsPerRequest;
				params.maxTxFragSize = config->maxTxFragSize;
				params.selectTimeout = ConvertTimespan(config->selectTimeout);
				params.solConfirmTimeout = ConvertTimespan(config->solicitedConfirmTimeout);
				params.unsolClassMask = ConvertClassField(config->unsolClassMask);
				params.unsolConfirmTimeout = ConvertTimespan(config->unsolicitedConfirmTimeout);
				params.unsolRetryTimeout = ConvertTimespan(config->unsolicitedRetryPeriod);
				

				return params;
			}

			opendnp3::MasterParams Conversions::ConvertConfig(MasterConfig^ config)
			{
				opendnp3::MasterParams mp;

				mp.disableUnsolOnStartup = config->disableUnsolOnStartup;
				mp.integrityOnEventOverflowIIN = config->integrityOnEventOverflowIIN;
				mp.responseTimeout = ConvertTimespan(config->responseTimeout);
				mp.startupIntegrityClassMask = ConvertClassField(config->startupIntegrityClassMask);
				mp.eventScanOnEventsAvailableClassMask = ConvertClassField(config->eventScanOnEventsAvailableClassMask);
				mp.taskRetryPeriod = ConvertTimespan(config->taskRetryPeriod);
				mp.timeSyncMode = (opendnp3::TimeSyncMode) config->timeSyncMode;
				mp.unsolClassMask = ConvertClassField(config->unsolClassMask);

				return mp;
			}

			opendnp3::MasterStackConfig Conversions::ConvertConfig(MasterStackConfig^ config)
			{
				opendnp3::MasterStackConfig cfg;
				cfg.master = ConvertConfig(config->master);
				cfg.link = ConvertConfig(config->link);
				return cfg;
			}

			opendnp3::OutstationStackConfig Conversions::ConvertConfig(OutstationStackConfig^ config)
			{
				opendnp3::OutstationStackConfig cfg;
				cfg.dbTemplate = ConvertConfig(config->databaseTemplate);
				IndexMode indexMode = config->databaseTemplate->GetIndexMode();
				cfg.outstation = ConvertConfig(config->outstation, (opendnp3::IndexMode) indexMode);
				cfg.link = ConvertConfig(config->link);
				return cfg;
			}

			opendnp3::DatabaseTemplate Conversions::ConvertConfig(DatabaseTemplate^ config)
			{
				return opendnp3::DatabaseTemplate(
					config->binaries->Count,
					config->doubleBinaries->Count,
					config->analogs->Count,
					config->counters->Count,
					config->frozenCounters->Count,
					config->binaryOutputStatii->Count,
					config->analogOutputStatii->Count,
					config->timeAndIntervals->Count
				);								
			}

		}
	}
}
