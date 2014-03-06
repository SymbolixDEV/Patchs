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

#ifndef _UPDATEFIELDS_H
#define _UPDATEFIELDS_H

enum EObjectFields
{
    OBJECT_FIELD_GUID                                = 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    OBJECT_FIELD_DATA                                = 0x0002, // Size: 2, Type: LONG, Flags: PUBLIC
    OBJECT_FIELD_TYPE                                = 0x0004, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    OBJECT_FIELD_ENTRY                               = 0x0005, // Size: 1, Type: INT, Flags: PUBLIC
    OBJECT_FIELD_SCALE                               = 0x0006, // Size: 1, Type: FLOAT, Flags: PUBLIC
    OBJECT_END                                       = 0x0007
};

enum EItemFields
{
    ITEM_FIELD_OWNER                                 = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    ITEM_FIELD_CONTAINED                             = OBJECT_END + 0x0002, // Size: 2, Type: LONG, Flags: PUBLIC
    ITEM_FIELD_CREATOR                               = OBJECT_END + 0x0004, // Size: 2, Type: LONG, Flags: PUBLIC
    ITEM_FIELD_GIFTCREATOR                           = OBJECT_END + 0x0006, // Size: 2, Type: LONG, Flags: PUBLIC
    ITEM_FIELD_STACK_COUNT                           = OBJECT_END + 0x0008, // Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
    ITEM_FIELD_DURATION                              = OBJECT_END + 0x0009, // Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
    ITEM_FIELD_SPELL_CHARGES                         = OBJECT_END + 0x000A, // Size: 5, Type: INT, Flags: OWNER, ITEM_OWNER
    ITEM_FIELD_FLAGS                                 = OBJECT_END + 0x000F, // Size: 1, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_1_1                       = OBJECT_END + 0x0010, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_1_3                       = OBJECT_END + 0x0012, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_2_1                       = OBJECT_END + 0x0013, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_2_3                       = OBJECT_END + 0x0015, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_3_1                       = OBJECT_END + 0x0016, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_3_3                       = OBJECT_END + 0x0018, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_4_1                       = OBJECT_END + 0x0019, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_4_3                       = OBJECT_END + 0x001B, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_5_1                       = OBJECT_END + 0x001C, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_5_3                       = OBJECT_END + 0x001E, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_6_1                       = OBJECT_END + 0x001F, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_6_3                       = OBJECT_END + 0x0021, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_7_1                       = OBJECT_END + 0x0022, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_7_3                       = OBJECT_END + 0x0024, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_8_1                       = OBJECT_END + 0x0025, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_8_3                       = OBJECT_END + 0x0027, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_9_1                       = OBJECT_END + 0x0028, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_9_3                       = OBJECT_END + 0x002A, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_10_1                      = OBJECT_END + 0x002B, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_10_3                      = OBJECT_END + 0x002D, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_11_1                      = OBJECT_END + 0x002E, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_11_3                      = OBJECT_END + 0x0030, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_12_1                      = OBJECT_END + 0x0031, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_12_3                      = OBJECT_END + 0x0033, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_13_1                      = OBJECT_END + 0x0034, // Size: 2, Type: INT, Flags: PUBLIC
    ITEM_FIELD_ENCHANTMENT_13_3                      = OBJECT_END + 0x0036, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    ITEM_FIELD_PROPERTY_SEED                         = OBJECT_END + 0x0037, // Size: 1, Type: INT, Flags: PUBLIC
    ITEM_FIELD_RANDOM_PROPERTIES_ID                  = OBJECT_END + 0x0038, // Size: 1, Type: INT, Flags: PUBLIC
    ITEM_FIELD_DURABILITY                            = OBJECT_END + 0x0039, // Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
    ITEM_FIELD_MAXDURABILITY                         = OBJECT_END + 0x003A, // Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
    ITEM_FIELD_CREATE_PLAYED_TIME                    = OBJECT_END + 0x003B, // Size: 1, Type: INT, Flags: PUBLIC
    ITEM_FIELD_MOD_MASK                              = OBJECT_END + 0x003C,
    ITEM_END                                         = OBJECT_END + 0x003D,
};

enum EItemDynamicFields
{
    ITEM_DYNAMIC_FIELD_MODIFIERS                     = ITEM_END + 0x0000,
    ITEM_DYNAMIC_END                                 = ITEM_END + 0x0048
};

enum EContainerFields
{
    CONTAINER_FIELD_SLOT_1                           = ITEM_END + 0x0000, // Size: 72, Type: LONG, Flags: PUBLIC
    CONTAINER_FIELD_NUM_SLOTS                        = ITEM_END + 0x0048, // Size: 1, Type: INT, Flags: PUBLIC
    CONTAINER_END                                    = ITEM_END + 0x0049
};

enum EUnitFields
{
    UNIT_FIELD_CHARM                                 = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_SUMMON                                = OBJECT_END + 0x0002, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_CRITTER                               = OBJECT_END + 0x0004, // Size: 2, Type: LONG, Flags: PRIVATE
    UNIT_FIELD_CHARMEDBY                             = OBJECT_END + 0x0006, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_SUMMONEDBY                            = OBJECT_END + 0x0008, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_CREATEDBY                             = OBJECT_END + 0x000A, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_TARGET                                = OBJECT_END + 0x000C, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_CHANNEL_OBJECT                        = OBJECT_END + 0x000E, // Size: 2, Type: LONG, Flags: PUBLIC
	UNIT_CHANNEL_SPELL                               = OBJECT_END + 0x0010, // Size: 1, Type: INT, Flags: PUBLIC
	UNIT_FIELD_SUMMONED_BY_HOME_REALM                = OBJECT_END + 0x0011, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_BYTES_0                               = OBJECT_END + 0x0012, // Size: 1, Type: BYTES, Flags: PUBLIC UNIT_FIELD_DISPLAY_POWER ?
    UNIT_OVERRIDE_DISPLAY_POWER_ID                   = OBJECT_END + 0x0013,
    UNIT_FIELD_HEALTH                                = OBJECT_END + 0x0014, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER1                                = OBJECT_END + 0x0015, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER2                                = OBJECT_END + 0x0016, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER3                                = OBJECT_END + 0x0017, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER4                                = OBJECT_END + 0x0018, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER5                                = OBJECT_END + 0x0019, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXHEALTH                             = OBJECT_END + 0x001A, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER1                             = OBJECT_END + 0x001B, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER2                             = OBJECT_END + 0x001C, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER3                             = OBJECT_END + 0x001D, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER4                             = OBJECT_END + 0x001E, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER5                             = OBJECT_END + 0x001F, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER1            = OBJECT_END + 0x0020, // Size: 5, Type: FLOAT, Flags: PRIVATE, OWNER, UNUSED2
    UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER1= OBJECT_END + 0x0025, // Size: 5, Type: FLOAT, Flags: PRIVATE, OWNER, UNUSED2
    UNIT_FIELD_LEVEL                                 = OBJECT_END + 0x002A, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_FACTIONTEMPLATE                       = OBJECT_END + 0x002B, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_VIRTUAL_ITEM_SLOT_ID                        = OBJECT_END + 0x002C, // Size: 3, Type: INT, Flags: PUBLIC
    /* About UNIT_VIRTUAL_ITEM_SLOT_ID 3 fileds
    UNIT_FIELD_VIRTUAL_ITEM_ID1                      = OBJECT_END + 0x002C,
    UNIT_FIELD_VIRTUAL_ITEM_ID2                      = OBJECT_END + 0x002D,
    UNIT_FIELD_VIRTUAL_ITEM_ID3                      = OBJECT_END + 0x002E,*/
    UNIT_FIELD_FLAGS                                 = OBJECT_END + 0x002F, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_FLAGS_2                               = OBJECT_END + 0x0030, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_AURASTATE                             = OBJECT_END + 0x0031, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_BASEATTACKTIME                        = OBJECT_END + 0x0032, // Size: 2, Type: INT, Flags: PUBLIC
    UNIT_FIELD_RANGEDATTACKTIME                      = OBJECT_END + 0x0034, // Size: 1, Type: INT, Flags: PRIVATE
    UNIT_FIELD_BOUNDINGRADIUS                        = OBJECT_END + 0x0035, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_FIELD_COMBATREACH                           = OBJECT_END + 0x0036, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_FIELD_DISPLAYID                             = OBJECT_END + 0x0037, // Size: 1, Type: INT, Flags: DYNAMIC
    UNIT_FIELD_NATIVEDISPLAYID                       = OBJECT_END + 0x0038, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MOUNTDISPLAYID                        = OBJECT_END + 0x0039, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MINDAMAGE                             = OBJECT_END + 0x003A, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, SPECIAL_INFO
    UNIT_FIELD_MAXDAMAGE                             = OBJECT_END + 0x003B, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, SPECIAL_INFO
    UNIT_FIELD_MINOFFHANDDAMAGE                      = OBJECT_END + 0x003C, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, SPECIAL_INFO
    UNIT_FIELD_MAXOFFHANDDAMAGE                      = OBJECT_END + 0x003D, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, SPECIAL_INFO
    UNIT_FIELD_BYTES_1                               = OBJECT_END + 0x003E, // Size: 1, Type: BYTES, Flags: PUBLIC  UNIT_FIELD_BYTES_1 ?/
    UNIT_FIELD_PETNUMBER                             = OBJECT_END + 0x003F, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_PET_NAME_TIMESTAMP                    = OBJECT_END + 0x0040, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_PETEXPERIENCE                         = OBJECT_END + 0x0041, // Size: 1, Type: INT, Flags: OWNER
    UNIT_FIELD_PETNEXTLEVELEXP                       = OBJECT_END + 0x0042, // Size: 1, Type: INT, Flags: OWNER
    UNIT_DYNAMIC_FLAGS                               = OBJECT_END + 0x0043, // Size: 1, Type: INT, Flags: DYNAMIC
    UNIT_MOD_CAST_SPEED                              = OBJECT_END + 0x0044, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_MOD_CAST_HASTE                              = OBJECT_END + 0x0045, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_MOD_HASTE                                   = OBJECT_END + 0x0046,
    UNIT_MOD_HASTE_REGEN                             = OBJECT_END + 0x0047,
    UNIT_CREATED_BY_SPELL                            = OBJECT_END + 0x0048, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_NPC_FLAGS                                   = OBJECT_END + 0x0049, // Size: 2, Type: INT, Flags: DYNAMIC
    UNIT_NPC_EMOTESTATE                              = OBJECT_END + 0x004B, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_STAT0                                 = OBJECT_END + 0x004C, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT1                                 = OBJECT_END + 0x004D, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT2                                 = OBJECT_END + 0x004E, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT3                                 = OBJECT_END + 0x004F, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT4                                 = OBJECT_END + 0x0050, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT0                              = OBJECT_END + 0x0051, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT1                              = OBJECT_END + 0x0052, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT2                              = OBJECT_END + 0x0053, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT3                              = OBJECT_END + 0x0054, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT4                              = OBJECT_END + 0x0055, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT0                              = OBJECT_END + 0x0056, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT1                              = OBJECT_END + 0x0057, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT2                              = OBJECT_END + 0x0058, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT3                              = OBJECT_END + 0x0059, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT4                              = OBJECT_END + 0x005A, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RESISTANCES                           = OBJECT_END + 0x005B, // Size: 7, Type: INT, Flags: PRIVATE, OWNER, SPECIAL_INFO
    UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE            = OBJECT_END + 0x0062, // Size: 7, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE            = OBJECT_END + 0x0069, // Size: 7, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_BASE_MANA                             = OBJECT_END + 0x0070, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_BASE_HEALTH                           = OBJECT_END + 0x0071, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_BYTES_2                               = OBJECT_END + 0x0072, // Size: 1, Type: BYTES, Flags: PUBLIC      UNIT_FIELD_SHAPESHIFT_FORM??
    UNIT_FIELD_ATTACK_POWER                          = OBJECT_END + 0x0073, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_ATTACK_POWER_MOD_POS                  = OBJECT_END + 0x0074, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_ATTACK_POWER_MOD_NEG                  = OBJECT_END + 0x0075, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_ATTACK_POWER_MULTIPLIER               = OBJECT_END + 0x0076, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER                   = OBJECT_END + 0x0077, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS           = OBJECT_END + 0x0078, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG           = OBJECT_END + 0x0079, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER        = OBJECT_END + 0x007A, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_MINRANGEDDAMAGE                       = OBJECT_END + 0x007B, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_MAXRANGEDDAMAGE                       = OBJECT_END + 0x007C, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POWER_COST_MODIFIER                   = OBJECT_END + 0x007D, // Size: 7, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POWER_COST_MULTIPLIER                 = OBJECT_END + 0x0084, // Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_MAXHEALTHMODIFIER                     = OBJECT_END + 0x008B, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_HOVERHEIGHT                           = OBJECT_END + 0x008C, // Size: 1, Type: FLOAT, Flags: PUBLIC    
    UNIT_FIELD_MIN_ITEM_LEVEL                        = OBJECT_END + 0x008D,
    UNIT_FIELD_MAXITEMLEVEL                          = OBJECT_END + 0x008E, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_WILD_BATTLE_PET_LEVEL                 = OBJECT_END + 0x008F,
    UNIT_FIELD_BATTLE_PET_COMPANION_GUID             = OBJECT_END + 0x0090,
    UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP   = OBJECT_END + 0x0092,
    UNIT_END                                         = OBJECT_END + 0x0093
};

enum EUnitDynamicField
{
    UNIT_DYNAMIC_FIELD_PASSIVE_SPELLS                = OBJECT_END + 0x0000,
    UNIT_DYNAMIC_END                                 = OBJECT_END + 0x0101
};

enum EPlayerFields
{
    PLAYER_DUEL_ARBITER                              = UNIT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    PLAYER_FLAGS                                     = UNIT_END + 0x0002, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILDRANK                                 = UNIT_END + 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILDDELETE_DATE                          = UNIT_END + 0x0004, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILDLEVEL                                = UNIT_END + 0x0005, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_BYTES                                     = UNIT_END + 0x0006, // Size: 1, Type: BYTES, Flags: PUBLIC
    PLAYER_BYTES_2                                   = UNIT_END + 0x0007, // Size: 1, Type: BYTES, Flags: PUBLIC
    PLAYER_BYTES_3                                   = UNIT_END + 0x0008, // Size: 1, Type: BYTES, Flags: PUBLIC
    PLAYER_DUEL_TEAM                                 = UNIT_END + 0x0009, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILD_TIMESTAMP                           = UNIT_END + 0x000A, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_QUEST_LOG_1_1                             = UNIT_END + 0x000B, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_VISIBLE_ITEM_1_ENTRYID                    = UNIT_END + 0x02F9, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_1_ENCHANTMENT                = UNIT_END + 0x02FA, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_2_ENTRYID                    = UNIT_END + 0x02FB, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_2_ENCHANTMENT                = UNIT_END + 0x02FC, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_3_ENTRYID                    = UNIT_END + 0x02FD, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_3_ENCHANTMENT                = UNIT_END + 0x02FE, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_4_ENTRYID                    = UNIT_END + 0x02FF, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_4_ENCHANTMENT                = UNIT_END + 0x0300, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_5_ENTRYID                    = UNIT_END + 0x0301, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_5_ENCHANTMENT                = UNIT_END + 0x0302, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_6_ENTRYID                    = UNIT_END + 0x0303, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_6_ENCHANTMENT                = UNIT_END + 0x0304, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_7_ENTRYID                    = UNIT_END + 0x0305, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_7_ENCHANTMENT                = UNIT_END + 0x0306, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_8_ENTRYID                    = UNIT_END + 0x0307, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_8_ENCHANTMENT                = UNIT_END + 0x0308, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_9_ENTRYID                    = UNIT_END + 0x0309, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_9_ENCHANTMENT                = UNIT_END + 0x030A, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_10_ENTRYID                   = UNIT_END + 0x030B, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_10_ENCHANTMENT               = UNIT_END + 0x030C, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_11_ENTRYID                   = UNIT_END + 0x030D, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_11_ENCHANTMENT               = UNIT_END + 0x030E, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_12_ENTRYID                   = UNIT_END + 0x030F, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_12_ENCHANTMENT               = UNIT_END + 0x0310, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_13_ENTRYID                   = UNIT_END + 0x0311, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_13_ENCHANTMENT               = UNIT_END + 0x0312, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_14_ENTRYID                   = UNIT_END + 0x0313, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_14_ENCHANTMENT               = UNIT_END + 0x0314, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_15_ENTRYID                   = UNIT_END + 0x0315, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_15_ENCHANTMENT               = UNIT_END + 0x0316, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_16_ENTRYID                   = UNIT_END + 0x0317, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_16_ENCHANTMENT               = UNIT_END + 0x0318, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_17_ENTRYID                   = UNIT_END + 0x0319, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_17_ENCHANTMENT               = UNIT_END + 0x031A, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_18_ENTRYID                   = UNIT_END + 0x031B, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_18_ENCHANTMENT               = UNIT_END + 0x031C, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_19_ENTRYID                   = UNIT_END + 0x031D, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_19_ENCHANTMENT               = UNIT_END + 0x031E, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_CHOSEN_TITLE                              = UNIT_END + 0x031F, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_FAKE_INEBRIATION                          = UNIT_END + 0x0320, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_FIELD_HOME_PLAYER_REALM                   = UNIT_END + 0x0321,
    PLAYER_FIELD_CURRENT_SPEC_ID                     = UNIT_END + 0x0322,
    PLAYER_FIELD_TAXI_MOUNT_ANIM_KIT_ID              = UNIT_END + 0x0323,                       //MIRROR_ALL
    PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY    = UNIT_END + 0x0324,                       //MIRROR_ALL
    PLAYER_END_NOT_SELF                              = UNIT_END + 0x0325,

    PLAYER_FIELD_INV_SLOT_HEAD                       = UNIT_END + 0x0325, // Size: 46, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_PACK_SLOT_1                         = UNIT_END + 0x0353, // Size: 32, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_BANK_SLOT_1                         = UNIT_END + 0x0373, // Size: 56, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_BANKBAG_SLOT_1                      = UNIT_END + 0x03AB, // Size: 14, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_VENDORBUYBACK_SLOT_1                = UNIT_END + 0x03B9, // Size: 24, Type: LONG, Flags: PRIVATE

    PLAYER_FARSIGHT                                  = UNIT_END + 0x03D1, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES                       = UNIT_END + 0x03D3, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES1                      = UNIT_END + 0x03D5, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES2                      = UNIT_END + 0x03D7, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES3                      = UNIT_END + 0x03D9, // Size: 2, Type: LONG, Flags: PRIVATE

    PLAYER_FIELD_COINAGE                             = UNIT_END + 0x03DB, // Size: 2, Type: LONG, Flags: PRIVATE   
    PLAYER_XP                                        = UNIT_END + 0x03DD, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_NEXT_LEVEL_XP                             = UNIT_END + 0x03DE, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_SKILL_LINEID_0                            = UNIT_END + 0x03DF, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_SKILL_STEP_0                              = UNIT_END + 0x041F, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_SKILL_RANK_0                              = UNIT_END + 0x045F, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_SKILL_STARTINGRANK                        = UNIT_END + 0x049F, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_SKILL_MAX_RANK_0                          = UNIT_END + 0x04DF, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_SKILL_MODIFIER_0                          = UNIT_END + 0x051F, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_SKILL_TALENT_0                            = UNIT_END + 0x055F, // Size: 64, Type: TWO_SHORT, Flags: PRIVATE 

    PLAYER_CHARACTER_POINTS                          = UNIT_END + 0x059F, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_MAX_TALENT_TIERS                          = UNIT_END + 0x05A0,
    PLAYER_TRACK_CREATURES                           = UNIT_END + 0x05A1, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_TRACK_RESOURCES                           = UNIT_END + 0x05A2, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_EXPERTISE                                 = UNIT_END + 0x05A3, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_OFFHAND_EXPERTISE                         = UNIT_END + 0x05A4, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_RANGED_EXPERTISE                          = UNIT_END + 0x05A5,
    PLAYER_BLOCK_PERCENTAGE                          = UNIT_END + 0x05A6, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_DODGE_PERCENTAGE                          = UNIT_END + 0x05A7, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_PARRY_PERCENTAGE                          = UNIT_END + 0x05A8, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_CRIT_PERCENTAGE                           = UNIT_END + 0x05A9, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_RANGED_CRIT_PERCENTAGE                    = UNIT_END + 0x05AA, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_OFFHAND_CRIT_PERCENTAGE                   = UNIT_END + 0x05AB, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_SPELL_CRIT_PERCENTAGE1                    = UNIT_END + 0x05AC, // Size: 7, Type: FLOAT, Flags: PRIVATE
    PLAYER_SHIELD_BLOCK                              = UNIT_END + 0x05B3, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_SHIELD_BLOCK_CRIT_PERCENTAGE              = UNIT_END + 0x05B4, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_MASTERY                                   = UNIT_END + 0x05B5, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_PVP_POWER_DAMAGE                    = UNIT_END + 0x05B6,
    PLAYER_FIELD_PVP_POWER_HEALING                   = UNIT_END + 0x05B7,
    PLAYER_EXPLORED_ZONES_1                          = UNIT_END + 0x05B8, // Size: 156, Type: BYTES, Flags: PRIVATE

    PLAYER_REST_STATE_EXPERIENCE                     = UNIT_END + 0x0680, //it's PLAYER_REST_STATE_BONUS_POOL ??
    PLAYER_FIELD_MOD_DAMAGE_DONE_POS                 = UNIT_END + 0x0681, // Size: 7, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_NEG                 = UNIT_END + 0x0688, // Size: 7, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_PCT                 = UNIT_END + 0x068F, // Size: 7, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_HEALING_DONE_POS                = UNIT_END + 0x0696, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_HEALING_PCT                     = UNIT_END + 0x0697, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_HEALING_DONE_PCT                = UNIT_END + 0x0698, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_PERIODIC_HEALING_DONE_PERCENT   = UNIT_END + 0x0699,
    PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS              = UNIT_END + 0x069A, // Size: 3, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_SPELL_POWER_PCT                 = UNIT_END + 0x069D, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_RESILIENCE_PERCENT              = UNIT_END + 0x069E,
    PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_AP_PCT      = UNIT_END + 0x069F, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_OVERRIDE_APBY_SPELL_POWER_PERCENT   = UNIT_END + 0x06A0,
    PLAYER_FIELD_MOD_TARGET_RESISTANCE               = UNIT_END + 0x06A1, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE      = UNIT_END + 0x06A2, // Size: 1, Type: INT, Flags: PRIVATE

	PLAYER_FIELD_LIFETIME_MAX_RANK                   = UNIT_END + 0x06A3, // Size: 1, Type: INT, Flags: PRIVATE
    //PLAYER_FIELD_BYTES                               = UNIT_END + 0x06A3, // Size: 1, Type: BYTES, Flags: PRIVATE PLAYER_FIELD_LIFETIME_MAX_RANK?

    PLAYER_SELF_RES_SPELL                            = UNIT_END + 0x06A4, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_PVP_MEDALS                          = UNIT_END + 0x06A5, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_BUYBACK_PRICE_1                     = UNIT_END + 0x06A6, // Size: 12, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_BUYBACK_TIMESTAMP_1                 = UNIT_END + 0x06B2, // Size: 12, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_KILLS                               = UNIT_END + 0x06BE, // Size: 1, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_FIELD_LIFETIME_HONORABLE_KILLS            = UNIT_END + 0x06BF, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_WATCHED_FACTION_INDEX               = UNIT_END + 0x06C0, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_COMBAT_RATING_1                     = UNIT_END + 0x06C1, // Size: 26, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_ARENA_TEAM_INFO_1_1                 = UNIT_END + 0x06DC, // Size: 21, Type: INT, Flags: PRIVATE

    PLAYER_FIELD_BATTLEGROUND_RATING                 = UNIT_END + 0x06F1, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MAX_LEVEL                           = UNIT_END + 0x06F2, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_RUNE_REGEN_1                              = UNIT_END + 0x06F3, // Size: 4, Type: FLOAT, Flags: PRIVATE
    PLAYER_NO_REAGENT_COST_1                         = UNIT_END + 0x06F7, // Size: 4, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_GLYPH_SLOTS_1                       = UNIT_END + 0x06FB, // Size: 6, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_GLYPHS_1                            = UNIT_END + 0x0701, // Size: 6, Type: INT, Flags: PRIVATE

    PLAYER_GLYPHS_ENABLED                            = UNIT_END + 0x0707, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_PET_SPELL_POWER                           = UNIT_END + 0x0708, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_RESEARCHING_1                       = UNIT_END + 0x0709, // Size: 8, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_PROFESSION_SKILL_LINE_1                   = UNIT_END + 0x0711, // Size: 2, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_UI_HIT_MODIFIER                     = UNIT_END + 0x0713, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_UI_SPELL_HIT_MODIFIER               = UNIT_END + 0x0714, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_HOME_REALM_TIME_OFFSET              = UNIT_END + 0x0715, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_RANGED_HASTE                    = UNIT_END + 0x0716, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_PET_HASTE                       = UNIT_END + 0x0717, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID            = UNIT_END + 0x0718,
    PLAYER_FIELD_OVERRIDE_SPELLS_ID                  = UNIT_END + 0x071A,

    //PLAYER_FIELD_DAILY_QUESTS_1                    = UNIT_END + 0x0489, // (Should not in MOP anymore, just temp for compile)
    //PLAYER_FIELD_MOD_HASTE                         = UNIT_END + 0x04D2, // (Should not in MOP anymore, just temp for compile)
    //PLAYER_FIELD_BYTES                             = UNIT_END + 0x0439, // (Should not in MOP anymore, just temp for compile)
    //PLAYER_FIELD_BYTES2                            = UNIT_END + 0x0456, // (Should not in MOP anymore, just temp for compile)

    PLAYER_END                                       = UNIT_END + 0x071B
};

enum EPlayerDynamicFields
{
    PLAYER_DYNAMIC_FIELD_RESEARCH_SITES              = PLAYER_END + 0x0000,
    PLAYER_DYNAMIC_FIELD_DAILY_QUESTS_COMPLETED      = PLAYER_END + 0x0002,
    PLAYER_DYNAMIC_END                               = PLAYER_END + 0x0004
};

enum EGameObjectFields
{
    OBJECT_FIELD_CREATED_BY                          = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    GAMEOBJECT_DISPLAYID                             = OBJECT_END + 0x0002, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_FLAGS                                 = OBJECT_END + 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_PARENTROTATION                        = OBJECT_END + 0x0004, // Size: 4, Type: FLOAT, Flags: PUBLIC
    GAMEOBJECT_DYNAMIC                               = OBJECT_END + 0x0008, // Size: 1, Type: TWO_SHORT, Flags: DYNAMIC
    GAMEOBJECT_FACTION                               = OBJECT_END + 0x0009, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_LEVEL                                 = OBJECT_END + 0x000A, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_BYTES_1                               = OBJECT_END + 0x000B, // Size: 1, Type: BYTES, Flags: PUBLIC
    GAMEOBJECT_END                                   = OBJECT_END + 0x000C
};

enum EDynamicObjectFields
{
    DYNAMICOBJECT_CASTER                             = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    DYNAMICOBJECT_BYTES                              = OBJECT_END + 0x0002, // Size: 1, Type: INT, Flags: DYNAMIC //DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID
    DYNAMICOBJECT_SPELLID                            = OBJECT_END + 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
    DYNAMICOBJECT_RADIUS                             = OBJECT_END + 0x0004, // Size: 1, Type: FLOAT, Flags: PUBLIC
    DYNAMICOBJECT_CASTTIME                           = OBJECT_END + 0x0005, // Size: 1, Type: INT, Flags: PUBLIC
    DYNAMICOBJECT_END                                = OBJECT_END + 0x0006
};

enum ECorpseFields
{
    CORPSE_FIELD_OWNER                               = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    CORPSE_FIELD_PARTY                               = OBJECT_END + 0x0002, // Size: 2, Type: LONG, Flags: PUBLIC
    CORPSE_FIELD_DISPLAY_ID                          = OBJECT_END + 0x0004, // Size: 1, Type: INT, Flags: PUBLIC
    CORPSE_FIELD_ITEM                                = OBJECT_END + 0x0005, // Size: 19, Type: INT, Flags: PUBLIC
    CORPSE_FIELD_BYTES_1                             = OBJECT_END + 0x0018, // Size: 1, Type: BYTES, Flags: PUBLIC //CORPSE_FIELD_SKINID
    CORPSE_FIELD_BYTES_2                             = OBJECT_END + 0x0019, // Size: 1, Type: BYTES, Flags: PUBLIC //CORPSE_FIELD_FACIAL_HAIR_STYLE_ID
    CORPSE_FIELD_FLAGS                               = OBJECT_END + 0x001A, // Size: 1, Type: INT, Flags: PUBLIC
    CORPSE_FIELD_DYNAMIC_FLAGS                       = OBJECT_END + 0x001B, // Size: 1, Type: INT, Flags: DYNAMIC
    CORPSE_END                                       = OBJECT_END + 0x001C
};

enum EAreaTriggerFields
{
	AREATRIGGER_CASTER                               = OBJECT_END + 0x0000, // Size: 2, Type: INT, Flags: PUBLIC
	AREATRIGGER_DURATION                             = OBJECT_END + 0x0002, // Size: 1, Type: INT, Flags: PUBLIC
	AREATRIGGER_SPELLID                              = OBJECT_END + 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
	AREATRIGGER_SPELLVISUALID                        = OBJECT_END + 0x0004, // Size: 1, Type: INT, Flags: PUBLIC
    AREATRIGGER_END                                  = OBJECT_END + 0x0005
};

enum ESceneObjectFields
{
    SCENE_SCRIPT_PACKAGE_ID                          = OBJECT_END + 0x0000,
    SCENE_RND_SEED_VAL                               = OBJECT_END + 0x0001,
    SCENE_CREATE_BY                                  = OBJECT_END + 0x0002,
    SCENE_END                                        = OBJECT_END + 0x0004
};

#endif // _UPDATEFIELDS_H