/************************************
SymbolixDEV
Change This player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);  (MAP, X, Y, Z, O)
Adding you own Zone/Place
*/
 
#include "ScriptPCH.h"
#include "Chat.h"
 
class cs_commands : public CommandScript
{
public:
    cs_commands() : CommandScript("cs_commands") {}
 
    ChatCommand * GetCommands() const
    {
        static ChatCommand WorldChatCommandTable[] =
        {
            {"mall",        0,                              false,          &HandleMallCommand,     "", NULL},
            {"jump",        0,                              false,          &HandleJumpCommand,     "", NULL},
            {"stair",       0,                              false,          &HandleStairCommand,     "", NULL},
            {"maze",        0,                              false,          &HandleMazeCommand,     "", NULL},
            {"duel",        0,                              false,          &HandleDuelCommand,     "", NULL},
            {NULL,          0,                              false,          NULL,                                           "", NULL}
        };
 
        return WorldChatCommandTable;
    }
 
    static bool HandleMallCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player->IsInCombat())
            if (player->IsInArena())
                if (player->GetMap()->IsBattleground())
                    if (player->IsInFlight())
                    {
                        player->GetSession()->SendNotification("You cannot use this in combat!");
                        return false;
                    }
 
        if (player->GetTeam() == ALLIANCE)
            player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);                // Alliance mall coords (MAP, X, Y, Z, O)
        else
            player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);              // Horde mall coords (MAP, X, Y, Z, O)
        return true;
    }
 
    static bool HandleJumpCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player->IsInCombat())
            if (player->IsInArena())
                if (player->GetMap()->IsBattleground())
                    if (player->IsInFlight())
                    {
                        player->GetSession()->SendNotification("You cannot use this in combat!");
                        return false;
                    }
        player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);             //Jump Event
        return true;
    }
 
    static bool HandleStairCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player->IsInCombat())
            if (player->IsInArena())
                if (player->GetMap()->IsBattleground())
                    if (player->IsInFlight())
                    {
                        player->GetSession()->SendNotification("You cannot use this in combat!");
                        return false;
                    }
        player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);             // Stair event
        return true;
    }
 
    static bool HandleMazeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player->IsInCombat())
            if (player->IsInArena())
                if (player->GetMap()->IsBattleground())
                    if (player->IsInFlight())
                    {
                        player->GetSession()->SendNotification("You cannot use this in combat!");
                        return false;
                    }
        player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);             //  Maze Event
        return true;
    }
 
    static bool HandleMazeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player->IsInCombat())
            if (player->IsInArena())
                if (player->GetMap()->IsBattleground())
                    if (player->IsInFlight())
                    {
                        player->GetSession()->SendNotification("You cannot use this in combat!");
                        return false;
                    }
        player->TeleportTo(0, 0.0f, 0.0f, 0.0f, 0.0f);             // DuelZone
        return true;
    }
};
 
void AddSC_cs_commands()
{
    new cs_commands();
}
