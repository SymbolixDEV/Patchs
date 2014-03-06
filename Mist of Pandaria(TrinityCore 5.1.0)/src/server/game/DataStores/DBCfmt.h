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

#ifndef TRINITY_DBCSFRM_H
#define TRINITY_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - bool, p - field present in sql dbc, a - field absent in sql dbc

char const Achievementfmt[]="niiisxiixixxiix";
const std::string CustomAchievementfmt = "pppaaaapapaapp";
const std::string CustomAchievementIndex = "ID";
char const AchievementCriteriafmt[] = "niiiixiiiisiiiiixxiiiiii";
char const AreaTableEntryfmt[]="iiinixxxxxxxisiiiiifxxxxxxxxxx";
char const AreaGroupEntryfmt[] = "niiiiiii";
char const AreaPOIEntryfmt[]="niiiiiiiiiiiffixixxixxx";
char const AreaTriggerEntryfmt[]="nifffxxxfffffxxi";
char const ArmorLocationfmt[] = "nfffff";
char const AuctionHouseEntryfmt[] = "niiix";
char const BankBagSlotPricesEntryfmt[] = "ni";
char const BarberShopStyleEntryfmt[] = "nixxxiii";
char const BattleMasterListEntryfmt[]="niiiiiiiiiiixsiiiixxxx";
char const CharStartOutfitEntryfmt[] = "dbbbXiiiiiiiiiiiiiiiiiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxii";
char const CharTitlesEntryfmt[] = "nxsxix";
char const ChatChannelsEntryfmt[] = "nixsx";
char const ChrClassesEntryfmt[]="nixsxxxixiiiixxxix";
char const ChrRacesEntryfmt[]="nxixiixixxxxixsxxxxxixxxxxxxxxxxxxxx";
char const ChrClassesXPowerTypesfmt[] = "nii";
char const ChrSpecializationfmt[] = "nxiixiiixxxxxx";
char const ChrSpecializationSpellsfmt[]="niiix";
char const CinematicSequencesEntryfmt[] = "nxxxxxxxxx";
char const CreatureDisplayInfofmt[]="nixxfxxxxxxxxxxxxxx";
char const CreatureModelDatafmt[]="nxxxxxxxxxxxxxffxxxxxxxxxxxxxxxxx";
char const CreatureFamilyfmt[] = "nfifiiiiixsx";
char const CreatureSpellDatafmt[] = "niiiixxxx";
char const CreatureTypefmt[] = "nxx";
char const CurrencyTypesfmt[] = "nixxxiiiiix";
char const DestructibleModelDatafmt[] = "ixxixxxixxxixxxixxxxxxxx";
char const DungeonEncounterfmt[] = "iiixisxx";
char const DurabilityCostsfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const DurabilityQualityfmt[] = "nf";
char const EmotesEntryfmt[] = "nxxiiixx";
char const EmotesTextEntryfmt[] = "nxixxxxxxxxxxxxxxxx";
char const FactionEntryfmt[] = "niiiiiiiiiiiiiiiiiiffixsxi";
char const FactionTemplateEntryfmt[] = "niiiiiiiiiiiii";
char const GameObjectDisplayInfofmt[] = "nsxxxxxxxxxxffffffxxx";
char const GemPropertiesEntryfmt[] = "nixxii";
char const GlyphPropertiesfmt[] = "niii";
char const GlyphSlotfmt[] = "nii";
char const GtBarberShopCostBasefmt[] = "xf";
char const GtCombatRatingsfmt[] = "xf";
char const GtOCTHpPerStaminafmt[] = "df";
char const GtChanceToMeleeCritBasefmt[] = "xf";
char const GtChanceToMeleeCritfmt[] = "xf";
char const GtChanceToSpellCritBasefmt[] = "xf";
char const GtChanceToSpellCritfmt[] = "xf";
char const GtOCTClassCombatRatingScalarfmt[] = "df";
char const GtOCTRegenHPfmt[] = "f";
//char const GtOCTRegenMPfmt[] = "f";
char const GtRegenMPPerSptfmt[] = "xf";
char const GtSpellScalingfmt[] = "df";
char const GtOCTBaseHPByClassfmt[] = "df";
char const GtOCTBaseMPByClassfmt[] = "df";
char const GuildPerkSpellsfmt[] = "dii";
char const Holidaysfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiixxsiix";
char const ImportPriceArmorfmt[] = "nffff";
char const ImportPriceQualityfmt[] = "nf";
char const ImportPriceShieldfmt[] = "nf";
char const ImportPriceWeaponfmt[] = "nf";
char const ItemPriceBasefmt[] = "diff";
char const ItemReforgefmt[] = "nifif";
char const ItemBagFamilyfmt[] = "nx";
char const ItemArmorQualityfmt[] = "nfffffffi";
char const ItemArmorShieldfmt[] = "nifffffff";
char const ItemArmorTotalfmt[] = "niffff";
char const ItemClassfmt[]="nxfx";
char const ItemDamagefmt[] = "nfffffffi";
char const ItemDisenchantLootfmt[] = "niiiiii";
//char const ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
char const ItemLimitCategoryEntryfmt[] = "nxii";
char const ItemRandomPropertiesfmt[] = "nxiiixxs";
char const ItemRandomSuffixfmt[] = "nsxiiiiiiiiii";
char const ItemSetEntryfmt[] = "dsiiiiiiiiiixxxxxxxiiiiiiiiiiiiiiiiii";
char const LFGDungeonEntryfmt[]="nsiiiiiiiiixxixixxxxxxx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const LockEntryfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
char const PhaseEntryfmt[] = "nsi";
char const MailTemplateEntryfmt[] = "nxs";
char const MapEntryfmt[]="nxiixsixxixiffxiixi";
char const MapDifficultyEntryfmt[]="diisiix";
char const MovieEntryfmt[]="nxxxx";
char const MountCapabilityfmt[] = "niiiiiii";
char const MountTypefmt[] = "niiiiiiiiiiiiiiiiiiiiiiii";
char const NameGenfmt[] = "dsii";
char const OverrideSpellDatafmt[] = "niiiiiiiiiixx";
char const QuestFactionRewardfmt[] = "niiiiiiiiii";
char const QuestSortEntryfmt[] = "nx";
char const QuestXPfmt[] = "niiiiiiiiii";
char const PvPDifficultyfmt[] = "diiiii";
char const RandomPropertiesPointsfmt[] = "niiiiiiiiiiiiiii";
char const ScalingStatDistributionfmt[] = "niiiiiiiiiiiiiiiiiiiixi";
char const ScalingStatValuesfmt[]="iniiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const SkillLinefmt[]="nisxixixx";
char const SkillLineAbilityfmt[]="niiiiiiiiixxx";
char const SoundEntriesfmt[]="nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const SpecializationSpellfmt[] = "xiiix";
char const SpellCastTimefmt[] = "nixx";
char const SpellCategoriesEntryfmt[]="niiiiiiiix";
const std::string CustomSpellDifficultyfmt = "ppppp";
const std::string CustomSpellDifficultyIndex = "id";
char const SpellDurationfmt[] = "niii";
char const SpellEffectEntryfmt[]="niifiiiffiiiiiifiifiiixixiiiix";
const std::string CustomSpellEffectEntryfmt = "pppppppppppppppppppppppppppppp";
const std::string CustomSpellEffectEntryIndex = "Id";
char const SpellEffectScalingfmt[]="xfffn";
char const SpellEntryfmt[]="nssxxiiifiiiiiiiiiiiiiixi";
const std::string CustomSpellEntryfmt = "paaaappppppppppppppppppap";
const std::string CustomSpellEntryIndex = "Id";
char const SpellFocusObjectfmt[] = "nx";
char const SpellItemEnchantmentfmt[]="nxiiiiiiiiisxiiiiiiixxxxx";
char const SpellItemEnchantmentConditionfmt[] = "nbbbbbxxxxxbbbbbbbbbbiiiiiXXXXX";
char const SpellRadiusfmt[]="nfxxf";
char const SpellRangefmt[] = "nffffixx";
char const SpellReagentsEntryfmt[]="niiiiiiiiiiiiiiiixx";
char const SpellScalingEntryfmt[]="niiiifixx";
char const SpellTotemsEntryfmt[] = "niiii";
char const SpellTargetRestrictionsEntryfmt[]="niifxiiii";
char const SpellPowerEntryfmt[]="niiiiiixxffif";
char const SpellInterruptsEntryfmt[]="niiixixi";
char const SpellEquippedItemsEntryfmt[]="niiiii";
char const SpellMiscEntryfmt[]="niiiiiiiiiiiiixiiifiiiii";
const std::string CustomSpellMiscEntryfmt = "pppppppppppppppppppppppp";
const std::string CustomSpellMiscEntryIndex = "Id";
char const SpellAuraOptionsEntryfmt[]="niiiiii";
char const SpellAuraRestrictionsEntryfmt[]="niiiiiiiiii";
char const SpellCastingRequirementsEntryfmt[]="dixxixi";
char const SpellClassOptionsEntryfmt[]="nxiiixi";
char const SpellCooldownsEntryfmt[]="niiiii";
char const SpellLevelsEntryfmt[]="niiiii";
char const SpellRuneCostfmt[]="niiixi";
char const SpellShapeshiftEntryfmt[] = "nixixx";
char const SpellShapeshiftFormfmt[] = "nxxiixiiixxiiiiiiiixx";
char const StableSlotPricesfmt[] = "ni";
char const SummonPropertiesfmt[] = "niiiii";
char const TalentEntryfmt[]="niiiiiixiis";
char const TaxiNodesEntryfmt[] = "nifffsiiixx";
char const TaxiPathEntryfmt[] = "niii";
char const TaxiPathNodeEntryfmt[] = "diiifffiiii";
char const TotemCategoryEntryfmt[] = "nxii";
char const VehicleEntryfmt[]="nixffffiiiiiiiifffffffffffffffssssfifiixx";
char const VehicleSeatEntryfmt[] = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiixxxxxxxxxxxxxxxxxxxx";
char const WMOAreaTableEntryfmt[] = "niiixxxxxiixxxx";
char const WorldMapAreaEntryfmt[] = "xinxffffixxxxx";
char const WorldMapOverlayEntryfmt[]="nxiiiixxxxxxxxxx";
char const WorldSafeLocsEntryfmt[]="nifffxx";

#endif
