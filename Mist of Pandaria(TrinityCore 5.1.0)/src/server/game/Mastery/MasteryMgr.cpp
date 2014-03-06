#include "MasteryMgr.h"

void MasteryMgr::LoadFromDB()
{
	uint32 oldMSTime = getMSTime();

	PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_MASTERIES);
	PreparedQueryResult result = WorldDatabase.Query(stmt);

	if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Masteries. DB table `masteries` is empty.");
        return;
    }

	uint32 count = 0;

	do
    {
        Field* fields = result->Fetch();

		Mastery m;

		uint32 id = fields[0].GetInt32();
		m.base = fields[1].GetFloat();
		m.ratio = fields[2].GetFloat();

		masteriesMap[(TalentTree)id] = m;

        ++count;
    } while (result->NextRow());

	sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Masteries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}


Mastery MasteryMgr::getMastery(TalentTree specId)
{
	std::map<TalentTree, Mastery>::const_iterator itr = masteriesMap.find(specId);
	if(itr != masteriesMap.end())
	{
		return itr->second;
	}
	else // Not loaded Mastery
	{
		Mastery m;
		m.base = 0.0f;
		m.ratio = 0.0f;
		return m;
	}
}