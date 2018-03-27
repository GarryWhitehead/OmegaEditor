#include "position.h"
#include "components/sprite.h"
#include "utility\file_log.h"

void PositionComponent::ConvertY(float worldH, glm::vec2 dim, MainWindow::CoordSystem coordSys)
{
	if (coordSys == MainWindow::CoordSystem::BOTTOM_LEFT) {
		m_pos.y = worldH - (m_pos.y + dim.y);				// convert 0,0 to bottom-left
	}
	else if (coordSys == MainWindow::CoordSystem::TOP_LEFT) {
		m_pos.y = worldH - (m_pos.y - dim.y);
	}
}

void Serialise(Archive* arch, PositionComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_pos, var_info(info.name + ".m_pos"));
	Serialise(arch, comp.m_rotation, var_info(info.name + ".m_rotation"));
	Serialise(arch, comp.mStaticEntity, var_info(info.name + ".mStaticEntity"));
}
