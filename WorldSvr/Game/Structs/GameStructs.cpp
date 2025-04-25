#include "GameStructs.h"

SERVERCONTEXTEX* g_pServerContextEx = (SERVERCONTEXTEX*)0x02162488;

USERCONTEXT* SERVERCONTEXTEX::GetUserCtxByUserIdx(int UserIdx)
{
	return reinterpret_cast<USERCONTEXT* (*)(SERVERCONTEXTEX*, int)>(0x007E0FC0)(this, UserIdx);
}

USERCONTEXT* SERVERCONTEXTEX::GetUserCtxByCharIdx(int CharacterIdx)
{
	return reinterpret_cast<USERCONTEXT * (*)(SERVERCONTEXTEX*, int)>(0x00ADD2C0)(this, CharacterIdx);
}

USERDATACONTEXT* SERVERCONTEXTEX::GetUserDataCtxByCharIdx(int characterIdx)
{
	return reinterpret_cast<USERDATACONTEXT* (*)(SERVERCONTEXTEX*, int)>(0x00ADD2C0)(this, characterIdx);
}

USERDATACONTEXT* SERVERCONTEXTEX::GetUserDataCtxByUserIdx(WORD objIdx, OBJIDX_TYPE objType)
{
	WORD tmpObjectIdx			= objIdx;
	int tmpObjectType			= objType-1;
	USERDATACONTEXT* result		= 0;

	if (tmpObjectType >= 0 && tmpObjectType <= 4)
	{
		result = reinterpret_cast<USERDATACONTEXT * (*)(WORD*)>(((INT64*)0x2160680)[objType])(&tmpObjectIdx);
	}

	return result;
}
