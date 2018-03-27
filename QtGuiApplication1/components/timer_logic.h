#pragma once
#include "component.h"
#include "utility/serialisation.h"
#include "glm.hpp"
#include <chrono>

using TimePoint = std::chrono::system_clock::time_point;
using Seconds = std::chrono::seconds;

class TimerComponent : public ArchivableComponent<TimerComponent>
{
public:
	TimerComponent() : ArchivableComponent<TimerComponent>(*this, TIMED_LOGIC_COMPONENT) {}

	TimerComponent(long on, long off, bool constant) : 
		ArchivableComponent<TimerComponent>(*this, TIMED_LOGIC_COMPONENT),
		m_onTime(on),
		m_offTime(off),
		m_constantLoop(constant)
	{}

	TimerComponent(long on, bool constant) :
		ArchivableComponent<TimerComponent>(*this, TIMED_LOGIC_COMPONENT),
		m_onTime(on),
		m_constantLoop(constant)
	{}

	long GetTimeOn() const { return m_onTime; }
	long GetTimeOff() const { return m_offTime; }
	bool IsConstantLoop() const { return m_constantLoop; }

	friend void Serialise(Archive* arch, TimerComponent& comp, const var_info& info);

private:

	bool m_timerRunning;
	bool m_constantLoop;

	long m_onTime;
	long m_offTime;

	Seconds m_onTimeChrono;
	Seconds m_offTimeChrono;

	TimePoint m_startTime;
	TimePoint m_endTime;
	TimePoint m_currTime;
	
};