/*
	System Purpose:
		Allows us to map and lookup userdata by its pointer.
*/

#pragma once
#include "../Types.h"
#include "../Registry.h"

static const char* UserdataMapKey = "UserdataMap";

static void StructBind_Init_UserdataMap(lua_State* L) {
	M_Print("Initializing Userdata map");
	StructBind_Init_RegistryKey(L,UserdataMapKey);
	StructBind_Set_Mode(L,"v");
	lua_pop(L,1);
}

static void StructBind_Map_Userdata(lua_State* L, void* Address, int Index) {
	M_To_Absolute(Index)
	StructBind_Push_RegistryKey(L,UserdataMapKey);
	M_SetTable(
		lua_pushlightuserdata(L,Address),
		lua_pushvalue(L,Index)
	)
	lua_pop(L,1);
	M_Print("Mapping pointer %p to userdata %p",Address,lua_touserdata(L,Index));
}

static void StructBind_Unmap_Userdata(lua_State* L, void* Address) {
	StructBind_Push_RegistryKey(L,UserdataMapKey);
	M_SetTable(
		lua_pushlightuserdata(L,Address),
		lua_pushnil(L)
	)
	lua_pop(L,1);
	M_Print("Unmapping pointer %p", Address);
}

static void StructBind_Lookup_Userdata(lua_State* L, void* Address) {
	StructBind_Push_RegistryKey(L,UserdataMapKey);
	M_GetTable(lua_pushlightuserdata(L,Address))
	lua_remove(L,-2);
}
