/* Script de Tydrheal & Sungis */
/* Battleground: Temple de Kotmogu */


#include "Battleground.h"
#include "BattlegroundTK.h"
#include "Creature.h"
#include "GameObject.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"

// Debuff spell

BattlegroundTK::BattlegroundTK()
{
	BgObjects.resize(BG_TK_OBJECT_MAX);
    BgCreatures.resize(BG_CREATURES_MAX_TK);

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_TK_START_TWO_MINUTES; 
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_TK_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_TK_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_TK_HAS_BEGUN;
}

BattlegroundTK::~BattlegroundTK()
{
}

void BattlegroundTK::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
		BattlegroundTK::CalculatePoints(diff);

        if (GetElapsedTime() >= 27*MINUTE*IN_MILLISECONDS)
        {
            if (m_Team_Scores[TEAM_ALLIANCE] == 0)
            {
                if (m_Team_Scores[TEAM_HORDE] == 0)        // No one scored - result is tie
                    EndBattleground(WINNER_NONE);
                else                                 // Horde has more points and thus wins
                    EndBattleground(HORDE);
            }
            else if (m_Team_Scores[TEAM_HORDE] == 0)
                EndBattleground(ALLIANCE);           // Alliance has > 0, Horde has 0, alliance wins
            else if (m_Team_Scores[TEAM_HORDE] == m_Team_Scores[TEAM_ALLIANCE]) // Team score equal, winner is alliance
                EndBattleground(HORDE);
            else if (m_Team_Scores[TEAM_HORDE] > m_Team_Scores[TEAM_ALLIANCE])  // Last but not least, check who has the higher score
                EndBattleground(HORDE);
            else
                EndBattleground(ALLIANCE);
        }

        else if (GetElapsedTime() > uint32(_minutesElapsed * MINUTE * IN_MILLISECONDS) +  3 * MINUTE * IN_MILLISECONDS)
            ++_minutesElapsed;

		for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
			if (Player* player = ObjectAccessor::FindPlayer(itr->first))
			{
				if (player->GetGUID() == m_orbOwners[0])
					if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_BLUE))
						BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);

				if (player->GetGUID() == m_orbOwners[1])
					if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_PURPLE))
						BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);

				if (player->GetGUID() == m_orbOwners[2])
					if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_GREEN))
						BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);

				if (player->GetGUID() == m_orbOwners[3])
					if (player->IsMounted() || !player->HasAura(BG_TK_AURA_ORB_ORANGE))
						BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);
			}
    }
}

void BattlegroundTK::CalculatePoints(uint32 diff)
{
	if (pointsTimer <= 0)
	{
		for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
			if (Player* player = ObjectAccessor::FindPlayer(itr->first))
			{
				if (player->GetGUID() == m_orbOwners[0] ||
						player->GetGUID() == m_orbOwners[1] ||
						player->GetGUID() == m_orbOwners[2] ||
						player->GetGUID() == m_orbOwners[3])
					if (player->GetTeam() == ALLIANCE && player->GetExactDist2d(1783.319336f, 1333.339722f) <= 50.0f)
					{
						UpdateScore(ALLIANCE, BG_TK_CENTER_POINTS);
						UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_CENTER_POINTS);
					}

				if (player->GetGUID() == m_orbOwners[0] ||
						player->GetGUID() == m_orbOwners[1] ||
						player->GetGUID() == m_orbOwners[2] ||
						player->GetGUID() == m_orbOwners[3])
					if (player->GetTeam() == HORDE && player->GetExactDist2d(1783.319336f, 1333.339722f) <= 50.0f)
					{
						UpdateScore(HORDE, BG_TK_CENTER_POINTS);
						UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_CENTER_POINTS);
					}

				if (player->GetGUID() == m_orbOwners[0] ||
						player->GetGUID() == m_orbOwners[1] ||
						player->GetGUID() == m_orbOwners[2] ||
						player->GetGUID() == m_orbOwners[3])
					if (player->GetTeam() == ALLIANCE && player->GetExactDist2d(1783.319336f, 1333.339722f) > 50.0f && player->GetExactDist2d(1783.319336f, 1333.339722f) <= 100.0f)
					{
						UpdateScore(ALLIANCE, BG_TK_INDOOR_POINTS);
						UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_INDOOR_POINTS);
					}

				if (player->GetGUID() == m_orbOwners[0] ||
						player->GetGUID() == m_orbOwners[1] ||
						player->GetGUID() == m_orbOwners[2] ||
						player->GetGUID() == m_orbOwners[3])
					if (player->GetTeam() == HORDE && player->GetExactDist2d(1783.319336f, 1333.339722f) > 50.0f && player->GetExactDist2d(1783.319336f, 1333.339722f) <= 100.0f)
					{
						UpdateScore(HORDE, BG_TK_INDOOR_POINTS);
						UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_INDOOR_POINTS);
					}

				if (player->GetGUID() == m_orbOwners[0] ||
						player->GetGUID() == m_orbOwners[1] ||
						player->GetGUID() == m_orbOwners[2] ||
						player->GetGUID() == m_orbOwners[3])
					if (player->GetTeam() == ALLIANCE && player->GetExactDist2d(1783.319336f, 1333.339722f) > 100.0f)
					{
						UpdateScore(ALLIANCE, BG_TK_OUTDOOR_POINTS);
						UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_OUTDOOR_POINTS);
					}

				if (player->GetGUID() == m_orbOwners[0] ||
						player->GetGUID() == m_orbOwners[1] ||
						player->GetGUID() == m_orbOwners[2] ||
						player->GetGUID() == m_orbOwners[3])
					if (player->GetTeam() == HORDE && player->GetExactDist2d(1783.319336f, 1333.339722f) > 100.0f)
					{
						UpdateScore(HORDE, BG_TK_OUTDOOR_POINTS);
						UpdatePlayerScore(player, SCORE_POINTS_SCORED, BG_TK_OUTDOOR_POINTS);
					}

					pointsTimer = 4000;
			}
		} else pointsTimer -= diff;
}

void BattlegroundTK::Reset()
{
    Battleground::Reset();

	pointsTimer			= 4000;
    m_orbOwners[0]		= 0;
    m_orbOwners[1]		= 0;
	m_orbOwners[2]		= 0;
    m_orbOwners[3]		= 0;

    _orbState[0]        = BG_TK_ORB_STATE_ON_BASE;
    _orbState[1]        = BG_TK_ORB_STATE_ON_BASE;
	_orbState[2]        = BG_TK_ORB_STATE_ON_BASE;
    _orbState[3]        = BG_TK_ORB_STATE_ON_BASE;

    m_Team_Scores[TEAM_ALLIANCE]      = 0;
    m_Team_Scores[TEAM_HORDE]         = 0;

    if (sBattlegroundMgr->IsBGWeekend(GetTypeID()))
    {
        m_HonorWinKills = 3;
        m_HonorEndKills = 4;
    }

    else
    {
        m_HonorWinKills = 1;
        m_HonorEndKills = 2;
    }

	m_IsInformedNearVictory		= false;
	bgEnd = false;
}

void BattlegroundTK::StartingEventCloseDoors()
{
    DoorClose(BG_TK_OBJECT_DOOR_A);
	DoorClose(BG_TK_OBJECT_DOOR_H);
    SpawnBGObject(BG_TK_OBJECT_DOOR_A, RESPAWN_IMMEDIATELY);
	SpawnBGObject(BG_TK_OBJECT_DOOR_H, RESPAWN_IMMEDIATELY);
    for (uint32 i = BG_TK_OBJECT_ORB_BLUE; i <= BG_TK_OBJECT_ORB_ORANGE; ++i)
        SpawnBGObject(i, RESPAWN_ONE_DAY);
}

void BattlegroundTK::StartingEventOpenDoors()
{
    DoorOpen(BG_TK_OBJECT_DOOR_A);
    DoorOpen(BG_TK_OBJECT_DOOR_H);

    for (uint32 i = BG_TK_OBJECT_ORB_BLUE; i <= BG_TK_OBJECT_ORB_ORANGE; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundTK::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);

    BattlegroundTKScore* sc = new BattlegroundTKScore;

    PlayerScores[player->GetGUID()] = sc;
}

void BattlegroundTK::RespawnOrbAfterDrop(uint32 orb)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

	switch (orb)
	{
		case BG_TK_OBJECT_ORB_BLUE:
			SpawnBGObject(BG_TK_OBJECT_ORB_BLUE, RESPAWN_IMMEDIATELY);
			SendMessageToAll(LANG_BG_TK_ORB_BLUE_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
			_orbState[BG_TK_OBJECT_ORB_BLUE] = BG_TK_ORB_STATE_ON_BASE;
			break;

		case BG_TK_OBJECT_ORB_PURPLE:
			SpawnBGObject(BG_TK_OBJECT_ORB_PURPLE, RESPAWN_IMMEDIATELY);
			SendMessageToAll(LANG_BG_TK_ORB_PURPLE_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
			_orbState[BG_TK_OBJECT_ORB_PURPLE] = BG_TK_ORB_STATE_ON_BASE;
			break;

		case BG_TK_OBJECT_ORB_GREEN:
			SpawnBGObject(BG_TK_OBJECT_ORB_GREEN, RESPAWN_IMMEDIATELY);
			SendMessageToAll(LANG_BG_TK_ORB_GREEN_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
			_orbState[BG_TK_OBJECT_ORB_GREEN] = BG_TK_ORB_STATE_ON_BASE;
			break;

		case BG_TK_OBJECT_ORB_ORANGE:
			SpawnBGObject(BG_TK_OBJECT_ORB_ORANGE, RESPAWN_IMMEDIATELY);
			SendMessageToAll(LANG_BG_TK_ORB_ORANGE_RESPAWN, CHAT_MSG_BG_SYSTEM_NEUTRAL);
			_orbState[BG_TK_OBJECT_ORB_ORANGE] = BG_TK_ORB_STATE_ON_BASE;
			break;
	}

    PlaySoundToAll(BG_TK_SOUND_ORB_RETURNED);

    SetOrbPicker(0, orb);
}

void BattlegroundTK::EventPlayerClickedOnFlag(Player* Source, GameObject* target_obj)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

	if(HasAnOrb(Source))
		return;

    int32 message_id = 0;
    ChatMsg type = CHAT_MSG_BG_SYSTEM_NEUTRAL;

	// Udapte orb blue picked up
    if (GetOrbState(BG_TK_OBJECT_ORB_BLUE) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_BLUE] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_BLUE;

		if (Source->GetTeam() == ALLIANCE)
		{
			type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
			PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
		}

		if (Source->GetTeam() ==  HORDE)
		{
			type = CHAT_MSG_BG_SYSTEM_HORDE;
			PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
		}

        SpawnBGObject(BG_TK_OBJECT_ORB_BLUE, RESPAWN_ONE_DAY);
		Source->CastSpell(Source, BG_TK_AURA_ORB_BLUE, true);
        SetOrbPicker(Source->GetGUID(), BG_TK_OBJECT_ORB_BLUE);
        _orbState[BG_TK_OBJECT_ORB_BLUE] = BG_TK_ORB_STATE_ON_PLAYER;
    }

    // Udapte orb purple picked up
    if (GetOrbState(BG_TK_OBJECT_ORB_PURPLE) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_PURPLE] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_PURPLE;

        if (Source->GetTeam() == ALLIANCE)
		{
			type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
			PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
		}

		if (Source->GetTeam() == HORDE)
		{
			type = CHAT_MSG_BG_SYSTEM_HORDE;
			PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
		}

        SpawnBGObject(BG_TK_OBJECT_ORB_PURPLE, RESPAWN_ONE_DAY);
		Source->CastSpell(Source, BG_TK_AURA_ORB_PURPLE, true);
        SetOrbPicker(Source->GetGUID(), BG_TK_OBJECT_ORB_PURPLE);
        _orbState[BG_TK_OBJECT_ORB_PURPLE] = BG_TK_ORB_STATE_ON_PLAYER;
    }

	// Udapte orb green picked up
	if (GetOrbState(BG_TK_OBJECT_ORB_GREEN) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_GREEN] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_GREEN;
        if (Source->GetTeam() == ALLIANCE)
		{
			type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
			PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
		}

		if (Source->GetTeam() ==  HORDE)
		{
			type = CHAT_MSG_BG_SYSTEM_HORDE;
			PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
		}

        SpawnBGObject(BG_TK_OBJECT_ORB_GREEN, RESPAWN_ONE_DAY);
		Source->CastSpell(Source, BG_TK_AURA_ORB_GREEN, true);
        SetOrbPicker(Source->GetGUID(), BG_TK_OBJECT_ORB_GREEN);
        _orbState[BG_TK_OBJECT_ORB_GREEN] = BG_TK_ORB_STATE_ON_PLAYER;
    }

	// Udapte orb orange picked up
	if (GetOrbState(BG_TK_OBJECT_ORB_ORANGE) == BG_TK_ORB_STATE_ON_BASE
        && BgObjects[BG_TK_OBJECT_ORB_ORANGE] == target_obj->GetGUID())
    {
        message_id = LANG_BG_TK_PICKEDUP_ORB_ORANGE;

        if (Source->GetTeam() == ALLIANCE)
		{
			type = CHAT_MSG_BG_SYSTEM_ALLIANCE;
			PlaySoundToAll(BG_TK_SOUND_ALLIANCE_ORB_PICKED_UP);
		}

		if (Source->GetTeam() ==  HORDE)
		{
			type = CHAT_MSG_BG_SYSTEM_HORDE;
			PlaySoundToAll(BG_TK_SOUND_HORDE_ORB_PICKED_UP);
		}

        SpawnBGObject(BG_TK_OBJECT_ORB_ORANGE, RESPAWN_ONE_DAY);
		Source->CastSpell(Source, BG_TK_AURA_ORB_ORANGE, true);
        SetOrbPicker(Source->GetGUID(), BG_TK_OBJECT_ORB_ORANGE);
        _orbState[BG_TK_OBJECT_ORB_ORANGE] = BG_TK_ORB_STATE_ON_PLAYER;
    }

    if (!message_id)
        return;

	UpdatePlayerScore(Source, SCORE_ORB_POSSESIONS, 1);
	Battleground::PSendMessageToAll(message_id, type, NULL, Source->GetName().c_str());
    Source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundTK::RemovePlayer(Player* player, uint64 guid, uint32 /*team*/)
{
    if (m_orbOwners[BG_TK_OBJECT_ORB_BLUE] == guid)
    {
        if (!player)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlegroundTK: Removing offline player who has the ORB!");
			player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_BLUE);
            SetOrbPicker(0, BG_TK_OBJECT_ORB_BLUE);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);
    }

    if (m_orbOwners[BG_TK_OBJECT_ORB_PURPLE] == guid)
    {
        if (!player)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlegroundTK: Removing offline player who has the ORB!");
			player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_PURPLE);
            SetOrbPicker(0, BG_TK_OBJECT_ORB_PURPLE);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);
    }

	if (m_orbOwners[BG_TK_OBJECT_ORB_GREEN] == guid)
    {
        if (!player)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlegroundTK: Removing offline player who has the ORB!");
			player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_GREEN);
            SetOrbPicker(0, BG_TK_OBJECT_ORB_GREEN);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);
    }

	if (m_orbOwners[BG_TK_OBJECT_ORB_ORANGE] == guid)
    {
        if (!player)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlegroundTK: Removing offline player who has the ORB!");
			player->RemoveAurasDueToSpell(BG_TK_AURA_ORB_ORANGE);
            SetOrbPicker(0, BG_TK_OBJECT_ORB_ORANGE);
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);
        }
        else
            BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);
    }
}

void BattlegroundTK::UpdateScore(uint16 team, int16 points)
{
    ASSERT(team == ALLIANCE || team == HORDE);
    uint8 teamindex = GetTeamIndexByTeamId(team); // 0 = Alliance 1 = Horde

    if (points > 0)
	{
		if (points + m_Team_Scores[teamindex] >= BG_TK_MAX_TEAM_SCORE)
		{
			if (!bgEnd)
			{
				m_Team_Scores[teamindex] = BG_TK_MAX_TEAM_SCORE;
				UpdateWorldState(((teamindex == TEAM_ALLIANCE)?BG_TK_RESOURCES_ALLIANCE:BG_TK_RESOURCES_HORDE), m_Team_Scores[teamindex]);
				EndBattleground((teamindex == TEAM_ALLIANCE)?ALLIANCE:HORDE);
				bgEnd = true; // Prevent from UpdateScore repeat EndBattleground
			}
		}
		else
		{
			m_Team_Scores[teamindex] += points;
			UpdateWorldState(((teamindex == TEAM_ALLIANCE)?BG_TK_RESOURCES_ALLIANCE:BG_TK_RESOURCES_HORDE), m_Team_Scores[teamindex]);
		}

		if (!m_IsInformedNearVictory && m_Team_Scores[teamindex] > BG_TK_WARNING_NEAR_VICTORY_SCORE)
		{
			if (team == ALLIANCE)
				SendMessageToAll(LANG_BG_TK_A_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
			else
				SendMessageToAll(LANG_BG_TK_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);

			PlaySoundToAll(BG_TK_SOUND_NEAR_VICTORY);
			m_IsInformedNearVictory = true;
		}
    }
}

WorldSafeLocsEntry const* BattlegroundTK::GetClosestGraveYard(Player* player)
{
	if (player->GetTeam() == ALLIANCE)
		return sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[0]);

	else
		return sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[1]);
}

bool BattlegroundTK::SetupBattleground()
{
    // Orbs
    if (!AddObject(BG_TK_OBJECT_ORB_BLUE, BG_OBJECT_TK_ORB_BLUE, 1716.808105f, 1249.910645f, 10.675041f, 0.0f, 0, 0, 0.9996573f, 0.02617699f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TK_OBJECT_ORB_PURPLE, BG_OBJECT_TK_ORB_PURPLE, 1850.244995f, 1416.825195f, 10.662804f, 0.0f, 0, 0, 0.008726535f, 0.9999619f, RESPAWN_IMMEDIATELY)
		|| !AddObject(BG_TK_OBJECT_ORB_GREEN, BG_OBJECT_TK_ORB_GREEN, 1716.775635f, 1416.752686f, 10.678053f, 0.0f, 0, 0, 0.008726535f, 0.9999619f, RESPAWN_IMMEDIATELY)
		|| !AddObject(BG_TK_OBJECT_ORB_ORANGE, BG_OBJECT_TK_ORB_ORANGE, 1850.234375f, 1250.272827f, 10.689357f, 0.0f, 0, 0, 0.008726535f, 0.9999619f, RESPAWN_IMMEDIATELY)
        // Horde gate
        || !AddObject(BG_TK_OBJECT_DOOR_H, BG_OBJECT_DOOR_TK_H, 1780.151733f, 1565.395874f, 22.469021f, 4.70132f, 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        // Alliance gate
        || !AddObject(BG_TK_OBJECT_DOOR_A, BG_OBJECT_DOOR_TK_A, 1783.735107f, 1104.960205f, 19.374258f, 1.62f, 0.0f, 0.0f, 0.71615f, 0.697947f, RESPAWN_IMMEDIATELY)
		)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "BattleGroundTK: Failed to spawn some object Battleground not created!");
        return false;
    }

    WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[0]); // Alliance
    if (!sg || !AddSpiritGuide(TK_SPIRIT_MAIN_ALLIANCE, sg->x, sg->y, sg->z, 2.498876f, ALLIANCE))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "BattleGroundTK: Failed to spawn Alliance spirit guide! Battleground not created!");
        return false;
    }

    sg = sWorldSafeLocsStore.LookupEntry(BG_TK_GraveyardIds[1]); // Horde
    if (!sg || !AddSpiritGuide(TK_SPIRIT_MAIN_HORDE, sg->x, sg->y, sg->z, 5.936957f, HORDE))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "BatlteGroundTK: Failed to spawn Horde spirit guide! Battleground not created!");
        return false;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "BattleGroundTK: BG objects and spirit guides spawned");

    return true;
}

void BattlegroundTK::EndBattleground(uint32 winner)
{
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);

    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    Battleground::EndBattleground(winner);
}

void BattlegroundTK::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
	
	UpdateScore(killer->GetTeam(), BG_TK_PLAYER_KILL_POINTS);
	UpdatePlayerScore(killer, SCORE_POINTS_SCORED, BG_TK_PLAYER_KILL_POINTS);

	if (m_orbOwners[0] == player->GetGUID())
        BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_BLUE);

	if (m_orbOwners[1] == player->GetGUID())
		BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_PURPLE);

	if (m_orbOwners[2] == player->GetGUID())
		BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_GREEN);

	if (m_orbOwners[3] == player->GetGUID())
		BattlegroundTK::RespawnOrbAfterDrop(BG_TK_OBJECT_ORB_ORANGE);

    Battleground::HandleKillPlayer(player, killer);
}

void BattlegroundTK::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    switch (type)
    {
        case SCORE_ORB_POSSESIONS : // Orbs owned
            ((BattlegroundTKScore*)itr->second)->OrbPossesions += value;
            break;

        case SCORE_POINTS_SCORED: // Points earned
            ((BattlegroundTKScore*)itr->second)->PointsScored += value;
            break;

        default:
            Battleground::UpdatePlayerScore(Source, type, value, doAddHonor);
            break;
    }
}

bool BattlegroundTK::HasAnOrb(Player* player)
{
	if (m_orbOwners[0] == player->GetGUID())
		return true;
	else if (m_orbOwners[1] == player->GetGUID())
		return true;
	else if (m_orbOwners[2] == player->GetGUID())
		return true;
	else if (m_orbOwners[3] == player->GetGUID())
		return true;
	else return false;
}

void BattlegroundTK::FillInitialWorldStates(WorldPacket& data)
{
	data << uint32(BG_TK_RESOURCES_ALLIANCE) << uint32(m_Team_Scores[TEAM_ALLIANCE]);
    data << uint32(BG_TK_RESOURCES_HORDE) << uint32(m_Team_Scores[TEAM_HORDE]);
}

uint32 BattlegroundTK::GetPrematureWinner()
{
    if (m_Team_Scores[TEAM_ALLIANCE] > m_Team_Scores[TEAM_HORDE])
        return ALLIANCE;
    else if (m_Team_Scores[TEAM_HORDE] > m_Team_Scores[TEAM_ALLIANCE])
        return HORDE;

    return Battleground::GetPrematureWinner();
}