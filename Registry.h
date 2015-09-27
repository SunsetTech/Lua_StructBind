#pragma once
#include "lua.h"
#include "Macros.h"

const char* RegistryDataKey = "StructBind_Data";

void StructBind_Push_RegistryKey(lua_State* L, const char* Key) {
	lua_getfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
	lua_getfield(L,-1,Key);
	lua_remove(L,-2);
}

void StructBind_Init_RegistryTable(lua_State* L) {
	lua_newtable(L);
	lua_setfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
}

void StructBind_Init_RegistryKey(lua_State* L, const char* Key) {
	lua_getfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
	M_SetField(lua_newtable(L),Key);
	M_GetField(Key);
	lua_remove(L,-2);
}

void StructBind_Set_Mode(lua_State* L, const char* Mode) {
	lua_newtable(L);
	M_SetField(lua_pushstring(L,Mode),"__mode");
	lua_setmetatable(L,-2);
}
