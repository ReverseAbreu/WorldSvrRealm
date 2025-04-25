#pragma once
#include <iomanip>
#include "Game/Structs/GameStructs.h"

#define LibLog(UserNum, ipAddress)		MakeLibLog(__FUNCTION__, __LINE__, UserNum, ipAddress)
#define CloseSocketLog(pUserCtx) CloseSocketWithLog(pUserCtx, __FUNCTION__, __FILE__, __LINE__, 0)
#define SockClose ((int(*)(USERCONTEXT* pUserCtx))0x0B43330)

inline int ErrorFuncTraceAndReturn(const char* func, const char* file, int line, int fnum)
{
	typedef int(*TLOGERROR)(const char*, const char*, int, int);
	TLOGERROR LOGERROR = (TLOGERROR)0x0527960;

	return LOGERROR(func, file, line, fnum);
}

inline void MakeLibLog(const char* func, int line, DWORD UserNum, char ipAddress[16])
{
	std::time_t t = std::time(nullptr);
	char mbstr[100];

	if (std::strftime(mbstr, sizeof(mbstr), "[%A %c]: ", std::localtime(&t)))
	{
		std::stringstream ss;
		ss << "WorldSvr_" << GetServerIdx() << "_" << GetGroupIdx() << ".log";
		std::ofstream ofile(std::string("/var/log/cabal/LibLog/") + ss.str(), std::ios::app);
		ofile << mbstr << func << " Line: " << line << " UserNum: " << UserNum << " Sender: " << ipAddress << "\n";
		ofile.close();
	}
}

inline int CloseSocketWithLog(USERCONTEXT* pUserCtx, const char* func, const char* file, int line, int fnum)
{
	ErrorFuncTraceAndReturn(func, file, line, fnum);
	return SockClose(pUserCtx);
}