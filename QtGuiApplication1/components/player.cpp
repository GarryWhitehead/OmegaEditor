#include "components/player.h"
#include "sprite.h"


void Serialise(Archive *arch, PlayerComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_accelX, var_info(info.name + ".mMoveAccel"));
	Serialise(arch, comp.m_defaultMoveVelocity, var_info(info.name + ".mMoveVelocity"));
	Serialise(arch, comp.m_defaultJumpAccel, var_info(info.name + ".mJumpAccel"));
	Serialise(arch, comp.m_defaultVelocityY, var_info(info.name + ".mJumpVelocity"));
}