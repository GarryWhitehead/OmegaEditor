#include "animation.h"

void Serialise(Archive *arch, AnimationComponent& comp, const var_info& info)
{
	Serialise(arch, comp.mAnimationType, var_info(info.name + ".animationType"));
	Serialise(arch, comp.mTotal, var_info(info.name + ".total"));
	Serialise(arch, comp.mFrameIndex, var_info(info.name + ".frameIndex"));
	Serialise(arch, comp.mStartIndex, var_info(info.name + ".startIndex"));
}