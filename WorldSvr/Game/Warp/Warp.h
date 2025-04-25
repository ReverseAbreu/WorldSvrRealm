#pragma once
#include <Core.h>
#include <Game/Structs/GameStructs.h>

class Warp : public Singleton<Warp>
{
public:

	static int OnC2SWarpCommand(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);

	void Init();
};

extern Warp* g_pWarp;