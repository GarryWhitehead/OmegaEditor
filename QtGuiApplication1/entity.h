#ifndef _ENTITY_H
#define _ENTITY_H
#include <unordered_map>
#include <memory>
#include <vector>
#include <assert.h>
#include "utility/serialisation.h"

class Entity
{
public:

	Entity() : mCompReference(0), mUniqueID(0), mIsActive(true) {}
	Entity(uint32_t ID, uint32_t ref) : mUniqueID(ID), mCompReference(ref) {}

	// helper functions
	void UpdateEntity(uint32_t ID, uint32_t compBits) { mCompReference = compBits; mUniqueID = ID; }
	uint32_t GetCompBits() { return mCompReference; }

	// functions for serialisation of entity data
	friend void Serialise(Archive *arch, Entity& e, const var_info& info);

private:

	uint32_t mCompReference;
	uint32_t mUniqueID;
	bool mIsActive;
};

static void Serialise(Archive *arch, Entity& e, const var_info& info)
{
	Serialise(arch, e.mIsActive, var_info(info.name + ".m_active"));
	Serialise(arch, e.mCompReference, var_info(info.name + "m_component_ref"));
	Serialise(arch, e.mUniqueID, var_info("m_components"));
}

#endif // _GAMEOBJECT