#include "WindowList.h"
#include "Memory/Memory.h"
#include "Proc/Proc.h"
#include "Common/Json/json.hpp"
#include "Common/LibUtils.h"
#include "Game/Management/Management.h"

int WindowList::OnC2SWindowList(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	ALIAS_PTR(sC2S_WINDOWDETECTED, pC2S_WindowDetected, pProcessDataCtx->cpPacket);
	ALIAS_PTR(USERCONTEXT, pUserCtx, pProcessDataCtx->pUserCtx);
	ALIAS_PTR(USERDATACONTEXT, pUserDataCtx, pUserCtx->pData);

	if (sizeof(sC2S_WINDOWDETECTED) != pProcessDataCtx->iLen)
		return P_ERROR;

	if (!pUserDataCtx->bIsActvteLink)	
		return P_ERROR;	

	std::stringstream output;

	output << "Window: " << pC2S_WindowDetected->szWindowFinded
		<< " | UserId: " << pUserDataCtx->GetUserName()
		<< " | CharacterName: " << pUserDataCtx->GetCharacterName()
		<< " | CharacterIdx: " << pUserDataCtx->GetCharacterIdx()
		<< " | UserNum: " << pUserDataCtx->GetUserNum();

	Management::WriteLogs(logPath + "/WindowListDetected.log", output.str());
	return P_OK;
}


void WindowList::Initialize()
{
	REGISTERCUSTOMPROC(C2S_WINDOWDETECTED, WindowList::OnC2SWindowList);
	Management::EnsureDir(logPath);
}