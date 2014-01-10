/*
PvP KillStreak
*/
#include "ScriptPCH.h"
#include "Chat.h"

class Kill_You : public PlayerScript
{
public:
	Kill_You() : PlayerScript("Kill_You") { }

	void OnPVPKill(Player * killer, Player * victim)
	{
		if (killer->GetGUID() == victim->GetGUID())
			return;
		/*--Gives killer money--*/
		killer->ModifyMoney(1000);
		//Resets casters full power
		killer->ResetAllPowers();	
		//Set killer spell
		killer->CastSpell(killer, 23128, true);
		//killer info
		ChatHandler(killer->GetSession()).PSendSysMessage("You killed %s for gold!", victim->GetName().c_str());
		//Deceased info
		ChatHandler(victim->GetSession()).PSendSysMessage("You were killed by %s!", killer->GetName().c_str());
		//Define kill experience
        uint32 level = killer->getLevel();
        uint32 secLevel = victim->getLevel();
		uint32 exp = 0; 
		//Set experience
		if (secLevel <= ( level - 10 ) || secLevel >= ( 10 + level) )
			return;
		if(level <= 10)
			exp = 300;
		else if(level <= 20 )
			exp = 100 * 2;
		else if(level <= 30)
			exp = 100 * 3;
		else if (level <= 40)
			exp = 100 * 4;
		else if (level <= 50)
			exp = 100 * 5;
		else if (level <= 60 )
			exp = 100 * 6;
		else if (level <= 70 )
			exp = 100 * 7;
		killer->GiveXP(exp, killer);
	}
};

void AddSC_Kill_You()
{
	new Kill_You;
}