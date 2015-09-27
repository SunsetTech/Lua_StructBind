#pragma once
#include "stdbool.h"
#include "lua.h"

typedef struct {
	bool IsOwned;
	bool IsReference;
	bool IsNative;
	void* Address;
} StructBind_Userdata_Info;

typedef void (*StructBind_Allocator)(lua_State*,StructBind_Userdata_Info*);
typedef void (*StructBind_Transallocate)(lua_State*,StructBind_Userdata_Info*,StructBind_Allocator);
typedef void (*StructBind_Copy)(lua_State*,StructBind_Userdata_Info*);
typedef lua_CFunction StructBind_Modify;
typedef lua_CFunction StructBind_Destroy;

typedef struct {
	bool IsOwnable;
	bool IsReferenceable;
	bool IsConst;
	StructBind_Transallocate Transallocate;
	StructBind_Copy Copy;
	StructBind_Modify Modify;
	StructBind_Destroy Destroy;
} StructBind_Userdata_Settings;

typedef void (*StructBind_Constructor)(lua_State*,void*);
typedef void (*StructBind_Copy_Constructor)(lua_State*,void*,void*);

StructBind_Userdata_Info* StructBind_Userdata_GetInfo(void* Address) {
	return (StructBind_Userdata_Info*)Address;
}
StructBind_Userdata_Settings* StructBind_Userdata_GetSettings(void* Address) {
	return 
		(StructBind_Userdata_Settings*)(
			(char*)Address
			+sizeof(StructBind_Userdata_Info)
		);
}
void* StructBind_Userdata_GetDataRegion(void* Address) {
	return
		(void*)(
			(char*)Address
			+ sizeof(StructBind_Userdata_Info)
			+ sizeof(StructBind_Userdata_Settings)
		);
}

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
