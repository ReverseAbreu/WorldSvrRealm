#include "Warp.h"
#include <fstream>
#include "Common/Json/json.hpp"
#include "Common/LibUtils.h"

extern Warp* g_pWarp = Warp::GetInstance();

//41 bytes packet warpcommand

int Warp::OnC2SWarpCommand(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	ALIAS_PTR(C2S_WARPCOMMAND, p, pProcessDataCtx->cpPacket);
	ALIAS_PTR(USERCONTEXT, pUserCtx, pProcessDataCtx->pUserCtx);
	ALIAS_PTR(USERDATACONTEXT, pUserDataCtx, pUserCtx->pData);

	if (!pUserDataCtx->bIsActvteLink)
		return P_FAIL;

	auto pWorld	= pUserDataCtx->sPosData.pWorld;

	for (auto iter = pWorld->m_sNpcsData.begin(); iter != pWorld->m_sNpcsData.end(); iter++)
	{
		if (auto pNpcData = iter->second)
		{
			if (p->bNpcsIdx == pNpcData->iIndex && pNpcData->_ntType == ::NT_WARP)
			{
				int iNpcPosX			= pNpcData->iPosX;
				int iNpcPosY			= pNpcData->iPosY;

				if (abs(pUserDataCtx->sPosData.iPosXCur - pNpcData->iPosX) > 8 || abs(pUserDataCtx->sPosData.iPosYCur - pNpcData->iPosY) > 8)
				{
					return P_FAIL;
				}
			}
		}
	}
	
	return P_OK;
}


void Warp::Init()
{
	REGISTER_PROC(g_sUsrProcedureMap, MAINCMD_VALUE_EX::CSC_WARPCOMMAND, OnC2SWarpCommand);
}
