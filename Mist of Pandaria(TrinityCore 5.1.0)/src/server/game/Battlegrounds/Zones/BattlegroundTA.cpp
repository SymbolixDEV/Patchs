/* # Script de Sungis : Tol'Viron Arena # */

#include "BattlegroundTA.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundTA::BattlegroundTA()
{
    BgObjects.resize(BG_TA_OBJECT_MAX);

    StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattlegroundTA::~BattlegroundTA()
{

}

void BattlegroundTA::StartingEventCloseDoors()
{
    for (uint32 i = BG_TA_OBJECT_DOOR_1; i <= BG_TA_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundTA::StartingEventOpenDoors()
{
    for (uint32 i = BG_TA_OBJECT_DOOR_1; i <= BG_TA_OBJECT_DOOR_2; ++i)
        DoorOpen(i);
}

void BattlegroundTA::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundScore;
    UpdateArenaWorldState();
}

void BattlegroundTA::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTA::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!killer)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTA::HandleAreaTrigger(Player* player, uint32 trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
	
	Battleground::HandleAreaTrigger(player, trigger);
}

void BattlegroundTA::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(0xe11) << uint32(GetAlivePlayersCountByTeam(ALLIANCE));
	data << uint32(0xe12) << uint32(GetAlivePlayersCountByTeam(HORDE));
    UpdateArenaWorldState();
}

void BattlegroundTA::Reset()
{
    //call parent's reset
    Battleground::Reset();
}

bool BattlegroundTA::SetupBattleground()
{
    // gates
    if (!AddObject(BG_TA_OBJECT_DOOR_1, BG_TA_OBJECT_TYPE_DOOR_1, 1293.561f, 1601.938f, 31.60557f, -1.457349f, 0, 0, -0.6658813f, 0.7460576f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TA_OBJECT_DOOR_2, BG_TA_OBJECT_TYPE_DOOR_2, 1278.648f, 1730.557f, 31.60557f, 1.684245f, 0, 0, 0.7460582f, 0.6658807f, RESPAWN_IMMEDIATELY))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundTA: Failed to spawn some objects!");
        return false;
    }

    return true;
}