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

const char* KeepAliveKey = "KeepAlive";

void StructBind_Init_KeepAlive(lua_State* L) {
	StructBind_Init_RegistryKey(L,KeepAliveKey);
	lua_pop(L,1);
}

bool StructBind_Is_KeptAlive(lua_State* L, int ValueIndex) {
	StructBind_Push_RegistryKey(L,KeepAliveKey);
	M_GetTable(lua_pushvalue(L,M_Stack_Index(ValueIndex,1)))
	int Is_KeptAlive = lua_toboolean(L,-1);
	lua_pop(L,2);
	return Is_KeptAlive;
}

void StructBind_Enable_KeepAlive(lua_State* L, int ValueIndex) {
	assert(!StructBind_Is_KeptAlive(L,ValueIndex));
	StructBind_Push_RegistryKey(L,KeepAliveKey);
	M_SetTable(
		lua_pushvalue(L,M_Stack_Index(ValueIndex,1)),
		lua_pushboolean(L,true)
	)
	lua_pop(L,1);
}

void StructBind_Disable_KeepAlive(lua_State* L, int ValueIndex) {
	assert(StructBind_Is_KeptAlive(L,ValueIndex));
	StructBind_Push_RegistryKey(L,KeepAliveKey);
	M_SetTable(
		lua_pushvalue(L,M_Stack_Index(ValueIndex,1)),
		lua_pushnil(L)
	)
	lua_pop(L,1);
}
