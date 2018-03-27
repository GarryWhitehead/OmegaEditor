#include "EntityManager.h"
#include "OmegaEngineEditor.h"
#include "entity.h"
#include "SpriteManager.h"
#include "CustomGraphicsView.h"
#include "NewEntityWin.h"
#include "utility/file_log.h"
#include "components/component.h"
#include "components\position.h"
#include "components\velocity.h"
#include "components/sprite.h"
#include "components/animation.h"
#include "components/timer_logic.h"
#include "components/acceleration.h"
#include "components/collision.h"
#include "components/message_component.h"
#include "components/health.h"
#include "components/player.h"
#include "components/platform.h"
#include "components/input_component.h"
#include <qwidget.h>
#include <qmessagebox.h>
#include "glm.hpp"

EntityManager *entityManager = nullptr;

EntityManager::EntityManager() : m_guID(1)
{
	entityManager = this;
	m_deletedIDs.clear();
}

EntityManager::~EntityManager()
{
}

bool EntityManager::AddEntity(bool state, uint32_t ID, NewEntityWin *window, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	uint32_t compBits = 0;
	std::unordered_map<uint32_t, std::shared_ptr<Component> > inner;
	
	// if the function is in editing mode, delete the entity and components and create again with the same ID
	if (state == EDIT_ENTITY) {
		m_entityData.erase(ID);
		m_componentData.erase(ID);
	}

	// Check for position component
	glm::vec3 pos;
	float rot;
	if (window->GetPositionTabData(pos, rot) == Qt::Checked) {
		auto &posComp = std::make_shared<PositionComponent>(pos, rot);
		compBits |= POSITION_BIT;
		inner.insert(std::make_pair(POSITION_COMPONENT, std::move(posComp)));
	}

	// Check for velocity component
	glm::vec3 vel;
	glm::vec3 buoy;
	if(window->GetVelocityTabData(vel, buoy) == Qt::Checked) {
		auto &velComp = std::make_shared<VelocityComponent>(vel, buoy);
		compBits |= VELOCITY_BIT;
		inner.insert(std::make_pair(VELOCITY_COMPONENT, std::move(velComp)));
	}
	
	// Check for acceleration component
	glm::vec3 accel;
	float mass = 0.0f;
	if(window->GetAccelerationTabData(accel, mass) == Qt::Checked) {
		auto &accelComp = std::make_shared<AccelerationComponent>(accel, mass);
		compBits |= ACCELERATION_BIT;
		inner.insert(std::make_pair(ACCELERATION_COMPONENT, std::move(accelComp)));
	}

	// Check for animation component
	uint32_t frameCount = 0;
	uint32_t startIndex = 0;
	uint32_t frameIndex = 0;
	uint32_t animType = 0;
	if(window->GetAnimationTabData(frameCount, startIndex, frameIndex, animType) == Qt::Checked) {
		auto &animComp = std::make_shared<AnimationComponent>(frameCount, startIndex, frameIndex, animType);
		compBits |= ANIMATION_BIT;
		inner.insert(std::make_pair(ANIMATION_COMPONENT, std::move(animComp)));
	}

	// Check for health component
	uint32_t maxHealth = 0;
	uint32_t lives = 0;
	if(window->GetHealthTabData(maxHealth, lives) == Qt::Checked) {
		auto &healthComp = std::make_shared<HealthComponent>(maxHealth, lives);
		compBits |= HEALTH_COMPONENT_BIT;
		inner.insert(std::make_pair(HEALTH_COMPONENT, std::move(healthComp)));
	}

	// Check for timed component
	long onTime = 0L;
	long offTime = 0L;
	uint32_t onType = NO_MSG_ID;
	uint32_t loopedType = NO_MSG_ID;
	uint32_t constantType = NO_MSG_ID;
	bool isConstantLoop = false;

	if(window->GetTimedTabData(isConstantLoop, onTime, offTime, onType, constantType, loopedType) == Qt::Checked) {
		std::shared_ptr<TimerComponent> timedComp;
		std::shared_ptr<MessageComponent> msgComp;
		if (isConstantLoop) {							// constantly loop on/off
			timedComp = std::make_shared<TimerComponent>(onTime, offTime, true);
			msgComp = std::make_shared<MessageComponent>(onType, loopedType, NO_MSG_ID);
		}
		else {															// constant off logic
			timedComp = std::make_shared<TimerComponent>(onTime, false);
			msgComp = std::make_shared<MessageComponent>(onType, NO_MSG_ID, constantType);
		}
		compBits |= TIMED_LOGIC_BIT;
		compBits |= MESSAGE_COMPONENT_BIT;
		inner.insert(std::make_pair(TIMED_LOGIC_COMPONENT, std::move(timedComp)));
		inner.insert(std::make_pair(MESSAGE_COMPONENT, std::move(msgComp)));
	}

	// Check for collision component
	uint32_t collBits = 0;
	float elasticity = 0.0f;
	bool isLethal, isFloor, isImpulse, isBreakable, isMoving, isHiding;
	if(window->GetCollisionTabData(collBits, elasticity) == Qt::Checked) {
		auto &collComp = std::make_shared<CollisionComponent>(elasticity, collBits);
		compBits |= COLLISION_BIT;
		inner.insert(std::make_pair(COLLISION_COMPONENT, std::move(collComp)));
	}

	// Check for sprite component
	glm::vec2 size;
	glm::vec2 uv;
	bool isActive = true;
	uint32_t imageIndex = 0;
	uint32_t repeatCount = 1;
	uint32_t spriteType = 0;
	if (window->GetSpriteTabData(isActive, size, uv, imageIndex, repeatCount, spriteType) == Qt::Checked) {
		auto &spriteComp = std::make_shared<SpriteComponent>(size, uv, isActive, imageIndex, spriteType, repeatCount);
		compBits |= SPRITE_BIT;
		inner.insert(std::make_pair(SPRITE_COMPONENT, std::move(spriteComp)));
	}

	// Check for player coomponent
	glm::vec3 moveAccel, moveVel;
	glm::vec3 jumpAccel, jumpVel;
	if(window->GetPlayerTabData(moveAccel, moveVel, jumpAccel, jumpVel) == Qt::Checked) {	
		auto &playerComp = std::make_shared<PlayerComponent>(moveAccel, moveVel, jumpAccel, jumpVel);
		compBits |= PLAYER_BIT;
		inner.insert(std::make_pair(PLAYER_COMPONENT, std::move(playerComp)));
	}

	// Check for input coomponent - at present, has no data values. Just used as a flag to indicate that this entity is player controlled
	if(window->GetInputTabData() == Qt::Checked) {
		auto &inputComp = std::make_shared<InputComponent>();
		compBits |= INPUT_BIT;
		inner.insert(std::make_pair(INPUT_COMPONENT, std::move(inputComp)));
	}

	// create uniquw entity ID - if entity has a player component, ID will be zero. Otherwise, the next free ID is returned
	uint32_t entityID;
	if (state == EDIT_ENTITY) {
		entityID = ID;
	}
	else {
		if (compBits & PLAYER_BIT) {
			entityID = PLAYER_ENTITY_ID;
		}
		else {
			entityID = this->GetUID();
		}
	}

	// add entity ID and component data to map
	m_componentData.insert(std::make_pair(entityID, inner));

	// if a sprite has been added, inform the sprite manager 
	if ((compBits & POSITION_BIT) && (compBits & SPRITE_BIT)) {
		if (!this->UpdateSpriteData(state, entityID, mainView, mainScene)) {
			QMessageBox::information(window, "File error", "Error adding sprite data to screeen");
		}
	}

	// Finish by creating entity and adding ID and component mask
	auto& entity = std::make_unique<Entity>();
	entity->UpdateEntity(entityID, compBits);
	m_entityData.insert(std::make_pair(entityID, std::move(entity)));

	return true;
}

bool EntityManager::UpdateSpriteData(bool state, uint32_t entityID, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	// get required data from components
	glm::vec3 pos = std::dynamic_pointer_cast<PositionComponent>(m_componentData[entityID][POSITION_COMPONENT])->GetPosition();
	uint32_t index = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entityID][SPRITE_COMPONENT])->GetImageIndex();
	uint32_t type = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entityID][SPRITE_COMPONENT])->GetType();
	uint32_t repeat = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entityID][SPRITE_COMPONENT])->GetRepeatCount();
	float rotation = std::dynamic_pointer_cast<PositionComponent>(m_componentData[entityID][POSITION_COMPONENT])->GetRotation();
	
	// add data to sprite manager map
	if (!p_spriteManager->AddSpriteData(state, entityID, pos, index, type, repeat, rotation, mainView, mainScene)) {
		return false;
	}
	return true;
}

bool EntityManager::UpdateAllSpriteData(CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	// iterate through all the entites contained with the map and use this data to update the sprite manager 
	for (auto& entity : m_entityData) {
		uint32_t ID = entity.first;
		uint32_t compBits = entity.second->GetCompBits();
		if ((compBits & SPRITE_BIT) && (compBits & POSITION_BIT)) {
			if (!this->UpdateSpriteData(NEW_ENTITY, ID, mainView, mainScene)) {
				return false;
			}
		}
	}
	return true;
}


void EntityManager::AddEntityDataToWindow(uint32_t entID, NewEntityWin *window)
{
	uint32_t compBits = m_entityData[entID]->GetCompBits();

	if (compBits & POSITION_BIT) {
		window->UpdatePositionStatus(true);

		auto& posComp = std::dynamic_pointer_cast<PositionComponent>(m_componentData[entID][POSITION_COMPONENT]);
		glm::vec3 pos = posComp->GetPosition();
		float rot = posComp->GetRotation();
		window->SetPositionTabData(pos, rot);
	}

	if (compBits & VELOCITY_BIT) {
		window->UpdateVelocityStatus(true);
		
		auto& velComp = std::dynamic_pointer_cast<VelocityComponent>(m_componentData[entID][VELOCITY_COMPONENT]);
		glm::vec3 vel = velComp->GetVelocity();
		glm::vec3 buoy = velComp->GetBuoyancy();
		window->SetVelocityTabData(vel, buoy);
	}

	if (compBits & ACCELERATION_BIT) {
		window->UpdateAccelerationStatus(true);

		auto& accelComp = std::dynamic_pointer_cast<AccelerationComponent>(m_componentData[entID][ACCELERATION_COMPONENT]);
		glm::vec3 accel = accelComp->GetAcceleration();
		float mass = accelComp->GetMass();
		window->SetAccelerationTabData(accel, mass);
	}

	if (compBits & ANIMATION_BIT) {
		window->UpdateAnimationStatus(true);
		
		auto& animComp = std::dynamic_pointer_cast<AnimationComponent>(m_componentData[entID][ANIMATION_COMPONENT]);
		window->SetAnimationTabData(animComp->GetFrameCount(), animComp->GetStartIndex(), animComp->GetFrameIndex(), animComp->GetType());
	}

	if (compBits & HEALTH_COMPONENT_BIT) {
		window->UpdateHealthStatus(true);
		
		auto& healthComp = std::dynamic_pointer_cast<HealthComponent>(m_componentData[entID][HEALTH_COMPONENT]);
		window->SetHealthTabData(healthComp->GetMaxHealth(), healthComp->GetLives());
	}

	if (compBits & TIMED_LOGIC_BIT) {
		window->UpdateTimedStatus(true);
		
		auto& timerComp = std::dynamic_pointer_cast<TimerComponent>(m_componentData[entID][TIMED_LOGIC_COMPONENT]);
		auto& msgComp = std::dynamic_pointer_cast<MessageComponent>(m_componentData[entID][MESSAGE_COMPONENT]);
		window->SetTimedTabData(timerComp->IsConstantLoop(), timerComp->GetTimeOn(), timerComp->GetTimeOff(), msgComp->GetOnType(), msgComp->GetConstantOffType(), msgComp->GetLoopedOffType());
	}

	if (compBits & COLLISION_BIT) {
		window->UpdateCollisionStatus(true);
		
		auto& collComp = std::dynamic_pointer_cast<CollisionComponent>(m_componentData[entID][COLLISION_COMPONENT]);
		window->SetCollisionTabData(collComp->GetCollisionBits(), collComp->GetElasticity());
	}

	if (compBits & SPRITE_BIT) {
		window->UpdateSpriteStatus(true);

		auto& spriteComp = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entID][SPRITE_COMPONENT]);
		glm::vec2 size = spriteComp->GetDimensions();
		glm::vec2 uv = spriteComp->GetUvCoords();
		window->SetSpriteTabData(spriteComp->IsActive(), size, uv, spriteComp->GetImageIndex(), spriteComp->GetRepeatCount(), spriteComp->GetType());
	}

	if (compBits & PLAYER_BIT) {
		window->UpdatePlayerStatus(true);
		
		auto& playerComp = std::dynamic_pointer_cast<PlayerComponent>(m_componentData[entID][PLAYER_COMPONENT]);
		glm::vec3 moveAccel = playerComp->GetMoveAccel();
		glm::vec3 moveVel = playerComp->GetMoveVelocity();
		glm::vec3 jumpAccel = playerComp->GetJumpAccel();
		glm::vec3 jumpVel = playerComp->GetJumpVelocity();
		window->SetPlayerTabData(moveAccel, moveVel, jumpAccel, jumpVel);
	}
}

SpriteManager::SpriteData EntityManager::CreateEntityCopy(int32_t id)
{
	std::unordered_map<uint32_t, std::shared_ptr<Component> > inner;

	// start by generating a new unique ID and entity 
	uint32_t newID = this->GetUID();
	uint32_t compBits = m_entityData[id]->GetCompBits();

	// add to the map
	auto newEnt = std::make_unique<Entity>(newID, compBits);
	m_entityData.insert(std::make_pair(newID, std::move(newEnt)));

	// Now create new components and transfer the data from the original into the new entity components.
	// unfortuantely just copying the original into a new map wont work as the shared pointers will point to the same data
	if (compBits & POSITION_BIT) {

		auto& oldPos = std::dynamic_pointer_cast<PositionComponent>(m_componentData[id][POSITION_COMPONENT]);
		auto& newPos = std::make_shared<PositionComponent>(oldPos->GetPosition(), oldPos->GetRotation());
		inner.insert(std::make_pair(POSITION_COMPONENT, newPos));
	}

	if (compBits & VELOCITY_BIT) {

		auto& oldVel = std::dynamic_pointer_cast<VelocityComponent>(m_componentData[id][VELOCITY_COMPONENT]);
		auto& newVel = std::make_shared<VelocityComponent>(oldVel->GetVelocity(), oldVel->GetBuoyancy());
		inner.insert(std::make_pair(VELOCITY_COMPONENT, newVel));
	}

	if (compBits & ACCELERATION_BIT) {

		auto& oldAccel = std::dynamic_pointer_cast<AccelerationComponent>(m_componentData[id][ACCELERATION_COMPONENT]);
		auto& newAccel = std::make_shared<AccelerationComponent>(oldAccel->GetAcceleration(), oldAccel->GetMass());
		inner.insert(std::make_pair(ACCELERATION_COMPONENT, newAccel));
	}

	if (compBits & ANIMATION_BIT) {

		auto& oldAnim = std::dynamic_pointer_cast<AnimationComponent>(m_componentData[id][ANIMATION_COMPONENT]);
		auto& newAnim = std::make_shared<AnimationComponent>(oldAnim->GetFrameCount(), oldAnim->GetStartIndex(), oldAnim->GetFrameIndex(), oldAnim->GetType());
		inner.insert(std::make_pair(ANIMATION_COMPONENT, newAnim));
	}

	if (compBits & HEALTH_COMPONENT_BIT) {

		auto& oldHealth = std::dynamic_pointer_cast<HealthComponent>(m_componentData[id][HEALTH_COMPONENT]);
		auto& newHealth = std::make_shared<HealthComponent>(oldHealth->GetMaxHealth(), oldHealth->GetLives());
		inner.insert(std::make_pair(HEALTH_COMPONENT, newHealth));
	}

	if (compBits & TIMED_LOGIC_BIT) {

		auto& oldTimer = std::dynamic_pointer_cast<TimerComponent>(m_componentData[id][TIMED_LOGIC_COMPONENT]);
		auto& newTimer = std::make_shared<TimerComponent>(oldTimer->IsConstantLoop(), oldTimer->GetTimeOn(), oldTimer->GetTimeOff());
		inner.insert(std::make_pair(TIMED_LOGIC_COMPONENT, newTimer));

		auto& oldMsg = std::dynamic_pointer_cast<MessageComponent>(m_componentData[id][MESSAGE_COMPONENT]);
		auto& newMsg = std::make_shared<MessageComponent>(oldMsg->GetOnType(), oldMsg->GetConstantOffType(), oldMsg->GetLoopedOffType());
		inner.insert(std::make_pair(MESSAGE_COMPONENT, newMsg));
	}

	if (compBits & COLLISION_BIT) {

		auto& oldColl = std::dynamic_pointer_cast<CollisionComponent>(m_componentData[id][COLLISION_COMPONENT]);
		auto& newColl = std::make_shared<CollisionComponent>(oldColl->GetElasticity(), oldColl->GetCollisionBits());
		inner.insert(std::make_pair(COLLISION_COMPONENT, newColl));
	}

	if (compBits & SPRITE_BIT) {

		auto& oldSprite = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[id][SPRITE_COMPONENT]);
		auto& newSprite = std::make_shared<SpriteComponent>(oldSprite->GetDimensions(), oldSprite->GetUvCoords(), oldSprite->IsActive(), oldSprite->GetImageIndex(), oldSprite->GetRepeatCount(), oldSprite->GetType());
		inner.insert(std::make_pair(SPRITE_COMPONENT, newSprite));
	}

	if (compBits & PLAYER_BIT) {

		auto& oldPlayer = std::dynamic_pointer_cast<PlayerComponent>(m_componentData[id][PLAYER_COMPONENT]);
		auto& newPlayer = std::make_shared<PlayerComponent>(oldPlayer->GetMoveAccel(), oldPlayer->GetMoveVelocity(), oldPlayer->GetJumpAccel(), oldPlayer->GetJumpVelocity());
		inner.insert(std::make_pair(PLAYER_COMPONENT, newPlayer));
	}

	// insert newly created components into the map
	m_componentData.insert(std::make_pair(newID, inner));

	// Fill the sprite struct with data relevant to the copied entity for adding to the sprite manager
	SpriteManager::SpriteData sprite;
	sprite.entityID = newID;
	sprite.pos = std::dynamic_pointer_cast<PositionComponent>(inner[POSITION_COMPONENT])->GetPosition();
	sprite.index = std::dynamic_pointer_cast<SpriteComponent>(inner[SPRITE_COMPONENT])->GetImageIndex();
	sprite.repeatCount = std::dynamic_pointer_cast<SpriteComponent>(inner[SPRITE_COMPONENT])->GetRepeatCount();
	sprite.type = std::dynamic_pointer_cast<SpriteComponent>(inner[SPRITE_COMPONENT])->GetType();
	sprite.rotation = std::dynamic_pointer_cast<PositionComponent>(inner[POSITION_COMPONENT])->GetRotation();

	return sprite;
}

void EntityManager::DeleteEntityFromMap(int ID)
{
	m_componentData.erase(ID);
	m_entityData.erase(ID);

	// add the ID to be deleted for reuse later
	m_deletedIDs.push_back(ID);
}

bool EntityManager::FileSerialisedData(EntityManager *data, const std::string filename, SaveMode save_mode, std::ios_base::openmode flags, FileMode mode)
{
	bool success = true;
	std::fstream file;
	if (save_mode == SaveMode::SAVE_BINARY) {
		flags |= std::fstream::binary;
	}

	auto arch = new TextArchive(file, mode);
	file.open(filename, flags);
	if (!file.is_open()) {
		success = false;
	}
	else {
		Serialise(arch, *data, var_info(""));
	}
	return success;
}

// =======================================================================================================================================================================================
// Utility functions =====================================================================================================================================================================

void EntityManager::ConvertCoordsToVulkanQt(MainWindow::CoordSystem coordSys)
{
	for (auto& entity : m_entityData) {
		uint32_t compBits = entity.second->GetCompBits();
		if (compBits & POSITION_BIT) {
			glm::vec2 dim = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entity.first][SPRITE_COMPONENT])->GetDimensions();
			std::dynamic_pointer_cast<PositionComponent>(m_componentData[entity.first][POSITION_COMPONENT])->ConvertY(MainWindow::WORLD_HEIGHT, dim, coordSys);
		}
	}
}

void EntityManager::ConvertUvToVulkanQt(MainWindow::CoordSystem coordSys)
{
	for (auto& entity : m_entityData) {
		uint32_t compBits = entity.second->GetCompBits();
		if (compBits & SPRITE_BIT) {
			glm::vec2 dim = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entity.first][SPRITE_COMPONENT])->GetDimensions();
			glm::uint32_t type = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entity.first][SPRITE_COMPONENT])->GetType();

			SpriteManager::AtlasID atlasID = (type == MAIN_SPRITE) ? SpriteManager::AtlasID::MAIN_ATLAS : SpriteManager::AtlasID::BACKGROUND_ATLAS;
			std::dynamic_pointer_cast<SpriteComponent>(m_componentData[entity.first][SPRITE_COMPONENT])->ConvertY(p_spriteManager->GetTextureHeight(atlasID), dim, coordSys);
		}
	}
}

glm::vec2 EntityManager::ConvertCoords(glm::vec3 pos, uint32_t id)
{
	glm::vec2 dim = std::dynamic_pointer_cast<SpriteComponent>(m_componentData[id][SPRITE_COMPONENT])->GetDimensions();
	glm::vec2 newPos = glm::vec2(0.0f, (MainWindow::WORLD_HEIGHT - (pos.y + dim.y)));			// bottom left(0,0) = texHeight - (uvY + spriteHeight)
	return newPos;
}

uint32_t EntityManager::GetUID()
{
	uint32_t ID = 0;
	if (m_deletedIDs.empty()) {
		ID = ++m_guID;
	}
	else {
		ID = m_deletedIDs.back();
		m_deletedIDs.pop_back();
	}
	return ID;
}

uint32_t EntityManager::GetEntityCount()
{
	return m_entityData.size();
}


// ======================================================================================================================================================================================
// Component update functions ===========================================================================================================================================================

void EntityManager::UpdateSpriteComponent(uint32_t ID, QPointF scenePos)
{
	std::dynamic_pointer_cast<PositionComponent>(m_componentData[ID][POSITION_COMPONENT])->UpdatePosition(glm::vec3(scenePos.x(), scenePos.y(), 0.0f));
}

void EntityManager::UpdateSpriteComponent(uint32_t ID, glm::vec3 pos)
{
	std::dynamic_pointer_cast<PositionComponent>(m_componentData[ID][POSITION_COMPONENT])->UpdatePosition(pos);
}

void EntityManager::UpdatePositionComponent(uint32_t ID, float rotation)
{
	std::dynamic_pointer_cast<PositionComponent>(m_componentData[ID][POSITION_COMPONENT])->UpdateRotation(rotation);
}

// =====================================================================================================================================================================================
// Serialise functions for LEVEL class =================================================================================================================================================

void Serialise(Archive *arch, std::unordered_map<uint32_t, std::shared_ptr<Component> >& mp, const var_info& info)
{
	uint32_t type;
	uint32_t sz = static_cast<uint32_t>(mp.size());
	Serialise(arch, sz, var_info(info.name + ".size"));

	auto iter = mp.begin();
	for (uint32_t c = 0; c < sz; ++c) {
		if (mp.size() != sz) {

			Serialise(arch, type, var_info(info.name + "Key::[" + std::to_string(c) + "]"));
			switch (type) {
			case SPRITE_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<SpriteComponent>()));
				break;
			case POSITION_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<PositionComponent>()));
				break;
			case ANIMATION_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<AnimationComponent>()));
				break;
			case ACCELERATION_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<AccelerationComponent>()));
				break;
			case VELOCITY_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<VelocityComponent>()));
				break;
			case COLLISION_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<CollisionComponent>()));
				break;
			case INPUT_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<InputComponent>()));
				break;
			case PLAYER_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<PlayerComponent>()));
				break;
			case PLATFORM_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<PlatformComponent>()));
				break;
			case TIMED_LOGIC_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<TimerComponent>()));
				break;
			case MESSAGE_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<MessageComponent>()));
				break;
			case HEALTH_COMPONENT:
				mp.insert(std::make_pair(type, std::make_shared<HealthComponent>()));
				break;
			}
			Serialise(arch, *mp.at(type), var_info(info.name + "Data::[" + std::to_string(c) + "]"));
		}
		else {
			uint32_t type = iter->first;
			Serialise(arch, type, var_info(info.name + "Key::[" + std::to_string(c) + "]"));
			Serialise(arch, *iter->second, var_info(info.name + "Data::[" + std::to_string(c) + "]"));
		}
		++iter;
	}
}

void Serialise(Archive *arch, std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::shared_ptr<Component> > >& mp, const var_info& info)
{
	uint32_t ID;
	uint32_t sz = static_cast<uint32_t>(mp.size());
	Serialise(arch, sz, var_info(info.name + ".size"));
	std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::shared_ptr<Component> > >::iterator iter = mp.begin();
	for (uint32_t c = 0; c < sz; ++c) {
		if (mp.size() != sz) {

			std::unordered_map<uint32_t, std::shared_ptr<Component> > inner_mp;

			Serialise(arch, ID, var_info(info.name + "Key::[" + std::to_string(c) + "]"));
			Serialise(arch, inner_mp, var_info(info.name + "Data::[" + std::to_string(c) + "]"));

			mp.insert(std::make_pair(ID, std::move(inner_mp)));
		}
		else {
			ID = iter->first;

			Serialise(arch, ID, var_info(info.name + "Key::[" + std::to_string(c) + "]"));
			Serialise(arch, iter->second, var_info(info.name + "Data::[" + std::to_string(c) + "]"));
		}
		++iter;
	}
}

void Serialise(Archive *arch, std::unordered_map<uint32_t, std::unique_ptr<Entity> >& mp, const var_info& info)
{
	uint32_t ID;
	uint32_t sz = static_cast<uint32_t>(mp.size());
	Serialise(arch, sz, var_info(info.name + ".size"));

	std::unordered_map<uint32_t, std::unique_ptr<Entity> >::iterator iter = mp.begin();

	for (uint32_t c = 0; c < sz; ++c) {
		if (mp.size() != sz) {

			Serialise(arch, ID, var_info(info.name + "Key::[" + std::to_string(c) + "]"));

			mp.insert(std::make_pair(ID, std::make_unique<Entity>()));
			Serialise(arch, *mp.at(ID), var_info(info.name + "Data::[" + std::to_string(c) + "]"));
		}
		else {
			ID = iter->first;

			Serialise(arch, ID, var_info(info.name + "Key::[" + std::to_string(c) + "]"));
			Serialise(arch, *iter->second, var_info(info.name + "Data::[" + std::to_string(c) + "]"));
		}
		++iter;
	}
}

void Serialise(Archive *arch, EntityManager& ent, const var_info& info)
{
	Serialise(arch, ent.m_entityData, info.name + "m_entities");
	Serialise(arch, ent.m_componentData, info.name + "m_components");
}