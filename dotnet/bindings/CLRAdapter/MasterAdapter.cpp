
#include "Stdafx.h"
#include "MasterAdapter.h"
#include "CommandProcessorAdapter.h"
#include "MasterScanAdapter.h"
#include "Conversions.h"
#include "TaskCallbackAdapter.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			MasterAdapter::MasterAdapter(asiodnp3::IMaster* apMaster) :
				pMaster(apMaster),
				commandAdapter(gcnew CommandProcessorAdapter(apMaster->GetCommandProcessor()))
			{}

			ICommandProcessor^ MasterAdapter::GetCommandProcessor()
			{
				return commandAdapter;
			}

			IStackStatistics^ MasterAdapter::GetStackStatistics()
			{
				auto stats = pMaster->GetStackStatistics();
				return Conversions::ConvertStackStats(stats);
			}

			void MasterAdapter::Enable()
			{
				pMaster->Enable();
			}

			void MasterAdapter::Disable()
			{
				pMaster->Disable();
			}

			void MasterAdapter::Shutdown()
			{
				pMaster->Shutdown();
			}

			IMasterScan^ MasterAdapter::AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, TaskConfig^ config)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				auto scan = pMaster->AddAllObjectsScan(gvid, Conversions::ConvertTimespan(period), Convert(config));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddClassScan(ClassField field, System::TimeSpan period, TaskConfig^ config)
			{
				auto scan = pMaster->AddClassScan(Conversions::ConvertClassField(field), Conversions::ConvertTimespan(period), Convert(config));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, TaskConfig^ config)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				auto scan = pMaster->AddRangeScan(gvid, start, stop, Conversions::ConvertTimespan(period), Convert(config));
				return gcnew MasterScanAdapter(scan);
			}

			IMasterScan^ MasterAdapter::AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, TaskConfig^ config)
			{
				auto vec = ConvertToVectorOfHeaders(headers);
				auto scan = pMaster->AddScan(Conversions::ConvertTimespan(period), vec, Convert(config));
				return gcnew MasterScanAdapter(scan);
			}

			void MasterAdapter::Scan(IEnumerable<Header^>^ headers, TaskConfig^ config)
			{
				auto vec = ConvertToVectorOfHeaders(headers);
				pMaster->Scan(vec, Convert(config));
			}

			void MasterAdapter::ScanAllObjects(System::Byte group, System::Byte variation, TaskConfig^ config)
			{
				pMaster->ScanAllObjects(opendnp3::GroupVariationID(group, variation), Convert(config));
			}

			void MasterAdapter::ScanClasses(ClassField field, TaskConfig^ config)
			{
				pMaster->ScanClasses(Conversions::ConvertClassField(field), Convert(config));
			}

			void MasterAdapter::ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, TaskConfig^ config)
			{
				opendnp3::GroupVariationID gvid(group, variation);
				pMaster->ScanRange(gvid, start, stop, Convert(config));
			}

			void MasterAdapter::Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config)
			{
				pMaster->Write(Conversions::ConvertMeas(value), index, Convert(config));
			}

			std::vector<asiodnp3::Header> MasterAdapter::ConvertToVectorOfHeaders(IEnumerable<Header^>^ headers)
			{
				std::vector<asiodnp3::Header> ret;
				for each(auto header in headers)
				{
					asiodnp3::Header output;
					if (Convert(header, output))
					{
						ret.push_back(output);
					}
				}
				return ret;
			}

			bool MasterAdapter::Convert(Header^ header, asiodnp3::Header& output)
			{
				switch (header->qualifier)
				{
				case(QualifierCode::ALL_OBJECTS) :
				{
					output = asiodnp3::Header::AllObjects(header->group, header->variation);
					return true;
				}
				case(QualifierCode::UINT8_CNT) :
				{
					auto value = dynamic_cast<CountHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertCount8(value);
						return true;
					}
				}
				case(QualifierCode::UINT16_CNT) :
				{
					auto value = dynamic_cast<CountHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertCount16(value);
						return true;
					}
				}
				case(QualifierCode::UINT8_START_STOP) :
				{
					auto value = dynamic_cast<RangeHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertRange8(value);
						return true;
					}
				}
				case(QualifierCode::UINT16_START_STOP) :
				{
					auto value = dynamic_cast<RangeHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertRange16(value);
						return true;
					}
				}
				default:
					return false;
				}
			}

			asiodnp3::Header MasterAdapter::Convert(Header^ header)
			{
				return asiodnp3::Header::AllObjects(header->group, header->variation);
			}

			asiodnp3::Header MasterAdapter::ConvertCount8(CountHeader^ header)
			{
				return asiodnp3::Header::Count8(header->group, header->variation, static_cast<uint8_t>(header->count));
			}

			asiodnp3::Header MasterAdapter::ConvertCount16(CountHeader^ header)
			{
				return asiodnp3::Header::Count16(header->group, header->variation, header->count);
			}

			asiodnp3::Header MasterAdapter::ConvertRange8(RangeHeader^ header)
			{
				return asiodnp3::Header::Range8(header->group, header->variation, static_cast<uint8_t>(header->start), static_cast<uint8_t>(header->stop));
			}

			asiodnp3::Header MasterAdapter::ConvertRange16(RangeHeader^ header)
			{
				return asiodnp3::Header::Range16(header->group, header->variation, header->start, header->stop);
			}

			opendnp3::TaskConfig MasterAdapter::Convert(TaskConfig^ config)
			{			
				return opendnp3::TaskConfig(Convert(config->taskId), CreateTaskCallback(config->callback), opendnp3::User(config->user->Number));
			}

			opendnp3::TaskId MasterAdapter::Convert(TaskId^ id)
			{
				return id->IsValid ? opendnp3::TaskId::Defined(id->Value) : opendnp3::TaskId::Undefined();
			}

			opendnp3::ITaskCallback* MasterAdapter::CreateTaskCallback(ITaskCallback^ callback)
			{
				return (callback == nullptr) ? nullptr : TaskCallbackAdapter::Create(callback);
			}
		}
	}
}