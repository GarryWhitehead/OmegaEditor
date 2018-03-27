#pragma once
#include "components/component.h"

class HealthComponent : public ArchivableComponent<HealthComponent>
{
public:
	HealthComponent() : ArchivableComponent<HealthComponent>(*this, HEALTH_COMPONENT) {}

	HealthComponent(uint32_t health, uint32_t lives) : 
		ArchivableComponent<HealthComponent>(*this, HEALTH_COMPONENT),
		m_lives(lives),
		m_maxHealth(health)
	{}

	~HealthComponent() {}
	
	uint32_t GetMaxHealth() { return m_maxHealth; }
	uint32_t GetLives() { return m_lives; }

	// the serialise functions need to be able to access the private variables
	friend void Serialise(Archive *arch, HealthComponent& sprite, const var_info& info);

private:

	bool m_isDead;
	uint32_t m_lives;
	uint32_t m_maxHealth;
	uint32_t m_currHealth;
};




