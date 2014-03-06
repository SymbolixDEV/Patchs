#ifndef SPYMGR_H
#define SPYMGR_H

#include <vector>
#include "SharedDefines.h"
#include <string>
#include "ace/Singleton.h"
#include <map>

/*Auteur : Eryldor
 *
 *Classe pour gérer la surveillance des joueur
 */

class SpyMgr
{
public:
    SpyMgr();

    void SpyPlayer(uint32 guid);
    void SpyGuild(uint32 guild);
    void UnspyPlayer(uint32 guild);
    void UnspyGuild(uint32 guild);
    void SpyChannel(std::string name); //Independant de la faction
    void UnspyChannel(std::string name);
    void SpyGroup(uint32 guid, std::string playerName);
    void UnspyGroup(uint32 guid, std::string playerName);

    bool IsPlayerSpied(uint32 guid);
    bool IsGuildSpied(uint32 guild);
    bool IsChannelSpied(std::string channel);
    bool IsGroupSpied(uint32 guid, std::string &name);

private:

    std::vector<uint32> m_spiedPlayers;
    std::vector<uint32> m_spiedGuilds;
    std::vector<std::string> m_spiedChannels;
    std::map <uint32, std::string> m_spiedGroup;
};

class SpyMgrGlobal : public SpyMgr
{
    friend class ACE_Singleton<SpyMgrGlobal, ACE_Null_Mutex>;
public:
    SpyMgrGlobal();

    void LoadBlackListedWords();
    void SaveBlackListedWords();

    void AddBlackListedWord(std::string word);
    void DelBlackListedWord(std::string word);

    std::vector<std::string> GetBlacklistedWordList();
    bool IsWordBlackListed(std::string word);

private:
    std::vector<std::string> m_blackListedWords;
};

#define sSpyMgr ACE_Singleton<SpyMgrGlobal, ACE_Null_Mutex>::instance()

#endif
