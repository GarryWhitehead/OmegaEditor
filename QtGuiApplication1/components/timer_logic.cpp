#include "components/timer_logic.h"

void Serialise(Archive* arch, TimerComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_constantLoop, var_info(info.name + ".m_constantLoop"));
	Serialise(arch, comp.m_onTime, var_info(info.name + ".m_onTime"));
	Serialise(arch, comp.m_offTime, var_info(info.name + ".m_offTime"));
}
