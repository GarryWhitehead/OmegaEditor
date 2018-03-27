#include "sprite.h"
#include "glm.hpp"
#include "utility\file_log.h"

void SpriteComponent::ConvertY(float texHeight, glm::vec2 dim, MainWindow::CoordSystem coordSys)
{
	if (coordSys == MainWindow::CoordSystem::BOTTOM_LEFT) {
		m_uv.y = texHeight - (m_uv.y + dim.y);
	}
	else if (coordSys == MainWindow::CoordSystem::TOP_LEFT) {
		m_uv.y = texHeight - (m_uv.y - dim.y);
	}
	
}

void Serialise(Archive* arch, SpriteComponent& sprite, const var_info& info)
{
	Serialise(arch, sprite.m_is_active, var_info(info.name + ".is_active"));
	Serialise(arch, sprite.m_dim, var_info(info.name + ".m_dim"));
	Serialise(arch, sprite.m_uv, var_info(info.name + ".m_uv"));
	Serialise(arch, sprite.mIndex, var_info(info.name + ".mIndex"));
	Serialise(arch, sprite.mSpriteType, var_info(info.name + ".mSpriteType"));
	Serialise(arch, sprite.mRepeatSprite, var_info(info.name + ".mRepeatSprite"));
}

void Serialise(Archive *arch, glm::vec2& vec2, const var_info& info)
{
	Serialise(arch, vec2.x, var_info(info.name + ".x"));
	Serialise(arch, vec2.y, var_info(info.name + ".y"));
}

void Serialise(Archive *arch, glm::vec3& vec, const var_info& info)
{
	Serialise(arch, vec.x, var_info(info.name + ".x"));
	Serialise(arch, vec.y, var_info(info.name + ".y"));
	Serialise(arch, vec.z, var_info(info.name + ".z"));
}

