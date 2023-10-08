#pragma once
#include "interfaces.h"
struct CUserCmd;

namespace hooks
{
	void Init();
	void Destroy();

	using CreateMoveFn = bool(__thiscall*)(void*, float, CUserCmd*);
	inline CreateMoveFn oCreateMove = nullptr;
	bool __stdcall CreateMove(float frameTime, CUserCmd* cmd);
}

