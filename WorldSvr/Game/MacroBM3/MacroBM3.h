#pragma once
#include <Core.h>
#include <Game/Structs/GameStructs.h>
#include <vector>

#pragma pack(push,1)
struct C2S_BM3MACROREQUEST : C2S_HEADER_EX
{
	bool status;
};

struct S2C_BM3MACRORESPONSE : S2C_HEADER_EX
{
	int result;

	void Initialize(WORD len, WORD exCmd, int res)
	{
		wMagicCode			= MAGIC_CODE;
		wPayLoadLen			= len;
		wMainCmd			= MAINCMD_VALUE_EX::CSC_CUSTOM_PACKET;
		wMainCmdEx			= exCmd;
		result				= res;
	}
};

struct C2S_SKILLTOUSER : C2S_HEADER
{
	WORD skillIdx;
	BYTE slotIdx;
	bool status;
	char data[1];
};
#pragma pack(pop)

namespace MacroBM3 
{
	extern std::vector<int> v_BM3MacroServiceKindList;
	extern std::vector<int> v_BM3MacroPlatinumBuffGrade;

	void Initialize();
	int OnReqUseBattleMode3(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);
	void LoadBM3MacroCFG();
};