
#include "ConfigReader.h"


#include "JNIHelpers.h"

#include <iostream>
#include <assert.h>

using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterStackConfig cfg;
	
	cfg.link = ConvertLinkConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
	cfg.app = ConvertAppConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;"));
	cfg.master = ConvertMasterConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "masterConfig", "Lcom/automatak/dnp3/MasterConfig;"));

	return cfg;
}

SlaveStackConfig ConfigReader::ConvertSlaveStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveStackConfig cfg;

	cfg.link = ConvertLinkConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
	cfg.app = ConvertAppConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;")); 
	cfg.slave = ConvertOutstationConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "outstationConfig", "Lcom/automatak/dnp3/OutstationConfig;"));
	cfg.device = ConvertDatabaseConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;"));	

	return cfg;
}

SlaveConfig ConfigReader::ConvertOutstationConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveConfig cfg;
	
	cfg.mMaxControls  = JNIHelpers::GetIntField(apEnv, jCfg, "maxControls");
	cfg.mDisableUnsol = JNIHelpers::GetBoolField(apEnv, jCfg, "disableUnsol");
	cfg.mUnsolMask  = ClassMask(JNIHelpers::GetIntField(apEnv, jCfg, "unsolMask"));
	cfg.mAllowTimeSync = JNIHelpers::GetBoolField(apEnv, jCfg, "allowTimeSync");
	cfg.mTimeSyncPeriod  = JNIHelpers::GetLongField(apEnv, jCfg, "timeSyncPeriodMs");
	cfg.mUnsolPackDelay  = JNIHelpers::GetLongField(apEnv, jCfg, "unsolPackDelayMs");
	cfg.mUnsolRetryDelay  = JNIHelpers::GetLongField(apEnv, jCfg, "unsolRetryDelayMs");
	cfg.mMaxFragSize  = JNIHelpers::GetIntField(apEnv, jCfg, "maxFragSize");
	cfg.mVtoWriterQueueSize = JNIHelpers::GetIntField(apEnv, jCfg, "vtoWriterQueueSize");

	jint maxBinaryEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxBinaryEvents");
	jint maxAnalogEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxAnalogEvents");
	jint maxCounterEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxCounterEvents");
	jint maxVtoEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxVtoEvents");

	cfg.mEventMaxConfig = EventMaxConfig(maxBinaryEvents, maxAnalogEvents, maxCounterEvents, maxVtoEvents);
	
	cfg.mStaticBinary = ConvertGrpVar(apEnv, jCfg, "staticBinaryInput");
	cfg.mStaticAnalog = ConvertGrpVar(apEnv, jCfg, "staticAnalogInput");
	cfg.mStaticCounter = ConvertGrpVar(apEnv, jCfg, "staticCounter");
	cfg.mStaticSetpointStatus = ConvertGrpVar(apEnv, jCfg, "staticAnalogOutputStatus");
	
	cfg.mEventBinary = ConvertGrpVar(apEnv, jCfg, "eventBinaryInput");
	cfg.mEventAnalog = ConvertGrpVar(apEnv, jCfg, "eventAnalogInput");
	cfg.mEventCounter = ConvertGrpVar(apEnv, jCfg, "eventCounter");
	cfg.mEventVto = ConvertGrpVar(apEnv, jCfg, "eventVto");		
	
	return cfg;
}

GrpVar ConfigReader::ConvertGrpVar(JNIEnv* apEnv, jobject jCfg, const char* fieldId)
{
	jobject obj = JNIHelpers::GetObjectField(apEnv, jCfg, fieldId, "Lcom/automatak/dnp3/GroupVariation;");
	int grp = JNIHelpers::GetIntField(apEnv, obj, "group");
	int var = JNIHelpers::GetIntField(apEnv, obj, "variation");
	return GrpVar(grp, var);
}

DeviceTemplate ConfigReader::ConvertDatabaseConfig(JNIEnv* apEnv, jobject jCfg)
{
	DeviceTemplate cfg;

	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "binaryInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			cfg.mBinary.push_back(EventPointRecord(IntToPointClass(mask)));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "analogInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			double db = JNIHelpers::GetDoubleField(apEnv, record, "deadband");
			cfg.mBinary.push_back(DeadbandPointRecord(IntToPointClass(mask), db));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "counterInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			cfg.mCounter.push_back(EventPointRecord(IntToPointClass(mask)));
		});
	}	
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "binaryOutputStatii", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {			
			cfg.mControlStatus.push_back(PointRecord());
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "analogOutputStatii", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {			
			cfg.mSetpointStatus.push_back(PointRecord());
		});
	}
	
	return cfg;
}


MasterConfig ConfigReader::ConvertMasterConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterConfig cfg;

	cfg.FragSize = JNIHelpers::GetIntField(apEnv, jCfg, "maxRequestFragmentSize");
	cfg.VtoWriterQueueSize = JNIHelpers::GetIntField(apEnv, jCfg, "vtoWriterQueueSize");
	cfg.UseNonStandardVtoFunction = JNIHelpers::GetBoolField(apEnv, jCfg, "useNonStandardVtoFunction");	
	cfg.AllowTimeSync = JNIHelpers::GetBoolField(apEnv, jCfg, "allowTimeSync");
	cfg.DoUnsolOnStartup = JNIHelpers::GetBoolField(apEnv, jCfg, "doUnsolOnStartup");
	cfg.EnableUnsol = JNIHelpers::GetBoolField(apEnv, jCfg, "enableUnsol");
	cfg.UnsolClassMask = JNIHelpers::GetIntField(apEnv, jCfg, "unsolClassMask");
	cfg.IntegrityRate = JNIHelpers::GetLongField(apEnv, jCfg, "integrityRateMs");
	cfg.TaskRetryRate = JNIHelpers::GetLongField(apEnv, jCfg, "taskRetryRateMs");
	
	jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "scans", "Ljava/util/List;");

	JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject scan) {
		int mask = JNIHelpers::GetIntField(apEnv, scan, "classMask");
		long rate = JNIHelpers::GetLongField(apEnv, scan, "scanRateMs");
		cfg.AddExceptionScan(mask, rate); 
	});	

	return cfg;
}

AppConfig ConfigReader::ConvertAppConfig(JNIEnv* apEnv, jobject jCfg)
{
	AppConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);

	cfg.RspTimeout = JNIHelpers::GetLongField(apEnv, jCfg, "rspTimeoutMs");
	cfg.NumRetry = JNIHelpers::GetIntField(apEnv, jCfg, "numRetry"); 
	cfg.FragSize = JNIHelpers::GetIntField(apEnv, jCfg, "maxFragSize");
	
	return cfg;	
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* apEnv, jobject jCfg)
{
	LinkConfig cfg(true, false);
	
	cfg.IsMaster = JNIHelpers::GetBoolField(apEnv, jCfg, "isMaster");
	cfg.UseConfirms = JNIHelpers::GetBoolField(apEnv, jCfg, "useConfirms");
	cfg.NumRetry = JNIHelpers::GetIntField(apEnv, jCfg, "numRetry");
	cfg.LocalAddr = JNIHelpers::GetIntField(apEnv, jCfg, "localAddr");	
	cfg.RemoteAddr = JNIHelpers::GetIntField(apEnv, jCfg, "remoteAddr");		

	return cfg;
}



