#pragma once
#include "string.h"
#define M_Namespace StructBind

#define M_V_Module

#ifdef D_StructBind_Verbose
	#define M_Print(P_Format,...) printf(P_Format "\n", ##__VA_ARGS__);
#else
	#define M_Print(P_Format,...)
#endif

#define M_Method(P_MethodName)\
	M_V_Module##_##P_MethodName

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

#define M_Userdata_Metamethod_GetUserdata()\
	StructBind_Userdata* Userdata = lua_touserdata(L,1);

#define M_Userdata_Metamethod_Index_GetArgs() \
	M_Userdata_Metamethod_GetUserdata()\
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

#define M_IsKey(P_Key) (strcmp(Key,P_Key) == 0)

#define M_Stack_Index(P_Index,P_Offset) (P_Index > 0 ? P_Index : P_Index -1)
#define M_Iterate_Table(P_Index,P_Body)\
	lua_pushnil(L);\
	while (lua_next(L,P_Index) != 0) {\
		P_Body\
		lua_pop(L,1);\
	}

#define M_To_Absolute(P_Variable) P_Variable = lua_absindex(L,P_Variable);
