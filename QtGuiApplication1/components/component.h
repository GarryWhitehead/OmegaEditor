#ifndef _COMPONENT_H
#define _COMPONENT_H
#include <memory>
#include <vector>
#include "utility/serialisation.h"

class System;

// This is a bit of a hack to get this working with serialisation - should be using enums
// TODO: write own enum class which is serialised
const uint32_t COLLISION_COMPONENT = 0;
const uint32_t INPUT_COMPONENT = 1;
const uint32_t POSITION_COMPONENT = 2;
const uint32_t VELOCITY_COMPONENT = 3;
const uint32_t SPRITE_COMPONENT = 4;
const uint32_t ANIMATION_COMPONENT = 5;
const uint32_t PLAYER_COMPONENT = 6;
const uint32_t ACCELERATION_COMPONENT = 7;
const uint32_t PLATFORM_COMPONENT = 8;
const uint32_t TIMED_LOGIC_COMPONENT = 9;
const uint32_t TWO_STATE_LOGIC_COMPONENT = 10;
const uint32_t MESSAGE_COMPONENT = 11;
const uint32_t HEALTH_COMPONENT = 12;
const uint32_t NO_COMPONENT = 13;

const uint32_t COLLISION_BIT = 1 << 0;
const uint32_t INPUT_BIT = 1 << 1;
const uint32_t POSITION_BIT = 1 << 2;
const uint32_t VELOCITY_BIT = 1 << 3;
const uint32_t SPRITE_BIT = 1 << 4;
const uint32_t ANIMATION_BIT = 1 << 5;
const uint32_t PLAYER_BIT = 1 << 6;
const uint32_t ACCELERATION_BIT = 1 << 7;
const uint32_t PLATFORM_BIT = 1 << 8;
const uint32_t TIMED_LOGIC_BIT = 1 << 9;
const uint32_t TWO_STATE_LOGIC_BIT = 1 << 10;
const uint32_t MESSAGE_COMPONENT_BIT = 1 << 11;
const uint32_t HEALTH_COMPONENT_BIT = 1 << 12;

class Component
{
public:

	Component(uint32_t type) : m_type(type) {}

	virtual ~Component() {}

	// helper functions
	uint32_t ComponentType() { return m_type; }

	// required to allow serilisation function access to protected data members
	friend void Serialise(Archive *arch, Component& comp, const var_info& info);

protected:

	virtual void pack_unpack(Archive *arch, const var_info& info);

	uint32_t m_type;

	std::vector<std::shared_ptr<System> > m_observers;
};


// derived serialisation class required for component derived classes
template <typename T>
class ArchivableComponent : public Component
{
public:

	ArchivableComponent(T& derived, uint32_t type) : m_derived(derived), Component(type) {}
	~ArchivableComponent() {}

protected:

	void pack_unpack(Archive *arch, const var_info& info)
	{
		Serialise(arch, m_derived, info);
	}

private:

	T &m_derived;
};

void Serialise(Archive *arch, Component& comp, const var_info& info);


#endif // _COMPONENT_H
