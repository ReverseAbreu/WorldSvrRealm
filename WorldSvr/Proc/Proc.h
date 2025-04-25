#pragma once
#include <PacketManager/PacketManager.h>
#include <PacketManager/Protodefs.h>
#include <PacketManager/C2SPackets.h>
#include <Game/Structs/GameStructs.h>
#include <unordered_map>

class Proc : public Singleton<Proc>
{
public:
	std::unordered_map<int, std::vector<std::function<int(int*, PROCESSDATACONTEXT*)>>> m_procedures; //[extcmd][functions]
	std::unordered_map<WORD, std::function<int(int*, PROCESSDATACONTEXT*)>> mHandleIPCPacket;

	static int OnProcessCustomPackets(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);
	static int ExCustomIPCCMD(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);

	void RegisterProcedure(MAINCMD_VALUE_CUSTOM customCmd, std::function<int(int*, PROCESSDATACONTEXT*)> function);
	void RegisterIPCProcedure(WORD exCustomCMD, std::function<int(int*, PROCESSDATACONTEXT*)> fN);
	void Init();
};

extern Proc* g_Procedure;

#define REGISTERCUSTOMPROC(ext_cmd,function)				g_Procedure->RegisterProcedure(ext_cmd,function)
#define REGISTERHANDLE_IPC_UNQ(p, f)						g_Procedure->RegisterIPCProcedure(p, f);