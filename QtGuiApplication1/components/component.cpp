#include "component.h"

// serialisation function
void Serialise(Archive *arch, Component& comp, const var_info& info)
{
	comp.pack_unpack(arch, info);
	//Serialise(arch, static_cast<int32_t>(comp.m_type), var_info(info.name + ".m_type"));
}

void Component::pack_unpack(Archive *arch, const var_info& info)
{}
