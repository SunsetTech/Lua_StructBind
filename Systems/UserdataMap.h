/*
	System Purpose:
		Allows us to map and lookup userdata by its pointer.
*/

#pragma once
#include "../Registry.h"

const char* UserdataMapKey = "UserdataMap";

void StructBind_Init_UserdataMap(lua_State* L) {
	StructBind_Init_RegistryKey(L,UserdataMapKey);
	StructBind_Set_Mode(L,"v");
	lua_pop(L,1);
}

void StructBind_Map_Userdata(lua_State* L, void* Address, int ValueIndex) {
	StructBind_Push_RegistryKey(L,UserdataMapKey);
	M_SetTable(
		lua_pushlightuserdata(L,Address),
		lua_pushvalue(L,ValueIndex-2)
	)
	lua_pop(L,1);
}

void StructBind_Unmap_Userdata(lua_State* L, void* Address) {
	StructBind_Push_RegistryKey(L,UserdataMapKey);
	M_SetTable(
		lua_pushlightuserdata(L,Address),
		lua_pushnil(L)
	)
	lua_pop(L,1);
}

void StructBind_Lookup_Userdata(lua_State* L, void* Address) {
	StructBind_Push_RegistryKey(L,UserdataMapKey);
	M_GetTable(lua_pushlightuserdata(L,Address))
	lua_remove(L,-2);
}
