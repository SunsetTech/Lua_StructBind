#pragma once
#include "stdbool.h"
#include "lua.h"

typedef struct StructBind_Userdata StructBind_Userdata;

typedef struct {
	bool IsOwned;
	bool IsNative;
	void* Address;
} StructBind_Userdata_Info;

typedef void (*StructBind_Copy)(lua_State*,int);
typedef lua_CFunction StructBind_Modify;
typedef lua_CFunction StructBind_Destroy;

typedef struct {
	bool IsOwnable;
	bool IsReferenceable;
	bool IsConst;
	StructBind_Copy Copy;
	StructBind_Modify Modify;
	StructBind_Destroy Destroy;
} StructBind_Userdata_Settings;

struct StructBind_Userdata {
	StructBind_Userdata_Info Info;
	StructBind_Userdata_Settings Settings;
};

typedef void (*StructBind_Allocator)(lua_State*,StructBind_Userdata*);

static void* StructBind_Userdata_GetDataRegion(StructBind_Userdata* Address) {
	return (void*)(Address + 1);
}
