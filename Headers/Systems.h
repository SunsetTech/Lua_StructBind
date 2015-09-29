#pragma once

#include "Systems/UserdataMap.h"
#include "Systems/KeepAlive.h"
#include "Systems/Ownership.h"
#include "Systems/Reference.h"

static void StructBind_Init_Systems(lua_State* L) {
	M_Print("Initializing Systems")
	StructBind_Init_UserdataMap(L);
	StructBind_Init_KeepAlive(L);
	StructBind_Init_OwnershipMap(L);
	StructBind_Init_ReferenceMap(L);
}








