#include "lua.h"
#include "lauxlib.h"
#define D_StructBind_Verbose
#include "../Headers/StructBind.h"

M_Def_Lua(StructBind_IsConst) {
	StructBind_Userdata* Userdata = lua_touserdata(L,1);
	lua_pushboolean(L,Userdata->Settings.IsConst);
	return 1;
}

M_Def_Lua(StructBind_SetConst) {
	StructBind_Userdata* Userdata = lua_touserdata(L,1);
	Userdata->Settings.IsConst = lua_toboolean(L,2);
	return 0;
}

M_Def_Lua(StructBind_IsNative) {
	StructBind_Userdata* Userdata = lua_touserdata(L,1);
	lua_pushboolean(L,Userdata->Info.IsNative);
	return 1;
}

M_Def_Lua(StructBind_IsOwned) {
	StructBind_Userdata* Userdata = lua_touserdata(L,1);
	lua_pushboolean(L,Userdata->Info.IsOwned);
	return 1;
}

M_Def_Lua(StructBind_GetOwner) {
	StructBind_Get_Owner(L,1);
	return 1;
}

M_Def_Lua(StructBind_GetReferences) {
	lua_newtable(L);
	int Index = lua_absindex(L,-1);
	StructBind_Retrieve_ReferenceTable(L,1);
	M_Iterate_Table(
		-1,
		lua_len(L,Index);
		lua_pushnumber(L,1);
		lua_arith(L,LUA_OPADD);
		lua_pushvalue(L,-2);
		lua_settable(L,Index);
	)
	lua_pop(L,1);
	return 1;
}

M_Def_Lua(StructBind_Copy) {
	StructBind_Utils_Copy(L,1);
	return 0;
}

int luaopen_StructBind(lua_State* L) {
	StructBind_Init(L);
	lua_newtable(L);
		M_SetField(M_Push_Function(StructBind_IsConst),"IsConst");
		M_SetField(M_Push_Function(StructBind_SetConst),"SetConst");
		M_SetField(M_Push_Function(StructBind_IsNative),"IsNative");
		M_SetField(M_Push_Function(StructBind_IsOwned),"IsOwned");
		M_SetField(M_Push_Function(StructBind_GetOwner),"GetOwner");
		M_SetField(M_Push_Function(StructBind_GetReferences),"GetReferences");
		M_SetField(M_Push_Function(StructBind_Copy),"Copy");
	return 1;
}
