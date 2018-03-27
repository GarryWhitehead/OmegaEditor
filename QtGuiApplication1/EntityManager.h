#pragma once
#include <unordered_map>
#include <memory>
#include "ui_NewEntity.h"
#include "utility/serialisation.h"
#include "OmegaEngineEditor.h"
#include "glm.hpp"
#include "SpriteManager.h"

class Component;
class Entity;
class EntityManager;
class CustomGraphicsView;
class NewEntityWin;

extern EntityManager *entityManager;

class EntityManager
{
public:

	static const bool NEW_ENTITY = false;
	static const bool EDIT_ENTITY = true;
	static const int PLAYER_ENTITY_ID = 0;

	EntityManager();
	~EntityManager();

	uint32_t GetUID();

	bool AddEntity(bool state, uint32_t ID, NewEntityWin *window, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	void AddEntityDataToWindow(uint32_t entID, NewEntityWin *window);
	void DeleteEntityFromMap(int ID);
	uint32_t GetEntityCount();
	bool FileSerialisedData(EntityManager *data, const std::string filename, SaveMode save_mode, std::ios_base::openmode flags, FileMode mode);
	bool UpdateSpriteData(bool state, uint32_t entityID, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	bool UpdateAllSpriteData(CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	void ConvertCoordsToVulkanQt(MainWindow::CoordSystem coordSys);
	void ConvertUvToVulkanQt(MainWindow::CoordSystem coordSys);
	glm::vec2 ConvertCoords(glm::vec3 pos, uint32_t id);
	SpriteManager::SpriteData CreateEntityCopy(int32_t id);

	// component update functions
	void UpdateSpriteComponent(uint32_t ID, QPointF scenePos);
	void UpdateSpriteComponent(uint32_t ID, glm::vec3 pos);
	void UpdatePositionComponent(uint32_t ID, float rotation);

	// for serialisation purposes
	friend void Serialise(Archive *arch, EntityManager& ent, const var_info& info);

	friend class ListEntityWin;

private:

	uint32_t m_guID;
	std::vector<uint32_t> m_deletedIDs;

	std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::shared_ptr<Component> > > m_componentData;

	std::unordered_map<uint32_t, std::unique_ptr<Entity> > m_entityData;
};



