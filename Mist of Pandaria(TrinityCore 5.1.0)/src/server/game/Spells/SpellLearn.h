#ifndef _SPELL_LEARN_MGR_H
#define _SPELL_LEARN_MGR_H

#include <ace/Singleton.h>
#include "Common.h"
#include "SharedDefines.h"
#include "Spell.h"
#include "Player.h"

struct SpellLearn
{
	uint32 spellId;
	uint32 faction;

	void UpdateForPlayer(Player *player, bool remove = false);
};

class SpellLearnMgr
{
	friend class ACE_Singleton<SpellLearnMgr, ACE_Null_Mutex>;

private:
	SpellLearnMgr();
	~SpellLearnMgr();

	typedef std::list<SpellLearn> SpellList;
	typedef std::map<uint32, SpellList*> SpecialisationList;
	typedef std::vector<SpecialisationList*> LevelsList;
	typedef std::map<uint32, LevelsList*> SpellLearnMap;
	typedef std::map<uint32, std::list<uint32> > SpecializationSpecByClass;


	SpellLearnMap sSpellLearnMap;
	SpecializationSpecByClass sSpecializationMap;

public:
	void Load();
	void UpdatePlayerSpells(Player* player);
};

#define sSpellLearnMgr ACE_Singleton<SpellLearnMgr, ACE_Null_Mutex>::instance()
#endif