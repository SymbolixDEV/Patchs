#include "ScriptPCH.h"

class TORSYSTEM : public PlayerScript
{
    public:
        TORSYSTEM() : PlayerScript("TORSYSTEM") {	}
//SymbolixDEV
    void OnPVPKill(Player* Killer, Player* Dead)
    {
	if (Killer->GetMap()->IsBattleArena())
	{
		Killer->GetSession()->SendNotification("You just killed %s and stole his soul + You Earned 2 Badge!", Dead->GetName()); 
		Killer->AddItem(29434, 2); // Adding Badge Line for winner
		Dead->GetSession()->SendNotification("You just Dead from %s + You Cuted from 1 Badge"), Killer->Getname()); //Remove 1 badge if player die for another player
		Dead->AddItem(29434, -1); // -1 Removing 1 badge from losser
	}
    }
};

void AddSC_TORSYSTEM()
{
    new TORSYSTEM;
}
