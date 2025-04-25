#include "MacroBM3.h"
#include "Proc/Proc.h"
#include "Common/Json/json.hpp"

std::vector<int> MacroBM3::v_BM3MacroServiceKindList;
std::vector<int> MacroBM3::v_BM3MacroPlatinumBuffGrade;

int MacroBM3::OnReqUseBattleMode3(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	ALIAS_PTR(C2S_BM3MACROREQUEST, p, pProcessDataCtx->cpPacket);
	ALIAS_PTR(USERCONTEXT, pUserCtx, pProcessDataCtx->pUserCtx);
	ALIAS_PTR(USERDATACONTEXT, pUserDataCtx, pUserCtx->pData);

	if (pProcessDataCtx->iLen != sizeof(C2S_BM3MACROREQUEST))
		return P_OK;

	if (/*(std::find(v_BM3MacroServiceKindList.begin(), v_BM3MacroServiceKindList.end(), pUserDataCtx->iServiceKind) != v_BM3MacroServiceKindList.end()) &&*/ (std::find(v_BM3MacroPlatinumBuffGrade.begin(), v_BM3MacroPlatinumBuffGrade.end(), pUserDataCtx->PlatinumBuffGrade) != v_BM3MacroPlatinumBuffGrade.end()))
	{
		S2C_BM3MACRORESPONSE bm3Response;
		bm3Response.Initialize(sizeof(S2C_BM3MACRORESPONSE), MAINCMD_VALUE_CUSTOM::S2C_BM3MACRORESULT, p->status);
		pUserCtx->UnicastE(&bm3Response, sizeof(S2C_BM3MACRORESPONSE));
	}

	return P_OK;
}

void MacroBM3::LoadBM3MacroCFG()
{
	try
	{
		std::ifstream f("/etc/cabal/Data/PluginCfg/BM3MacroCfg.json");
		if (f.is_open())
		{
			nlohmann::json data = nlohmann::json::parse(f);

			if (data.contains("ServiceKindList"))
				v_BM3MacroServiceKindList = data["ServiceKindList"].get<std::vector<int>>();

			if (data.contains("PlatinumBuffGrade"))
				v_BM3MacroPlatinumBuffGrade = data["PlatinumBuffGrade"].get<std::vector<int>>();
		}
	}
	catch (std::exception& except)
	{
		//.... vou fazer algo depois
	}
}

void MacroBM3::Initialize()
{
	REGISTERCUSTOMPROC(MAINCMD_VALUE_CUSTOM::S2C_BM3MACRORESULT, OnReqUseBattleMode3);

	LoadBM3MacroCFG();
}
