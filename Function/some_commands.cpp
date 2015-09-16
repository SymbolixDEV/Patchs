/*
Script by: SymbolixDEV
*/

#include "ScriptPCH.h"
#include "Chat.h"

static const uint32 buffIdstools[] = { 43223, 5862, 33377, 33779, 31305, 70692, 42995, 48162, 48074, 48170, 36880, 467, 48469 };

class SymbolixDEV_commands : public CommandScript
{
        public:
        SymbolixDEV_commands() : CommandScript("SymbolixDEV_commands"){	}

        ChatCommand * GetCommands() const
        {
                static ChatCommand WorldChatCommandTable[] = 
                {
                        {"mall",        0,                              false,          &HandleMallCommand,     "", NULL},
						{"mail",        0,                              false,          &HandleMailCommand,     "", NULL},
						{"bank",        0,                              false,          &HandleBankCommand,     "", NULL},
						{"maxskill",        0,                              false,          &HandleMaxSkillCommand,     "", NULL},
						{"buff",        0,                              false,          &HandleBuffCommand,     "", NULL},
						{"heal",        0,                              false,          &HandleHealCommand,     "", NULL},
						{"chat",        0,                              false,          &HandleChatCommand,     "", NULL},
						{"changerace",             SEC_PLAYER,  false, &HandleChangeRaceCommand,           "", NULL },
						{"changefaction",			SEC_PLAYER,  false, &HandleChangeFactionCommand,		"", NULL },
						{"maxskills",			    SEC_PLAYER,  false, &HandleMaxSkillsCommand,	    	"", NULL },
						{"customize",			    SEC_PLAYER,  false, &HandleCustomizeCommand,	       	"", NULL },
                        {NULL,          0,                              false,          NULL,                                           "", NULL}
                };

                return WorldChatCommandTable;
        }
		 
		  static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Relog to change race of your character.");
        return true;
    }

    static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Relog to change faction of your character.");
        return true;
    }

    static bool HandleMaxSkillsCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->UpdateSkillsForLevel();
		handler->PSendSysMessage("Your weapon skills are now maximized.");
        return true;
    }

	static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Relog to customize your character.");
        return true;
    }
	
	static bool HandleBuffCommand(ChatHandler * handler, const char * args)
		{
		        Player * pl = handler->GetSession()->GetPlayer();
                if(pl->InArena())
                {
                        pl->GetSession()->SendNotification("You can't use that item in an arena match!");
                        return false;
                }
       
                pl->RemoveAurasByType(SPELL_AURA_MOUNTED);
                for(int i = 0; i < 7; i++)
                    pl->AddAura(auras[i], pl);
                handler->PSendSysMessage("|cffB400B4You have been buffed, enjoy!");
                return true;
 
		}
		 
		 	static bool HandleHealCommand(ChatHandler * handler, const char * args)
			{
			Player * me = handler->GetSession()->GetPlayer();
	
			me-> SetHealth(me->GetMaxHealth());
			handler->PSendSysMessage("You Have Been Healed!");
			return true;
			}
		 
		 static bool HandleMaxSkillCommand(ChatHandler* handler, char const* /*args*/)
		{
        Player* SelectedPlayer = handler->getSelectedPlayer();
        if (!SelectedPlayer)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // each skills that have max skill value dependent from level seted to current level max skill value
        SelectedPlayer->UpdateSkillsToMaxSkillsForLevel();
        return true;
		}
	
		static bool HandleBankCommand(ChatHandler* handler, char const* /*args*/)
		{
			handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());
			return true;
		}
	
		static bool HandleMailCommand(ChatHandler* handler, const char* /*args*/)
		{
        // format: name "subject text" "mail text"
        Player* target;
        uint64 targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        char* tail1 = strtok(NULL, "");
        if (!tail1)
            return false;

        char const* msgSubject = handler->extractQuotedArg(tail1);
        if (!msgSubject)
            return false;

        char* tail2 = strtok(NULL, "");
        if (!tail2)
            return false;

        char const* msgText = handler->extractQuotedArg(tail2);
        if (!msgText)
            return false;

        // msgSubject, msgText isn't NUL after prev. check
        std::string subject = msgSubject;
        std::string text    = msgText;

        // from console show not existed sender
        MailSender sender(MAIL_NORMAL, handler->GetSession() ? handler->GetSession()->GetPlayer()->GetGUIDLow() : 0, MAIL_STATIONERY_GM);

        //- TODO: Fix poor design
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        MailDraft(subject, text)
            .SendMailTo(trans, MailReceiver(target, GUID_LOPART(targetGuid)), sender);

        CharacterDatabase.CommitTransaction(trans);

        std::string nameLink = handler->playerLink(targetName);
        handler->PSendSysMessage(LANG_MAIL_SENT, nameLink.c_str());
        return true;
		}
		static bool HandleChatCommand(ChatHandler * handler, const char * args)
		{
			if(!*args)
        return false;
				char message[1024];
 
                if(handler->GetSession()->GetSecurity() >= SEC_PLAYER)
                {
        snprintf(message, 1024, "[Chat][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, handler->GetSession()->GetPlayer()->GetName().c_str(), MSG_COLOR_LIGHTBLUE, args);
        sWorld->SendServerMessage(SERVER_MSG_STRING, message, NULL);
		}
  return true;
        static bool HandleMallCommand(ChatHandler* handler, const char* /*args*/)
        {
                Player* player = handler->GetSession()->GetPlayer();
                if (player->isInCombat())
                {
                        player->GetSession()->SendNotification("You cannot use this in combat!");
                        return false;
                }

                if (player->GetTeam() == ALLIANCE)
                        player->TeleportTo(0, -4108.489746f, -1397.591675f, 180.133575f, 6.030600f);             // Alliance mall coords (MAP, X, Y, Z, O)
                else
                        player->TeleportTo(0, -4108.489746f, -1397.591675f, 180.133575f, 6.030600f);              // Horde mall coords (MAP, X, Y, Z, O)
                return true;
        }
};

void AddSC_SymbolixDEV_commands()
{
        new SymbolixDEV_commands();
}
