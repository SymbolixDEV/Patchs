#include "ScriptPCH.h"

class npc_transmogrify : public CreatureScript
{
public:
	npc_transmogrify() : CreatureScript("npc_transmogrify") { }

	bool OnGossipHello(Player* pPlayer, Creature* pUnit)
	{
		pPlayer->PlayerTalkClass->ClearMenus();
		for (uint8 Slot = EQUIPMENT_SLOT_START; Slot < EQUIPMENT_SLOT_TABARD; Slot++) // EQUIPMENT_SLOT_END
		{
			if (Item* pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, Slot))
					if(char* SlotName = GetSlotName(Slot))
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, SlotName, EQUIPMENT_SLOT_END, Slot);
		}
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Remove all transmogrifications", EQUIPMENT_SLOT_END+2, 0, "Remove transmogrifications from all equipped items?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Update Menu", EQUIPMENT_SLOT_END+1, 0);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pUnit->GetGUID());         
		return true;
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pUnit, uint32 sender, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus();
		if(sender == EQUIPMENT_SLOT_END) // Show items you can use
		{
			if (Item* OLD = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, uiAction))
			{
				uint32 GUID = pPlayer->GetGUIDLow();
				Items[GUID].clear();
				uint32 limit = 0;
				for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
				{
					if(limit > 30)
						break;
					if (Item* pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
					{
						uint32 Display = pItem->GetTemplate()->DisplayInfoID;
						if(IsSuitable(pItem, OLD, pPlayer))
						{
							if(Items[GUID].find(Display) == Items[GUID].end())
							{
								limit++;
								Items[GUID][Display] = pItem;
								pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, pItem->GetTemplate()->Name1, uiAction, Display, "Using this item for transmogrification will bind it to you and make it non-refundable and non-tradeable.\nDo you wish to continue?\n\n"+ pItem->GetTemplate()->Name1, GetSellPrice(OLD, pPlayer), false);
							}
						}
					}
				}

				for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
				{
					if (Bag* pBag = pPlayer->GetBagByPos(i))
						for (uint32 j = 0; j < pBag->GetBagSize(); j++)
						{
							if(limit > 30)
								break;
							if (Item* pItem = pPlayer->GetItemByPos(i, j))
							{
								uint32 Display = pItem->GetTemplate()->DisplayInfoID;
								if(IsSuitable(pItem, OLD, pPlayer))
									if(Items[GUID].find(Display) == Items[GUID].end())
									{
										limit++;
										Items[GUID][Display] = pItem;
										pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, pItem->GetTemplate()->Name1, uiAction, Display, "Using this item for transmogrification will bind it to you and make it non-refundable and non-tradeable.\nDo you wish to continue?\n\n"+ pItem->GetTemplate()->Name1, GetSellPrice(OLD, pPlayer), false);
									}
							}
						}
				}

				pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Remove transmogrification", EQUIPMENT_SLOT_END+3, uiAction, "Remove transmogrification from "+(std::string)GetSlotName(uiAction)+"?", 0, false);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back..", EQUIPMENT_SLOT_END+1, 0);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pUnit->GetGUID());
			}
			else
				OnGossipHello(pPlayer, pUnit);
		}
		else if(sender == EQUIPMENT_SLOT_END+1) // Back
			OnGossipHello(pPlayer, pUnit);
		else if(sender == EQUIPMENT_SLOT_END+2) // Remove Transmogrifications
		{
			for (uint8 Slot = EQUIPMENT_SLOT_START; Slot < EQUIPMENT_SLOT_END; Slot++)
				if (Item* pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, Slot))
				{
					if(pItem->FakeEntry)
					{
						CharacterDatabase.PExecute("DELETE FROM fake_items WHERE guid = %u", pItem->GetGUIDLow());
						pItem->FakeEntry = NULL;
					}
					pPlayer->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (Slot * 2), pItem->GetEntry());
                }
				OnGossipHello(pPlayer, pUnit);
		}
		else if(sender == EQUIPMENT_SLOT_END+3) // Remove Transmogrification from single item
		{
			if (Item* pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, uiAction))
			{
				if(pItem->FakeEntry)
				{
					CharacterDatabase.PExecute("DELETE FROM fake_items WHERE guid = %u", pItem->GetGUIDLow());
					pItem->FakeEntry = NULL;
				}
                else
                    pPlayer->GetSession()->SendNotification("This item is not transmogrified!");
				pPlayer->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (uiAction * 2), pItem->GetEntry());
			}
			OnGossipSelect(pPlayer, pUnit, EQUIPMENT_SLOT_END, uiAction);
		}
		else // Transmogrify
		{
			uint32 GUID = pPlayer->GetGUIDLow();
			if(Item* OLD = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, sender))
			{
				if(Items[GUID].find(uiAction) != Items[GUID].end() && Items[GUID][uiAction]->IsInWorld())
				{
					Item* pItem = Items[GUID][uiAction];
					if(pItem->GetOwnerGUID() == pPlayer->GetGUID() && (pItem->IsInBag() || pItem->GetBagSlot() == INVENTORY_SLOT_BAG_0) && IsSuitable(pItem, OLD, pPlayer))
					{
                        QueryResult queryResult = CharacterDatabase.PQuery("SELECT `fakeEntry` FROM `fake_items` WHERE `guid` = '%u'",OLD->GetGUIDLow());
                        if(!queryResult){
						    pPlayer->ModifyMoney(-1*GetSellPrice(OLD, pPlayer)); // take cost
						    pItem->SetNotRefundable(pPlayer);
						    pItem->SetBinding(true); // soulbound
						    uint32 FakeEntry = pItem->GetEntry();
                            CharacterDatabase.PExecute("INSERT INTO fake_items VALUES (%u, %u)", OLD->GetGUIDLow(), FakeEntry); // save for future usage.
						    OLD->FakeEntry = FakeEntry;
						    pPlayer->UpdateUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (sender * 2), FakeEntry); 
						    pPlayer->GetSession()->SendAreaTriggerMessage("%s transmogrified", GetSlotName(sender));
                        }else
                            pPlayer->GetSession()->SendNotification("This item is already transmogrified");
					}
					else
						pPlayer->GetSession()->SendNotification("Selected items are not suitable");
				}
				else
					pPlayer->GetSession()->SendNotification("Selected item does not exist");
			}
			else
				pPlayer->GetSession()->SendNotification("Equipment slot is empty");
			Items[GUID].clear();
			OnGossipSelect(pPlayer, pUnit, EQUIPMENT_SLOT_END, sender);
		}
		return true;
	}

private:
	std::map<uint64, std::map<uint32, Item*> > Items; // Items[GUID][DISPLAY] = item

	char * GetSlotName(uint8 slot)
	{
		switch(slot)
		{
		case EQUIPMENT_SLOT_HEAD      : return "Head";
		case EQUIPMENT_SLOT_SHOULDERS : return "Shoulders";
		case EQUIPMENT_SLOT_BODY      : return "Shirt";
		case EQUIPMENT_SLOT_CHEST     : return "Chest";
		case EQUIPMENT_SLOT_WAIST     : return "Waist";
		case EQUIPMENT_SLOT_LEGS      : return "Legs";
		case EQUIPMENT_SLOT_FEET      : return "Feet";
		case EQUIPMENT_SLOT_WRISTS    : return "Wrists";
		case EQUIPMENT_SLOT_HANDS     : return "Hands";
		case EQUIPMENT_SLOT_BACK      : return "Back";
		case EQUIPMENT_SLOT_MAINHAND  : return "Main hand";
		case EQUIPMENT_SLOT_OFFHAND   : return "Off hand";
		case EQUIPMENT_SLOT_RANGED    : return "Ranged";
		case EQUIPMENT_SLOT_TABARD    : return "Tabard";
		default: return NULL;
		}
	}

	bool IsSuitable(Item* pItem, Item* OLD, Player* pPlayer)
	{
		if(OLD->GetTemplate()->DisplayInfoID != pItem->GetTemplate()->DisplayInfoID)
		{
			if(const ItemTemplate* FakeItemTemplate = sObjectMgr->GetItemTemplate(OLD->FakeEntry))
				if(FakeItemTemplate->DisplayInfoID == pItem->GetTemplate()->DisplayInfoID)
					return false;
			if(pPlayer->CanUseItem(pItem, false) == EQUIP_ERR_OK)
            {
                uint32 NClass = pItem->GetTemplate()->Class;
                uint32 OClass = OLD->GetTemplate()->Class;
                uint32 NSubClass = pItem->GetTemplate()->SubClass;
                uint32 OSubClass = OLD->GetTemplate()->SubClass;
                uint32 NEWinv = pItem->GetTemplate()->InventoryType;
                uint32 OLDinv = OLD->GetTemplate()->InventoryType;
                if(NClass == OClass) 
	                if(NClass == ITEM_CLASS_WEAPON && NSubClass != ITEM_SUBCLASS_WEAPON_FISHING_POLE && OSubClass != ITEM_SUBCLASS_WEAPON_FISHING_POLE)
	                {
		                if(NSubClass == OSubClass || ((NSubClass == ITEM_SUBCLASS_WEAPON_BOW || NSubClass == ITEM_SUBCLASS_WEAPON_GUN || NSubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW) && (OSubClass == ITEM_SUBCLASS_WEAPON_BOW || OSubClass == ITEM_SUBCLASS_WEAPON_GUN || OSubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW)))
			                if(NEWinv == OLDinv || (NEWinv == INVTYPE_WEAPON && (OLDinv == INVTYPE_WEAPONMAINHAND || OLDinv == INVTYPE_WEAPONOFFHAND)))
				                return true;
	                }
	                else if(NClass == ITEM_CLASS_ARMOR)
		                if(NSubClass == OSubClass)
			                if(NEWinv == OLDinv || (NEWinv == INVTYPE_CHEST && OLDinv == INVTYPE_ROBE) || (NEWinv == INVTYPE_ROBE && OLDinv == INVTYPE_CHEST))
				                return true;
            }
		}
		return false;
	}

	uint32 GetSellPrice(Item* pItem, Player* pPlayer)
	{
		uint32 Price = 1000 * 100 * 100;
		return Price;
	}
};

void AddSC_Transmogrification()
{
    new npc_transmogrify();
}