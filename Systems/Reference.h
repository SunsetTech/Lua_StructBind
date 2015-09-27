/*
	System Purpose:
		To keep track of what objects hold references to other objects.
		To prevent reference memory from being GCd until it is no longer referenced.
*/

#pragma once
#include "assert.h"
#include "../Types.h"
#include "../Registry.h"

const char* ReferenceMapKey = "ReferenceMap";

void StructBind_Init_ReferenceMap(lua_State* L) {
	StructBind_Init_RegistryKey(L,ReferenceMapKey);
	StructBind_Set_Mode(L,"k");
	lua_pop(L,1);
}

void StructBind_Ensure_ReferenceMap_Exists(lua_State* L, int ReferencerIndex) {
	StructBind_Push_RegistryKey(L,ReferenceMapKey);
	lua_pushvalue(L,M_Stack_Index(ReferencerIndex,1));
	lua_gettable(L,-2);
	if (lua_isnoneornil(L,-1)) {
		lua_pop(L,1);
		M_SetTable(
			lua_pushvalue(L,M_Stack_Index(ReferencerIndex,1)),
			lua_newtable(L)
		)
	} else {
		lua_pop(L,1);
	}
	lua_pop(L,1);
}

void StructBind_Retrieve_ReferenceTable(lua_State* L, int ReferencerIndex) {
	StructBind_Ensure_ReferenceMap_Exists(L,ReferencerIndex);
	StructBind_Push_RegistryKey(L,ReferenceMapKey);
	M_GetTable(lua_pushvalue(L,M_Stack_Index(ReferencerIndex,1)))
	lua_remove(L,-2);
}

void StructBind_Add_Reference(lua_State* L, int ReferencerIndex, int ID, int ReferenceIndex) {
	StructBind_Userdata_Info* Referencer_Info = lua_touserdata(L,ReferencerIndex);
	StructBind_Userdata_Info* Reference_Info = lua_touserdata(L,ReferenceIndex);
	assert(Reference_Info->Settings.IsReferenceable);

	StructBind_Retrieve_ReferenceTable(L, ReferencerIndex);
		M_SetTable(
			lua_pushnumber(L,ID),
			lua_pushvalue(L,M_Stack_Index(ReferenceIndex,2))
		)
	lua_pop(L,1);
}

void StructBind_Drop_Reference(lua_State* L, int ReferencerIndex, int ID) {
	StructBind_Retrieve_ReferenceTable(L,ReferencerIndex);
		M_SetTable(
			lua_pushnumber(L,ID),
			lua_pushnil(L)
		)
	lua_pop(L,1);
}
