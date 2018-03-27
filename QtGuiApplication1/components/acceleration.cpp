#include "acceleration.h"
#include "sprite.h"

void Serialise(Archive* arch, AccelerationComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_acceleration, var_info(info.name + ".m_acceleration"));
	Serialise(arch, comp.m_mass, var_info(info.name + ".m_mass"));
}
