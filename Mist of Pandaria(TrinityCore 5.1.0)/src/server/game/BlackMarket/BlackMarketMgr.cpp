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

#include "Common.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "BlackMarketMgr.h"
#include "Item.h"
#include "Language.h"
#include "Log.h"
#include <vector>

// BMAuctionEntry

bool BMAuctionEntry::LoadFromDB(Field* fields)
{
    id = fields[0].GetUInt32();
	templateId = fields[1].GetUInt32();
	startTime = fields[2].GetUInt32();
	bid = fields[3].GetUInt64();
    bidder = fields[4].GetUInt32();

    bm_template = sBlackMarketMgr->GetTemplate(templateId);
	if (!bm_template)
		return false;
	return true;
}

void BMAuctionEntry::SaveToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BLACKMARKET_AUCTION);
    stmt->setUInt32(0, id);
    stmt->setUInt32(1, templateId);
    stmt->setUInt32(2, startTime);
    stmt->setUInt64(3, bid);
    stmt->setUInt32(4, bidder);
    trans->Append(stmt);
}

void BMAuctionEntry::DeleteFromDB(SQLTransaction& trans)
{
	PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BLACKMARKET_AUCTION);
	stmt->setUInt32(0, id);
	trans->Append(stmt);
}

void BMAuctionEntry::UpdateToDB(SQLTransaction& trans)
{
	PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_BLACKMARKET_AUCTION);
	stmt->setUInt64(0, bid);
	stmt->setUInt32(1, bidder);
	stmt->setUInt32(2, id);
	trans->Append(stmt);
}

uint32 BMAuctionEntry::TimeLeft()
{ 
	uint32 endTime = startTime + bm_template->duration;
	uint32 curTime = time(NULL);
	return (endTime >= curTime) ? endTime - curTime : 0;
}

// BlackMarketMgr

BlackMarketMgr::BlackMarketMgr()
{
}

BlackMarketMgr::~BlackMarketMgr()
{
	for(BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
        delete itr->second;
    for(BMAuctionTemplateMap::const_iterator itr = GetTemplatesBegin(); itr != GetTemplatesEnd(); ++itr)
        delete itr->second;
}


void BlackMarketMgr::LoadTemplates()
{
	uint32 oldMSTime = getMSTime();

	PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_BLACKMARKET_TEMPLATE);
	PreparedQueryResult result = WorldDatabase.Query(stmt);

	if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 BlackMarket templates. DB table `blackmarket_template` is empty.");

        return;
    }

	uint32 count = 0;

	do
    {
        Field* fields = result->Fetch();

        BMAuctionTemplate* bm_template = new BMAuctionTemplate();
        bm_template->id			= fields[0].GetUInt32();
		bm_template->itemEntry	= fields[1].GetUInt32();
		bm_template->itemCount	= fields[2].GetUInt32();
		bm_template->seller		= fields[3].GetUInt32();
		bm_template->startBid	= fields[4].GetUInt64();
		bm_template->duration	= fields[5].GetUInt32();
		bm_template->chance		= fields[6].GetUInt32();

		BMTemplatesMap[bm_template->id] = bm_template;

        ++count;
    } while (result->NextRow());

	sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u BlackMarket templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
	
}

void BlackMarketMgr::LoadAuctions()
{
	uint32 oldMSTime = getMSTime();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_BLACKMARKET_AUCTIONS);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 BlackMarket Auctions. DB table `blackmarket` is empty.");

        return;
    }

    uint32 count = 0;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    do
    {
        Field* fields = result->Fetch();

        BMAuctionEntry* auction = new BMAuctionEntry();
        if (!auction->LoadFromDB(fields))
        {
			auction->DeleteFromDB(trans);
            delete auction;
            continue;
        }

		BMAuctionsMap[auction->id] = auction;

        ++count;

    } while (result->NextRow());

    CharacterDatabase.CommitTransaction(trans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u BlackMarket Auctions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void BlackMarketMgr::Update()
{
	uint32 curTime = time(NULL);

	SQLTransaction trans = CharacterDatabase.BeginTransaction();

	// Delete expired auctions
	for(BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd();)
	{
		BMAuctionEntry* auction = itr->second;
		if (auction->IsExpired())
		{
			if (auction->bidder)
				SendAuctionWon(auction, trans);
			auction->DeleteFromDB(trans);
			BMAuctionsMap.erase((itr++)->first);
		}
		else
			++itr;

	}

	// Add New Auctions;
	int32 add = sWorld->getIntConfig(CONFIG_BLACKMARKET_MAX_AUCTIONS) - BMAuctionsMap.size();
	if (add > 0)
		CreateAuctions(add, trans);

	CharacterDatabase.CommitTransaction(trans);
}

uint32 BlackMarketMgr::GetNewAuctionId()
{
	uint32 newId = 1;
	while(GetAuction(newId)) { ++newId; }
	return newId;
}

void BlackMarketMgr::CreateAuctions(uint32 number, SQLTransaction& trans)
{
	if (BMTemplatesMap.empty())
		return;

	for(uint32 i=0; i < number; ++i)
	{

		// Select a template
		std::vector<uint32> templateList;
		uint32 rand = urand(1, 100);

		for(BMAuctionTemplateMap::const_iterator itr = GetTemplatesBegin(); itr != GetTemplatesEnd(); ++itr)
		{
			if(itr->second->chance >= rand)
			{
				templateList.push_back(itr->first);
			}
		}

		for(BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
			templateList.erase(std::remove(templateList.begin(), templateList.end(), itr->second->templateId), templateList.end());

		if(templateList.empty())
			continue;

		BMAuctionTemplate *selTemplate = GetTemplate(templateList[urand(0, templateList.size())]);

		if(!selTemplate)
			continue;


		BMAuctionEntry* auction = new BMAuctionEntry;
		auction->id = GetNewAuctionId();
		auction->bid = selTemplate->startBid;
		auction->bidder = 0;
		auction->startTime = time(NULL) + sWorld->getIntConfig(CONFIG_BLACKMARKET_AUCTION_DELAY)
										+ urand(0, sWorld->getIntConfig(CONFIG_BLACKMARKET_AUCTION_DELAY_MOD)*2)
										- sWorld->getIntConfig(CONFIG_BLACKMARKET_AUCTION_DELAY_MOD) / 2;

		auction->bm_template = selTemplate;
		auction->templateId = selTemplate->id;

		BMAuctionsMap[auction->id] = auction;
		auction->SaveToDB(trans);
	}
}

void BlackMarketMgr::BuildBlackMarketAuctionsPacket(WorldPacket& data, uint32 guidLow)
{
	uint32 count = 0;

	data << uint32(1); // unk
	data.WriteBits(count, 20); // placeholder

	for(BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
	{
		BMAuctionEntry* auction = itr->second;

		if (!auction->IsActive())
			continue;

		data.WriteBit((guidLow == auction->bidder));
		
		++count;
	}

	data.FlushBits();

	for(BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
	{
		BMAuctionEntry* auction = itr->second;

		if (!auction->IsActive())
			continue;

		data << uint32(auction->bm_template->seller); //seller
		data << uint32(auction->TimeLeft()); //time left
		data << uint64(0); //unk
		data << uint64(0); //unk
		data << uint64(auction->bid); // price
		data << uint32(auction->id); // auction id
		data << uint32(0); //unk
		data << uint32(auction->bm_template->itemCount); //stack count
		data << uint32(auction->bm_template->itemEntry); //item id
		data << uint32(0); //unk
	}

	data.PutBits<uint32>(32, count, 20);

	sLog->outInfo(LOG_FILTER_NETWORKIO, ">> Sent %u BlackMarket Auctions", count);
}

void BlackMarketMgr::UpdateAuction(BMAuctionEntry* auction, uint64 newPrice, Player* newBidder)
{
	SQLTransaction trans = CharacterDatabase.BeginTransaction();
	
	if (auction->bidder)
		SendAuctionOutbidded(auction, newPrice, newBidder, trans);

	auction->bid = newPrice;
	auction->bidder = newBidder->GetGUIDLow();

	auction->UpdateToDB(trans);

    CharacterDatabase.CommitTransaction(trans);
}

std::string BMAuctionEntry::BuildAuctionMailSubject(BMMailAuctionAnswers response)
{
    std::ostringstream strm;
    strm << bm_template->itemEntry << ":0:" << response << ':' << id << ':' << bm_template->itemCount;
    return strm.str();
}

std::string BMAuctionEntry::BuildAuctionMailBody(uint32 lowGuid)
{
    std::ostringstream strm;
    strm.width(16);
    strm << std::right << std::hex << MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);   // HIGHGUID_PLAYER always present, even for empty guids
    strm << std::dec << ':' << bid << ':' << 0;
    strm << ':' << 0 << ':' << 0;
    return strm.str();
}

void BlackMarketMgr::SendAuctionOutbidded(BMAuctionEntry* auction, uint32 newPrice, Player* newBidder, SQLTransaction& trans)
{
	WorldPacket data(SMSG_BLACK_MARKET_OUT_BID, 12);

	data << uint32(1);
	data << uint32(auction->bm_template->itemEntry);
	data << uint32(1);

	if (Player* bidder = sObjectAccessor->FindPlayer(MAKE_NEW_GUID(auction->bidder, 0, HIGHGUID_PLAYER)))
	{
		bidder->GetSession()->SendPacket(&data);

		MailDraft(auction->BuildAuctionMailSubject(BM_AUCTION_OUTBIDDED), auction->BuildAuctionMailBody(auction->bm_template->seller))
            .AddMoney(auction->bid)
            .SendMailTo(trans, MailReceiver(bidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
	}
}

void BlackMarketMgr::SendAuctionWon(BMAuctionEntry* auction, SQLTransaction& trans)
{
	if (Player* bidder = sObjectAccessor->FindPlayer(MAKE_NEW_GUID(auction->bidder, 0, HIGHGUID_PLAYER)))
	{
		WorldPacket data(SMSG_BLACK_MARKET_BID_WON, 12);
		data << uint32(auction->bm_template->itemEntry);
		data << uint32(1);
		data << uint32(1);
		bidder->GetSession()->SendPacket(&data);

		ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(auction->bm_template->itemEntry);
        if (!itemTemplate)
            return;

		Item* pItem = Item::CreateItem(auction->bm_template->itemEntry, auction->bm_template->itemCount, bidder);

		MailDraft(auction->BuildAuctionMailSubject(BM_AUCTION_WON), auction->BuildAuctionMailBody(auction->bidder))
            .AddItem(pItem)
            .SendMailTo(trans, MailReceiver(bidder, auction->bidder), MailSender(auction), MAIL_CHECK_MASK_COPIED);
	}
}