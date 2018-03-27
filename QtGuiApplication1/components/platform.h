#pragma once
#include "component.h"
#include "utility/serialisation.h"
#include "glm.hpp"

const uint32_t PLATFORM_STATIC = 0;
const uint32_t PLATFORM_MOVING = 1;
const uint32_t PLATFORM_UNSTABLE = 2;

class PlatformComponent : public ArchivableComponent<PlatformComponent>
{
public:
	PlatformComponent() : ArchivableComponent<PlatformComponent>(*this, PLATFORM_COMPONENT),
		mType(PLATFORM_STATIC)
	{}

	// helper functions
	uint32_t GetPlatformType() { return mType; }

private:

	uint32_t mType;

	// used for moving platforms to signy the moevment position boundarys
	glm::vec2 mStartPosition;
	glm::vec2 mEndPosition;

	// used for unstable platforms to signy the time before removing

};