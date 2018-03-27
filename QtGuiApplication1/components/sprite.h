#ifndef _SPRITE_H
#define _SPRITE_H
#include "component.h"
#include "glm.hpp"
#include "OmegaEngineEditor.h"
#include <unordered_map>

class SpriteComponent;

const uint32_t BACKGROUND_SPRITE = 0;
const uint32_t MAIN_SPRITE = 1;

class SpriteComponent : public ArchivableComponent<SpriteComponent>
{
public:
	SpriteComponent() : ArchivableComponent<SpriteComponent>(*this, SPRITE_COMPONENT) {}

	SpriteComponent(glm::vec2 size, glm::vec2 uv, bool active, uint32_t ind, uint32_t type, uint32_t repeat) : 
		ArchivableComponent<SpriteComponent>(*this, SPRITE_COMPONENT), 
		m_dim(size),
		m_uv(uv),
		m_is_active(active),
		mIndex(ind),
		mSpriteType(type),
		mRepeatSprite(repeat)
	{}

	~SpriteComponent() {}

	glm::vec2 GetDimensions() const { return m_dim; }
	glm::vec2 GetUvCoords() const { return m_uv; }
	uint32_t GetImageIndex() const { return mIndex; }
	uint32_t GetType() const { return mSpriteType; }
	uint32_t GetRepeatCount() const { return mRepeatSprite; }
	bool IsActive() const { return m_is_active; }
	void ConvertY(float texHeight, glm::vec2 dim, MainWindow::CoordSystem coordSys);

	// the serialise functions need to be able to access the private variables
	friend void Serialise(Archive *arch, SpriteComponent& sprite, const var_info& info);

private:
	bool m_is_active;
	glm::vec2 m_dim;
	glm::vec2 m_uv;
	uint32_t mIndex;
	uint32_t mSpriteType;
	uint32_t mRepeatSprite;
};

extern void Serialise(Archive *arch, glm::vec2& vec2, const var_info& info);
extern void Serialise(Archive *arch, glm::vec3& vec2, const var_info& info);

#endif // _SPRITE_H
