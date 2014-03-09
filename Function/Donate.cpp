/*script made by: SymbolixDEV
Complete : 40%
*/
class donate_npc : public CreatureScript
{
        public:
                donate_npc() : CreatureScript("donate_npc"){}
     
                bool OnGossipHello(Player * pPlayer, Creature * pCreature)
                {
                        pPlayer->ADD_GOSSIP_ITEM(4, "What is donate ?!", GOSSIP_SENDER_MAIN, 1);
                                                pPlayer->ADD_GOSSIP_ITEM(4, "Close me! ", GOSSIP_SENDER_MAIN, 100);
                        pPlayer->PlayerTalkClass->SendGossipMenu(9425, pCreature->GetGUID());
                                                                       
                        return true;
                }
     
                bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
                {
                pPlayer->PlayerTalkClass->ClearMenus();
                                       
					switch(pPlayer->getClass())
                    {
						
						case CLASS_WARRIOR:
							{
								pCreature->MonsterWhisper("text!", pPlayer->GetGUID(), true);
							break;
 
                        case CLASS_PALADIN:
                            {
								pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_HUNTER:
                            {
                                pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_ROGUE:
							{
								pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_PRIEST:
                            {
								pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_DEATH_KNIGHT:
                            {
                                pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
						case CLASS_SHAMAN:
                            {
								pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_MAGE:
                            {
                                pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_WARLOCK:
                            {
								pCreature->MonsterWhisper("Text here!", pPlayer->GetGUID(), true);
                            }break;
 
                        case CLASS_DRUID:
                            {
                                pCreature->MonsterWhisper("Text here!!", pPlayer->GetGUID(), true);
                            }break;
 
                        case 100:
                            {
                                pPlayer->CLOSE_GOSSIP_MENU();
                            }break;
                                     
                    }
       return true;
};
     
void AddSC_donate_npc()
{
        new donate_npc();
}
