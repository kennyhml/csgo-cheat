#include "hooks.h"
#include "../../ext/minhook/MinHook.h"

void hooks::Init()
{
	MH_Initialize();

	MH_CreateHook(
		memory::Get(interfaces::clientMode, 24),
		&CreateMove,
		reinterpret_cast<void**>(&oCreateMove)
	);

	MH_EnableHook(MH_ALL_HOOKS);
}

void hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd)
{
	const bool result = oCreateMove(interfaces::clientMode, frameTime, cmd);

	if (!cmd->commandNumber) { return result; }

	if (result) { interfaces::engine->SetViewAngles(cmd->viewAngles); }

	globals::localPlayer = interfaces::entityList->GetClientEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());

	if (!GetAsyncKeyState(VK_CONTROL) & 1) { return false; }

	if (!globals::localPlayer || !globals::localPlayer->IsAlive()) { return false; }

	CVector eyePos;
	globals::localPlayer->GetEyePosition(eyePos);

	CVector aimPunch;
	globals::localPlayer->GetAimPunch(aimPunch);

	CVector dst = eyePos + CVector{ cmd->viewAngles + aimPunch }.ToVector() * 5000.f;

	CTrace trace;
	interfaces::trace->TraceRay({ eyePos, dst }, 0x46004009, globals::localPlayer, trace);

	if (!trace.entity
		|| !trace.entity->IsPlayer()
		|| !trace.entity->IsAlive()
		|| !trace.entity->IsEnemy()) {
		return false;
	}

	cmd->buttons |= IN_ATTACK;
	return false;
}