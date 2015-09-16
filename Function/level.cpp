/*
<--------------------------------------------------------------------------->
 - Developer(s): SymbolixDEV
 - Complete: %100
 - ScriptName: 'Level NPC' 
 - Comment: untested.
<--------------------------------------------------------------------------->
*/

#include "ScriptPCH.h"
     
 
class level_npc : public CreatureScript
{
	public:
	level_npc() : CreatureScript("level_npc"){}
	bool OnGossipHello(Player * pPlayer, Creature * pCreature)
	{
	  	pPlayer->ADD_GOSSIP_ITEM(4, "Please give me 80 Level!!", GOSSIP_SENDER_MAIN, 1);
	  	pPlayer->PlayerTalkClass->SendGossipMenu(9425, pCreature->GetGUID());
	  	return true;
	}
	
	bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus();
		switch(pPlayer->getClass())
		{
			case 1:
			  {
			      pPlayer->GiveLevel(80);
				    pPlayer->SetMoney(99000000);
				    pPlayer->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
				    pPlayer->TeleportTo(571, 5807.95f, 587.651f, 660.938f, 1.69498f);
				    pPlayer->PlayerTalkClass->SendCloseGossip();
			  }break;                             
		}
		return true;
	}
};
     
void AddSC_level_npc()
{
        new level_npc();
}
