#pragma once
#include "stdbool.h"
#include "lua.h"
#include "Macros.h"
#include "Types.h"
#include "Systems.h"

void StructBind_Init(lua_State* L) {
	StructBind_Init_RegistryTable(L);
	StructBind_Init_Systems(L);
}

void* StructBind_Allocator_FromUserdata(lua_State* L, StructBind_Userdata_Info* Info_Address) {
	return Info_Address+sizeof(StructBind_Userdata_Info);
}

int StructBind_MT_GC_Error(lua_State* L) {
	M_Userdata_Metamethod_GetInfo()
	assert(Info_Address->IsOwned);
	return 0;
}

int StructBind_MT_GC_Base(lua_State* L) {
	M_Userdata_Metamethod_GetInfo()
	if (Info_Address->Settings.IsOwnable) {
		if (!Info_Address->IsOwned) {
			return Info_Address->Settings.Destructor(L);
		}
		return 0;
	} else {
		return Info_Address->Settings.Destructor(L);
	}
	return 0;
}

int StructBind_MT_NewIndex_Base(lua_State* L) {
	M_Userdata_Metamethod_GetInfo();
	assert(!Info_Address->IsConst);
	return Info_Address->Settings.NewIndex(L);
}

void* StructBind_New_Userdata(lua_State* L, size_t Size) {
	return lua_newuserdata(L,sizeof(StructBind_Userdata_Info) + Size);
}

void StructBind_Wrapper_Setup_Metatable(lua_State* L, Lua_CMethod Attach_Metamethods) {
	lua_newtable(L);
		Attach_Metamethods(L);
		M_SetField(lua_pushcfunction(L,StructBind_MT_GC_Base),"__gc")
	lua_setmetatable(L,-2);
}

void StructBind_Wrap_NewStruct
	(
		lua_State* L, size_t DataSize, StructBind_Allocator Allocator, StructBind_Constructor Constructor, Lua_CMethod Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		StructBind_Userdata_Info* Info_Address = StructBind_New_Userdata(L,DataSize);
		Info_Address->IsOwned = false;
		Info_Address->Settings = Settings;
		Info_Address->Address = Allocator(L,Info_Address);
		Constructor(L,Info_Address->Address);
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Info_Address->Address,-1);
	}

void StructBind_Wrap_CopyStruct
	(
		lua_State* L, size_t DataSize, void* StructData, StructBind_Allocator Allocator, StructBind_Copy_Constructor Copy_Constructor, Lua_CMethod Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		StructBind_Userdata_Info* Info_Address = StructBind_New_Userdata(L,DataSize);
		Info_Address->IsOwned = false;
		Info_Address->Settings = Settings;
		Info_Address->Address = Allocator(L,Info_Address);
		Copy_Constructor(L,Info_Address->Address,StructData);
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Info_Address->Address,-1);
	}
void StructBind_Wrap_Pointer
	(
		lua_State* L, void* Address, Lua_CMethod Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		Settings.IsReference = true;
		StructBind_Userdata_Info* Info_Address = StructBind_New_Userdata(L,0);
		Info_Address->IsOwned = false;
		Info_Address->Settings = Settings;
		Info_Address->Address = Address;
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Info_Address->Address,-1);
	}
