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

void StructBind_Allocator_FromUserdata(lua_State* L, void* Userdata_Address) {
	StructBind_Userdata_GetInfo(Userdata_Address)->Address = StructBind_Userdata_GetDataRegion(Userdata_Address);
}

int StructBind_LuaFunction_NoOp(lua_State* L) {
	return 0;
}

int StructBind_MT_GC_Error(lua_State* L) {
	M_Userdata_Metamethod_GetInfo()
	assert(Info_Address->IsOwned);
	return 0;
}

int StructBind_MT_GC_Base(lua_State* L) {
	M_Userdata_Metamethod_GetSettings()
	if (Settings_Address->IsOwnable) {
		if (!Info_Address->IsOwned) {
			return Settings_Address->Destroy(L);
		}
		return 0;
	} else {
		return Settings_Address->Destroy(L);
	}
	return 0;
}

int StructBind_MT_NewIndex_Base(lua_State* L) {
	M_Userdata_Metamethod_GetSettings()
	assert(!Info_Address->IsConst);
	return Settings_Address->Modify(L);
}

void* StructBind_New_Userdata(lua_State* L, StructBind_Userdata_Settings Settings, size_t DataSize) {
	void* Address = lua_newuserdata(L,sizeof(StructBind_Userdata_Info) + sizeof(StructBind_Userdata_Settings) + DataSize);
	
	StructBind_Userdata_Info* Info_Address = StructBind_Userdata_GetInfo(Address);
	*Info_Address = (StructBind_Userdata_Info){false,false,false,NULL};
	
	StructBind_Userdata_Settings* Settings_Address = StructBind_Userdata_GetSettings(Address);
	*Settings_Address = Settings;
	
	return Address;
}

void StructBind_Wrapper_Setup_Metatable(lua_State* L, lua_CFunction Attach_Metamethods) {
	lua_newtable(L);
		Attach_Metamethods(L);
		M_SetField(lua_pushcfunction(L,StructBind_MT_NewIndex_Base),"__newindex")
		M_SetField(lua_pushcfunction(L,StructBind_MT_GC_Base),"__gc")
	lua_setmetatable(L,-2);
}

void StructBind_Wrap_New
	(
		lua_State* L, size_t DataSize, StructBind_Allocator Allocator, StructBind_Constructor Constructor, lua_CFunction Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		void* Userdata_Address = StructBind_New_Userdata(L,Settings,DataSize);
		StructBind_Userdata_Info* Info_Address = StructBind_Userdata_GetInfo(Userdata_Address);
		Allocator(L,Userdata_Address);
		Constructor(L,Info_Address->Address);
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Info_Address->Address,-1);
	}

void StructBind_Wrap_Copy
	(
		lua_State* L, size_t DataSize, void* StructData, StructBind_Allocator Allocator, StructBind_Copy_Constructor Copy_Constructor, lua_CFunction Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		StructBind_Userdata_Info* Info_Address = StructBind_New_Userdata(L,Settings,DataSize);
		Info_Address->IsOwned = false;
		Allocator(L,Info_Address);
		Copy_Constructor(L,Info_Address->Address,StructData);
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Info_Address->Address,-1);
	}
void StructBind_Wrap_Pointer
	(
		lua_State* L, void* Address, lua_CFunction Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		StructBind_Userdata_Info* Info_Address = StructBind_New_Userdata(L,Settings,0);
		Info_Address->IsReference = true;
		Info_Address->IsNative = false;
		Info_Address->Address = Address;
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Info_Address->Address,-1);
	}
