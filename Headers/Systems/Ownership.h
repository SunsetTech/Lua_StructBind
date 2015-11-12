/*
	System Purpose:
		To manage ownership over memory and keep track of who owns what.
		To help ensure that memory is not double freed, or leaked, or freed while referenced.
*/

#pragma once
#include "lauxlib.h"
#include "stdbool.h"
#include "assert.h"
#include "../Registry.h"
#include "../Types.h"
#include "UserdataMap.h"

static const char* OwnershipMapKey = "OwnershipMap";

static void StructBind_Init_OwnershipMap(lua_State* L) {
	M_Print("Initializing ownership map");
	StructBind_Init_RegistryKey(L,OwnershipMapKey); //Ownership is mapped Child->Parent
	//The child keys are weak, this prevents gc of parent objects as long as the child object still has a strong reference to it somewhere
	//Preventing the parent from being collected until the child is fully dereferenced, which prevents the parent from cleaning up the childs memory while it's still in use
	StructBind_Set_Mode(L,"k");
	lua_pop(L,1);
}

static void StructBind_Get_Owner(lua_State* L, int Index) {
	M_To_Absolute(Index)
	StructBind_Push_RegistryKey(L,OwnershipMapKey);
	M_GetTable(lua_pushvalue(L,Index))
	lua_remove(L,-2);
}

static bool StructBind_Is_Owner(lua_State* L, int OwnerIndex, int ChildIndex) {
	M_To_Absolute(OwnerIndex) M_To_Absolute(ChildIndex)
	StructBind_Userdata* Child = (StructBind_Userdata*) lua_touserdata(L,ChildIndex);
	if (!Child->Settings.IsOwnable) {
		return false;
	}
	StructBind_Get_Owner(L,ChildIndex);
	int IsOwner = lua_compare(L,-1,OwnerIndex,LUA_OPEQ);
	lua_pop(L,1);
	return IsOwner;
}

static void StructBind_Take_Ownership(lua_State* L, int OwnerIndex, int ChildIndex) {
	M_Print("Giving ownership over %p to %p",lua_touserdata(L,ChildIndex),lua_touserdata(L,OwnerIndex))
	M_To_Absolute(OwnerIndex) M_To_Absolute(ChildIndex)
	StructBind_Userdata* Child = (StructBind_Userdata*) lua_touserdata(L,ChildIndex);
	if (Child->Info.IsNative) luaL_error(L,"Cannot take ownership over native object %p",Child);
	if (!Child->Settings.IsOwnable) luaL_error(L,"%p is not ownable",Child);
	if (Child->Info.IsOwned) luaL_error(L,"%p is already owned",Child);
	StructBind_Push_RegistryKey(L,OwnershipMapKey);
	M_SetTable(
		lua_pushvalue(L,ChildIndex),
		lua_pushvalue(L,OwnerIndex)
	)
	lua_pop(L,1);
	Child->Info.IsOwned = true;
}

static void StructBind_Release_Ownership(lua_State* L, int OwnerIndex, int ChildIndex) {
	M_Print("Releasing ownership of %p over %p",lua_touserdata(L,ChildIndex),lua_touserdata(L,OwnerIndex));
	M_To_Absolute(OwnerIndex) M_To_Absolute(ChildIndex)
	StructBind_Userdata* Child = (StructBind_Userdata*) lua_touserdata(L,ChildIndex);
	if (!Child->Info.IsOwned) luaL_error(L,"%p is not owned",Child);
	if (!StructBind_Is_Owner(L,OwnerIndex,ChildIndex)) luaL_error(L,"%p does not own %p",lua_touserdata(L,OwnerIndex),Child);
	StructBind_Push_RegistryKey(L,OwnershipMapKey);
	M_SetTable(
		lua_pushvalue(L,ChildIndex),
		lua_pushnil(L)
	)
	lua_pop(L,1);
	Child->Info.IsOwned = false;
}

static void StructBind_Swap_Ownership(lua_State* L, int OwnerIndex, void* FromAddress, int ToIndex) {
	M_To_Absolute(OwnerIndex) M_To_Absolute(ToIndex)
	M_If_Wrapped(
		FromAddress,
		M_Print("Swapping ownership of %p over %p to %p",lua_touserdata(L,OwnerIndex),FromAddress,lua_touserdata(L,ToIndex))
		StructBind_Release_Ownership(L,OwnerIndex,-1);
		lua_pop(L,1);
	)
	StructBind_Take_Ownership(L,OwnerIndex,ToIndex);
}
