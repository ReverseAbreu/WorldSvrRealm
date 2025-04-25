#include "EventAlert.h"
#include "Memory/Memory.h"
#include "Common/Json/json.hpp"

std::unordered_map<INT64, sEventAlertItemInfo>EventAlert::mItemLooting;
int EventAlert::OnIPCEventAlert(int* pProcessLayer, PROCESSDATACONTEXT* pProcessDataCtx)
{
	ALIAS_PTR(sIPC_EVENT_ALERT, pIPC_EVENT_ALERT, pProcessDataCtx->cpPacket);

	sS2C_EVENT_ALERT S2C_EVENT_ALERT;
	memcpy(&S2C_EVENT_ALERT.EVENT_ALERT_OWNER, &pIPC_EVENT_ALERT->EVENT_ALERT_OWNER, sizeof(sEVENT_ALERT_OWNER));
	BroadCastEToEveryWorld(&S2C_EVENT_ALERT, sizeof(sS2C_EVENT_ALERT));

	return P_OK;
}

void EventAlert::SendIPC(USERCONTEXT* pUserCtx, USERDATACONTEXT* pUserDataCtx, INT64 ItemId, INT64 ItemOpt, BYTE Event)
{
	sIPC_EVENT_ALERT IPC_EVENT_ALERT;
	IPC_EVENT_ALERT.bToServerIdx = GetServerIdx();
	IPC_EVENT_ALERT.bFmServerIdx = GetServerIdx();
	IPC_EVENT_ALERT.bFmGroupIdx = GetGroupIdx();
	IPC_EVENT_ALERT.bFmProcessIdx = static_cast<BYTE>(pUserCtx->pProcessLayer[60]);
	IPC_EVENT_ALERT.EVENT_ALERT_OWNER.Event = Event;
	IPC_EVENT_ALERT.EVENT_ALERT_OWNER.MapId = pUserDataCtx->sPosData.pWorld->iWorldIdx;
	IPC_EVENT_ALERT.EVENT_ALERT_OWNER.CharIdx = pUserDataCtx->GetCharacterIdx();
	IPC_EVENT_ALERT.EVENT_ALERT_OWNER.Channel = GetGroupIdx();
	IPC_EVENT_ALERT.EVENT_ALERT_OWNER.ItemId = ItemId;
	IPC_EVENT_ALERT.EVENT_ALERT_OWNER.ItemOpt = ItemOpt;
	strcpy(IPC_EVENT_ALERT.EVENT_ALERT_OWNER.CharName, pUserDataCtx->GetCharacterName());
	IPC_CALL_PROC(*reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(0x2162628) + 0x30), &IPC_EVENT_ALERT, sizeof(sIPC_EVENT_ALERT));
}

void EventAlert::ItemLooting(USERCONTEXT* pUserCtx, sS2C_ITEMLOOTING* pS2C_ITEMLOOTING, size_t iLen)
{
	pUserCtx->UnicastE(pS2C_ITEMLOOTING, iLen);

	if (pS2C_ITEMLOOTING->bResult == eLOOTING_RESULT::LR_SUCCESS)
	{
		int ItemId = pS2C_ITEMLOOTING->ItemIdx & MASK_ITEMKINDINDEX;
		auto it = mItemLooting.find(ItemId);
		if ((it != mItemLooting.end()) && (pS2C_ITEMLOOTING->Duration == 0))
		{
			int ItemLevel = (pS2C_ITEMLOOTING->ItemIdx >> SHF_UPGRADECORE) & 31;
			if (ItemLevel >= it->second.MinLvL && ItemLevel <= it->second.MaxLvL)
			{
				for (auto& Opt : it->second.vOptList)
				{
					if (Opt == -1 || Opt == pS2C_ITEMLOOTING->ItemOpt)
					{
						if (USERDATACONTEXT* pUserDataCtx = reinterpret_cast<USERDATACONTEXT*>(pUserCtx->pData))
							SendIPC(pUserCtx, pUserDataCtx, pS2C_ITEMLOOTING->ItemIdx, pS2C_ITEMLOOTING->ItemOpt, eTypeEventAlert::E_ITEM_DROP);

						break;
					}
				}
			}
		}
	}
}

void EventAlert::ItemLootingWithRules(USERCONTEXT* _pUserCtx, sS2C_ITEMLOOTING_RULES* pS2C_ITEMLOOTING_RULES, size_t iLen)
{
	typedef void(*TOriFunc)(USERCONTEXT*, sS2C_ITEMLOOTING_RULES*, size_t);
	TOriFunc OriFunc = (TOriFunc)0x008CE690;

	OriFunc(_pUserCtx, pS2C_ITEMLOOTING_RULES, iLen);

	int ItemId = pS2C_ITEMLOOTING_RULES->ItemIdx & MASK_ITEMKINDINDEX;
	auto it = mItemLooting.find(ItemId);
	if ((it != mItemLooting.end()) && (pS2C_ITEMLOOTING_RULES->Duration == 0))
	{
		int ItemLevel = (pS2C_ITEMLOOTING_RULES->ItemIdx >> SHF_UPGRADECORE) & 31;
		if (ItemLevel >= it->second.MinLvL && ItemLevel <= it->second.MaxLvL)
		{
			for (auto& Opt : it->second.vOptList)
			{
				if (Opt == -1 || Opt == pS2C_ITEMLOOTING_RULES->ItemOpt)
				{
					if (USERCONTEXT* pUserCtx = g_pServerContextEx->GetUserCtxByCharIdx(pS2C_ITEMLOOTING_RULES->OwnerIdx))
					{
						if (USERDATACONTEXT* pUserDataCtx = reinterpret_cast<USERDATACONTEXT*>(pUserCtx->pData))
							SendIPC(pUserCtx, pUserDataCtx, pS2C_ITEMLOOTING_RULES->ItemIdx, pS2C_ITEMLOOTING_RULES->ItemOpt, eTypeEventAlert::E_ITEM_DROP);
					}

					break;
				}
			}
		}
	}
}

void EventAlert::ItemCreated(USERCONTEXT* pUserCtx, sS2C_ITEMCREATED* pS2C_ITEMCREATED)
{
	pUserCtx->UnicastE(pS2C_ITEMCREATED, pS2C_ITEMCREATED->wPayLoadLen);

	if (USERDATACONTEXT* pUserDataCtx = reinterpret_cast<USERDATACONTEXT*>(pUserCtx->pData))
	{
		for (int i = 0; i < pS2C_ITEMCREATED->bItemCnt; i++)
		{
			int ItemId = pS2C_ITEMCREATED->ItemList[i].ItemIdx & MASK_ITEMKINDINDEX;
			auto it = mItemLooting.find(ItemId);
			if ((it != mItemLooting.end()) && (pS2C_ITEMCREATED->ItemList[i].Duration == 0))
			{
				int ItemLevel = (pS2C_ITEMCREATED->ItemList[i].ItemIdx >> SHF_UPGRADECORE) & 31;
				if (ItemLevel >= it->second.MinLvL && ItemLevel <= it->second.MaxLvL)
				{
					for (auto& Opt : it->second.vOptList)
					{
						if (Opt == -1 || Opt == pS2C_ITEMCREATED->ItemList[i].ItemOpt)
						{
							SendIPC(pUserCtx, pUserDataCtx, pS2C_ITEMCREATED->ItemList[i].ItemIdx, pS2C_ITEMCREATED->ItemList[i].ItemOpt, eTypeEventAlert::E_ITEM_CREATE);
							break;
						}
					}
				}
			}
		}
	}
}

void EventAlert::LoadItemList()
{
	std::ifstream f("/etc/cabal/Data/PluginCfg/EventAlert.json");
	if (f.is_open())
	{
		try
		{
			nlohmann::json data = nlohmann::json::parse(f);
			for (const auto& item : data["ItemList"])
			{
				const auto& itemIds		= item["itemid"];
				const auto& itemOpts	= item["itemopt"];
				int minLvL				= item["minLvL"].get<int>();
				int maxLvl				= item["maxLvL"].get<int>();
				for (const auto& itemId : itemIds)
				{
					INT64 id = itemId.get<INT64>();
					mItemLooting[id].MinLvL = minLvL;
					mItemLooting[id].MaxLvL = maxLvl;
					for (const auto& opt : itemOpts)
					{
						INT64 option = opt.get<INT64>();
						if (std::find(mItemLooting[id].vOptList.begin(), mItemLooting[id].vOptList.end(), option) == mItemLooting[id].vOptList.end())
							mItemLooting[id].vOptList.push_back(option);
					}
				}
			}
		}
		catch (const std::exception& except)
		{
			except.what();
		}
	}
}

void EventAlert::Initialize()
{
	//using namespace Memory;
	LoadItemList();
	Hook::SetHook(ItemLooting, 0x00839963, eHookTypes::CALL);
	Hook::SetHook(ItemLootingWithRules, 0x008C78F5, eHookTypes::CALL);
	Hook::SetHook(ItemLootingWithRules, 0x008C778E, eHookTypes::CALL);
	Hook::SetHook(ItemCreated, 0x005F7906, eHookTypes::CALL);
	/*HookFunc<HookType::CALL>(ItemLooting, 0x00839963);
	HookFunc<HookType::CALL>(ItemLootingWithRules, 0x008C78F5);
	HookFunc<HookType::CALL>(ItemLootingWithRules, 0x008C778E);
	HookFunc<HookType::CALL>(ItemCreated, 0x005F7906);*/
}
