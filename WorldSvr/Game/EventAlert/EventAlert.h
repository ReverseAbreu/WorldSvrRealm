#pragma once
#include "Core.h"
#include <Game/Structs/GameStructs.h>
#include <unordered_map>

enum eTypeEventAlert : BYTE
{
	E_INVALID_EVENT = 0,
	E_ITEM_DROP,
	E_ITEM_CREATE,
	E_MAX_EVENT
};

enum eLOOTING_RESULT
{
	LR_SUCCESS = 96,
	LR_OWNFAIL,
	LR_AFTRIMG,
	LR_ALREADY_USE_SLOT,
	LR_ANTIONLINEGAME,
	LR_OUTOFRANGE,
	LR_SUCESSWITHRULES = 104,
};

#pragma pack( push, 1 )
struct sS2C_ITEMLOOTING : public S2C_HEADER //PacketID: 0x99
{
	BYTE	bResult;
	INT64	ItemIdx;
	INT64	ItemOpt;
	WORD	SlotPos;
	DWORD	Duration;
};

struct sS2C_ITEMLOOTING_RULES : public S2C_HEADER //PacketID: 0x7F4
{
	int OwnerIdx;
	INT64 ItemIdx;
	INT64 ItemOpt;
	DWORD Duration;
};

struct ITEMCREATED_ITEMLIST
{
	INT64 ItemIdx;
	INT64 ItemOpt;
	WORD SlotPos;
	DWORD Duration;
};

struct sS2C_ITEMCREATED : public S2C_HEADER //PacketID: 0x19E
{
	BYTE bType;
	BYTE bItemCnt;
	ITEMCREATED_ITEMLIST ItemList[1];
};

struct sEVENT_ALERT_OWNER
{
	BYTE Event;
	int MapId;
	int Channel;
	int CharIdx;
	INT64 ItemId;
	INT64 ItemOpt;
	char CharName[16];

	sEVENT_ALERT_OWNER() : Event(0), MapId(0), CharIdx(0), ItemId(0), ItemOpt(0), CharName{ 0 } {}
};

struct sS2C_EVENT_ALERT: public exS2C_HEADER
{
	sEVENT_ALERT_OWNER EVENT_ALERT_OWNER;
	sS2C_EVENT_ALERT() : exS2C_HEADER(sizeof(*this), MAINCMD_VALUE_EX::CSC_CUSTOM_PACKET, MAINCMD_VALUE_CUSTOM::S2C_EVENT_ALERT){}
};

struct sIPC_EVENT_ALERT : public exROUTEDUPLEX_HDR
{
	sEVENT_ALERT_OWNER EVENT_ALERT_OWNER;
	sIPC_EVENT_ALERT() 
		: exROUTEDUPLEX_HDR(sizeof(*this), MAINCMD_VALUE::IPC_ROUTEPACKET, eMAINCMD_VALUE::MAINCMD_CUSTOM_IPC, eMAINCMDIPC_VALUE_EX::IPC_EVENT_ALERT) 
	{}
};
#pragma pack( pop )


struct sEventAlertItemInfo
{
	int MinLvL;
	int MaxLvL;
	std::vector<INT64> vOptList;
};

namespace EventAlert
{
	extern std::unordered_map<INT64, sEventAlertItemInfo> mItemLooting;
	int OnIPCEventAlert(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx);
	void SendIPC(USERCONTEXT* pUserCtx, USERDATACONTEXT* pUserDataCtx, INT64 ItemId, INT64 ItemOpt, BYTE Event);
	void ItemLooting(USERCONTEXT* pUserCtx, sS2C_ITEMLOOTING* pS2C_ITEMLOOTING, size_t iLen);
	void ItemLootingWithRules(USERCONTEXT* _pUserCtx, sS2C_ITEMLOOTING_RULES* pS2C_ITEMLOOTING_RULES, size_t iLen);
	void ItemCreated(USERCONTEXT* pUserCtx, sS2C_ITEMCREATED* pS2C_ITEMCREATED);
	void LoadItemList();
	void Initialize();
}