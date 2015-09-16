//by SymbolixDEV
#include "ScriptPCH.h"
#include "Chat.h"

/* Colors */
#define MSG_COLOR_WHITE  "|cffffffff"
#define MSG_COLOR_LIGHTBLUE "|cffADD8E6"

class anticheatcommand : public CommandScript
{
public:
    anticheatcommand() : CommandScript("Anticheat") { }
 
        ChatCommand* GetCommands() const
    {
        static ChatCommand IngameCommandTable[] =
        {
        { "anticheat",           SEC_GAMEMASTER,         true,  &HandleAnticheatCommand,                "", NULL },
        { NULL,             0,                  false, NULL,                              "", NULL }
        };
                 return IngameCommandTable;
    }
        static bool HandleAnticheatCommand(ChatHandler * handler, const char * args)
    {
   if(!*args)
                        return false;
        char message[1024];
 
                if(handler->GetSession()->GetSecurity() >= SEC_GAMEMASTER)
                {
                                  snprintf(message, 1024, "[GM-Anticheat]Banned From - [%s%s|r]: %s%s|r", MSG_COLOR_WHITE, handler->GetSession()->GetPlayer()->GetName().c_str(), MSG_COLOR_LIGHTBLUE, args);
                                sWorld->SendServerMessage(SERVER_MSG_STRING, message, NULL);
    }
  return true;
  }
};

void AddSC_anticheat()
{
        new anticheatcommand;
}
