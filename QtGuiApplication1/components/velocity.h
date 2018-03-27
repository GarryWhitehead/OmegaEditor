#ifndef _VELOCITY_H
#define _VELOCITY_H
#include "component.h"
#include "glm.hpp"

class VelocityComponent : public ArchivableComponent<VelocityComponent>
{
public:
	VelocityComponent() : ArchivableComponent<VelocityComponent>(*this, VELOCITY_COMPONENT) {}

	VelocityComponent(glm::vec3 vel, glm::vec3 buoy) : 
		ArchivableComponent<VelocityComponent>(*this, VELOCITY_COMPONENT), 
		m_velocity(vel),
		m_buoyancy(buoy)
	{}

	glm::vec3 GetVelocity() { return m_velocity; }
	glm::vec3 GetBuoyancy() { return m_buoyancy; }

	friend void Serialise(Archive* arch, VelocityComponent& comp, const var_info& info);

private:

	glm::vec3 m_velocity;
	glm::vec3 m_buoyancy;
};

#endif
