#include "ScriptMgr.h"
#include "Chat.h"
#include "Channel.h"
#include "ObjectMgr.h"
#include "Guild.h"
#include "Group.h"
#include "GuildMgr.h"
#include <cctype>

class spy : public PlayerScript
{
public:
    spy() : PlayerScript("spy_player_script")
    {
        m_accent_char = L"ÀÁÂÃÄÅ@ÇÈÉÊËÌÍÎÏÒÓÔÕÖØÙÚÛÜÝ$£àáâãäåçèéêëìíîïðòóôõöøùúûüýÿ";
        m_normal_char = L"AAAAAAACEEEEIIIIOOOOOOUUUUYSEAAAAAACEEEEIIIIOOOOOOOUUUUYY";
        m_separator = L"&#\'[|-_\\)]=}+\"!/;.,?<>*({`%§";
    }

    void OnChat(Player *player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel)
    {
        if(CheckMessage(player, msg, 0, 0, channel, 0, 0))
            return;

        if(channel && player)
        {
            WorldPacket notify(SMSG_SERVER_MESSAGE); //On prépare un paquet
            notify << uint32(SERVER_MSG_STRING);

            SessionMap sessionmap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator itr = sessionmap.begin(); itr != sessionmap.end(); ++itr)
            {
                if(itr->second && itr->second->GetPlayer())
                {
                    Player *plr = itr->second->GetPlayer();
                    if(!plr && !plr->isGameMaster())
                        continue;

                    if(plr->GetSpyMgr().IsChannelSpied(std::string(channel->GetName())))
                    {
                        std::stringstream ss;
                        if(player->GetTeamId() == TEAM_ALLIANCE)
                            ss << "|cffff8f4b[" << channel->GetName() << " - A2] ";
                        else
                            ss << "|cffff8f4b[" << channel->GetName() << " - H2] ";

                        ss << "|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h : " << msg;

                        notify << ss.str().c_str();
                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());

                        continue;
                    }

                    if(plr->GetSpyMgr().IsPlayerSpied(player->GetGUIDLow()))
                    {
                        std::stringstream ss;
                        if(player->GetTeamId() == TEAM_ALLIANCE)
                            ss << "|cffff8f4b[" << channel->GetName() << " - A2] ";
                        else
                            ss << "|cffff8f4b[" << channel->GetName() << " - H2] ";

                        ss << "|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h : " << msg;

                        notify << ss.str().c_str();
                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());

                        continue;
                    }
                }
            }
        }
    }

    void OnChat(Player* player, uint32 type, uint32 /*lang*/, std::string& msg)
    {
        if(CheckMessage(player, msg, type, 0, 0, 0, 0))
            return;

        if(type == CHAT_MSG_ADDON)
            return;

        if(player)
        {
            SessionMap sessionmap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator itr = sessionmap.begin(); itr != sessionmap.end(); ++itr)
            {
                if(itr->second && itr->second->GetPlayer())
                {
                    Player *plr = itr->second->GetPlayer();
                    if(!plr && !plr->isGameMaster())
                        continue;

                    if(plr->GetSpyMgr().IsPlayerSpied(player->GetGUIDLow()))
                    {
                        std::stringstream ss;
                        if(type == CHAT_MSG_YELL)
                            ss << "|cffff0000|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h crie : " << msg;
                        else if(type == CHAT_MSG_EMOTE)
                            ss << "|cffff9000|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
                        else
                            ss << "|cffffffff|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h dit : " << msg;

                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());
                    }
                }
            }
        }
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Group* group)
    {
        if(CheckMessage(player, msg, 0, 0, 0, 0, group))
            return;

        if(player)
        {
            SessionMap sessionmap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator itr = sessionmap.begin(); itr != sessionmap.end(); ++itr)
            {
                if(itr->second && itr->second->GetPlayer())
                {
                    Player *plr = itr->second->GetPlayer();
                    if(!plr && !plr->isGameMaster())
                        continue;

                    std::string name;
                    if(plr->GetSpyMgr().IsGroupSpied(group->GetLowGUID(), name))
                    {
                        std::stringstream ss;
                        ss << "|cff4bb5ff[Groupe de " << name << "] |Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());
                        return;
                    }

                    if(plr->GetSpyMgr().IsPlayerSpied(player->GetGUIDLow()))
                    {
                        std::stringstream ss;
                        ss << "|cff4bb5ff[Groupe] |Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());
                        return;
                    }
                }
            }
        }
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver)
    {
        if(CheckMessage(player, msg, 0, receiver, 0, 0, 0))
            return;

        if(player && receiver)
        {
            SessionMap sessionmap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator itr = sessionmap.begin(); itr != sessionmap.end(); ++itr)
            {
                if(itr->second && itr->second->GetPlayer())
                {
                    Player *plr = itr->second->GetPlayer();
                    if(!plr && !plr->isGameMaster())
                        continue;

                    std::stringstream ss;

                    if(plr->GetSpyMgr().IsPlayerSpied(player->GetGUIDLow()) || plr->GetSpyMgr().IsPlayerSpied(receiver->GetGUIDLow()))
                    {
                        ss << "|cfffd52e7|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h chuchote à |Hplayer:" << receiver->GetName() << "|h[" << receiver->GetName()<< "]|h : " << msg;
                    }

                    if(!ss.str().empty())
                    {
                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());
                    }
                }
            }
        }
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Guild* guild)
    {
        if(CheckMessage(player, msg, 0, 0, 0, guild, 0))
            return;

        if(player && guild)
        {
            SessionMap sessionmap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator itr = sessionmap.begin(); itr != sessionmap.end(); ++itr)
            {
                if(itr->second && itr->second->GetPlayer())
                {
                    Player *plr = itr->second->GetPlayer();
                    if(!plr && !plr->isGameMaster())
                        continue;

                    std::stringstream ss;

                    if(plr->GetSpyMgr().IsGuildSpied(guild->GetId()) || plr->GetSpyMgr().IsPlayerSpied(player->GetGUIDLow()))
                    {
                        ss << "|cff00c821[Guild] |Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
                        ChatHandler(plr->GetSession()).PSendSysMessage(ss.str().c_str());
                    }
                }
            }
        }
    }

    bool CheckMessage(Player *player, std::string msg, uint32 type, Player *receiver, Channel *channel, Guild *guild, Group *group)
    {
        std::string utfmsg = msg;
        std::wstring message;
        if(!Utf8toWStr(utfmsg, message))
            return false;
        for(int i = 0 ; i < message.size() ; i++) //On remplace les caractère accentué par des non accentué
        {
            for(int j = 0 ; j < m_accent_char.size() ; j++)
            {
                //sLog->outString("%c <=> %c", message[i], m_accent_char[j]);
                if(message[i] == m_accent_char[j])
                    message[i] = m_normal_char[j];
            }

            for(int j = 0 ; j < m_separator.size() ; j++)
            {
                if(message[i] == m_separator[j])
                    message[i] = ' ';
            }
        }

        wstrToLower(message);
        WStrToUtf8(message, utfmsg);

        //On splitte chaque mot
        std::vector<std::string> m_worldList;
        char *p;
        for (p = strtok( (char*)utfmsg.c_str(), " " ); p; p = strtok( NULL, " " ))
        {
            m_worldList.push_back(std::string(p));
        }

        bool found = false;

        for (int i = 0 ; i < m_worldList.size() ; i++)
        {
            if(sSpyMgr->IsWordBlackListed(m_worldList[i]))
            {
                found = true;
                break;
            }
        }

        if(found)
        {
            std::stringstream ss;

            if(player && receiver) //MP
            {
                ss << "[Mot blacklisté]|cfffd52e7|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h chuchote à |Hplayer:" << receiver->GetName() << "|h[" << receiver->GetName()<< "]|h : " << msg;
            }
            else if(player && channel)
            {
                if(player->GetTeamId() == TEAM_ALLIANCE)
                    ss << "[Mot blacklisté]|cffff8f4b[" << channel->GetName() << " - A2] ";
                else
                    ss << "[Mot blacklisté]|cffff8f4b[" << channel->GetName() << " - H2] ";

                ss << "|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h : " << msg;
            }
            else if(player && guild)
            {
                ss << "[Mot blacklisté]|cff00c821[Guild] |Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
            }
            else if(player && group)
            {
                ss << "[Mot blacklisté]|cff4bb5ff[Groupe] |Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
            }
            else if(player)
            {
                if(type == CHAT_MSG_YELL)
                    ss << "[Mot blacklisté]|cffff0000|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h crie : " << msg;
                else if(type == CHAT_MSG_EMOTE)
                    ss << "[Mot blacklisté]|cffff9000|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h " << msg;
                else
                    ss << "[Mot blacklisté]|cffffffff|Hplayer:" << player->GetName() << "|h[" << player->GetName()<< "]|h dit : " << msg;
            }

            WorldPacket data(SMSG_SERVER_MESSAGE);
            data << uint32(SERVER_MSG_STRING);
            data << ss.str().c_str();
            sWorld->SendGlobalGMMessage(&data);
        }

        return found;
    }

private:
    std::wstring m_accent_char;
    std::wstring m_normal_char;
    std::wstring m_separator;
};

class spy_cs : public CommandScript
{
public:
    spy_cs() : CommandScript("cs_spy") {}

    ChatCommand *GetCommands() const
    {
        static ChatCommand playerCommandTable [] =
        {
            {"add", SEC_MODERATOR, false, &HandleAddPlayer, "Ajouter un joueur à espionner", NULL},
            {"del", SEC_MODERATOR, false, &HandleDelPlayer, "Arrete l'espionnage d'un joueur", NULL},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand groupCommandTable [] =
        {
            {"add", SEC_MODERATOR, false, &HandleAddGroup, "Ajouter un groupe à espionner", NULL},
            {"del", SEC_MODERATOR, false, &HandleDelGroup, "Arrete l'espionnage d'un groupe", NULL},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand wordCommandTable [] =
        {
            {"add", SEC_MODERATOR, false, &HandleAddWord, "Ajouter un mot à la blacklist", NULL},
            {"del", SEC_MODERATOR, false, &HandleDelWord, "Supprime un mot de la blacklist", NULL},
            {"list", SEC_MODERATOR, false, &HandleListWord, "Liste les mot de la blacklist commençant par...", NULL},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand guildCommandTable [] =
        {
            {"add", SEC_MODERATOR, false, &HandleAddGuild, "Ajouter une guilde à espionner", NULL},
            {"del", SEC_MODERATOR, false, &HandleDelGuild, "Arrete l'espionnage d'une guilde", NULL},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand channelCommandTable [] =
        {
            {"add", SEC_MODERATOR, false, &HandleAddChannel, "Ajouter un canal de discussion à espionner", NULL},
            {"del", SEC_MODERATOR, false, &HandleDelChannel, "Arrete l'espionnage d'un canal de discussion", NULL},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand spyCommandTable [] =
        {
            {"player", SEC_MODERATOR, false, NULL, "", playerCommandTable},
            {"channel", SEC_MODERATOR, false, NULL, "", channelCommandTable},
            {"guild", SEC_MODERATOR, false, NULL, "", guildCommandTable},
            {"group", SEC_MODERATOR, false, NULL, "", groupCommandTable},
            {"word", SEC_MODERATOR, false, NULL, "", wordCommandTable},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand cmd [] =
        {
            {"prism", SEC_MODERATOR, false, NULL, "", spyCommandTable},
            {NULL, 0, false, NULL, "", NULL}
        };

        return cmd;
    }

    static bool HandleAddGroup(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        std::string plr(args);

        normalizePlayerName(plr);

        Player *player = sObjectAccessor->FindPlayerByName(plr);
        if(player)
        {
            Player *mj = handler->GetSession()->GetPlayer();
            if(mj)
            {
                Group *group = player->GetGroup();
                if(group)
                {
                    mj->GetSpyMgr().SpyGroup(group->GetLowGUID(), player->GetName());
                    handler->PSendSysMessage("Le groupe du joueur %s est maintenant espionné", player->GetName().c_str());
                }
                else
                    handler->PSendSysMessage("Le joueur %s n'est pas dans un groupe", player->GetName().c_str());
            }
        }
        else
            handler->PSendSysMessage("Le joueur %s est introuvable", plr.c_str());

        return true;
    }

    static bool HandleDelGroup(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        std::string plr(args);

        normalizePlayerName(plr);

        Player *player = sObjectAccessor->FindPlayerByName(plr);
        if(player)
        {
            Player *mj = handler->GetSession()->GetPlayer();
            if(mj)
            {
                Group *group = player->GetGroup();
                if(group)
                {
                    mj->GetSpyMgr().UnspyGroup(group->GetLowGUID(), player->GetName());
                    handler->PSendSysMessage("Le groupe du joueur %s n'est plus espionné", player->GetName().c_str());
                }
                else
                    handler->PSendSysMessage("Le joueur %s n'est pas dans un groupe", player->GetName().c_str());
            }
        }
        else
            handler->PSendSysMessage("Le joueur %s est introuvable", plr.c_str());

        return true;
    }

    static bool HandleListWord(ChatHandler *handler, const char *args)
    {
        std::vector<std::string> wordList = sSpyMgr->GetBlacklistedWordList();
        std::vector<std::string> resultList;

        if(!args)
            resultList = wordList;
        else
        {
            for(int i = 0 ; i < wordList.size() ; i++)
            {
                std::string begin = wordList[i].substr(0, strlen(args));
                if(begin == args)
                    resultList.push_back(wordList[i]);
            }
        }

        handler->PSendSysMessage("Liste de résultat :");

        for(int i = 0 ; i < resultList.size() ; i++)
            handler->PSendSysMessage("%s", resultList[i].c_str());

        return true;
    }

    static bool HandleAddWord(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        std::string acceptedChar = "abcdefghijklmnopqrstuvwxyz";

        std::string word(args);

        for(int i = 0 ; i < word.size() ; i++)
        {
            bool found = false;
            for(int j = 0 ; j < acceptedChar.size() ; j++)
            {
                if(word[i] == acceptedChar[j])
                    found = true;
            }

            if(!found)
            {
                handler->PSendSysMessage("Le mot entré est invalide.");
                return true;
            }
        }

        sSpyMgr->AddBlackListedWord(word);

        std::stringstream ss;
        ss << "Le mot \"" << word << "\" a été ajouté à la blacklist";

        handler->SendGlobalGMSysMessage(ss.str().c_str());

        return true;
    }

    static bool HandleDelWord(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        std::string acceptedChar = "abcdefghijklmnopqrstuvwxyz";

        std::string word(args);

        for(int i = 0 ; i < word.size() ; i++)
        {
            bool found = false;
            for(int j = 0 ; j < acceptedChar.size() ; j++)
            {
                if(word[i] == acceptedChar[j])
                    found = true;
            }

            if(!found)
            {
                handler->PSendSysMessage("Le mot entré est invalide.");
                return true;
            }
        }

        sSpyMgr->DelBlackListedWord(word);

        std::stringstream ss;
        ss << "Le mot \"" << word << "\" a été retiré de la blacklist";

        handler->SendGlobalGMSysMessage(ss.str().c_str());

        return true;
    }

    static bool HandleAddGuild(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        Guild *guild = sGuildMgr->GetGuildByName(std::string(args));
        if(!guild)
        {
            handler->PSendSysMessage("Guilde introuvable");
            return true;
        }

        Player *plr = handler->GetSession()->GetPlayer();
        if(plr)
        {
            plr->GetSpyMgr().SpyGuild(guild->GetId());
            handler->PSendSysMessage("La guilde %s est espionné", guild->GetName().c_str());
        }
        return true;
    }

    static bool HandleDelGuild(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        Guild *guild = sGuildMgr->GetGuildByName(std::string(args));
        if(!guild)
        {
            handler->PSendSysMessage("Guilde introuvable");
            return true;
        }

        Player *plr = handler->GetSession()->GetPlayer();
        if(plr)
        {
            plr->GetSpyMgr().UnspyGuild(guild->GetId());
            handler->PSendSysMessage("La guilde %s n'est plus espionné", guild->GetName().c_str());
        }
        return true;
    }

    static bool HandleAddChannel(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        Player *plr = handler->GetSession()->GetPlayer();
        if(plr)
        {
            plr->GetSpyMgr().SpyChannel(args);
            handler->PSendSysMessage("Le canal de discussion %s est espionné", args);
        }
        return true;
    }

    static bool HandleDelChannel(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        Player *plr = handler->GetSession()->GetPlayer();
        if(plr)
        {
            plr->GetSpyMgr().UnspyChannel(args);
            handler->PSendSysMessage("Le canal de discussion %s est espionné", args);
        }
        return true;
    }

    static bool HandleDelPlayer(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        std::string plr(args);

        normalizePlayerName(plr);

        Player *player = sObjectAccessor->FindPlayerByName(plr);
        if(player)
        {
            Player *mj = handler->GetSession()->GetPlayer();
            if(mj)
            {
                mj->GetSpyMgr().UnspyPlayer(player->GetGUIDLow());

                handler->PSendSysMessage("Le joueur %s n'est plus espionné", player->GetName().c_str());
            }
        }
        else
            handler->PSendSysMessage("Le joueur %s est introuvable", plr.c_str());

        return true;
    }

    static bool HandleAddPlayer(ChatHandler *handler, const char *args)
    {
        if(!args)
            return false;

        std::string plr(args);

        normalizePlayerName(plr);

        Player *player = sObjectAccessor->FindPlayerByName(plr);
        if(player)
        {
            Player *mj = handler->GetSession()->GetPlayer();
            if(mj)
            {
                mj->GetSpyMgr().SpyPlayer(player->GetGUIDLow());

                handler->PSendSysMessage("Le joueur %s est maintenant espionné", player->GetName().c_str());
            }
        }
        else
            handler->PSendSysMessage("Le joueur %s est introuvable", plr.c_str());

        return true;
    }
};

void AddSc_spy()
{
    new spy_cs;
    new spy;
}
