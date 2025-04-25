#pragma once
#include <Core.h>

struct PROCESSDATACONTEXT
{
	int* pUserCtx;
	char* cpPacket;
	WORD iLen;
	int iUniqIdx;
	int _iData0;
	int _iData1;
	int _iData2;
};

class PROCEDUREMAP
{
	typedef int(*PROCEDURE)(int pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);

private:
	std::vector<PROCEDURE>	procLists_;
	const char* szName_;
	bool					isLocking_;

	int						counts_;
	INT64					lengthSum_;
	char					procTime_[80];

public:
	template <typename T>
	void AddProc(T proc, const char* name, bool lock = true)
	{
		procLists_.push_back(reinterpret_cast<PROCEDURE>(proc));
		szName_		= name;
		isLocking_	= lock;
	}

	void IsLocking(bool lock) {
		isLocking_ = lock;
	}

	bool IsLocking() {
		return isLocking_;
	}

	PROCEDUREMAP() :
		szName_(""),
		isLocking_(true),
		counts_(0),
		lengthSum_(0) {}
};

#define REGISTER_PROC(map, idx, proc)					\
	if(map[idx] == NULL) map[idx] = new PROCEDUREMAP;	\
	map[idx]->AddProc(proc,  #idx ": " #proc);			

#define REGISTER_NOLOCK_PROC(map, idx, proc)			\
	REGISTER_PROC(map, idx, proc);						\
	map[idx]->IsLocking(false);


extern PROCEDUREMAP** g_sSysProcedureMap;
extern PROCEDUREMAP** g_sUsrProcedureMap;
