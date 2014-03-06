/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BLACK_MARKET_MGR_H
#define _BLACK_MARKET_MGR_H

#include <ace/Singleton.h>

#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCStructure.h"

class Item;
class Player;
class WorldPacket;

enum BMMailAuctionAnswers
{
    BM_AUCTION_OUTBIDDED           = 0,
    BM_AUCTION_WON                 = 1,
};

#define BLACKMARKET_AUCTION_HOUSE 7

struct BMAuctionTemplate
{
    uint32 id;
    uint32 itemEntry;
    uint32 itemCount;
    uint32 seller;
	uint32 duration;
	uint32 startBid;
	uint32 chance;
};
typedef struct BMAuctionTemplate BMAuctionTemplate;

struct BMAuctionEntry
{
    uint32 id;
	uint32 templateId;
    uint32 startTime;
	uint32 bid;
    uint32 bidder;
	BMAuctionTemplate* bm_template;

    // helpers
    void DeleteFromDB(SQLTransaction& trans);
    void SaveToDB(SQLTransaction& trans);
    bool LoadFromDB(Field* fields);
	void UpdateToDB(SQLTransaction& trans);

	uint32 EndTime() { return startTime + bm_template->duration; }
	uint32 TimeLeft();
	bool IsActive() { return (time(NULL) >= startTime); }
	bool IsExpired() {return EndTime() < time(NULL); }

	std::string BuildAuctionMailSubject(BMMailAuctionAnswers response);
	std::string BuildAuctionMailBody(uint32 lowGuid);
};
typedef struct BMAuctionEntry BMAuctionEntry;


class BlackMarketMgr
{
    friend class ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>;

    private:
        BlackMarketMgr();
        ~BlackMarketMgr();

		typedef std::map<uint32, BMAuctionTemplate*> BMAuctionTemplateMap;
		typedef std::map<uint32, BMAuctionEntry*> BMAuctionEntryMap;

		BMAuctionTemplateMap BMTemplatesMap;
		BMAuctionEntryMap BMAuctionsMap;

	public:

		BMAuctionTemplate* GetTemplate(uint32 id) const
		{
			BMAuctionTemplateMap::const_iterator itr = BMTemplatesMap.find(id);
			return itr != BMTemplatesMap.end() ? itr->second : NULL;
		}

		uint32 GetTemplatesCount() { return BMTemplatesMap.size(); }

		BMAuctionTemplateMap::iterator GetTemplatesBegin() { return BMTemplatesMap.begin(); }
		BMAuctionTemplateMap::iterator GetTemplatesEnd() { return BMTemplatesMap.end(); }


		BMAuctionEntry* GetAuction(uint32 id) const
		{
			BMAuctionEntryMap::const_iterator itr = BMAuctionsMap.find(id);
			return itr != BMAuctionsMap.end() ? itr->second : NULL;
		}

		uint32 GetAuctionCount() { return BMAuctionsMap.size(); }

		BMAuctionEntryMap::iterator GetAuctionsBegin() { return BMAuctionsMap.begin(); }
		BMAuctionEntryMap::iterator GetAuctionsEnd() { return BMAuctionsMap.end(); }


        //auction messages
        void SendAuctionWon(BMAuctionEntry* auction, SQLTransaction& trans);
        void SendAuctionOutbidded(BMAuctionEntry* auction, uint32 newPrice, Player* newBidder, SQLTransaction& trans);

		void LoadTemplates();
        void LoadAuctions();

		uint32 GetNewAuctionId();
		void CreateAuctions(uint32 number, SQLTransaction& trans);
		void UpdateAuction(BMAuctionEntry* auction, uint64 newPrice, Player* newBidder);

        void Update();

		void BuildBlackMarketAuctionsPacket(WorldPacket& data, uint32 guidLow);

    private:
		
};

#define sBlackMarketMgr ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>::instance()

#endif
