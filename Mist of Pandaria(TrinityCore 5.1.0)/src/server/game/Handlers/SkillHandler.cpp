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
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Player.h"
#include "Pet.h"
#include "UpdateMask.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "SpellLearn.h"

void WorldSession::HandleSetSpecializationOpcode(WorldPacket& recvData)
{
    uint32 tabId;
    recvData >> tabId;

    int32 specId = -1;
    uint8 _class = _player->getClass();
    for (uint32 i = 0; i < sChrSpecializationStore.GetNumRows(); ++i)
        if (ChrSpecializationEntry const* chrSpec = sChrSpecializationStore.LookupEntry(i))
            if (chrSpec->ClassId == _class && chrSpec->TabId == tabId)
            {
                specId = chrSpec->Id;
                break;
            }

    if (specId != -1 && _player->GetPrimaryTalentTree(_player->GetActiveSpec()) == 0)
    {
        _player->SetPrimaryTalentTree(_player->GetActiveSpec(), specId);
        _player->SendTalentsInfoData(false);

        sSpellLearnMgr->UpdatePlayerSpells(_player);
    }
}

void WorldSession::HandleLearnTalentOpcode(WorldPacket& recvData)
{
    uint32 count;

    count = recvData.ReadBits(25);

    for (uint32 i = 0; i < count; ++i)
    {
        uint16 talentId;
        recvData >> talentId;
        if (_player->LearnTalent(talentId, 0))
            _player->SendTalentsInfoData(false);
    }
}

void WorldSession::HandleLearnPreviewTalents(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LEARN_PREVIEW_TALENTS");

    int32 tabPage;
    uint32 talentsCount;
    recvPacket >> tabPage;    // talent tree

    // prevent cheating (selecting new tree with points already in another)
    if (tabPage >= 0)   // -1 if player already has specialization
    {
        /*if (TalentTabEntry const* talentTabEntry = sTalentTabStore.LookupEntry(_player->GetPrimaryTalentTree(_player->GetActiveSpec())))
        {
            if (talentTabEntry->tabpage != uint32(tabPage))
            {
                recvPacket.rfinish();
                return;
            }
        }*/ //MOP disable it , need check later
    }

    recvPacket >> talentsCount;

    uint32 talentId, talentRank;

    for (uint32 i = 0; i < talentsCount; ++i)
    {
        recvPacket >> talentId >> talentRank;

        if (!_player->LearnTalent(talentId, talentRank))
        {
            recvPacket.rfinish();
            break;
        }
    }

    _player->SendTalentsInfoData(false);
}

void WorldSession::HandleTalentWipeConfirmOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_RESPEC_WIPE_CONFIRM");
    uint8 unk;
	ObjectGuid guid;

	recvData >> unk;

    guid[5] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[4]);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTalentWipeConfirmeOpcode - GUID : %u, unk : %u", GUID_LOPART(guid), unk);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTalentWipeConfirmOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);
        
    if (!GetPlayer()->ResetTalents())
    {
        WorldPacket data(SMSG_RESPEC_WIPE_CONFIRM);    //you have not any talent
        data << uint8(0);
        data << uint32(0);

        data << uint8(0);
        data << uint8(0);
        data << uint8(0);

        data << uint8(0);

        data << uint8(0);
        data << uint8(0);
        data << uint8(0);
        data << uint8(0);
        data << uint8(0);
        SendPacket(&data);
        return;
    }

    unit->CastSpell(_player, 14867, true);                  //spell: "Untalent Visual Effect"
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recvData)
{
    uint32 skillId;
    recvData >> skillId;

    if (!IsPrimaryProfessionSkill(skillId))
        return;

    GetPlayer()->SetSkill(skillId, 0, 0, 0);
}