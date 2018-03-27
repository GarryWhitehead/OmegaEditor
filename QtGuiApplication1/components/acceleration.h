#ifndef _ACCELERATION_H
#define _ACCELERATION_H
#include "component.h"
#include "glm.hpp"

class AccelerationComponent : public ArchivableComponent<AccelerationComponent>
{
public:
	AccelerationComponent() : ArchivableComponent<AccelerationComponent>(*this, ACCELERATION_COMPONENT) {}

	AccelerationComponent(glm::vec3 accel, float mass) : ArchivableComponent<AccelerationComponent>(*this, ACCELERATION_COMPONENT), 
		m_acceleration(accel),
		m_mass(mass)
	{}

	glm::vec3 GetAcceleration() const { return m_acceleration; }
	float GetMass() const { return m_mass;  }

	friend void Serialise(Archive* arch, AccelerationComponent& comp, const var_info& info);

private:

	glm::vec3 m_acceleration;
	float m_mass;

};


#endif
