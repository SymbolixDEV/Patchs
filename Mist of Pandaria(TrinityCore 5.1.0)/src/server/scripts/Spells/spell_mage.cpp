/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

/*
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mage_".
 */

/* UPDATE for PORTALS ( NEED TO FINISH AND APPLY IN DATABASE ):

-- Mage portal updates
-- TO DO : need to edit coordinates to blizzlike
-- TO DO : need to edit / add rows in gameobject_template for portals object ?
-- Example for ^ TO DO : INSERT INTO `gameobject_template` VALUES (206595, 22, 10105, 'Portal to Tol Barad', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 88341 ( spell for teleport; in our case : 120144, 132623, 132625), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 13329);

INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(120145, 571, 5805.05, 624.86, 647.76, 1.710), -- Ancient Teleport Dalaran
(120144, 571, 5805.05, 624.86, 647.76, 1.710), -- Ancient Portal Dalaran
(132623, 870, 1217.92, 1031.91, 425.96, 1.708),  -- Portal : Valle of Eternal Blossoms
(132625, 870, 1217.92, 1031.91, 425.96, 1.708),  -- Portal : Valle of Eternal Blossoms
(132627, 870, 1217.92, 1031.91, 425.96, 1.708),  -- Teleport  : Valle of Eternal Blossoms
(132621, 870, 1217.92, 1031.91, 425.96, 1.708);  -- Teleport  : Valle of Eternal Blossoms

*/

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Pet.h"

enum MageSpells
{
    SPELL_MAGE_BURNOUT                           = 29077,
    SPELL_MAGE_COLD_SNAP                         = 11958,
    SPELL_MAGE_FOCUS_MAGIC_PROC                  = 54648,
    SPELL_MAGE_FROST_WARDING_R1                  = 11189,
    SPELL_MAGE_FROST_WARDING_TRIGGERED           = 57776,
    SPELL_MAGE_INCANTERS_ABSORBTION_R1           = 44394,
    SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED    = 44413,
    SPELL_MAGE_IGNITE                            = 12654,
    SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE       = 29077,
    SPELL_MAGE_SQUIRREL_FORM                     = 32813,
    SPELL_MAGE_GIRAFFE_FORM                      = 32816,
    SPELL_MAGE_SERPENT_FORM                      = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM                   = 32818,
    SPELL_MAGE_WORGEN_FORM                       = 32819,
    SPELL_MAGE_SHEEP_FORM                        = 32820,
    SPELL_MAGE_GLYPH_OF_ETERNAL_WATER            = 70937,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT  = 70908,
    SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY  = 70907,
    SPELL_MAGE_GLYPH_OF_BLAST_WAVE               = 62126,

    SPELL_MAGE_FLAMESTRIKE                       = 2120,

    SPELL_MAGE_CHILLED_R1                        = 12484,
    SPELL_MAGE_CHILLED_R2                        = 12485,

    SPELL_MAGE_CONE_OF_COLD_AURA_R1              = 11190,
    SPELL_MAGE_CONE_OF_COLD_AURA_R2              = 12489,
    SPELL_MAGE_CONE_OF_COLD_TRIGGER_R1           = 83301,
    SPELL_MAGE_CONE_OF_COLD_TRIGGER_R2           = 83302,

    SPELL_MAGE_SHATTERED_BARRIER_R1              = 44745,
    SPELL_MAGE_SHATTERED_BARRIER_R2              = 54787,
    SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R1       = 55080,
    SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R2       = 83073,

	SPELL_MAGE_FINGERS_OF_FROST_PASSIVE          = 112965,
    SPELL_MAGE_FINGERS_OF_FROST                  = 44544,
	SPELL_MAGE_FINGERS_OF_FROST_2                = 126084,

	SPELL_MAGE_BRAIN_FREEZE_BUFF				 = 57761,
	SPELL_MAGE_BRAIN_FREEZE						 = 44549,

	SPELL_MAGE_RING_OF_FROST_SUMMON              = 113724,
	SPELL_MAGE_RING_OF_FROST_FREEZE              = 82691,
    SPELL_MAGE_RING_OF_FROST_DUMMY               = 91264,

	SPELL_MAGE_ICE_BLOCK                         = 45438,
    SPELL_MAGE_CONE_OF_COLD                      = 120,
    SPELL_MAGE_FROST_NOVA                        = 122,

	SPELL_MAGE_FROSTBOLT_HEAL					 = 126201,

	SPELL_MAGE_IMPROVED_MANA_GEM_TRIGGERED		 = 119313,

    SPELL_MAGE_ALTER_TIME_BASE                   = 108978,
    SPELL_MAGE_ALTER_TIME_AURA                   = 110909,
    SPELL_MAGE_ALTER_TIME_RESET                  = 127140,

	SPELL_MAGE_COMBUSTION_BASE					 = 11129,
	SPELL_MAGE_COMBUSTION_DOT					 = 83853,
	SPELL_MAGE_COMBUSTION_IMPACT				 = 118271,
	SPELL_MAGE_INFERNO_BLAST					 = 108853
};

enum MageIcons
{
    ICON_MAGE_SHATTER                            = 976,
    ICON_MAGE_IMPROVED_FLAMESTRIKE               = 37,
    ICON_MAGE_IMPROVED_FREEZE                    = 94,
    ICON_MAGE_INCANTER_S_ABSORPTION              = 2941,
    ICON_MAGE_IMPROVED_MANA_GEM                  = 1036
};

// Incanter's Absorbtion
class spell_mage_incanters_absorbtion_base_AuraScript : public AuraScript
{
    public:
        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORBTION_R1))
                return false;
            return true;
        }

        void Trigger(AuraEffect* aurEff, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
        {
            Unit* target = GetTarget();

            if (AuraEffect* talentAurEff = target->GetAuraEffectOfRankedSpell(SPELL_MAGE_INCANTERS_ABSORBTION_R1, EFFECT_0))
            {
                int32 bp = CalculatePct(absorbAmount, talentAurEff->GetAmount());
                target->CastCustomSpell(target, SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, NULL, NULL, true, NULL, aurEff);
            }
        }
};

// 11113 - Blast Wave
class spell_mage_blast_wave : public SpellScriptLoader
{
    public:
        spell_mage_blast_wave() : SpellScriptLoader("spell_mage_blast_wave") { }

        class spell_mage_blast_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_blast_wave_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FLAMESTRIKE))
                    return false;
                return true;
            }

            void CountTargets(std::list<WorldObject*>& targetList)
            {
                _targetCount = targetList.size();
            }

            void HandleImprovedFlamestrike()
            {
                if (_targetCount >= 2)
                    if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_FLAMESTRIKE, EFFECT_0))
                        if (roll_chance_i(aurEff->GetAmount()))
                        {
                            float x, y, z;
                            WorldLocation const* loc = GetExplTargetDest();
                            if (!loc)
                                return;

                            loc->GetPosition(x, y, z);
                            GetCaster()->CastSpell(x, y, z, SPELL_MAGE_FLAMESTRIKE, true);
                        }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_blast_wave_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                AfterCast += SpellCastFn(spell_mage_blast_wave_SpellScript::HandleImprovedFlamestrike);
            }

        private:
            uint32 _targetCount;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_blast_wave_SpellScript();
        }
};

// -44449 - Burnout
class spell_mage_burnout : public SpellScriptLoader
{
    public:
        spell_mage_burnout() : SpellScriptLoader("spell_mage_burnout") { }

        class spell_mage_burnout_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_burnout_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_BURNOUT))
                    return false;
                return true;
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                return eventInfo.GetDamageInfo()->GetSpellInfo(); // eventInfo.GetSpellInfo()
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                int32 mana = int32(eventInfo.GetDamageInfo()->GetSpellInfo()->CalcPowerCost(GetTarget(), eventInfo.GetDamageInfo()->GetSchoolMask()));
                mana = CalculatePct(mana, aurEff->GetAmount());

                GetTarget()->CastCustomSpell(SPELL_MAGE_BURNOUT, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_burnout_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_burnout_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_burnout_AuraScript();
        }
};

// 42208 - Blizzard
/// Updated 4.3.4
class spell_mage_blizzard : public SpellScriptLoader
{
   public:
       spell_mage_blizzard() : SpellScriptLoader("spell_mage_blizzard") { }

       class spell_mage_blizzard_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_blizzard_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/)
           {
               if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_CHILLED_R1))
                   return false;
               if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_CHILLED_R2))
                   return false;
               return true;
           }

           void AddChillEffect(SpellEffIndex /*effIndex*/)
           {
               Unit* caster = GetCaster();
               if (Unit* unitTarget = GetHitUnit())
               {
                   if (caster->IsScriptOverriden(GetSpellInfo(), 836))
                       caster->CastSpell(unitTarget, SPELL_MAGE_CHILLED_R1, true);
                   else if (caster->IsScriptOverriden(GetSpellInfo(), 988))
                       caster->CastSpell(unitTarget, SPELL_MAGE_CHILLED_R2, true);
               }
           }

           void Register()
           {
               OnEffectHitTarget += SpellEffectFn(spell_mage_blizzard_SpellScript::AddChillEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
           }
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_mage_blizzard_SpellScript();
       }
};

// Cold Snap - 11958
class spell_mage_cold_snap : public SpellScriptLoader
{
    public:
        spell_mage_cold_snap() : SpellScriptLoader("spell_mage_cold_snap") { }

        class spell_mage_cold_snap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cold_snap_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    // Resets cooldown of Ice Block, Frost Nova and Cone of Cold
                    player->RemoveSpellCooldown(SPELL_MAGE_ICE_BLOCK, true);
                    player->RemoveSpellCooldown(SPELL_MAGE_FROST_NOVA, true);
                    player->RemoveSpellCooldown(SPELL_MAGE_CONE_OF_COLD, true);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Cold Snap
                OnEffectHit += SpellEffectFn(spell_mage_cold_snap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_cold_snap_SpellScript();
        }
};

// 120 - Cone of Cold
/// Updated 4.3.4
class spell_mage_cone_of_cold : public SpellScriptLoader
{
    public:
        spell_mage_cone_of_cold() : SpellScriptLoader("spell_mage_cone_of_cold") { }

        class spell_mage_cone_of_cold_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cone_of_cold_SpellScript);

            void HandleConeOfColdScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    if (caster->HasAura(SPELL_MAGE_CONE_OF_COLD_AURA_R1)) // Improved Cone of Cold Rank 1
                        unitTarget->CastSpell(unitTarget, SPELL_MAGE_CONE_OF_COLD_TRIGGER_R1, true);
                    else if (caster->HasAura(SPELL_MAGE_CONE_OF_COLD_AURA_R2)) // Improved Cone of Cold Rank 2
                        unitTarget->CastSpell(unitTarget, SPELL_MAGE_CONE_OF_COLD_TRIGGER_R2, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_cone_of_cold_SpellScript::HandleConeOfColdScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_cone_of_cold_SpellScript();
        }
};

// 42955 Conjure Refreshment
/// Updated 5.1.0
struct ConjureRefreshmentData
{
    uint32 minLevel;
    uint32 maxLevel;
    uint32 spellId;
};

uint8 const MAX_CONJURE_REFRESHMENT_SPELLS = 8;
ConjureRefreshmentData const _conjureData[MAX_CONJURE_REFRESHMENT_SPELLS] =
{
    { 33, 43, 92739 },
    { 44, 53, 92799 },
    { 54, 63, 92802 },
    { 64, 73, 92805 },
    { 74, 79, 74625 },
    { 80, 84, 92822 },
    { 85, 89, 92727 },
	{ 90, 90, 116130},
};

// 42955 - Conjure Refreshment
class spell_mage_conjure_refreshment : public SpellScriptLoader
{
    public:
        spell_mage_conjure_refreshment() : SpellScriptLoader("spell_mage_conjure_refreshment") { }

        class spell_mage_conjure_refreshment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_conjure_refreshment_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_SPELLS; ++i)
                    if (!sSpellMgr->GetSpellInfo(_conjureData[i].spellId))
                        return false;
                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint8 level = GetHitUnit()->getLevel();
                for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_SPELLS; ++i)
                {
                    ConjureRefreshmentData const& spellData = _conjureData[i];
                    if (level < spellData.minLevel || level > spellData.maxLevel)
                        continue;
                    GetHitUnit()->CastSpell(GetHitUnit(), spellData.spellId);
                    break;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_conjure_refreshment_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_conjure_refreshment_SpellScript();
        }
};

// -543  - Fire Ward
// -6143 - Frost Ward
class spell_mage_fire_frost_ward : public SpellScriptLoader
{
    public:
        spell_mage_fire_frost_ward() : SpellScriptLoader("spell_mage_fire_frost_ward") { }

        class spell_mage_fire_frost_ward_AuraScript : public spell_mage_incanters_absorbtion_base_AuraScript
        {
            PrepareAuraScript(spell_mage_fire_frost_ward_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FROST_WARDING_TRIGGERED))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FROST_WARDING_R1))
                    return false;
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                canBeRecalculated = false;
                if (Unit* caster = GetCaster())
                {
                    // +80.68% from sp bonus
                    float bonus = 0.8068f;

                    bonus *= caster->SpellBaseHealingBonusDone(GetSpellInfo()->GetSchoolMask());
                    bonus *= caster->CalculateLevelPenalty(GetSpellInfo());

                    amount += int32(bonus);
                }
            }

            void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                if (AuraEffect* talentAurEff = target->GetAuraEffectOfRankedSpell(SPELL_MAGE_FROST_WARDING_R1, EFFECT_0))
                {
                    int32 chance = talentAurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue(); // SPELL_EFFECT_DUMMY with NO_TARGET

                    if (roll_chance_i(chance))
                    {
                        int32 bp = dmgInfo.GetDamage();
                        dmgInfo.AbsorbDamage(bp);
                        target->CastCustomSpell(target, SPELL_MAGE_FROST_WARDING_TRIGGERED, &bp, NULL, NULL, true, NULL, aurEff);
                        absorbAmount = 0;
                        PreventDefaultAction();
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_fire_frost_ward_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_fire_frost_ward_AuraScript::Absorb, EFFECT_0);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_fire_frost_ward_AuraScript::Trigger, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_fire_frost_ward_AuraScript();
        }
};

// 54646 - Focus Magic
class spell_mage_focus_magic : public SpellScriptLoader
{
    public:
        spell_mage_focus_magic() : SpellScriptLoader("spell_mage_focus_magic") { }

        class spell_mage_focus_magic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_focus_magic_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FOCUS_MAGIC_PROC))
                    return false;
                return true;
            }

            bool Load()
            {
                _procTarget = NULL;
                return true;
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                _procTarget = GetCaster();
                return _procTarget && _procTarget->isAlive();
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(_procTarget, SPELL_MAGE_FOCUS_MAGIC_PROC, true, NULL, aurEff);
            }

            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_focus_magic_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_focus_magic_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
            }

        private:
            Unit* _procTarget;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_focus_magic_AuraScript();
        }
};

// 44457 - Living Bomb
/// Updated 5.1.0
class spell_mage_living_bomb : public SpellScriptLoader
{
    public:
        spell_mage_living_bomb() : SpellScriptLoader("spell_mage_living_bomb") { }

        class spell_mage_living_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_living_bomb_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(44461))
                    return false;
                return true;
            }

			void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                canBeRecalculated = false;
                if (Unit* caster = GetCaster())
                {
                    amount = 0;
                    amount += int32(1072 + (caster->GetTotalSpellPowerValue(SPELL_SCHOOL_MASK_NORMAL, false)*(80.4f/100.0f)));
                }
            }

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode != AURA_REMOVE_BY_ENEMY_SPELL && removeMode != AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* caster = GetCaster())
                    caster->CastSpell(GetTarget(), uint32(aurEff->GetAmount()), true, NULL, aurEff);
            }
			
            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
				DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_living_bomb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
			}
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_living_bomb_AuraScript();
        }
};

// 11426 - Ice Barrier
/// Updated 5.1.0
class spell_mage_ice_barrier : public SpellScriptLoader
{
   public:
       spell_mage_ice_barrier() : SpellScriptLoader("spell_mage_ice_barrier") { }

       class spell_mage_ice_barrier_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_mage_ice_barrier_AuraScript);
			
		   void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
            {
				canBeRecalculated = false;
                if (Unit* caster = GetCaster())
                {
					amount = 0;
					amount += int32(4580 + (caster->GetTotalSpellPowerValue(SPELL_SCHOOL_MASK_NORMAL, false)*3.3f));
				}
            }

           void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
           {
               if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
                   return;

               if (GetTarget()->HasAura(SPELL_MAGE_SHATTERED_BARRIER_R1))
                   GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R1, true);
               else if (GetTarget()->HasAura(SPELL_MAGE_SHATTERED_BARRIER_R2))
                   GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_SHATTERED_BARRIER_FREEZE_R2, true);
           }
		   
           void Register()
           {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_ice_barrier_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
				DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_ice_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
           }
       };

       AuraScript* GetAuraScript() const
       {
           return new spell_mage_ice_barrier_AuraScript();
       }
};

// -11119 - Ignite
class spell_mage_ignite : public SpellScriptLoader
{
    public:
        spell_mage_ignite() : SpellScriptLoader("spell_mage_ignite") { }

        class spell_mage_ignite_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ignite_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_IGNITE))
                    return false;
                return true;
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                return eventInfo.GetProcTarget();
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                SpellInfo const* igniteDot = sSpellMgr->GetSpellInfo(SPELL_MAGE_IGNITE);
                int32 pct = 8 * GetSpellInfo()->GetRank();

                int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), pct) / igniteDot->GetMaxTicks());
                amount += eventInfo.GetProcTarget()->GetRemainingPeriodicAmount(eventInfo.GetActor()->GetGUID(), SPELL_MAGE_IGNITE, SPELL_AURA_PERIODIC_DAMAGE);
                GetTarget()->CastCustomSpell(SPELL_MAGE_IGNITE, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_ignite_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_ignite_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_ignite_AuraScript();
        }
};

// 543 - Mage Ward
/// Updated 4.3.4
class spell_mage_mage_ward : public SpellScriptLoader
{
   public:
       spell_mage_mage_ward() : SpellScriptLoader("spell_mage_mage_ward") { }

       class spell_mage_mage_ward_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_mage_mage_ward_AuraScript);

           void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
           {
               if (AuraEffect* aurEff = GetTarget()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_GENERIC, ICON_MAGE_INCANTER_S_ABSORPTION, EFFECT_0))
               {
                   int32 bp = CalculatePct(absorbAmount, aurEff->GetAmount());
                   GetTarget()->CastCustomSpell(GetTarget(), SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, NULL, NULL, true);
               }
           }

           void Register()
           {
               AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_mage_ward_AuraScript::HandleAbsorb, EFFECT_0);
           }
       };

       AuraScript* GetAuraScript() const
       {
           return new spell_mage_mage_ward_AuraScript();
       }
};

// 1463 - Mana Shield
/// Updated 4.3.4
class spell_mage_mana_shield : public SpellScriptLoader
{
    public:
       spell_mage_mana_shield() : SpellScriptLoader("spell_mage_mana_shield") { }

       class spell_mage_mana_shield_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_mage_mana_shield_AuraScript);

           void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
           {
               if (AuraEffect* aurEff = GetTarget()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_GENERIC, ICON_MAGE_INCANTER_S_ABSORPTION, EFFECT_0))
               {
                   int32 bp = CalculatePct(absorbAmount, aurEff->GetAmount());
                   GetTarget()->CastCustomSpell(GetTarget(), SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, NULL, NULL, true);
               }
           }

           void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
           {
               if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
                   GetTarget()->CastSpell(GetTarget(), SPELL_MAGE_INCANTERS_ABSORBTION_R1, true);
           }

           void Register()
           {
                AfterEffectManaShield += AuraEffectManaShieldFn(spell_mage_mana_shield_AuraScript::HandleAbsorb, EFFECT_0);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_mana_shield_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MANA_SHIELD, AURA_EFFECT_HANDLE_REAL);
           }
       };

       AuraScript* GetAuraScript() const
       {
           return new spell_mage_mana_shield_AuraScript();
       }
};

// -29074 - Master of Elements
class spell_mage_master_of_elements : public SpellScriptLoader
{
    public:
        spell_mage_master_of_elements() : SpellScriptLoader("spell_mage_master_of_elements") { }

        class spell_mage_master_of_elements_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_master_of_elements_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE))
                    return false;
                return true;
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                return eventInfo.GetDamageInfo()->GetSpellInfo(); // eventInfo.GetSpellInfo()
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                int32 mana = int32(eventInfo.GetDamageInfo()->GetSpellInfo()->CalcPowerCost(GetTarget(), eventInfo.GetDamageInfo()->GetSchoolMask()));
                mana = CalculatePct(mana, aurEff->GetAmount());

                if (mana > 0)
                    GetTarget()->CastCustomSpell(SPELL_MAGE_MASTER_OF_ELEMENTS_ENERGIZE, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, NULL, aurEff);
            }
			
            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_mage_master_of_elements_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_mage_master_of_elements_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_master_of_elements_AuraScript();
        }
};

enum SilvermoonPolymorph
{
    NPC_AUROSALIA       = 18744
};

// TODO: move out of here and rename - not a mage spell
class spell_mage_polymorph_cast_visual : public SpellScriptLoader
{
    public:
        spell_mage_polymorph_cast_visual() : SpellScriptLoader("spell_mage_polymorph_visual") { }

        class spell_mage_polymorph_cast_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_polymorph_cast_visual_SpellScript);

            static const uint32 PolymorhForms[6];

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                // check if spell ids exist in dbc
                for (uint32 i = 0; i < 6; i++)
                    if (!sSpellMgr->GetSpellInfo(PolymorhForms[i]))
                        return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetCaster()->FindNearestCreature(NPC_AUROSALIA, 30.0f))
                    if (target->GetTypeId() == TYPEID_UNIT)
                        target->CastSpell(target, PolymorhForms[urand(0, 5)], true);
            }

            void Register()
            {
                // add dummy effect spell handler to Polymorph visual
                OnEffectHitTarget += SpellEffectFn(spell_mage_polymorph_cast_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_polymorph_cast_visual_SpellScript();
        }
};

uint32 const spell_mage_polymorph_cast_visual::spell_mage_polymorph_cast_visual_SpellScript::PolymorhForms[6] =
{
    SPELL_MAGE_SQUIRREL_FORM,
    SPELL_MAGE_GIRAFFE_FORM,
    SPELL_MAGE_SERPENT_FORM,
    SPELL_MAGE_DRAGONHAWK_FORM,
    SPELL_MAGE_WORGEN_FORM,
    SPELL_MAGE_SHEEP_FORM
};

// 5405  - Replenish Mana (Mana Gem)
/// Updated 4.3.4
class spell_mage_replenish_mana : public SpellScriptLoader
{
   public:
       spell_mage_replenish_mana() : SpellScriptLoader("spell_mage_replenish_mana") { }

       class spell_mage_replenish_mana_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_replenish_mana_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/)
           {
               if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_IMPROVED_MANA_GEM_TRIGGERED))
                   return false;
               return true;
           }

           void HandleImprovedManaGem()
           {
               if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_MANA_GEM, EFFECT_0))
               {
                   int32 bp = CalculatePct(GetCaster()->GetMaxPower(POWER_MANA), aurEff->GetAmount());
                   GetCaster()->CastCustomSpell(GetCaster(), SPELL_MAGE_IMPROVED_MANA_GEM_TRIGGERED, &bp, &bp, NULL, true);
               }
           }

           void Register()
           {
               AfterCast += SpellCastFn(spell_mage_replenish_mana_SpellScript::HandleImprovedManaGem);
           }
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_mage_replenish_mana_SpellScript();
       }
};

// 31687 - Summon Water Elemental
class spell_mage_summon_water_elemental : public SpellScriptLoader
{
    public:
        spell_mage_summon_water_elemental() : SpellScriptLoader("spell_mage_summon_water_elemental") { }

        class spell_mage_summon_water_elemental_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_summon_water_elemental_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_GLYPH_OF_ETERNAL_WATER) || !sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY) || !sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();

                if (Player* player = caster->ToPlayer())
                    if (Guardian* elemental = player->GetGuardianPet())
                        // Check if the pet we are going to unsummon is the mage's water elemental
                        if (elemental->GetEntry() == uint32(sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY)->Effects[EFFECT_0].MiscValue) ||
                            elemental->GetEntry() == uint32(sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT)->Effects[EFFECT_0].MiscValue))
                            elemental->UnSummon();

                // Glyph of Eternal Water
                if (caster->HasAura(SPELL_MAGE_GLYPH_OF_ETERNAL_WATER))
                    caster->CastSpell(caster, SPELL_MAGE_SUMMON_WATER_ELEMENTAL_PERMANENT, true);
                else
                    caster->CastSpell(caster, SPELL_MAGE_SUMMON_WATER_ELEMENTAL_TEMPORARY, true);
            }

            void Register()
            {
                // add dummy effect spell handler to Summon Water Elemental
                OnEffectHit += SpellEffectFn(spell_mage_summon_water_elemental_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_summon_water_elemental_SpellScript();
        }
};

// 33395 Water Elemental's Freeze
/// Updated 4.3.4
class spell_mage_water_elemental_freeze : public SpellScriptLoader
{
   public:
       spell_mage_water_elemental_freeze() : SpellScriptLoader("spell_mage_water_elemental_freeze") { }

       class spell_mage_water_elemental_freeze_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_water_elemental_freeze_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/)
           {
               if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FINGERS_OF_FROST))
                   return false;
               return true;
           }

           void CountTargets(std::list<WorldObject*>& targetList)
           {
               _didHit = targetList.size();
           }

           void HandleImprovedFreeze()
           {
               if (!_didHit)
                   return;

               Unit* owner = GetCaster()->GetOwner();
               if (!owner)
                   return;

				Player* player = owner->ToPlayer();
				if(!player)
				   return;

				if(_didHit == 1)
				{
					player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST, true);

					if (Aura* finger = player->GetAura(SPELL_MAGE_FINGERS_OF_FROST))
						if(finger->GetStackAmount() == 2)
							player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST_2, true);
				}
				else
				{
					player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST, true);
					player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST, true);
					player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST_2, true);
				}
           }

           void Register()
           {
               OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_water_elemental_freeze_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
               AfterCast += SpellCastFn(spell_mage_water_elemental_freeze_SpellScript::HandleImprovedFreeze);
           }

       private:
           int32 _didHit;
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_mage_water_elemental_freeze_SpellScript();
       }
};

/* 
EXECUTE THOSE SQL QUERIES: 

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (113724, 'spell_mage_ring_of_frost'); 
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (82691, 'spell_mage_ring_of_frost_freeze'); 
*/

// 113724 - Ring of Frost
class spell_mage_ring_of_frost : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost() : SpellScriptLoader("spell_mage_ring_of_frost") { }

        class spell_mage_ring_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ring_of_frost_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) 
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_FREEZE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY))
                    return false;
                return true;
            }

            bool Load() 
            {
                ringOfFrost = NULL;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (ringOfFrost)
                    if (GetMaxDuration() - (int32)ringOfFrost->GetTimer() >= sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY)->GetDuration())
                        GetTarget()->CastSpell(ringOfFrost->GetPositionX(), ringOfFrost->GetPositionY(), ringOfFrost->GetPositionZ(), SPELL_MAGE_RING_OF_FROST_FREEZE, true);
            }

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                std::list<Creature*> MinionList;
                GetTarget()->GetAllMinionsByEntry(MinionList, GetSpellInfo()->Effects[EFFECT_0].MiscValue);

                for (std::list<Creature*>::iterator itr = MinionList.begin(); itr != MinionList.end(); itr++)
                {
                    TempSummon* summon = (*itr)->ToTempSummon();

                    if (ringOfFrost && summon)
                    {
                        if (summon->GetTimer() > ringOfFrost->GetTimer())
                        {
                            ringOfFrost->DespawnOrUnsummon();
                            ringOfFrost = summon;
                        }
                        else
                            summon->DespawnOrUnsummon();
                    }
                    else if (summon)
                        ringOfFrost = summon;
                }
            }

            TempSummon* ringOfFrost;

            void Register() 
            {
                 OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ring_of_frost_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                 OnEffectApply += AuraEffectApplyFn(spell_mage_ring_of_frost_AuraScript::Apply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const 
        {
            return new spell_mage_ring_of_frost_AuraScript();
        }
};

// 82691 - Ring of Frost
class spell_mage_ring_of_frost_freeze : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost_freeze() : SpellScriptLoader("spell_mage_ring_of_frost_freeze") { }

        class spell_mage_ring_of_frost_freeze_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_ring_of_frost_freeze_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) 
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_FREEZE))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                float outRadius = sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON)->Effects[EFFECT_0].CalcRadius();
                float inRadius  = 4.7f;

                for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if (Unit* unit = (*itr)->ToUnit())
                        if (unit->HasAura(SPELL_MAGE_RING_OF_FROST_DUMMY) || unit->HasAura(SPELL_MAGE_RING_OF_FROST_FREEZE) || unit->GetExactDist(GetExplTargetDest()) > outRadius || unit->GetExactDist(GetExplTargetDest()) < inRadius)
                            targets.erase(itr--);
            }

            void Register() 
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_ring_of_frost_freeze_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const 
        {
            return new spell_mage_ring_of_frost_freeze_SpellScript();
        }

        class spell_mage_ring_of_frost_freeze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ring_of_frost_freeze_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) 
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    if (GetCaster())
                        GetCaster()->CastSpell(GetTarget(), SPELL_MAGE_RING_OF_FROST_DUMMY, true);
            }

            void Register() 
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_ring_of_frost_freeze_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const 
        {
            return new spell_mage_ring_of_frost_freeze_AuraScript();
        }
};

// 116  - Frost Bolt
class spell_mage_frostbolt : public SpellScriptLoader
{
   public:
       spell_mage_frostbolt() : SpellScriptLoader("spell_mage_frostbolt") { }

       class spell_mage_frostbolt_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_frostbolt_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/)
           {
               if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FROSTBOLT_HEAL))
                   return false;
               return true;
           }

		   SpellCastResult CheckRequirement()
           {
                if (!GetExplTargetUnit() || (GetExplTargetUnit() == GetCaster()))
                    return SPELL_FAILED_BAD_TARGETS;

				if(GetExplTargetUnit()->IsFriendlyTo(GetCaster()))
				{
					if(GetExplTargetUnit()->GetOwner() && GetExplTargetUnit()->GetOwner() == GetCaster())
						return SPELL_CAST_OK;
					else
						return SPELL_FAILED_BAD_TARGETS;
				}

                return SPELL_CAST_OK;
           }

		   void HandleBeforeHit()
           {
                if(GetExplTargetUnit()->IsFriendlyTo(GetCaster()))
				{
					if(GetExplTargetUnit()->GetOwner() && GetExplTargetUnit()->GetOwner() == GetCaster())
					{
						PreventHitDamage();
						PreventHitAura();
						PreventHitDefaultEffect(EFFECT_1);
						GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_MAGE_FROSTBOLT_HEAL, true);
					}
				}	
           }

           void Register()
           {
			   OnCheckCast += SpellCheckCastFn(spell_mage_frostbolt_SpellScript::CheckRequirement);
			   BeforeHit += SpellHitFn(spell_mage_frostbolt_SpellScript::HandleBeforeHit);
           }
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_mage_frostbolt_SpellScript();
       }
};

// 30455  - Ice lance
class spell_mage_ice_lance : public SpellScriptLoader
{
   public:
       spell_mage_ice_lance() : SpellScriptLoader("spell_mage_ice_lance") { }

       class spell_mage_ice_lance_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_ice_lance_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/)
           {
               return true;
           }

		   void RecalculateDamage(SpellEffIndex /*effIndex*/)
           {
				if (GetHitUnit() && (GetHitUnit()->HasAuraState(AURA_STATE_FROZEN)))
					SetHitDamage(GetHitDamage()*(GetCaster()->HasAura(SPELL_MAGE_FINGERS_OF_FROST) ? 5 : 4));
				else if(GetCaster()->HasAura(SPELL_MAGE_FINGERS_OF_FROST))
				{
					SetHitDamage(GetHitDamage()*5);

					if (Player* player = GetCaster()->ToPlayer())
					{
						if (Aura* finger = player->GetAura(SPELL_MAGE_FINGERS_OF_FROST))
						{
							if(finger->GetStackAmount() == 2)
							{
								player->RemoveAura(SPELL_MAGE_FINGERS_OF_FROST_2);
								finger->SetStackAmount(1);
							}
							else if(finger->GetStackAmount() == 1)
								player->RemoveAura(SPELL_MAGE_FINGERS_OF_FROST);
						}
					}
				}
           }


           void Register()
           {
			   OnEffectHitTarget += SpellEffectFn(spell_mage_ice_lance_SpellScript::RecalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
           }
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_mage_ice_lance_SpellScript();
       }
};

// Finger of Frost
class spell_mage_finger_of_frost : public SpellScriptLoader
{
    public:
        spell_mage_finger_of_frost() : SpellScriptLoader("spell_mage_finger_of_frost") { }

        class spell_mage_finger_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_finger_of_frost_AuraScript);

            void OnProcHandler(ProcEventInfo& eventInfo)
            {
                if (!GetOwner())
                    return;

				if (Player* player = GetOwner()->ToPlayer())
                {

					if(!player->HasSpell(SPELL_MAGE_FINGERS_OF_FROST_PASSIVE))
						return;

					uint32 spellId = eventInfo.GetDamageInfo()->GetSpellInfo()->Id;
					uint32 chance = 0;

					switch(spellId)
					{
					case 116:
					case 44614:
						chance = 15;
						break;
					case 10:
						chance = 5;
						break;
					case 2948:
						chance = 10;
						break;
					}

					if(chance > urand(0, 99))
					{
						player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST, true);

						if (Aura* finger = player->GetAura(SPELL_MAGE_FINGERS_OF_FROST))
							if(finger->GetStackAmount() == 2)
								player->CastSpell(player, SPELL_MAGE_FINGERS_OF_FROST_2, true);
					}
                }
            }

            void Register()
            {
				OnProc += AuraProcFn(spell_mage_finger_of_frost_AuraScript::OnProcHandler);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_finger_of_frost_AuraScript();
        }
};

// Brain Freeze
class spell_mage_brain_freeze : public SpellScriptLoader
{
    public:
        spell_mage_brain_freeze() : SpellScriptLoader("spell_mage_brain_freeze") { }

        class spell_mage_brain_freeze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_brain_freeze_AuraScript);

            void OnProcHandler(ProcEventInfo& eventInfo)
            {
                if (!GetOwner())
                    return;

				if (Player* player = GetOwner()->ToPlayer())
					player->CastSpell(player, SPELL_MAGE_BRAIN_FREEZE_BUFF, true);
            }

            void Register()
            {
				OnProc += AuraProcFn(spell_mage_brain_freeze_AuraScript::OnProcHandler);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_brain_freeze_AuraScript();
        }
};

// 44572  - Deep freeze
class spell_mage_deep_freeze : public SpellScriptLoader
{
   public:
       spell_mage_deep_freeze() : SpellScriptLoader("spell_mage_deep_freeze") { }

       class spell_mage_deep_freeze_SpellScript : public SpellScript
       {
           PrepareSpellScript(spell_mage_deep_freeze_SpellScript);

           bool Validate(SpellInfo const* /*spellInfo*/)
           {
               return true;
           }

		   void HandleFingerOfFrost(SpellEffIndex /*effIndex*/)
           {
				if (Player* player = GetCaster()->ToPlayer())
                {
					if (Aura* finger = player->GetAura(SPELL_MAGE_FINGERS_OF_FROST))
					{
						if(finger->GetStackAmount() == 2)
						{
							player->RemoveAura(SPELL_MAGE_FINGERS_OF_FROST_2);
							finger->SetStackAmount(1);
						}
						else if(finger->GetStackAmount() == 1)
							player->RemoveAura(SPELL_MAGE_FINGERS_OF_FROST);
					}
				}
           }

           void Register()
           {
			   OnEffectHitTarget += SpellEffectFn(spell_mage_deep_freeze_SpellScript::HandleFingerOfFrost, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
           }
       };

       SpellScript* GetSpellScript() const
       {
           return new spell_mage_deep_freeze_SpellScript();
       }
};

class spell_mage_alter_time : public SpellScriptLoader
{
public :
    spell_mage_alter_time() : SpellScriptLoader("spell_mage_alter_time")
    {

    }

    class spell_mage_alter_time_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_alter_time_AuraScript)

        bool Validate(const SpellInfo *spellInfo)
        {
            if(!sSpellMgr->GetSpellInfo(SPELL_MAGE_ALTER_TIME_AURA)
                    || !sSpellMgr->GetSpellInfo(SPELL_MAGE_ALTER_TIME_RESET)
                    || !sSpellMgr->GetSpellInfo(SPELL_MAGE_ALTER_TIME_BASE))
                return false ;

            return true ;
        }

        bool Load()
        {
            if(GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER)
            {
                p_caster = GetCaster()->ToPlayer() ;
                return true ;
            }

            return false ;
        }

        void handleSavePlayerOnEffectApply(AuraEffect const* auraEff, AuraEffectHandleModes mode)
        {
			if(!p_caster)
			{
				if(GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER)
					p_caster = GetCaster()->ToPlayer();
				else
					return ;
			}

			if(p_caster)
			{
				m_uiHealth = p_caster->GetHealth() ;
				m_uiMana = p_caster->GetPower(POWER_MANA);
				WorldLocation temp(p_caster->GetMapId(), p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ() + 1, p_caster->GetOrientation());
				m_savedPos = temp ;
				
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Applying Alter Time ; saving player");
				p_caster->SavePlayerOnAlterTimeApply();
			}
		}

		void handleResetPlayerOnEffectRemove(AuraEffect const* auraEff, AuraEffectHandleModes mode)
		{
			if(!p_caster)
				return ;

			if(p_caster)
			{
				p_caster->SetHealth(m_uiHealth);
				p_caster->SetPower(POWER_MANA, int32(m_uiMana));
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Removing Alter Time ; player teleporting");
				p_caster->TeleportTo(m_savedPos, 0);
				
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Removing Alter Time ; auras resetting");
				if(GetCaster())
				{
					sLog->outDebug(LOG_FILTER_NETWORKIO, "Caster as Unit is not null");
					if(GetCaster()->ToPlayer())
					{
						sLog->outDebug(LOG_FILTER_NETWORKIO, "Caster as player is not null ; resetting !");
						GetCaster()->ToPlayer()->ResetPlayerOnAlterTimeExpire();
					}
				}
			}
		}

		void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_alter_time_AuraScript::handleSavePlayerOnEffectApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_alter_time_AuraScript::handleResetPlayerOnEffectRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }

        Player* p_caster ;
        uint32 m_uiHealth, m_uiMana ;
        WorldLocation m_savedPos ;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_mage_alter_time_AuraScript();
    }
};

class spell_mage_alter_time_triggerer : public SpellScriptLoader
{
public :
	spell_mage_alter_time_triggerer() : SpellScriptLoader("spell_mage_alter_time_triggerer")
	{

	}

	class spell_mage_alter_time_triggerer_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_mage_alter_time_triggerer_SpellScript);

		bool Validate(SpellInfo const* spellInfo)
		{
			return true ;
		}

		bool Load()
		{
			return true ;
		}

		void handleRemoveAuraOnEffectDummy(SpellEffIndex effectIndex)
		{
			if(GetCaster() && GetCaster()->ToPlayer() && GetCaster()->HasAura(SPELL_MAGE_ALTER_TIME_AURA))
				GetCaster()->RemoveAura(SPELL_MAGE_ALTER_TIME_AURA);
		}

		void Register()
		{
			OnEffectHitTarget += SpellEffectFn(spell_mage_alter_time_triggerer_SpellScript::handleRemoveAuraOnEffectDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_mage_alter_time_triggerer_SpellScript();
	}
};

class spell_mage_combustion : public SpellScriptLoader
{
public :
	spell_mage_combustion() : SpellScriptLoader("spell_mage_combustion")
	{

	}

	class spell_mage_combustion_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_mage_combustion_SpellScript);

		bool Validate(SpellInfo const* spellInfo)
		{
			return true ;
		}

		bool Load()
		{
			return true ;
		}

		void handleResetInfernoBlastCooldownOnCast()
		{
			sLog->outDebug(LOG_FILTER_NETWORKIO, "Entering Combusion OnCast Handler");
			if(GetCaster())
			{
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Combustion's Caster as unit is not null");
				if(Player* p = GetCaster()->ToPlayer())
				{
					sLog->outDebug(LOG_FILTER_NETWORKIO, "Combustion's Caster as player is not null ; resetting cooldown");
					SpellCooldowns cooldowns = p->GetSpellCooldownMap();
                    for(SpellCooldowns::iterator iter = cooldowns.begin() ; iter != cooldowns.end() ; ++iter)
                    {
                        SpellInfo const* spell = sSpellMgr->GetSpellInfo(iter->first);
                        if(!spell)
                            continue ;
                        sLog->outDebug(LOG_FILTER_NETWORKIO, "Player %s (guid : %u) has spell cooldown on spell %s (id : %u)", p->GetName().c_str(), p->GetGUIDLow(), spell->SpellName, spell->Id);
                    }
					p->RemoveSpellCooldown(108853, true);
					/*p->RemoveAllSpellCooldown();*/ // This test has proved that event this way, the cooldown is not reset => WHY ? 
				}
			}
		}

		void handleMiscOnEffectScriptEffect(SpellEffIndex effectIndex)
		{
			sLog->outDebug(LOG_FILTER_NETWORKIO, "Entering Combusion OnEffectHitTarget Handler");
			if(Unit* target = GetExplTargetUnit())
			{
				// Combusion impact = stun
				if(GetCaster())
				{
					sLog->outDebug(LOG_FILTER_NETWORKIO, "Combustion's Target not null ; ready to impact");
					GetCaster()->CastSpell(target, SPELL_MAGE_COMBUSTION_IMPACT);
				}

				// Periodic damages
				if(Aura* ignite = target->GetAura(SPELL_MAGE_IGNITE))
				{
					sLog->outDebug(LOG_FILTER_NETWORKIO, "Combustion's Target has Ignite aura ; ready to apply the dot");
					if(GetCaster() && target)
					{
						GetCaster()->CastSpell(target, SPELL_MAGE_COMBUSTION_DOT) ;
						Aura* combustionDot = target->GetAura(SPELL_MAGE_COMBUSTION_DOT) ;
						if(combustionDot && ignite)
						{
							sLog->outDebug(LOG_FILTER_NETWORKIO, "Applying the dot on Combusion target");
							combustionDot->GetEffect(0)->SetAmount(ignite->GetEffect(0)->GetAmount()) ;
						}
					}
				}
			}
		}

		void Register()
		{
			OnCast += SpellCastFn(spell_mage_combustion_SpellScript::handleResetInfernoBlastCooldownOnCast);
			OnEffectHitTarget += SpellEffectFn(spell_mage_combustion_SpellScript::handleMiscOnEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_mage_combustion_SpellScript();
	}
};

class spell_mage_inferno_blast : public SpellScriptLoader
{
public :
    spell_mage_inferno_blast() : SpellScriptLoader("spell_mage_inferno_blast")
    {

    }

    class spell_mage_inferno_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_inferno_blast_SpellScript)

        bool Validate(const SpellInfo *spellInfo)
        {
            return true ;
        }

        bool Load()
        {
            return true ;
        }

        void handleSpreadOnHit()
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast : OnHit Handler");
            if(GetExplTargetUnit() && GetCaster() && GetCaster()->ToPlayer())
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast : ExplicitTarget found (name %s, guid %u) ; casting Inferno Blast !", GetExplTargetUnit()->GetName().c_str(), GetExplTargetUnit()->GetGUID());
                GetCaster()->CastSpell(GetExplTargetUnit(), 118280, TRIGGERED_FULL_MASK);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_mage_inferno_blast_SpellScript::handleSpreadOnHit);
        }
    };

	SpellScript* GetSpellScript() const
	{
		return new spell_mage_inferno_blast_SpellScript();
	}
};

class spell_mage_inferno_blast_spreader : public SpellScriptLoader
{
public :
    spell_mage_inferno_blast_spreader() : SpellScriptLoader("spell_mage_inferno_blast_spreader")
    {

    }

    class spell_mage_inferno_blast_spreader_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_inferno_blast_spreader_SpellScript);

        bool Validate(const SpellInfo *spellInfo)
        {
            return true ;
        }

        bool Load()
        {
            return true ;
        }

        void handleTargetSelect(std::list<WorldObject*>& targets)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : OnObjectAreaTargetSelect Handler");
            if(Unit* caster = GetCaster())
            {
                Unit* realTarget = GetExplTargetUnit();
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : Checking real target");
                if(!realTarget)
                    return ;

                Position pos ;
                caster->GetPosition(&pos);

				sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : real target found (name %s, guid %u) ; position set ; checking glyph", realTarget->GetName().c_str(), realTarget->GetGUID());
                // Glyph of Discret Magic : target is valid only if less than 5 yards away from the PRIMARY TARGET
                if(caster->HasAura(134580))
                {
                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : caster has glyph ; checking availables targets");
                    for(std::list<WorldObject*>::iterator iter = targets.begin() ; iter != targets.end() ; ++iter)
                    {
                        Position targetPos ;
                        if(*iter)
                            (*iter)->GetPosition(&targetPos);
                        else
                            continue ;

                        if(pos.GetExactDist2d(&targetPos) > 5.0f)
                            targets.remove(*iter);
                    }
                }

                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : glyph test done ; resizing list (actual size : %u)", (uint32)targets.size());
                // And we resize. Even if there is already nothing.
                Trinity::Containers::RandomResizeList(targets, caster->HasAura(89926) ? 5 : 4);
            }
        }

        void handleSpreadAurasOnEffectScriptEffect(SpellEffIndex effectIndex)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : OnEffectHitTarget Handler");
            Unit* caster = GetCaster(); // Pure caster of the spell
            Unit* realTarget = GetExplTargetUnit(); // The target the mage is selecting
            Unit* otherTarget = GetHitUnit(); // The target on which the effects will be spread (this is shit)

            if(caster && realTarget && otherTarget)
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : pointers corrects");
                if(realTarget->GetGUID() == otherTarget->GetGUID()) // This would be tricky
                    return ;

                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : guids corrects (realTarget : %u ; otherTarget : %u)", realTarget->GetGUID(), otherTarget->GetGUID());
				Unit::AuraApplicationMap auras = realTarget->GetAppliedAuras(); // To find the auras to duplicate
                for(Unit::AuraApplicationMap::iterator iter = auras.begin() ; iter != auras.end() ; ++iter)
                {
					sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : looping");
					if(!iter->second)
						continue;

					if(Aura* actualAura = iter->second->GetBase())
                    {
						sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : found an aura (%s, %u) ; casterGUID = %u", actualAura->GetSpellInfo()->SpellName, actualAura->GetId(), actualAura->GetCasterGUID());
                        if(actualAura->GetCaster() == caster)
                        {
                            sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : aura (%s, %u) found on realTarget", actualAura->GetSpellInfo()->SpellName, actualAura->GetId());
                            Aura* copy ;
                            switch(actualAura->GetId())
                            {
                            case 12654 : case 11366 : case 44457 : case 118271 :
                                // Well, thanks Pexirn. A LOT !
                                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : good aura (%s, %u) found on realTarget", actualAura->GetSpellInfo()->SpellName, actualAura->GetId());
                                copy = Aura::TryCreate(actualAura->GetSpellInfo(), actualAura->GetEffectMask(), otherTarget, caster, NULL, NULL, actualAura->GetCasterGUID());
								break ;
                            default :
                                break ;
                            }

                            if(copy)
                            {
                                // Thanks Pexirn. Again.
                                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : copy aura has been created ; setting target in combat if it wasn't");
                                if(!otherTarget->isInCombat())
                                    otherTarget->SetInCombatWith(caster);

                                // One last time : THANKS PEXIRN ! (not fot the headache you bitch)
                                sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : enterring the twisted part (well, i hope not in fact)");
                                if(AuraApplication* auraApp = copy->GetApplicationOfTarget(caster->GetGUID()))
                                {
                                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Inferno Blast Spreader : we entered the twisted part. Good Lord, please, HEEEEEEEEEELP ME !");
                                    copy->_ApplyForTarget(otherTarget, caster, auraApp);
                                }
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_inferno_blast_spreader_SpellScript::handleTargetSelect, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_mage_inferno_blast_spreader_SpellScript::handleSpreadAurasOnEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_inferno_blast_spreader_SpellScript();
    }
};

class spell_mage_evocation : public SpellScriptLoader
{
public :
	spell_mage_evocation() : SpellScriptLoader("spell_mage_evocation")
	{

	}

	/*class spell_mage_evocation_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_mage_evocation_SpellScript);

		bool Validate(const SpellInfo* spellInfo)
		{
			return true ;
		}

		bool Load()
		{
			return true ;
		}

		void handleInstantManaRegenOnEffectApplyAura(SpellEffIndex effectIndex)
		{
			sLog->outDebug(LOG_FILTER_NETWORKIO, "Evocation : Entering OnEffectHitTarget (effectIndex = 2) Handler");
			if(GetCaster() && GetCaster()->ToPlayer())
			{
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Evocation : Caster is not null and is a player ; calculating 15% of totalMana ; totalMana = %u", GetCaster()->GetMaxPower(POWER_MANA));
				int32 totalMana = GetCaster()->GetMaxPower(POWER_MANA);
				totalMana /= 100 ;
				totalMana *= 15 ;
				
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Evocation : amount calculated : expecting %u mana points", totalMana);
				GetCaster()->ModifyPower(POWER_MANA, totalMana);
			}
		}

		void Register()
		{
			OnEffectHitTarget += SpellEffectFn(spell_mage_evocation_SpellScript::handleHealOnEffectApplyAura, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
		}
	};*/

	class spell_mage_evocation_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_mage_evocation_AuraScript);
		
		bool Validate(const SpellInfo* spellInfo)
		{
			return true ;
		}

		bool Load()
		{
			return true ;
		}

		void handleHealOnEffectApplyAura(AuraEffect const* auraEff)
		{
			sLog->outDebug(LOG_FILTER_NETWORKIO, "Evocation (Aura) : Entering OnEffectHitTarget (effectIndex = 1) Handler");
			// Do not apply heal if player hasn't the glyph
			if(GetCaster() && GetCaster()->ToPlayer())
			{
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Evocation (Aura) : caster is not null and is a player");
				if(!GetCaster()->HasAura(56380))
				{
					sLog->outDebug(LOG_FILTER_NETWORKIO, "Evocation (Aura) : caster doesn't have the glyph ; preventing default effect (apply aura : heal)");
					PreventDefaultAction();
				}
			}
		}

		void Register()
		{
			OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_evocation_AuraScript::handleHealOnEffectApplyAura, EFFECT_1, SPELL_AURA_OBS_MOD_HEALTH);
		}
	};

	/*SpellScript* GetSpellScript() const
	{
		return new spell_mage_evocation_SpellScript();
	}*/

	AuraScript* GetAuraScript() const
	{
		return new spell_mage_evocation_AuraScript();
	}
};

class spell_mage_time_warp : public SpellScriptLoader
{
public :
	spell_mage_time_warp() : SpellScriptLoader("spell_mage_time_warp")
	{

	}

	class spell_mage_time_warp_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_mage_time_warp_SpellScript);

		bool Validate(const SpellInfo* spellInfo)
		{
			return true ;
		}

		bool Load()
		{
			return true ;
		}

		void filterTargets(std::list<WorldObject*>& targets)
		{
			sLog->outDebug(LOG_FILTER_NETWORKIO, "Time Warp : Entering OnObjectAreaTargetSelect handler ; number of targets : %u", uint32(targets.size()));

			targets.remove_if(Trinity::UnitAuraCheck(true, 57723));
			targets.remove_if(Trinity::UnitAuraCheck(true, 57724));
			targets.remove_if(Trinity::UnitAuraCheck(true, 80354));
		}

		void handleApplyAuraAfterHit()
		{
			sLog->outDebug(LOG_FILTER_NETWORKIO, "Time Warp : Entering AfterHit Handler");
			if(GetHitUnit())
			{
				sLog->outDebug(LOG_FILTER_NETWORKIO, "Time Warp : GetHitUnit() not null ; applying aura");
				GetHitUnit()->CastSpell(GetHitUnit(), 80354, true);
			}
		}

		void Register()
		{
			OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::filterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
			OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::filterTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
			OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::filterTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
			AfterHit += SpellHitFn(spell_mage_time_warp_SpellScript::handleApplyAuraAfterHit);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_mage_time_warp_SpellScript();
	}
};

void AddSC_mage_spell_scripts()
{
    new spell_mage_blast_wave();
    new spell_mage_blizzard();
    new spell_mage_burnout();
    new spell_mage_cold_snap();
    new spell_mage_cone_of_cold();
    new spell_mage_conjure_refreshment();
    new spell_mage_fire_frost_ward();
    new spell_mage_focus_magic();
    new spell_mage_frostbolt();
    new spell_mage_ice_barrier();
    new spell_mage_ignite();
    new spell_mage_living_bomb();
    new spell_mage_mage_ward();
    new spell_mage_mana_shield();
    new spell_mage_master_of_elements();
    new spell_mage_polymorph_cast_visual();
    new spell_mage_replenish_mana();
    new spell_mage_summon_water_elemental();
    new spell_mage_water_elemental_freeze();
	new spell_mage_ring_of_frost();
	new spell_mage_ring_of_frost_freeze();
	new spell_mage_ice_lance();
	new spell_mage_finger_of_frost();
	new spell_mage_deep_freeze();
	new spell_mage_alter_time();
	new spell_mage_alter_time_triggerer();
	new spell_mage_combustion();
	new spell_mage_inferno_blast();
	new spell_mage_inferno_blast_spreader();
	new spell_mage_evocation();
	new spell_mage_time_warp();
	new spell_mage_brain_freeze();
}
