#pragma once
#include "stdbool.h"
#include "lua.h"
#include "lauxlib.h"
#include "Macros.h"
#include "Types.h"
#include "Systems.h"

static void StructBind_Init(lua_State* L) {
	M_Print("Initializing StructBind");
	StructBind_Get_Registry(L);
	if (lua_isnoneornil(L,-1)) {
		lua_pop(L,-1);
		StructBind_Init_RegistryTable(L);
		StructBind_Init_Systems(L);
	} else {
		lua_pop(L,1);
	}
}

static void StructBind_Allocator_FromUserdata(lua_State* L, StructBind_Userdata* Userdata) {
	Userdata->Info.Address = StructBind_Userdata_GetDataRegion(Userdata);
	Userdata->Info.IsNative = true;
	M_Print("Allocated from Userdata using address %p",Userdata->Info.Address)
}

static int StructBind_LuaFunction_NoOp(lua_State* L) {
	return 0;
}

static int StructBind_MT_GC_Error(lua_State* L) {
	M_Userdata_Metamethod_GetUserdata()
	if (!Userdata->Info.IsOwned) {
		luaL_error(L,"Leaked memory at %p",Userdata->Info.Address);
	}
	return 0;
}

static int StructBind_MT_GC_Base(lua_State* L) {
	M_Userdata_Metamethod_GetUserdata()
	M_Print("Attempting to destroy %p",Userdata);
	if (Userdata->Settings.IsOwnable) {
		if (!Userdata->Info.IsOwned) {
			M_Print("Calling the destructor");
			return Userdata->Settings.Destroy(L);
		} else {
			M_Print("The object is owned, not calling the destructor");
		}
		return 0;
	} else {
		M_Print("Calling the destructor");
		return Userdata->Settings.Destroy(L);
	}
	return 0;
}

static int StructBind_MT_NewIndex_Base(lua_State* L) {
	M_Userdata_Metamethod_GetUserdata()
	if (Userdata->Settings.IsConst) {
		luaL_error(L,"Cannot modify const value %p",Userdata);
	}
	return Userdata->Settings.Modify(L);
}

static StructBind_Userdata* StructBind_New_Userdata(lua_State* L, StructBind_Userdata_Settings Settings, size_t DataSize) {
	M_Print("Allocating a new userdata with %i bytes of additional space",DataSize);
	StructBind_Userdata* Userdata = lua_newuserdata(L,sizeof(StructBind_Userdata) + DataSize);
	
	Userdata->Info = (StructBind_Userdata_Info){false,false,NULL};
	Userdata->Settings = Settings;
	return Userdata;
}

static void StructBind_Wrapper_Setup_Metatable(lua_State* L, lua_CFunction Attach_Metamethods) {
	lua_newtable(L);
		Attach_Metamethods(L);
		M_SetField(lua_pushcfunction(L,StructBind_MT_NewIndex_Base),"__newindex")
		M_SetField(lua_pushcfunction(L,StructBind_MT_GC_Base),"__gc")
	lua_setmetatable(L,-2);
}

static void StructBind_Wrap_New
	(
		lua_State* L, size_t DataSize, StructBind_Allocator Allocator, StructBind_Constructor Constructor, lua_CFunction Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		M_Print("Constructing a new wrapped object of size %i", DataSize);
		StructBind_Userdata* Userdata = StructBind_New_Userdata(L,Settings,DataSize);
		Allocator(L,Userdata);
		Constructor(L,Userdata->Info.Address);
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Userdata->Info.Address,-1);
	}

static void StructBind_Wrap_Copy
	(
		lua_State* L, size_t DataSize, void* StructData, StructBind_Allocator Allocator, StructBind_Copy_Constructor Copy_Constructor, lua_CFunction Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		StructBind_Userdata* Userdata = StructBind_New_Userdata(L,Settings,DataSize);
		Userdata->Info.IsOwned = false;
		Allocator(L,Userdata);
		Copy_Constructor(L,Userdata->Info.Address,StructData);
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Userdata->Info.Address,-1);
	}
static void StructBind_Wrap_Pointer
	(
		lua_State* L, void* Address, lua_CFunction Attach_Metamethods, StructBind_Userdata_Settings Settings
	) {
		StructBind_Userdata* Userdata = StructBind_New_Userdata(L,Settings,0);
		Userdata->Info.IsNative = false;
		Userdata->Info.Address = Address;
		StructBind_Wrapper_Setup_Metatable(L,Attach_Metamethods);
		StructBind_Map_Userdata(L,Userdata->Info.Address,-1);
	}

static void StructBind_Utils_Copy(lua_State* L, int Index) {
	StructBind_Userdata* Userdata = lua_touserdata(L,Index);
	if (Userdata->Settings.Copy != NULL) {
		M_Print("Copying object %p",Userdata);
		Userdata->Settings.Copy(L,Index);
	} else {
		luaL_error(L,"The object is not copyable");
	}
}
