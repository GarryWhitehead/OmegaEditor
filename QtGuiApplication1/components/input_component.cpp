#include "input_component.h"

void Serialise(Archive* arch, InputComponent& comp, const var_info& info)
{
	Serialise(arch, comp.m_empty, var_info(info.name + ".m_empty"));
}
