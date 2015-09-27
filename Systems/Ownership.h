/*
	System Purpose:
		To manage ownership over memory and keep track of who owns what.
		To help ensure that memory is not double freed, or leaked, or freed while referenced.
*/

#pragma once
#include "stdbool.h"
#include "assert.h"
#include "../Registry.h"
#include "../Types.h"

const char* OwnershipMapKey = "OwnershipMap";

void StructBind_Init_OwnershipMap(lua_State* L) {
	StructBind_Init_RegistryKey(L,OwnershipMapKey); //Ownership is mapped Child->Parent
	//The child keys are weak, this prevents gc of parent objects as long as the child object still has a strong reference to it somewhere
	//Preventing the parent from being collected until the child is fully dereferenced, which prevents the parent from cleaning up the childs memory while it's still in use
	StructBind_Set_Mode(L,"k");
	lua_pop(L,1);
}

bool StructBind_Is_Owner(lua_State* L, int OwnerIndex, int ChildIndex) {
	StructBind_Userdata_Info* Owner_Info = lua_touserdata(L,OwnerIndex);
	StructBind_Userdata_Info* Child_Info = lua_touserdata(L,ChildIndex);
	if (!Child_Info->IsOwned) {
		return false;
	}
	StructBind_Push_RegistryKey(L,OwnershipMapKey);
	M_GetTable(lua_pushvalue(L,M_Stack_Index(ChildIndex,1)))
	int IsOwner = lua_compare(L,-1,M_Stack_Index(OwnerIndex,2),LUA_OPEQ);
	lua_pop(L,2);
	return IsOwner;
}

void StructBind_Take_Ownership(lua_State* L, int OwnerIndex, int ChildIndex) {
	StructBind_Userdata_Info* Owner_Info = lua_touserdata(L,OwnerIndex);
	StructBind_Userdata_Info* Child_Info = lua_touserdata(L,ChildIndex);
	assert(Child_Info->Settings.IsOwnable && Child_Info->Settings.IsReference);
	assert(!Child_Info->IsOwned);
	StructBind_Push_RegistryKey(L,OwnershipMapKey);
	M_SetTable(
		lua_pushvalue(L,M_Stack_Index(ChildIndex,1)),
		lua_pushvalue(L,M_Stack_Index(OwnerIndex,2))
	)
	lua_pop(L,1);
	Child_Info->IsOwned = true;
}

void StructBind_Release_Ownership(lua_State* L, int OwnerIndex, int ChildIndex) {
	StructBind_Userdata_Info* Owner_Info = lua_touserdata(L,OwnerIndex);
	StructBind_Userdata_Info* Child_Info = lua_touserdata(L,ChildIndex);
	assert(Child_Info->IsOwned);
	assert(StructBind_Is_Owner(L,OwnerIndex,ChildIndex));
	StructBind_Push_RegistryKey(L,OwnershipMapKey);
	M_SetTable(
		lua_pushvalue(L,M_Stack_Index(ChildIndex,1)),
		lua_pushnil(L)
	)
	lua_pop(L,1);
	Child_Info->IsOwned = false;
}
