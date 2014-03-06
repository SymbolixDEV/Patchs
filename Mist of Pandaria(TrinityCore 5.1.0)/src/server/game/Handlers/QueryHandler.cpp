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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"



enum DBQueryType
{
    //BattlePetAbility           = 0xCBA43BD7,
    //BattlePetAbilityEffect     = 0xDD8B690E,
    //BattlePetAbilityState      = 0x3C556E43,
    //BattlePetAbilityTurn       = 0xECD8ECDC,
    //BattlePetBreedQuality      = 0x1B5A4EA6,
    //BattlePetBreedState        = 0x6AFB3206,
    DB_QUERY_BattlePetEffectProperties  = 0x63B4C4BA,
    //BattlePetNPCTeamMember     = 0xF2059DFA,
    //BattlePetSpecies           = 0x6C93F9B1,
    //BattlePetSpeciesState      = 0x15D87DD0,
    //BattlePetSpeciesXAbility   = 0x44237314,
    //BattlePetState             = 0x8F447330,
    //BattlePetVisual            = 0xC3ADEB43,
    DB_QUERY_NPC_TEXT          = 0x021826BB,
    /*Creature                   = 0xC9D6B6B3,
    GameObjects                = 0x13C403A5,*/
    DB_QUERY_ITEM_SPARSE       = 0x919BE54E,
    DB_QUERY_ITEM              = 0x50238EC2,
    /*ItemCurrencyCost           = 0x6FE05AE9,*/
    //ItemExtendedCost           = 0xBB858355,
    /*ItemUpgrade                = 0x7006463B,
    KeyChain                   = 0x6D8A2694,
    Locale                     = 0x3F85ABB7,
    Location                   = 0x394C3727,
    MapChallengeMode           = 0x383B4C27,
    MarketingPromotionsXLocale = 0xA1D3F1AD,
    Path                       = 0x94F46395,
    PathNode                   = 0x3B9E4CA2,
    PathNodeProperty           = 0xFE21C024,
    PathProperty               = 0x08E54F60,
    QuestPackageItem           = 0xCC2F84F0,
    RulesetItemUpgrade         = 0x6DB7086C,
    RulesetRaidLootUpgrade     = 0xED1FBB4D,
    SceneScript                = 0xD4B163CC,
    SceneScriptPackage         = 0xE8CB5E09,
    SceneScriptPackageMember   = 0xE44DB71C,
    SpellReagents              = 0xAB66C99F*/
};

void WorldSession::SendNameQueryOpcode(uint64 guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, (8+1+1+1+1+1+10));
    data.appendPackGUID(guid);
    if (!nameData)
    {
        data << uint8(1);                           // name unknown
        SendPacket(&data);
        return;
    }

    data << uint8(0);                               // name known
    data << nameData->m_name;                       // played name
    data << uint32(realmID);                              // realm name - only set for cross realm interaction (such as Battlegrounds)
    data << uint8(nameData->m_race);
    data << uint8(nameData->m_gender);
    data << uint8(nameData->m_class);

    data << uint8(0);                           // Name is not declined

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE name query %u", GUID_LOPART(guid));

    // This is disable by default to prevent lots of console spam
    // sLog->outInfo(LOG_FILTER_NETWORKIO, "HandleNameQueryOpcode %u", guid);

    SendNameQueryOpcode(guid);
}

void WorldSession::HandleRealmCache(WorldPacket& recvData)
{
	uint32 rev_realmid;
	recvData >> rev_realmid;
	sLog->outDebug(LOG_FILTER_NETWORKIO, "Send SMSG_REALM_CACHE rev_realmid %u realmID %u",rev_realmid, realmID);

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST);
	PreparedQueryResult result = LoginDatabase.Query(stmt);

	std::string realmName = "";
	WorldPacket data(SMSG_REALM_CACHE);
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 db_realmId = fields[0].GetUInt32();
			if (db_realmId == realmID)
				realmName = fields[1].GetString();
		}
	    while (result->NextRow());
	}

	data << rev_realmid;
    data << uint8(0);
    data << uint8(1); //realmid ?? not test
	data << realmName;
	data << realmName;
	SendPacket(&data);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);    
    data << uint32(time(NULL));
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;
    uint64 guid;
    recvData >> guid;

    CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(entry);
    if (ci)
    {

        std::string Name, SubName;
        Name = ci->Name;
        SubName = ci->SubName;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (CreatureLocale const* cl = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(cl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(cl->SubName, loc_idx, SubName);
            }
        }
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", ci->Name.c_str(), entry);
                                                            // guess size
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 100);
        data << uint32(entry);                              // creature entry
        data << Name;

        for (int i = 0; i < 7; i++)
            data << uint8(0); // name2, ..., name8

        data << SubName;
        data << ci->IconName;                               // "Directions" for guard, string for Icons 2.3.0
        data << uint8(0);                                   // unk string
        data << uint32(ci->type_flags);                     // flags
        data << uint32(ci->type_flags2);                    // unknown meaning
        data << uint32(ci->type);                           // CreatureType.dbc
        data << uint32(ci->family);                         // CreatureFamily.dbc
        data << uint32(ci->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(ci->KillCredit[0]);                  // new in 3.1, kill credit
        data << uint32(ci->KillCredit[1]);                  // new in 3.1, kill credit
        data << uint32(ci->Modelid1);                       // Modelid1
        data << uint32(ci->Modelid2);                       // Modelid2
        data << uint32(ci->Modelid3);                       // Modelid3
        data << uint32(ci->Modelid4);                       // Modelid4
        data << float(ci->ModHealth);                       // dmg/hp modifier
        data << float(ci->ModMana);                         // dmg/mana modifier
        data << uint8(ci->RacialLeader);
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            data << uint32(ci->questItems[i]);              // itemId[6], quest drop
        data << uint32(ci->movementId);                     // CreatureMovementInfo.dbc
        data << uint32(ci->expansionUnknown);               // unknown meaning
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (GUID: %u, ENTRY: %u)",
            GUID_LOPART(guid), entry);
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 100);
        data << uint32(entry);                              // creature entry
        data << "Inconnu";

        for (int i = 0; i < 7; i++)
            data << uint8(0); // name2, ..., name8

        data << "";
        data << "";                               // "Directions" for guard, string for Icons 2.3.0
        data << uint8(0);                                   // unk string
        data << uint32(0);                     // flags
        data << uint32(0);                    // unknown meaning
        data << uint32(0);                           // CreatureType.dbc
        data << uint32(0);                         // CreatureFamily.dbc
        data << uint32(0);                           // Creature Rank (elite, boss, etc)
        data << uint32(0);                  // new in 3.1, kill credit
        data << uint32(0);                  // new in 3.1, kill credit
        data << uint32(0);                       // Modelid1
        data << uint32(0);                       // Modelid2
        data << uint32(0);                       // Modelid3
        data << uint32(0);                       // Modelid4
        data << float(0);                       // dmg/hp modifier
        data << float(0);                         // dmg/mana modifier
        data << uint8(0);
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            data << uint32(0);              // itemId[6], quest drop
        data << uint32(0);                     // CreatureMovementInfo.dbc
        data << uint32(0);               // unknown meaning
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;
    uint64 guid;
    recvData >> guid;

    const GameObjectTemplate* info = sObjectMgr->GetGameObjectTemplate(entry);
    if (info)
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (GameObjectLocale const* gl = sObjectMgr->GetGameObjectLocale(entry))
            {
                ObjectMgr::GetLocaleString(gl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(gl->CastBarCaption, loc_idx, CastBarCaption);
            }
        }
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);
        data << uint32(entry);
        data << uint32(info->type);
        data << uint32(info->displayId);
        data << Name;
        data << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4
        data << IconName;                                   // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        data << CastBarCaption;                             // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        data << info->unk1;                                 // 2.0.3, string
        data.append(info->raw.data, MAX_GAMEOBJECT_DATA);
        data << float(info->size);                          // go size
        for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            data << uint32(info->questItems[i]);            // itemId[6], quest drop
        data << int32(info->unkInt32);                      // 4.x, unknown
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (GUID: %u, ENTRY: %u)",
            GUID_LOPART(guid), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received MSG_CORPSE_QUERY");

    Corpse* corpse = GetPlayer()->GetCorpse();

	uint32 mapid = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    uint32 corpsemapid = 0;
	ObjectGuid guid = (uint64)0;
	uint8 corpsefound = 0;

    if (corpse)
	{
		mapid = corpse->GetMapId();
		x = corpse->GetPositionX();
		y = corpse->GetPositionY();
		z = corpse->GetPositionZ();
		corpsemapid = mapid;
		guid = corpse->GetGUID();
		corpsefound = 1;

		// if corpse at different map
		if (mapid != _player->GetMapId())
		{
			// search entrance map for proper show entrance
			if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapid))
			{
				if (corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
				{
					// if corpse map have entrance
					if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
					{
						mapid = corpseMapEntry->entrance_map;
						x = corpseMapEntry->entrance_x;
						y = corpseMapEntry->entrance_y;
						z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
					}
				}
			}
		}
	}

	WorldPacket data(SMSG_CORPSE_QUERY);
    data << float(z);
    data << uint32(mapid);
    data << uint32(corpsemapid);
    data << float(y);
    data << float(x);

	data.WriteBit(guid[0]);
    data.WriteBit(corpsefound);
    data.WriteBit(guid[4]);
	data.WriteBit(guid[5]);
	data.WriteBit(guid[1]);
	data.WriteBit(guid[3]);
	data.WriteBit(guid[7]);
	data.WriteBit(guid[6]);
	data.WriteBit(guid[2]);
    data.FlushBits();

	data.WriteByteSeq(guid[0]);
	data.WriteByteSeq(guid[4]);
	data.WriteByteSeq(guid[3]);
	data.WriteByteSeq(guid[1]);
	data.WriteByteSeq(guid[2]);
	data.WriteByteSeq(guid[5]);
	data.WriteByteSeq(guid[6]);
	data.WriteByteSeq(guid[7]);
                                 
    SendPacket(&data);
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket& recvData)
{
    uint32 textID;
    uint64 guid;

    recvData >> textID;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID);

    recvData >> guid;
    GetPlayer()->SetSelection(guid);

    GossipText const* pGossip = sObjectMgr->GetGossipText(textID);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    data << textID;
    data << float(1.0f);

    for(int i = 0 ; i < 7 ; i++)
        data << uint32(0);

    data << uint32(textID);

    for(int i = 0 ; i < 7 ; i++)
        data << uint32(0);

    /*if (!pGossip)
    {
        for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i]=pGossip->Options[i].Text_0;
            Text_1[i]=pGossip->Options[i].Text_1;
        }

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textID))
            {
                for (int i = 0; i < MAX_LOCALES; ++i)
                {
                    ObjectMgr::GetLocaleString(nl->Text_0[i], loc_idx, Text_0[i]);
                    ObjectMgr::GetLocaleString(nl->Text_1[i], loc_idx, Text_1[i]);
                }
            }
        }

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if (Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if (Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for (int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }*/

    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

void SendNpcTextDBQueryResponse(WorldSession * p_Session, WorldPacket & p_Data, uint32 p_LocalTextID)
{
    GossipText const* l_Gossip = sObjectMgr->GetGossipText(p_LocalTextID);
    std::string l_Text1 = "Greetings $N";
    std::string l_Text2 = "Greetings $N";
    uint32 l_Language = 0;

    if (l_Gossip) 
    {
        if (l_Gossip->Options[0].Text_0.size())
            l_Text1 = l_Gossip->Options[0].Text_0;
        if (l_Gossip->Options[0].Text_1.size())
            l_Text2 = l_Gossip->Options[0].Text_1;
        l_Language = l_Gossip->Options[0].Language;
    }

    int l_SessionLocalIndex = p_Session->GetSessionDbLocaleIndex();
    if (l_SessionLocalIndex >= 0)
    {
        if (NpcTextLocale const* l_LocalData = sObjectMgr->GetNpcTextLocale(p_LocalTextID))
        {
            ObjectMgr::GetLocaleString(l_LocalData->Text_0[0], l_SessionLocalIndex, l_Text1);
            ObjectMgr::GetLocaleString(l_LocalData->Text_1[0], l_SessionLocalIndex, l_Text2);
        }
    }

    p_Data << uint32(p_LocalTextID);
    p_Data << uint32(l_Language);
    p_Data << uint16(l_Text1.size());
    p_Data << l_Text1;
    p_Data << uint16(l_Text2.size());
    p_Data << l_Text2;

    p_Data << uint32(0);	/// unk
    p_Data << uint32(0);	/// unk
    p_Data << uint32(0x01);	/// unk
}

bool SendItemSparseDBQueryResponse(WorldSession * p_Session, WorldPacket & p_Data, uint32 p_ItemEntry)
{
    ItemTemplate const * item = sObjectMgr->GetItemTemplate(p_ItemEntry);

    if(!item) return false;

    p_Data << uint32(item->ItemId);//OK
    p_Data << uint32(item->Quality);//OK
    p_Data << uint32(item->Flags);
    p_Data << uint32(item->Flags2);
    p_Data << float(item->Unk430_1);
    p_Data << float(item->Unk430_2);
    p_Data << uint32(0); //unk
    p_Data << uint32(item->BuyCount);//OK
    p_Data << int32(item->BuyPrice);//OK
    p_Data << uint32(item->SellPrice);//OK
    p_Data << uint32(item->InventoryType);
    p_Data << int32(item->AllowableClass);//OK
    p_Data << int32(item->AllowableRace);//OK
    p_Data << uint32(item->ItemLevel);//OK
    p_Data << uint32(item->RequiredLevel);//OK
    p_Data << uint32(item->RequiredSkill);
    p_Data << uint32(item->RequiredSkillRank);
    p_Data << uint32(item->RequiredSpell);
    p_Data << uint32(item->RequiredHonorRank);
    p_Data << uint32(item->RequiredCityRank);
    p_Data << uint32(item->RequiredReputationFaction);
    p_Data << uint32(item->RequiredReputationRank);
    p_Data << int32(item->MaxCount);//OK
    p_Data << int32(item->Stackable);//OK
    p_Data << uint32(item->ContainerSlots);//OK

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        p_Data << uint32(item->ItemStat[x].ItemStatType);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        p_Data << int32(item->ItemStat[x].ItemStatValue);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        p_Data << int32(item->ItemStat[x].ItemStatUnk1);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_STATS; ++x)
        p_Data << int32(item->ItemStat[x].ItemStatUnk2);

    p_Data << uint32(item->ScalingStatDistribution);
    p_Data << uint32(item->DamageType);
    p_Data << uint32(item->Delay);
    p_Data << float(item->RangedModRange);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
        p_Data << int32(item->Spells[x].SpellId);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
        p_Data << uint32(item->Spells[x].SpellTrigger);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
        p_Data << int32(item->Spells[x].SpellCharges);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
        p_Data << int32(item->Spells[x].SpellCooldown);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
        p_Data << uint32(item->Spells[x].SpellCategory);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
        p_Data << int32(item->Spells[x].SpellCategoryCooldown);

    p_Data << uint32(item->Bonding);//OK

    // item name
    std::string name = item->Name1;//OK
    p_Data << uint16(name.length());
    if (name.length())
        p_Data << name;

    for (uint32 i = 0; i < 3; ++i) // other 3 names
        p_Data << uint16(0);//OK

    std::string desc = item->Description;
    p_Data << uint16(desc.length());
    if (desc.length())
        p_Data << desc;

    p_Data << uint32(item->PageText);
    p_Data << uint32(item->LanguageID);
    p_Data << uint32(item->PageMaterial);
    p_Data << uint32(item->StartQuest);
    p_Data << uint32(item->LockID);
    p_Data << int32(item->Material);
    p_Data << uint32(item->Sheath);
    p_Data << int32(item->RandomProperty);
    p_Data << int32(item->RandomSuffix);
    p_Data << uint32(item->ItemSet);

    p_Data << uint32(item->Area);
    p_Data << uint32(item->Map);
    p_Data << uint32(item->BagFamily);
    p_Data << uint32(item->TotemCategory);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SOCKETS; ++x)
        p_Data << uint32(item->Socket[x].Color);

    for (uint32 x = 0; x < MAX_ITEM_PROTO_SOCKETS; ++x)
        p_Data << uint32(item->Socket[x].Content);

    p_Data << uint32(item->socketBonus);
    p_Data << uint32(item->GemProperties);
    p_Data << float(item->ArmorDamageModifier);
    p_Data << int32(item->Duration);
    p_Data << uint32(item->ItemLimitCategory);
    p_Data << uint32(item->HolidayId);
    p_Data << float(item->StatScalingFactor);    // StatScalingFactor
    p_Data << uint32(item->CurrencySubstitutionId);
    p_Data << uint32(item->CurrencySubstitutionCount);

    return true;
}

bool SendItemDBQueryResponse(WorldSession * p_Session, WorldPacket & p_Data, uint32 p_ItemEntry)
{
    ItemTemplate const * item = sObjectMgr->GetItemTemplate(p_ItemEntry);

    if(!item) return false;

    p_Data << uint32(item->ItemId);
    p_Data << uint32(item->Class);
    p_Data << uint32(item->SubClass);
    p_Data << int32(item->SoundOverrideSubclass);
    p_Data << uint32(item->Material);
    p_Data << uint32(item->DisplayInfoID);
    p_Data << uint32(item->InventoryType);
    p_Data << uint32(item->Sheath);

    return true;
}

void WorldSession::HandleDbQueryOpcode(WorldPacket& p_ReceivedPacket)
{
    uint32 l_QueryType;
    uint32 l_Count;

    p_ReceivedPacket >> l_QueryType;

    l_Count = p_ReceivedPacket.ReadBits(23);

    std::vector<ObjectGuid> l_Guids;
    std::vector<uint32>		l_requestedEntries;

    for (uint32 l_I = 0 ; l_I < l_Count ; l_I++)
    {
        ObjectGuid l_Guid;

        l_Guid[7] = p_ReceivedPacket.ReadBit();
        l_Guid[5] = p_ReceivedPacket.ReadBit();
        l_Guid[6] = p_ReceivedPacket.ReadBit();
        l_Guid[2] = p_ReceivedPacket.ReadBit();
        l_Guid[1] = p_ReceivedPacket.ReadBit();
        l_Guid[3] = p_ReceivedPacket.ReadBit();
        l_Guid[0] = p_ReceivedPacket.ReadBit();
        l_Guid[4] = p_ReceivedPacket.ReadBit();

        l_Guids.push_back(l_Guid);
    }

    for (uint32 l_I = 0 ; l_I < l_Count ; l_I++)
    {
        uint32	l_requestedEntry;

        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][7]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][6]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][1]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][2]);
        p_ReceivedPacket >> l_requestedEntry;
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][5]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][3]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][0]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][4]);

        l_requestedEntries.push_back(l_requestedEntry);
    }

    if (!l_Count)
        return;

    for(uint32 l_I = 0 ; l_I < l_Count ; l_I++)
    {
        WorldPacket l_Data(SMSG_DB_REPLY);
        l_Data << uint32(0x00);				/// Data size placeholder

        switch (l_QueryType)
        {
        case DB_QUERY_NPC_TEXT:
        {
            SendNpcTextDBQueryResponse(this, l_Data, l_requestedEntries[l_I]);
            break;
        }
        case DB_QUERY_ITEM_SPARSE:
        {
            if(!SendItemSparseDBQueryResponse(this, l_Data, l_requestedEntries[l_I])) continue; //dont send if no item
            break; //to disable the sent of the opcode
        }
        case DB_QUERY_ITEM:
        {
            if(!SendItemDBQueryResponse(this, l_Data, l_requestedEntries[l_I])) continue; //dont send if no item
            break; //to disable the sent of the opcode
        }
        case DB_QUERY_BattlePetEffectProperties:
        {
            l_Data << uint32(l_requestedEntries[l_I]);
            l_Data << uint32(38);
            l_Data << "Points";
            l_Data << "Accuracy";
            l_Data << uint32(43);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            break; //to disable the sent of the opcode
        }
        default:
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Received non handled db query type 0x%08.8X", l_QueryType);
            continue; //to disable the sent of the opcode
        }
        }

        l_Data << uint32(sObjectMgr->GetHotfixDate(l_requestedEntries[l_I], l_QueryType));
        l_Data << uint32(l_QueryType);
        l_Data << uint32(l_requestedEntries[l_I]);

        l_Data.wpos(0);
        l_Data << uint32(l_Data.size() - 16);

        SendPacket(&l_Data);
    }
}



/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PAGE_TEXT_QUERY");

    uint32 pageID;
    recvData >> pageID;
    recvData.read_skip<uint64>();                          // guid

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);
                                                            // guess size
        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);
        data << pageID;

        if (!pageText)
        {
            data << "Item page missing.";
            data << uint32(0);
            pageID = 0;
        }
        else
        {
            std::string Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, loc_idx, Text);

            data << Text;
            data << uint32(pageText->NextPage);
            pageID = pageText->NextPage;
        }
        SendPacket(&data);

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    uint32 unk;
    recvData >> unk;
    ObjectGuid guid;
    guid[0] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[0]);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CORPSE_MAP_POSITION_QUERY: Guid: %u unk %u", GUID_LOPART(guid), unk);

    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 count;
    recvData >> count; // quest count, max=25

    if (count >= MAX_QUEST_LOG_SIZE)
    {
        recvData.rfinish();
        return;
    }

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4+(4+4)*count);
    data << uint32(count); // count

    for (uint32 i = 0; i < count; ++i)
    {
        uint32 questId;
        recvData >> questId; // quest id

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                data << uint32(questId); // quest ID
                data << uint32(POI->size()); // POI count

                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Id);                // POI index
                    data << int32(itr->ObjectiveIndex);     // objective index
                    data << uint32(0);
                    data << uint32(itr->MapId);             // mapid
                    data << uint32(itr->AreaId);            // areaid
                    data << uint32(0);
                    data << uint32(itr->Unk2);              // unknown
                    data << uint32(itr->Unk3);              // unknown
                    data << uint32(itr->Unk4);              // unknown
                    data << uint32(0);
                    data << uint32(itr->points.size());     // POI points count

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x); // POI point x
                        data << int32(itr2->y); // POI point y
                    }
                }
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
            }
        }
        else
        {
            data << uint32(questId); // quest ID
            data << uint32(0); // POI count
        }
    }

    SendPacket(&data);
}
