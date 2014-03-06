#include "SpellLearn.h"
#include "SpellInfo.h"
#include "DBCStores.h"
#include "SpellMgr.h"
#include "World.h"


SpellLearnMgr::SpellLearnMgr()
{
}

SpellLearnMgr::~SpellLearnMgr()
{
}

void SpellLearnMgr::Load()
{
	for(uint32 i = 0 ; i < sChrSpecializationStore.GetNumRows() ; i++)
	{
		if(ChrSpecializationEntry const* specialisation = sChrSpecializationStore.LookupEntry(i))
		{
			sSpecializationMap[specialisation->ClassId].push_back(specialisation->Id);
		}
	}

	for(uint32 i = 0 ; i < sChrClassesStore.GetNumRows() ; i++)
	{
		if(ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(i))
		{
			//common branch without any spec
			sSpecializationMap[classEntry->ClassID].push_back(0);

			sSpellLearnMap[classEntry->ClassID] = new LevelsList;
			for(uint32 y = 0 ; y < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) ; y++)
			{
				sSpellLearnMap[classEntry->ClassID]->push_back(new SpecialisationList);
				
				for( std::list<uint32>::iterator itr = sSpecializationMap[classEntry->ClassID].begin() ; itr != sSpecializationMap[classEntry->ClassID].end() ; itr++ )
				{
					(*((*(sSpellLearnMap[classEntry->ClassID]))[y]))[*itr] = new SpellList;
				}
			}
		}
	}

	uint32 oldMSTime = getMSTime();

	PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_SPELLS_BY_LEVELS);
	PreparedQueryResult result = WorldDatabase.Query(stmt);

	if (!result)
	{
		sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spells to learn. DB table `spell_to_learn` is empty.");
		return;
	}

	uint32 count = 0;
	do
	{
		Field* fields           = result->Fetch();
		uint32 spellId			= fields[0].GetUInt32();
		uint32 classId      	= fields[1].GetUInt32();
		uint32 specId       	= fields[2].GetUInt32();
		uint32 level     		= fields[3].GetUInt32();
		uint32 faction			= fields[4].GetUInt32();

		SpellLearn spell;
		spell.spellId = spellId;
		spell.faction = faction;

		// Check informations

		ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(classId);
		if(!classEntry)
			continue;

		std::list<uint32>::iterator iter = std::find(sSpecializationMap[classId].begin(), sSpecializationMap[classId].end(), specId);
		if (iter == sSpecializationMap[classId].end())
			continue;

		if (faction > 3) faction = 0;

		level = std::min(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL), level);
		level = std::max(level, (uint32)1);

		// Add spell

		((*((*(sSpellLearnMap[classId]))[level-1]))[specId])->push_back(spell);

		++count;
	} while (result->NextRow());

	sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spells to learn in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}



void SpellLearn::UpdateForPlayer(Player *player, bool remove)
{
    if(remove)
    {
        player->removeSpell(spellId);
        return;
    }

	switch(faction)
	{
	    case 0: // Disabled Spell
		    player->removeSpell(spellId);
			player->RemoveAurasDueToSpell(spellId);
		    break;

	    case 1: // Alliance
		    if (player->GetTeamId() == TEAM_ALLIANCE)
			    player->learnSpell(spellId, false);
		    else
			{
			    player->removeSpell(spellId);
				player->RemoveAurasDueToSpell(spellId);
			}
		    break;

	    case 2: // Horde
		    if (player->GetTeamId() == TEAM_HORDE)
			    player->learnSpell(spellId, false);
		    else
			{
			    player->removeSpell(spellId);
				player->RemoveAurasDueToSpell(spellId);
			}
		    break;

	    case 3: // Both
		    player->learnSpell(spellId, false);
		    break;

	    default:
		    break;
	}
}

void SpellLearnMgr::UpdatePlayerSpells(Player* player)
{
	if(!player) return;

	uint8 classid = player->getClass();
	uint32 spec = player->GetPrimaryTalentTree(player->GetActiveSpec());
	uint8 level = std::min(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL), (uint32)player->getLevel());

	// Check for cheating/errors

	ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(player->getClass());
	if(!classEntry)
		return;


	std::list<uint32>::iterator iter = std::find(sSpecializationMap[classid].begin(), sSpecializationMap[classid].end(), spec);
	if (iter == sSpecializationMap[classid].end())
		return;


	if (LevelsList *levelsList = sSpellLearnMap[classid])
	{
		for(uint32 i = 0 ; i < level ; ++i)
		{
			if (SpecialisationList *specList = (*levelsList)[i])
			{
				if (SpellList *spellList = (*specList)[spec])
					for(SpellList::iterator itr = spellList->begin(); itr != spellList->end(); ++itr)
						itr->UpdateForPlayer(player);

				if (spec != 0)
					if (SpellList *spellList = (*specList)[0])
						for(SpellList::iterator itr = spellList->begin(); itr != spellList->end(); ++itr)
							itr->UpdateForPlayer(player);
			}
		}

        for(uint32 i = level ; i < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) ; i++)
        {
            if (SpecialisationList *specList = (*levelsList)[i])
            {
                if (SpellList *spellList = (*specList)[spec])
                    for(SpellList::iterator itr = spellList->begin(); itr != spellList->end(); ++itr)
                        itr->UpdateForPlayer(player, true);

                if (spec != 0)
                    if (SpellList *spellList = (*specList)[0])
                        for(SpellList::iterator itr = spellList->begin(); itr != spellList->end(); ++itr)
                            itr->UpdateForPlayer(player, true);
            }
        }
	}

}