#include "gamePCH.h"
#include "SpyMgr.h"
#include <DatabaseEnv.h>

SpyMgr::SpyMgr()
{

}

void SpyMgr::SpyPlayer(uint32 guid)
{
    for(int i = 0 ; i < m_spiedPlayers.size() ; i++)
        if(m_spiedPlayers[i] == guid)
            return;

    m_spiedPlayers.push_back(guid);
}

void SpyMgr::SpyGuild(uint32 guild)
{
    for(int i = 0 ; i < m_spiedGuilds.size() ; i++)
        if(m_spiedGuilds[i] == guild)
            return;

    m_spiedGuilds.push_back(guild);
}

void SpyMgr::SpyChannel(std::string name)
{
    for(int i = 0 ; i < m_spiedChannels.size() ; i++)
        if(m_spiedChannels[i] == name)
            return;

    m_spiedChannels.push_back(name);
}

void SpyMgr::UnspyPlayer(uint32 guid)
{
    for(std::vector<uint32>::iterator i = m_spiedPlayers.begin() ; i != m_spiedPlayers.end() ; ++i)
    {
        if(*i == guid)
        {
            m_spiedPlayers.erase(i);
            return;
        }
    }
}

void SpyMgr::UnspyGuild(uint32 guild)
{
    for(std::vector<uint32>::iterator i = m_spiedGuilds.begin() ; i != m_spiedGuilds.end() ; ++i)
    {
        if(*i == guild)
        {
            m_spiedGuilds.erase(i);
            return;
        }
    }
}

void SpyMgr::UnspyChannel(std::string name)
{
    for(std::vector<std::string>::iterator i = m_spiedChannels.begin() ; i != m_spiedChannels.end() ; ++i)
    {
        if(*i == name)
        {
            m_spiedChannels.erase(i);
            return;
        }
    }
}

bool SpyMgr::IsChannelSpied(std::string channel)
{
    for(std::vector<std::string>::const_iterator i = m_spiedChannels.begin() ; i != m_spiedChannels.end() ; ++i)
    {
        if(*i == channel)
        {
            return true;
        }
    }

    /*if(this != sSpyMgr)
        return sSpyMgr->IsChannelSpied(channel);*/

    return false;
}

bool SpyMgr::IsGuildSpied(uint32 guild)
{
    for(std::vector<uint32>::const_iterator i = m_spiedGuilds.begin() ; i != m_spiedGuilds.end() ; ++i)
    {
        if(*i == guild)
        {
            return true;
        }
    }

    /*if(this != sSpyMgr)
        return sSpyMgr->IsGuildSpied(guild);*/

    return false;
}

bool SpyMgr::IsPlayerSpied(uint32 guid)
{
    for(std::vector<uint32>::const_iterator i = m_spiedPlayers.begin() ; i != m_spiedPlayers.end() ; ++i)
    {
        if(*i == guid)
        {
            return true;
        }
    }

    /*if(this != sSpyMgr)
        return sSpyMgr->IsPlayerSpied(guid);*/

    return false;
}

void SpyMgr::SpyGroup(uint32 guid, std::string playerName)
{
    if(m_spiedGroup.find(guid) == m_spiedGroup.end())
        m_spiedGroup[guid] = playerName;
}

void SpyMgr::UnspyGroup(uint32 guid, std::string playerName)
{
    if(m_spiedGroup.find(guid) != m_spiedGroup.end())
        m_spiedGroup.erase(guid);
}

bool SpyMgr::IsGroupSpied(uint32 guid, std::string &name)
{
    if(m_spiedGroup.find(guid) != m_spiedGroup.end())
    {
        name = m_spiedGroup[guid];
        return true;
    }

    return false;
}

SpyMgrGlobal::SpyMgrGlobal()
{

}

bool SpyMgrGlobal::IsWordBlackListed(std::string word)
{
    for(std::vector<std::string>::const_iterator i = m_blackListedWords.begin() ; i != m_blackListedWords.end() ; ++i)
    {
        if(*i == word)
            return true;
    }

    return false;
}

void SpyMgrGlobal::AddBlackListedWord(std::string word)
{
    for(std::vector<std::string>::const_iterator i = m_blackListedWords.begin() ; i != m_blackListedWords.end() ; ++i)
    {
        if(*i == word)
            return;
    }

    m_blackListedWords.push_back(word);

    LoginDatabase.PQuery("INSERT INTO word_blacklist(word) VALUE ('%s')", word.c_str());
}

void SpyMgrGlobal::DelBlackListedWord(std::string word)
{
    for(std::vector<std::string>::iterator i = m_blackListedWords.begin() ; i != m_blackListedWords.end() ; ++i)
    {
        if(*i == word)
        {
            m_blackListedWords.erase(i);
            LoginDatabase.PQuery("DELETE FROM word_blacklist WHERE word = '%s'", word.c_str());
            return;
        }
    }
}

std::vector<std::string> SpyMgrGlobal::GetBlacklistedWordList()
{
    return m_blackListedWords;
}

void SpyMgrGlobal::LoadBlackListedWords()
{
    QueryResult result = LoginDatabase.Query("SELECT word FROM word_blacklist");

    if(result)
    {
        int c = 0;
        do
        {
            Field *field = result->Fetch();
            m_blackListedWords.push_back(field[0].GetString());
            c++;
        }
        while(result->NextRow());
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "%u mot blacklistés chargés", c);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "0 mot blacklisté chargé");
}
