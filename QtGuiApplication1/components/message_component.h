#pragma once
#include "component.h"
#include <functional>

const uint32_t DISABLE_ENTITY_MSG_ID = 0;
const uint32_t DESTROY_ENTITY_MSG_ID = 1;
const uint32_t OUT_OF_TIME_MSG_ID = 2;
const uint32_t NO_MSG_ID = 3;

class MessageComponent : public ArchivableComponent<MessageComponent>
{
public:	
	MessageComponent() : ArchivableComponent<MessageComponent>(*this, MESSAGE_COMPONENT) {}

	MessageComponent(uint32_t onMsg, uint32_t loopedOff, uint32_t constantOff) :
		ArchivableComponent<MessageComponent>(*this, MESSAGE_COMPONENT),
		m_onMsgID(onMsg),
		m_loopedOffMsgID(loopedOff),
		m_constantOffMsgID(constantOff)
	{}

	~MessageComponent() {}

	uint32_t GetOnType() const { return m_onMsgID; }
	uint32_t GetConstantOffType() const { return m_constantOffMsgID; }
	uint32_t GetLoopedOffType() const { return m_loopedOffMsgID; }

	// the serialise functions need to be able to access the private variables
	friend void Serialise(Archive *arch, MessageComponent& comp, const var_info& info);

private:
	uint32_t m_onMsgID;
	uint32_t m_constantOffMsgID;
	uint32_t m_loopedOffMsgID;
};