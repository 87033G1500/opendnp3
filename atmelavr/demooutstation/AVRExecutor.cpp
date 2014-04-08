
#include "AVRExecutor.h"

#include "AVRTimer.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include <CriticalSection.h>

using namespace openpal;

AVRExecutor* gpExecutor = nullptr;

SIGNAL(TIMER1_COMPA_vect)
{
	if(gpExecutor)
	{
		gpExecutor->Tick();		
	}	
}

void AVRExecutor::Tick()
{
	++ticks;
	if(ticks % 100 == 0)
	{
		// Toggle the LED from the main thread
		auto toggle = []() { PORTB ^= (1 << 7); };		
		this->PostLambda(toggle);
	}
}

void AVRExecutor::Init()
{	
	// Set LED as output 
	DDRB |= (7 << 0);
	
	 // Configure timer 1 for CTC mode
	TCCR1B |= (1 << WGM12);

	// Set CTC compare value to ~100Hz at 16MHz AVR clock, with prescale of 1024
	OCR1A = 155;
	
	// Start timer at FCPU / 1024
	TCCR1B |= ((1 << CS10) | (1 << CS12));

	TIMSK1 |= (1 << OCIE1A); // enable output compare interrupt for A compare
	
	gpExecutor = this;
	
	sei();
}

AVRExecutor::AVRExecutor() : ticks(0)
{	
	for(uint8_t i = 0; i < timers.Size(); ++i)
	{
		idleTimers.Enqueue(&timers[i]);
	}
}

MonotonicTimestamp AVRExecutor::GetTime()
{
	CriticalSection cs; // TODO - better way to atomic read ticks?
	return MonotonicTimestamp(ticks*10); // every tick represents 10 milliseconds since Init()				
}

ITimer* AVRExecutor::Start(const TimeDuration& duration, const Runnable& runnable)
{	
	return Start(GetTime().Add(duration), runnable);
}
	
ITimer* AVRExecutor::Start(const MonotonicTimestamp& ts, const Runnable& runnable)
{
	assert(idleTimers.IsNotEmpty());
	AVRTimer* pTimer = idleTimers.Pop();
	pTimer->Set(this, runnable, ts);
	return pTimer;
}
	
void AVRExecutor::Post(const Runnable& runnable)
{	
	assert(!work.IsFull());
	work.Enqueue(runnable);
}

bool AVRExecutor::RunOne()
{			
	CriticalSection cs; // TODO release interrupts before running task
	if(work.IsNotEmpty())
	{
		work.Peek().Run();
		work.Pop();
		return true;
	}
	else
	{
		return false;
	}
}
			
void AVRExecutor::OnCancel(AVRTimer* pTimer)
{

}
