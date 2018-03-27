#include "components/message_component.h"

void Serialise(Archive* arch, MessageComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_onMsgID, var_info(info.name + ".m_onMsgID"));
	Serialise(arch, comp.m_constantOffMsgID, var_info(info.name + ".m_constantOffMsgID"));
	Serialise(arch, comp.m_loopedOffMsgID, var_info(info.name + ".m_loopedOffMsgID"));
}