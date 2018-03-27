#include "velocity.h"
#include "sprite.h"

void Serialise(Archive* arch, VelocityComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_velocity, var_info(info.name + ".mVelocity"));
	Serialise(arch, comp.m_buoyancy, var_info(info.name + ".mBuoyancy"));
}
