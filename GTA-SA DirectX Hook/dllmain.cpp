// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"

typedef HRESULT(__stdcall* _EndScene)(IDirect3DDevice9* pDevice);
_EndScene oEndScene;

HRESULT __stdcall hkEndScene(IDirect3DDevice9* pDevice)
{
	D3DRECT BarRect = { 100,100,200,200 };
	pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(1, 1, 1, 1), 0.0f, 0);
	return oEndScene(pDevice);
}

DWORD APIENTRY MainThread(LPVOID lparam)
{
	while (!GetModuleHandleA("d3d9.dll"))//waiting till d3d9.dll gets loaded
	{
		Sleep(1);
	}
	// Endscene hook
	void** vTableDevice = *(void***)(*(DWORD*)0xC97C28);
	VTableHookManager* vmtHooks = new VTableHookManager(vTableDevice, 119);
	oEndScene = (_EndScene)vmtHooks->Hook(42, (void*)hkEndScene);
	////////////////////////////////////////////////////////////////////////////////
	return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}