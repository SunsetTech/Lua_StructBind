#pragma once
#include "string.h"
#include <LuaMacros/Macros.h>
#include "Types.h"

//courtesy gpt-3.5
#ifdef ASS // Lua 5.2 or higher
	#define MY_ABSINDEX(L, idx) ((idx) > 0 || (idx) <= LUA_REGISTRYINDEX ? (idx) : lua_gettop(L) + (idx) + 1)
#else // Lua 5.1
	#define MY_ABSINDEX(L, idx) lua_absindex(L, idx)
#endif

#ifdef D_StructBind_Verbose
	#define M_Print(P_Format,...) printf(P_Format "\n", ##__VA_ARGS__);
#else
	#define M_Print(P_Format,...)
#endif

#define M_Get_Userdata(P_Name,P_Index)\
	StructBind_Userdata* P_Name = (StructBind_Userdata*) lua_touserdata(L,1);

#define M_Get_Object(P_Type,P_Name,P_Index)\
	M_Get_Userdata(P_Name##_Userdata,P_Index)\
	P_Type* P_Name = (P_Type*) P_Name##_Userdata->Info.Address;

#define M_Userdata_Method_GetUserdata(P_Name)\
	M_Get_Userdata(P_Name,1);

#define M_Userdata_Metamethod_Index_GetArgs(P_Name)\
	M_Userdata_Method_GetUserdata(P_Name)\
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

#define M_To_Absolute(P_Variable) P_Variable = MY_ABSINDEX(L,P_Variable);
