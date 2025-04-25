#include <Proc/Proc.h>
#include "Game/EventAlert/EventAlert.h"
#include "Common/LibUtils.h"

extern Proc* g_Procedure = Proc::GetInstance();

int OnAppointInstanceLord(int pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	if (sizeof(C2S_APPOINT_INSTANCELORD) != pProcessDataCtx->iLen)
		return P_ERROR;

	ALIAS_PTR(USERCONTEXT, pUserCtx, pProcessDataCtx->pUserCtx);
	ALIAS_PTR(USERDATACONTEXT, pUserDataCtx, pUserCtx->pData);
	ALIAS_PTR(C2S_APPOINT_INSTANCELORD, pOnAppointInstanceLord, pProcessDataCtx->cpPacket);

	if (!pUserDataCtx->bIsActvteLink)
		return P_ERROR;

	if (pUserDataCtx->lordType)
	{
		if (USERCONTEXT* pTargetUserCtx = g_pServerContextEx->GetUserCtxByCharIdx(pOnAppointInstanceLord->characterIdx))
			return P_OK;
	}

	LibLog(pUserDataCtx->GetUserNum(), pUserCtx->ipAddress);
	return P_FAIL;
}

int OnCopyUserPositionReq(int pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	if (sizeof(ITS_USERPOSDATA_COPY_REQ) != pProcessDataCtx->iLen)
		return P_ERROR;

	ALIAS_PTR(USERCONTEXT, pUserCtx, pProcessDataCtx->pUserCtx);
	ALIAS_PTR(USERDATACONTEXT, pUserDataCtx, pUserCtx->pData);
	ALIAS_PTR(ITS_USERPOSDATA_COPY_REQ, pCopyUserPositionReq, pProcessDataCtx->cpPacket);

	if (!pUserDataCtx->bIsActvteLink)
		return P_ERROR;

	if (USERCONTEXT* pTargetUserCtx = g_pServerContextEx->GetUserCtxByCharIdx(pCopyUserPositionReq->RequestCharIdx))
		return P_OK;

	LibLog(pUserDataCtx->GetUserNum(), pUserCtx->ipAddress);
	return P_FAIL;
}

int Proc::OnProcessCustomPackets(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	ALIAS_PTR(C2S_HEADER_EX, p, pProcessDataCtx->cpPacket);
	ALIAS_PTR(USERCONTEXT, pUserCtx, pProcessDataCtx->pUserCtx);
	ALIAS_PTR(USERDATACONTEXT, pUserDataCtx, pUserCtx->pData);
	int result = P_OK;

	auto iter = g_Procedure->m_procedures.find(p->extCmd);

	if (iter != g_Procedure->m_procedures.end())
	if (g_Procedure->m_procedures.find(p->extCmd) != g_Procedure->m_procedures.end())
	{
		for (auto& it : iter->second)
		{
			if (!it(pProcessLayer, pProcessDataCtx))
			{
				result = P_ERROR;
				break;
			}
		}
	}

	return result;
}

void Proc::RegisterProcedure(MAINCMD_VALUE_CUSTOM customCmd, std::function<int(int*, PROCESSDATACONTEXT*)> function)
{
	m_procedures[customCmd].push_back(std::bind(function, std::placeholders::_1, std::placeholders::_2));
}

void Proc::RegisterIPCProcedure(WORD exCustomCMD, std::function<int(int*, PROCESSDATACONTEXT*)> fN)
{
	mHandleIPCPacket.insert(std::make_pair(exCustomCMD, fN));
}

int Proc::ExCustomIPCCMD(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	if (pProcessDataCtx->iLen < sizeof(exROUTEDUPLEX_HDR))
		return P_ERROR;

	exROUTEDUPLEX_HDR* pExData = (exROUTEDUPLEX_HDR*)pProcessDataCtx->cpPacket;
	auto it = g_Procedure->mHandleIPCPacket.find(pExData->exMainIPCCmd);
	if (it != g_Procedure->mHandleIPCPacket.end())
	{
		it->second(pProcessLayer, pProcessDataCtx);
		return P_OK;
	}

	return P_FAIL;
}

void Proc::Init()
{
	REGISTER_PROC(g_sUsrProcedureMap, MAINCMD_VALUE_EX::CSC_APPOINT_INSTANCELORD, OnAppointInstanceLord);

	REGISTER_PROC(g_sUsrProcedureMap, MAINCMD_VALUE_EX::ITC_USERPOSDATA_COPY_REQ, OnCopyUserPositionReq);

	REGISTER_PROC(g_sUsrProcedureMap, MAINCMD_VALUE_EX::CSC_CUSTOM_PACKET, OnProcessCustomPackets);

	REGISTER_PROC(g_sSysProcedureMap, eMAINCMD_VALUE::MAINCMD_CUSTOM_IPC, ExCustomIPCCMD);

	REGISTERHANDLE_IPC_UNQ(eMAINCMDIPC_VALUE_EX::IPC_EVENT_ALERT, &EventAlert::OnIPCEventAlert);
}
