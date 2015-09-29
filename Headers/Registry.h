#pragma once
#include "lua.h"
#include "Macros.h"

static const char* RegistryDataKey = "StructBind_Data";

static void StructBind_Get_Registry(lua_State* L) {
	lua_getfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
}

static void StructBind_Push_RegistryKey(lua_State* L, const char* Key) {
	M_Print("Pushing registry table %s onto the stack",Key);
	lua_getfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
	lua_getfield(L,-1,Key);
	lua_remove(L,-2);
}

static void StructBind_Init_RegistryTable(lua_State* L) {
	M_Print("Initializing registry at %s",RegistryDataKey)
	lua_newtable(L);
	lua_setfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
}

static void StructBind_Init_RegistryKey(lua_State* L, const char* Key) {
	M_Print("Initializing registry key %s", Key);
	lua_getfield(L,LUA_REGISTRYINDEX,RegistryDataKey);
	M_SetField(lua_newtable(L),Key);
	M_GetField(Key);
	lua_remove(L,-2);
}

static void StructBind_Set_Mode(lua_State* L, const char* Mode) {
	M_Print("Setting mode %s", Mode);
	lua_newtable(L);
	M_SetField(lua_pushstring(L,Mode),"__mode");
	lua_setmetatable(L,-2);
}
