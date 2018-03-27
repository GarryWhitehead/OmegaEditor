#ifndef _POSITION_H
#define _POSITION_H
#include "component.h"
#include "utility/serialisation.h"
#include "OmegaEngineEditor.h"
#include "glm.hpp"

const float FLOOR_POSITION = 32.0f;

class PositionComponent : public ArchivableComponent<PositionComponent>
{
public:
	PositionComponent() :ArchivableComponent<PositionComponent>(*this, POSITION_COMPONENT) {}

	PositionComponent(glm::vec3 pos, float rot) : 
		ArchivableComponent<PositionComponent>(*this, POSITION_COMPONENT), 
		m_pos(pos),
		m_rotation(rot), 
		mStaticEntity(false) 
	{}

	// function used for converting between vulkan and qt y coord
	void ConvertY(float worldH, glm::vec2 dim, MainWindow::CoordSystem coordSys);

	// helper functions
	glm::vec3 GetPosition() { return m_pos; }
	float GetRotation() { return m_rotation; }
	void UpdatePosition(glm::vec3 pos) { m_pos = pos; }
	void UpdateRotation(float rotation) { m_rotation = rotation; }

	// for serialisation purposes
	friend void Serialise(Archive* arch, PositionComponent& comp, const var_info& info);

private:
	glm::vec3 m_pos;
	float m_rotation;

	bool mStaticEntity;
};


#endif