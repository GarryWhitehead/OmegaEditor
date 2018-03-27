#pragma once
#include "component.h"
#include "utility/serialisation.h"
#include "glm.hpp"

const int MAX_MOVEMENT_TICKS = 40;
const int MAX_MOVE_VELOCITY = 200;
const int MAX_JUMP_VELOCITY = 100;
const int MAX_FALL_VELOCITY = 400;
const int LETHAL_FALL_TICKS = 30;

class PlayerComponent : public ArchivableComponent<PlayerComponent>
{
public:
	PlayerComponent() : ArchivableComponent<PlayerComponent>(*this, PLAYER_COMPONENT),
		m_fallAccel(0), 
		m_moveTicks(0), 
		m_jumpTicks(0), 
		m_fallTicks(0) {}

	PlayerComponent(glm::vec3 moveAccel, glm::vec3 moveVel, glm::vec3 jumpAccel, glm::vec3 jumpVel) :
		ArchivableComponent<PlayerComponent>(*this, PLAYER_COMPONENT),
		m_accelX(moveAccel), 
		m_moveVelocity(moveVel), 
		m_defaultJumpAccel(jumpAccel),
		m_defaultVelocityY(jumpVel)
	{}

	// helper functions
	glm::vec3 GetMoveAccel() { return m_accelX; }
	glm::vec3 GetMoveVelocity() { return m_moveVelocity; }
	glm::vec3 GetJumpAccel() { return m_defaultJumpAccel; }
	glm::vec3 GetJumpVelocity() { return m_defaultVelocityY; }

	friend void Serialise(Archive* arch, PlayerComponent& comp, const var_info& info);

private:
	
	// these are primarily used by entities with input control and allow us to determine their movement state 

	glm::vec3 m_accelX;
	glm::vec3 m_moveVelocity;			// velocity whilst entity is in movement state
	glm::vec3 m_defaultMoveVelocity;


	glm::vec3 m_jumpAccel;
	glm::vec3 m_velocityY;
	glm::vec3 m_defaultVelocityY;
	glm::vec3 m_defaultJumpAccel;


	glm::vec3 m_fallAccel;

	uint32_t m_moveTicks;
	uint32_t m_jumpTicks;
	uint32_t m_fallTicks;
};
