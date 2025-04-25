#pragma once
#pragma pack( push, 1 )
struct OBJIDXDATA;

struct C2S_HEADER
{
    WORD  wMagicCode;
    WORD  wPayLoadLen;
    DWORD dwCheckSum;
    WORD  wMainCmd;
};

struct C2S_HEADER_EX
{
    WORD  wMagicCode;
    WORD  wPayLoadLen;
    DWORD dwCheckSum;
    WORD  wMainCmd;
    WORD extCmd;
};

struct sC2S_HEADER
{
    WORD  wMagicCode;
    WORD  wPayLoadLen;
    DWORD dwCheckSum;
    WORD  wMainCmd;

    sC2S_HEADER()
        : wMagicCode(MAGIC_CODE), wPayLoadLen(0), dwCheckSum(0), wMainCmd(0) {}

    sC2S_HEADER(WORD len, WORD cmd)
        : wMagicCode(MAGIC_CODE), wPayLoadLen(len), dwCheckSum(0), wMainCmd(cmd) {}
};

struct exC2S_HEADER : public sC2S_HEADER
{
    WORD exMainCmd;
};


struct ROUTE_HDR
{
    WORD bOriginMainCmd;
    BYTE bToServerIdx;
    BYTE bToGroupIdx;
    BYTE bToWorldIdx;
    BYTE bToProcessIdx;
    ROUTE_HDR()
    {
        bOriginMainCmd = 0;
        bToServerIdx = 0;
        bToGroupIdx = 0;
        bToWorldIdx = 0;
        bToProcessIdx = 0;
    }

    ROUTE_HDR(WORD OriginMainCmd)
    {
        bOriginMainCmd = OriginMainCmd;
        bToServerIdx = 0;
        bToGroupIdx = 0;
        bToWorldIdx = 0;
        bToProcessIdx = 0;
    }
};

struct ROUTEDUPLEX_HDR : public ROUTE_HDR
{
    int						iUniqIdx;
    WORD					wToIndex, wFmIndex;
    BYTE					bFmServerIdx,
        bFmGroupIdx,
        bFmWorldIdx,
        bFmProcessIdx;

    ROUTEDUPLEX_HDR()
    {
        iUniqIdx = 0;
        wToIndex = 0;
        wFmIndex = 0;
        bFmServerIdx = 0;
        bFmGroupIdx = 0;
        bFmWorldIdx = 0;
        bFmProcessIdx = 0;
    }

    ROUTEDUPLEX_HDR(WORD OriginMainCmd) : ROUTE_HDR::ROUTE_HDR(OriginMainCmd)
    {
        iUniqIdx = 0;
        wToIndex = 0;
        wFmIndex = 0;
        bFmServerIdx = 0;
        bFmGroupIdx = 0;
        bFmWorldIdx = 0;
        bFmProcessIdx = 0;
    }
};

struct exROUTEDUPLEX_HDR : public sC2S_HEADER, ROUTEDUPLEX_HDR
{
    WORD exMainIPCCmd;

    exROUTEDUPLEX_HDR(WORD len, WORD cmd, WORD OriginMainCmd, WORD excmd) :
        exMainIPCCmd(excmd), sC2S_HEADER::sC2S_HEADER(len, cmd), ROUTEDUPLEX_HDR::ROUTEDUPLEX_HDR(OriginMainCmd) {}
};

struct ITS_USERPOSDATA_COPY_REQ : C2S_HEADER
{
    int RequestCharIdx;
    int TargetCharIdx;
};

struct C2S_APPOINT_INSTANCELORD : C2S_HEADER
{
    int characterIdx;
};

struct C2S_WARPCOMMAND : public sC2S_HEADER
{
    BYTE					bNpcsIdx;
    WORD					unk;
    WORD					bSlotIdx;		// 귀환석 Or 퀘스트던전 진입아이템 슬롯 인덱스

    WORD					wWarpIdx;		// 네비게이터로 워프시 타겟워프점 인덱스
    int						iQDungeonIdx;	// 인스턴트던전 진입시 던전의 인덱스
    int 					iNSetIdx;
    BYTE					bNSetIdx;

    WORD					position;		// GM 워프 인덱스 좌표

    WORD					userObjIdx;     // 워프 포스타워 설치된
    BYTE					data[11];
};

//Dungeon Entry
struct S_CSC_ENTRYDUNGEON_INFORMATIONS : C2S_HEADER
{
    DWORD dwDungeonID;
    DWORD dwDungeonType;
    DWORD dwUnk;
    DWORD dwCharIdx;
    DWORD dwInitLevelDungeon;
    DWORD dwWorldID;
};

//Dungeon Start
struct C2S_QDUNGEONMOA : public C2S_HEADER
{
    BYTE					 bIsActive;
};

//Dungeon End
struct C2S_QDUNGEONEND : public C2S_HEADER
{
    WORD					bSlotIdx;	
    BYTE					bIsSuccess;	
    BYTE					bCause;		
    BYTE					bNpcIdx;	
};

struct OBJIDXDATA2
{
    WORD idxNum;
    BYTE bWorldMob;
    BYTE objectType;

    DWORD get()
    {
        return *(DWORD*)this;
    }
};

struct DBUFF_TARGETDATA
{
    OBJIDXDATA2 sObjIdxData;
    BYTE targetType;
};

struct C2S_SKILLTOTARGET : public C2S_HEADER
{
    short               iSkillIdx;
    short               iSlotIdx;
    BYTE                bDummy[9];
    BYTE                bBuffKind;
    BYTE                bPlayersCount;
    DBUFF_TARGETDATA    sTargetData[1];
};

#pragma pack(pop)