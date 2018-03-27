#ifndef _COLLISION_H
#define _COLLISION_H
#include "component.h"

// bit masks for collision types
const uint32_t COLL_LETHAL_BIT = 1 << 0;
const uint32_t COLL_FLOOR_BIT = 1 << 1;
const uint32_t COLL_IMPULSE_BIT = 1 << 2;
const uint32_t COLL_BREAKABLE_BIT = 1 << 3;
const uint32_t COLL_HIDING_BIT = 1 << 4;
const uint32_t COLL_MOVING_BIT = 1 << 5;

class CollisionComponent : public ArchivableComponent<CollisionComponent>
{
public:
	CollisionComponent() : ArchivableComponent<CollisionComponent>(*this, COLLISION_COMPONENT) {}

	CollisionComponent(float elastic, uint32_t bits) : 
		ArchivableComponent<CollisionComponent>(*this, COLLISION_COMPONENT),
		m_elasticity(elastic),
		m_collBits(bits)
	{}

	// helper functions
	float GetElasticity() const { return m_elasticity; }
	uint32_t GetCollisionBits() const { return m_collBits; }

	friend void Serialise(Archive* arch, CollisionComponent& comp, const var_info& info);

private:

	float m_elasticity;
	uint32_t m_collBits;
};


#endif
