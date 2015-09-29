/*
	System Purpose:
		To keep objects alive that have left Lua scope.
		To prevent their referenced data from being garbage collected.
*/

#pragma once
#include "stdbool.h"
#include "assert.h"
#include "lua.h"
#include "../Registry.h"

static const char* KeepAliveKey = "KeepAlive";

static void StructBind_Init_KeepAlive(lua_State* L) {
	StructBind_Init_RegistryKey(L,KeepAliveKey);
	lua_pop(L,1);
}

static bool StructBind_Is_KeptAlive(lua_State* L, int ValueIndex) {
	M_To_Absolute(ValueIndex)
	StructBind_Push_RegistryKey(L,KeepAliveKey);
	M_GetTable(lua_pushvalue(L,ValueIndex))
	int Is_KeptAlive = lua_toboolean(L,-1);
	lua_pop(L,2);
	return Is_KeptAlive;
}

static void StructBind_Enable_KeepAlive(lua_State* L, int ValueIndex) {
	M_To_Absolute(ValueIndex)
	assert(!StructBind_Is_KeptAlive(L,ValueIndex));
	StructBind_Push_RegistryKey(L,KeepAliveKey);
	M_SetTable(
		lua_pushvalue(L,ValueIndex),
		lua_pushboolean(L,true)
	)
	lua_pop(L,1);
}

static void StructBind_Disable_KeepAlive(lua_State* L, int ValueIndex) {
	M_To_Absolute(ValueIndex);
	assert(StructBind_Is_KeptAlive(L,ValueIndex));
	StructBind_Push_RegistryKey(L,KeepAliveKey);
	M_SetTable(
		lua_pushvalue(L,ValueIndex),
		lua_pushnil(L)
	)
	lua_pop(L,1);
}
