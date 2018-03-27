#ifndef _ANIMATION_H
#define _ANIMATION_H
#include "component.h"

const uint32_t ANIM_ROTATE = 0;
const uint32_t ANIM_FRAME = 1;


class AnimationComponent : public ArchivableComponent<AnimationComponent>
{
public:
	AnimationComponent() : ArchivableComponent<AnimationComponent>(*this, ANIMATION_COMPONENT) {}

	AnimationComponent(uint32_t type, uint32_t total, uint32_t startIndex, uint32_t frameIndex) : 
		ArchivableComponent<AnimationComponent>(*this, ANIMATION_COMPONENT), 
		mAnimationType(type),
		mTotal(total),
		mStartIndex(startIndex),
		mFrameIndex(frameIndex)
	{}

	~AnimationComponent() {}

	uint32_t GetFrameCount() { return mTotal; }
	uint32_t GetStartIndex() { return mStartIndex; }
	uint32_t GetFrameIndex() { return mFrameIndex; }
	uint32_t GetType() { return mAnimationType; }
	
	// friend to component for seriliastion purposes
	friend void Serialise(Archive *arch, AnimationComponent& data, const var_info& info);

private:

	uint32_t mAnimationType;
	uint32_t mFrame;
	uint32_t mTotal;
	uint32_t mFrameIndex;
	uint32_t mStartIndex;
};



#endif // _ANIMATION_H
