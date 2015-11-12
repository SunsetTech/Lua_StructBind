/*
	System Purpose:
		To keep track of what objects hold references to other objects.
		To prevent reference memory from being GCd until it is no longer referenced.
*/

#pragma once
#include "assert.h"
#include "../Types.h"
#include "../Registry.h"
#include "UserdataMap.h"

static const char* ReferenceMapKey = "ReferenceMap";

static void StructBind_Init_ReferenceMap(lua_State* L) {
	M_Print("Initializing reference map");
	StructBind_Init_RegistryKey(L,ReferenceMapKey);
	StructBind_Set_Mode(L,"k");
	lua_pop(L,1);
}

static void StructBind_Ensure_ReferenceMap_Exists(lua_State* L, int ReferencerIndex) {
	M_To_Absolute(ReferencerIndex);
	M_Print("Ensuring reference map exists for %p",lua_touserdata(L,ReferencerIndex));
	StructBind_Push_RegistryKey(L,ReferenceMapKey);
	lua_pushvalue(L,ReferencerIndex);
	lua_gettable(L,-2);
	if (lua_isnoneornil(L,-1)) {
		lua_pop(L,1);
		M_SetTable(
			lua_pushvalue(L,ReferencerIndex),
			lua_newtable(L)
		)
	} else {
		lua_pop(L,1);
	}
	lua_pop(L,1);
}

static void StructBind_Retrieve_ReferenceTable(lua_State* L, int ReferencerIndex) {
	M_To_Absolute(ReferencerIndex)
	M_Print("Retrieiving reference map for %p", lua_touserdata(L,ReferencerIndex));
	StructBind_Ensure_ReferenceMap_Exists(L,ReferencerIndex);
	StructBind_Push_RegistryKey(L,ReferenceMapKey);
	M_GetTable(lua_pushvalue(L,ReferencerIndex))
	lua_remove(L,-2);
}

static void StructBind_Add_Reference(lua_State* L, int ReferencerIndex, int ID, int ReferenceIndex) {
	M_To_Absolute(ReferencerIndex);
	M_To_Absolute(ReferenceIndex);
	StructBind_Userdata* Reference = (StructBind_Userdata*) lua_touserdata(L,ReferenceIndex);
	if (!Reference->Settings.IsReferenceable) {
		luaL_error(L,"Cannot reference %p",Reference);
	}
	StructBind_Retrieve_ReferenceTable(L, ReferencerIndex);
	M_Print("Adding reference to %p from %p with ID %i",Reference,lua_touserdata(L,ReferencerIndex),ID)
		M_SetTable(
			lua_pushnumber(L,ID),
			lua_pushvalue(L,ReferenceIndex)
		)
	lua_pop(L,1);
}

static void StructBind_Get_Reference(lua_State* L, int ReferencerIndex, int ID) {
	StructBind_Retrieve_ReferenceTable(L,ReferencerIndex);
	M_Print("Retrieving reference ID %s for %p",ID,lua_touserdata(L,ReferencerIndex));
	M_GetTable(lua_pushnumber(L,ID));
	lua_remove(L,-2);
}

static void StructBind_Copy_References(lua_State* L, int FromIndex, int ToIndex) {
	M_Print("Copying references from %p to %p", lua_touserdata(L,FromIndex), lua_touserdata(L,ToIndex))
	M_To_Absolute(FromIndex) M_To_Absolute(ToIndex)
	StructBind_Retrieve_ReferenceTable(L,FromIndex);
	M_Iterate_Table(
		-1,
		int ID = lua_tonumber(L,-2);
		StructBind_Add_Reference(L,ToIndex,ID,-1);
	);
}

static void StructBind_Drop_Reference(lua_State* L, int ReferencerIndex, int ID) {
	StructBind_Retrieve_ReferenceTable(L,ReferencerIndex);
	M_Print("Dropping reference ID %i for %p",ID,lua_touserdata(L,ReferencerIndex));
		M_SetTable(
			lua_pushnumber(L,ID),
			lua_pushnil(L)
		)
	lua_pop(L,1);
}

static void StructBind_Swap_Reference(lua_State* L, int ReferencerIndex, int ID, void* FromAddress, int ToIndex) {
	M_To_Absolute(ReferencerIndex) M_To_Absolute(ToIndex)
	M_If_Wrapped(
		FromAddress,
		M_Print("Swapping reference ID %i for %p from %p to %p",ID,lua_touserdata(L,ReferencerIndex),FromAddress,lua_touserdata(L,ToIndex));
		StructBind_Drop_Reference(L,ReferencerIndex,ID);
		lua_pop(L,1);
	)
	StructBind_Add_Reference(L,ReferencerIndex,ID,ToIndex);
}
