#pragma once
#include "component.h"

class InputComponent : public ArchivableComponent<InputComponent>
{
public:

	InputComponent() : ArchivableComponent<InputComponent>(*this, INPUT_COMPONENT) {}

	friend void Serialise(Archive* arch, InputComponent& comp, const var_info& info);

private:

	uint32_t m_empty;
};