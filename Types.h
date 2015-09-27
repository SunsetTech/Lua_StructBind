#pragma once
#include "stdbool.h"
#include "lua.h"

typedef lua_CFunction Lua_CMethod;

typedef struct {
	bool IsOwnable;
	bool IsReference, IsReferenceable;
	bool IsConst;
	lua_CFunction NewIndex;
	lua_CFunction Destructor;
} StructBind_Userdata_Settings;

typedef struct {
	bool IsOwned;
	StructBind_Userdata_Settings Settings;
	void* Address;
} StructBind_Userdata_Info;

typedef void* (*StructBind_Allocator)(lua_State*,StructBind_Userdata_Info*);
typedef void (*StructBind_Constructor)(lua_State*,void*);
typedef void (*StructBind_Copy_Constructor)(lua_State*,void*,void*);

/*void StructBind_Userdata_Info_Init(StructBind_Userdata_Info* Info_Address, void* Data_Address,  bool Ownable, bool Referenceable, bool Const,  Lua_CMethod NewIndex, Lua_CMethod Destructor) {
	Info_Address->Address = Data_Address;
	Info_Address->IsOwnable = Ownable;
	Info_Address->IsOwned = false;
	Info_Address->IsReferenceable = Referenceable;
	Info_Address->IsReference = false;
	Info_Address->IsConst = Const;
	Info_Address->NewIndex = NewIndex;
	Info_Address->Destructor = Destructor;
}*/
