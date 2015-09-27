#pragma once
#include "string.h"
#define M_Namespace StructBind

#define M_SetField(P_ValueExpression,P_Key)\
	P_ValueExpression;\
	lua_setfield(L,-2,P_Key);

#define M_GetField(P_Key)\
	lua_getfield(L,-1,Key);

#define M_SetTable(P_KeyExpression,P_ValueExpression) \
	P_KeyExpression; \
	P_ValueExpression;\
	lua_settable(L,-3);

#define M_GetTable(P_KeyExpression) \
	P_KeyExpression; \
	lua_gettable(L,-2);

#define M_Userdata_Metamethod_GetInfo()\
	StructBind_Userdata_Info* Info_Address = lua_touserdata(L,1);

#define M_Userdata_Metamethod_GetStruct(P_Type) \
	M_Userdata_Metamethod_GetInfo()\
	P_Type* Struct_Address = Info_Address->Address;\

#define M_Userdata_Metamethod_Index_GetArgs(P_Type) \
	M_Userdata_Metamethod_GetStruct(P_Type)\
	const char* Key = lua_tostring(L,2);

#define M_If_Not_Wrapped(P_Address_Expression,P_Body)\
	StructBind_Lookup_Userdata(L, P_Address_Expression);\
	if (lua_isnoneornil(L,-1)) {\
		lua_pop(L,1);\
		P_Body\
	}

#define M_If_Wrapped(P_Address_Expression,P_Body)\
	StructBind_Lookup_Userdata(L,P_Address_Expression);\
	bool HasUserdata = !lua_isnoneornil(L,-1);\
	if (!HasUserdata){\
		lua_pop(L,1);\
	}\
	if (HasUserdata) {\
		P_Body\
	}\

#define M_IsKey(P_Key) (strcmp(Key,P_Key))
