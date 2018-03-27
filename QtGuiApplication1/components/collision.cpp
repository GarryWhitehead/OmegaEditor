#include "collision.h"

void Serialise(Archive* arch, CollisionComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_elasticity, var_info(info.name + ".m_elasticity"));
	Serialise(arch, comp.m_collBits, var_info(info.name + ".m_collisionType"));
}
