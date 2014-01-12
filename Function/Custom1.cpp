//Not Tested

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"

class CustomNPC : public CreatureScript
{
public:
	CustomNPC() : CreatureScript("CustomNPC") { }

	bool OnGossipHello(Player* plr, Creature* npc)
	{
		plr->ADD_GOSSIP_ITEM(2, "|cffFFFF00|Heal me", GOSSIP_SENDER_MAIN, 1);
		plr->ADD_GOSSIP_ITEM(3, "Close", GOSSIP_SENDER_MAIN, 2);
		plr->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, npc->GetGuid());
		return true;
		
	{

		bool OnGossipSelect(Player* plr, Creature* npc, uint32 sender, uint32 uiAction)
	{
		if (!plr)
			return false;
		plr->PlayerTalkClass->ClearMenus();

		switch(uiAction)
		{
		case 1:
			{
				uint32 MaxHealth = plr->GetMaxHealth();
				plr->SetHealth(MaxHealth);
				plr->GetSession()->SendAreaTriggerMessage("You've been healed!");
				plr->CLOSE_GOSSIP_MENU();
			}
		case 2:
			{
				plr->CLOSE_GOSSIP_MENU();
			}
		}
		return true;

		item_faction() : ItemScript("item_faction") { }
     
        bool OnUse(Player* player, Item* item, const SpellCastTargets &)
        {
                    if (player->IsInCombat())
                    {      
                    player->GetSession()->SendNotification("You can't use this item in combat!");
                            return false;
                    }
                    player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                    player->GetSession()->SendNotification("You need to relog,to change your faction !");
                    player->DestroyItemCount(item_id, 1, true, false);
                    return true;
            }
    
	}
};

void AddSC_CustomNPC()
{
	new CustomNPC();
}
