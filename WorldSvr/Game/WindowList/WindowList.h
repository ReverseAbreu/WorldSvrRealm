#pragma once
#include "Core.h"

#pragma pack( push, 1 )
struct sC2S_WINDOWDETECTED : public exC2S_HEADER
{
	char szWindowFinded[256];
};
#pragma pack( pop )

namespace WindowList
{
	static const std::string logPath = "/etc/cabal/Plugins/Logs/WindowListDetected";
	int OnC2SWindowList(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);
	void Initialize();
}