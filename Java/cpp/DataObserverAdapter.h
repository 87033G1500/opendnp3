#ifndef __DATA_OBSERVER_ADAPTER_H_
#define __DATA_OBSERVER_ADAPTER_H_

#include <jni.h>
#include <APL/DataInterfaces.h>

class DataObserverAdapter : public apl::IDataObserver
{
	public:
	DataObserverAdapter(JavaVM* apJVM, jobject aProxy);

	protected:

	void _Start();
	void _Update(const apl::Binary& arMeas, size_t aIndex);
	void _Update(const apl::Analog& arMeas, size_t aIndex);
	void _Update(const apl::Counter& arMeas, size_t aIndex);
	void _Update(const apl::SetpointStatus& arMeas, size_t aIndex);
	void _Update(const apl::ControlStatus& arMeas, size_t aIndex);
	void _End();

	private:
	JavaVM* mpJVM;
	jobject mProxy;
};

#endif
