
#include "Stdafx.h"
#include "OutstationAdapter.h"

#include "OutstationDatabaseAdapter.h"
#include "Conversions.h"

#include <opendnp3/outstation/Database.h>

namespace DNP3
{
namespace Adapter
{

OutstationAdapter::OutstationAdapter(asiodnp3::IOutstation* pOutstation_) :
	pOutstation(pOutstation_),
	databaseAdapter(gcnew OutstationDatabaseAdapter(pOutstation->GetDatabase()))
{}

DNP3::Interface::IDatabase^ OutstationAdapter::GetDatabase()
{
	return databaseAdapter;
}

void OutstationAdapter::Shutdown()
{
	pOutstation->Shutdown();
}


void OutstationAdapter::Enable()
{
	pOutstation->Enable();
}

void OutstationAdapter::Disable()
{
	pOutstation->Disable();
}

IStackStatistics^ OutstationAdapter::GetStackStatistics()
{
	auto stats = pOutstation->GetStackStatistics();
	return Conversions::ConvertStackStats(stats);
}

}
}
