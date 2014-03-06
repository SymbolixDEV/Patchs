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

#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "BlackMarket/BlackMarketMgr.h"
#include "Log.h"
#include "Language.h"
#include "Opcodes.h"
#include "UpdateMask.h"
#include "Util.h"
#include "AccountMgr.h"

//please DO NOT use iterator++, because it is slower than ++iterator!!!
//post-incrementation is always slower than pre-incrementation !

//void called when player click on black market npc
void WorldSession::HandleBlackMarketHello(WorldPacket& recvData)
{
    ObjectGuid guid;

	guid[4] = recvData.ReadBit();
	guid[1] = recvData.ReadBit();
	guid[5] = recvData.ReadBit();
	guid[3] = recvData.ReadBit();
	guid[0] = recvData.ReadBit();
	guid[7] = recvData.ReadBit();
	guid[2] = recvData.ReadBit();
	guid[6] = recvData.ReadBit();

	recvData.ReadByteSeq(guid[2]);
	recvData.ReadByteSeq(guid[6]);
	recvData.ReadByteSeq(guid[7]);
	recvData.ReadByteSeq(guid[0]);
	recvData.ReadByteSeq(guid[4]);
	recvData.ReadByteSeq(guid[5]);
	recvData.ReadByteSeq(guid[3]);
	recvData.ReadByteSeq(guid[1]);

	uint64 npcGuid = uint64(guid);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBlackMarketHello - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBlackMarketHello(npcGuid);
}


void WorldSession::SendBlackMarketHello(uint64 npcGuid)
{
	/*
	if (GetPlayer()->getLevel() < sWorld->getIntConfig(CONFIG_BLACK_MARKET_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_AUCTION_REQ), sWorld->getIntConfig(CONFIG_BLACK_MARKET_LEVEL_REQ));
        return;
    }
	*/
	
    WorldPacket data(SMSG_BLACK_MARKET_HELLO, 9);
    
	ObjectGuid guid = npcGuid;

	data.WriteBit(guid[2]);
	data.WriteBit(guid[4]);
	data.WriteBit(guid[6]);
	data.WriteBit(guid[7]);
	data.WriteBit(guid[1]);
	data.WriteBit(guid[0]);
	data.WriteBit(1);		// unk
	data.WriteBit(guid[5]);
	data.WriteBit(guid[3]);

	data.FlushBits();

	data.WriteByteSeq(guid[5]);
	data.WriteByteSeq(guid[7]);
	data.WriteByteSeq(guid[6]);
	data.WriteByteSeq(guid[2]);
	data.WriteByteSeq(guid[3]);
	data.WriteByteSeq(guid[0]);
	data.WriteByteSeq(guid[4]);
	data.WriteByteSeq(guid[1]);

    SendPacket(&data);
}

void WorldSession::HandleBlackMarketRequestItems(WorldPacket& recvData)
{
	ObjectGuid guid;
	uint32 unk;

	recvData >> unk;

	guid[2] = recvData.ReadBit();
	guid[5] = recvData.ReadBit();
	guid[4] = recvData.ReadBit();
	guid[0] = recvData.ReadBit();
	guid[7] = recvData.ReadBit();
	guid[3] = recvData.ReadBit();
	guid[6] = recvData.ReadBit();
	guid[1] = recvData.ReadBit();

	recvData.ReadByteSeq(guid[7]);
	recvData.ReadByteSeq(guid[5]);
	recvData.ReadByteSeq(guid[6]);
	recvData.ReadByteSeq(guid[4]);
	recvData.ReadByteSeq(guid[0]);
	recvData.ReadByteSeq(guid[3]);
	recvData.ReadByteSeq(guid[1]);
	recvData.ReadByteSeq(guid[2]);

	uint64 npcGuid = uint64(guid);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBlackMarketRequestItems - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }


	SendBlackMarketRequestItemsResult();

}

void WorldSession::SendBlackMarketRequestItemsResult()
{
	WorldPacket data(SMSG_BLACK_MARKET_REQUEST_ITEMS_RESULT, 9);
	sBlackMarketMgr->BuildBlackMarketAuctionsPacket(data, GetPlayer()->GetGUIDLow());
	SendPacket(&data);
}

void WorldSession::HandleBlackMarketBid(WorldPacket& recvData)
{
	ObjectGuid guid;
	uint32 itemid, id;
	uint64 price;

	recvData >> id;
	recvData >> price;
	recvData >> itemid;

	guid[7] = recvData.ReadBit();
	guid[4] = recvData.ReadBit();
	guid[0] = recvData.ReadBit();
	guid[2] = recvData.ReadBit();
	guid[5] = recvData.ReadBit();
	guid[3] = recvData.ReadBit();
	guid[1] = recvData.ReadBit();
	guid[6] = recvData.ReadBit();

	recvData.ReadByteSeq(guid[3]);
	recvData.ReadByteSeq(guid[7]);
	recvData.ReadByteSeq(guid[0]);
	recvData.ReadByteSeq(guid[1]);
	recvData.ReadByteSeq(guid[6]);
	recvData.ReadByteSeq(guid[5]);
	recvData.ReadByteSeq(guid[2]);
	recvData.ReadByteSeq(guid[4]);

	sLog->outDebug(LOG_FILTER_NETWORKIO, ">> HandleBid >> id : %u, price : %u, itemid : %u", id, price, itemid);

	uint64 npcGuid = uint64(guid);

	if (!price)
		return;

	Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBlackMarketBid - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }

	BMAuctionEntry *auction = sBlackMarketMgr->GetAuction(id);
	if (!auction)
		return;

	if(auction->bidder == GetPlayer()->GetGUIDLow()) // Trying to cheat
		return;

	if(auction->bid >= price && price != auction->bm_template->startBid) // Trying to cheat
		return;

	if(!GetPlayer()->ModifyMoney(-price))
		return; // trying to cheat


	sBlackMarketMgr->UpdateAuction(auction, price, GetPlayer());

	SendBlackMarketBidResult();
	SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketBidResult()
{
	WorldPacket data(SMSG_BLACK_MARKET_BID_RESULT, 12);
	data << uint32(0); //unk
	data << uint32(2); //unk
	data << uint32(2); //unk

	SendPacket(&data);
}