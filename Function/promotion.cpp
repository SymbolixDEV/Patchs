#include "ScriptPCH.h"
#include "Language.h"

class SymbolixDEV_NPC : public CreatureScript
{
public:
    SymbolixDEV_NPC() : CreatureScript("SymbolixDEV_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		player->ADD_GOSSIP_ITEM(1, "Resto Druid [PvP]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		player->ADD_GOSSIP_ITEM(1, "Feral Druid [PvP]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		player->ADD_GOSSIP_ITEM(1, "Balance Druid [PvP]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        
        if (sender == GOSSIP_SENDER_MAIN)
        {
            switch (action)
            {
				{
                case GOSSIP_ACTION_INFO_DEF + 1:
					player->AddItem(51419, 1);
					player->AddItem(51420, 1);
					player->AddItem(51421, 1);
					player->AddItem(51422, 1);
					player->AddItem(51424, 1);
					player->AddItem(50454, 1);
					player->AddItem(51341, 1);
					player->AddItem(51340, 1);
					player->AddItem(51342, 1);
					player->AddItem(50734, 1);
					player->AddItem(51409, 1);
					player->AddItem(51349, 1);
					player->AddItem(50400, 1);
					player->AddItem(50610, 1);
					player->AddItem(50726, 1);
					player->AddItem(51378, 1);
					player->AddItem(51377, 1);
					player->AddItem(51409, 1);
					player->AddItem(43236, 20);
					player->AddItem(51346, 1);
                    player->SaveToDB();
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

				case GOSSIP_ACTION_INFO_DEF + 2:
					player->AddItem(50402, 1);
					player->AddItem(50618, 1);
					player->AddItem(51425, 1);
					player->AddItem(51426, 1);
					player->AddItem(51427, 1);
					player->AddItem(51428, 1);
					player->AddItem(51430, 1);
					player->AddItem(50456, 1);
					player->AddItem(51378, 1);
					player->AddItem(51377, 1);
					player->AddItem(50363, 1);
					player->AddItem(50402, 1);
					player->AddItem(50402, 1);
				/*	player->AddItem(50402, 1);
					player->AddItem(50402, 1);
					player->AddItem(50402, 1);
					player->AddItem(50402, 1);
					player->AddItem(50402, 1);
					player->AddItem(50402, 1);
					player->AddItem(50402, 1); */
					player->SaveToDB();
                    player->PlayerTalkClass->SendCloseGossip();
					break;

				case GOSSIP_ACTION_INFO_DEF + 3:
					player->AddItem(51433, 1);
					player->AddItem(51434, 1);
					player->AddItem(51435, 1);
					player->AddItem(51436, 1);
					player->AddItem(50658, 1);
					player->AddItem(50398, 1);
					player->AddItem(51378, 1);
					player->AddItem(51377, 1);
					player->AddItem(54588, 1);
					player->AddItem(51438, 1);
					player->AddItem(50614, 1);
					player->AddItem(51407, 1);
					player->AddItem(51438, 1);
					player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
					break;
				}
            }
		}
            return true;
    }
};

void AddSC_SymbolixDEV_NPC()
{
    new SymbolixDEV_NPC();
}
