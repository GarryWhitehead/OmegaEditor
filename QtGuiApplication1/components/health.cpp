#include "components\health.h"

void Serialise(Archive* arch, HealthComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_lives, var_info(info.name + ".m_lives"));
	Serialise(arch, comp.m_maxHealth, var_info(info.name + ".m_maxHealth"));
}