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
 * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warr_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum WarriorSpells
{
    WARRIOR_SPELL_STORM_BOLT_STUN					= 132169,
    SPELL_WARRIOR_BLOODTHIRST						= 23881,
    SPELL_WARRIOR_CHARGE                            = 7922,
	SPELL_WARRIOR_CHARGE_TALENT						= 105771,
	SPELL_WARRIOR_CHARGE_TALENT_PASSIVE				= 103828,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_1                = 12162,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_2                = 12850,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_3                = 12868,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC         = 12721,
    SPELL_WARRIOR_EXECUTE                           = 5308,
    SPELL_WARRIOR_GLYPH_OF_EXECUTION                = 58367,
    SPELL_WARRIOR_GLYPH_OF_VIGILANCE                = 63326,
    SPELL_WARRIOR_SLAM                              = 50782,
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK     = 26654,
    SPELL_WARRIOR_TAUNT                             = 355,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1        = 46859,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2        = 46860,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1     = 64849,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2     = 64850,
    SPELL_WARRIOR_VIGILANCE_PROC                    = 50725,
    SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT         = 59665,
	SPELL_WARRIOR_RALLYING_CRY_TRIGGERED            = 97463,
	SPELL_WARRIOR_PHYSICAL_VULNERABILITY			= 81326,
	SPELL_WARRIOR_IMPENDING_VICTORY					= 118340,
	SPELL_WARRIOR_HEROIC_THROW						= 57755,
	SPELL_WARRIOR_WILD_STRIKE						= 100130,
	SPELL_WARRIOR_HEROIC_STRIKE						= 78,
	SPELL_WARRIOR_RAGING_BLOW_MAIN					= 96103,
	SPELL_WARRIOR_RAGING_BLOW_OFF					= 85384,
	SPELL_WARRIOR_ENRAGE							= 12880,
	SPELL_WARRIOR_STORM_BOLT						= 107570,
	SPELL_RAGING_BLOW_STACKS						= 131116,
    SPELL_PALADIN_BLESSING_OF_SANCTUARY             = 20911,
    SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY     = 25899,
    SPELL_PRIEST_RENEWED_HOPE                       = 63944,
    SPELL_GEN_DAMAGE_REDUCTION_AURA                 = 68066,
	SPELL_WARRIOR_HEROIC_LEAP_DAMAGE				= 52174,
	SPELL_WARRIOR_HEROIC_LEAP_SPEED					= 133278,
	SPELL_WARRIOR_ITEM_PVP_SET_4P_BONUS				= 133277,
	SPELL_WARRIOR_SECOND_WIND						= 29838,
	SPELL_WARRIOR_SECOND_WIND_REGEN					= 16491,
	SPELL_WARRIOR_SECOND_WIND_ICON					= 125667,
	SPELL_WARRIOR_BLOODTHIRDT_HEAL					= 117313,
	SPELL_WARRIOR_THUNDER_CLAP                      = 6343,
	SPELL_WARRIOR_DEEP_WOUNDS                       = 115767,
	SPELL_WARRIOR_BLOOD_AND_THUNDER                 = 84615,
	SPELL_WARRIOR_WEAKENED_BLOWS                    = 115798,
	SPELL_WARRIOR_GLYPH_OF_MORTAL_STRIKE            = 58368,
	SPELL_WARRIOR_MORTAL_STRIKE_AURA                = 12294,
	SPELL_WARRIOR_TASTE_FOR_BLOOD                   = 56638,
	SPELL_WARRIOR_ALLOW_OVERPOWER                   = 119962,
	SPELL_WARRIOR_SWORD_AND_BOARD                   = 50227,
	SPELL_WARRIOR_SHIELD_SLAM                       = 23922,
	SPELL_WARRIOR_COLOSSUS_SMASH                    = 86346,
	SPELL_WARRIOR_TASTE_FOR_BLOOD_DAMAGE_DONE       = 125831,
	SPELL_WARRIOR_DRAGON_ROAR_KNOCK_BACK            = 118895,
	SPELL_WARRIOR_MEAT_CLEAVER_PROC                 = 85739,
	SPELL_WARRIOR_GLYPH_OF_HINDERING_STRIKES        = 58366,
	SPELL_WARRIOR_SLUGGISH                          = 129923,
	SPELL_WARRIOR_SHOCKWAVE							= 46968,
    SPELL_WARRIOR_SHIELD_BLOCK_TRIGGERED			= 132404,
};

enum WarriorSpellIcons
{
    WARRIOR_ICON_ID_SUDDEN_DEATH                    = 1989,
};

/// Updated 5.1.0 : Bloodthirst - 23881
class spell_warr_bloodthirst : public SpellScriptLoader
{
    public:
        spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

        class spell_warr_bloodthirst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODTHIRST))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();
				int32 damage = GetHitDamage();

				SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.9f));
				caster->CastSpell(caster, SPELL_WARRIOR_BLOODTHIRDT_HEAL);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_bloodthirst_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_bloodthirst_SpellScript();
        }
};

/// Updated 5.1.0
class spell_warr_charge : public SpellScriptLoader
{
    public:
        spell_warr_charge() : SpellScriptLoader("spell_warr_charge") { }

        class spell_warr_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_charge_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_CHARGE))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 chargeBasePoints0 = GetEffectValue()*10;
                Unit* caster = GetCaster();
                caster->CastCustomSpell(caster, SPELL_WARRIOR_CHARGE, &chargeBasePoints0, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_charge_SpellScript();
        }
};

/// Updated 4.3.4
class spell_warr_concussion_blow : public SpellScriptLoader
{
    public:
        spell_warr_concussion_blow() : SpellScriptLoader("spell_warr_concussion_blow") { }

        class spell_warr_concussion_blow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_concussion_blow_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                SetHitDamage(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue()));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_concussion_blow_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_concussion_blow_SpellScript();
        }
};

/// Updated 5.1.0
class spell_warr_execute : public SpellScriptLoader
{
    public:
        spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

        class spell_warr_execute_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_execute_SpellScript);

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (GetHitUnit())
                {
                    SpellInfo const* spellInfo = GetSpellInfo();
                    int32 rageUsed = std::min<int32>(200 - spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask)), caster->GetPower(POWER_RAGE));
                    int32 newRage = std::max<int32>(0, caster->GetPower(POWER_RAGE) - rageUsed);
					uint8 level = caster->getLevel();
                    // Sudden Death rage save
                    if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_GENERIC, WARRIOR_ICON_ID_SUDDEN_DEATH, EFFECT_0))
                    {
                        int32 ragesave = aurEff->GetSpellInfo()->Effects[EFFECT_0].CalcValue() * 10;
                        newRage = std::max(newRage, ragesave);
                    }

                    caster->SetPower(POWER_RAGE, uint32(newRage));

                    int32 baseDamage = int32((level*(21 + 0.817*(level-7))) + caster->GetTotalAttackPowerValue(BASE_ATTACK) * (255.0f /100.0f));
                    SetHitDamage(baseDamage);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_execute_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_execute_SpellScript();
        }
};

// 59725 - Improved Spell Reflection
class spell_warr_improved_spell_reflection : public SpellScriptLoader
{
    public:
        spell_warr_improved_spell_reflection() : SpellScriptLoader("spell_warr_improved_spell_reflection") { }

        class spell_warr_improved_spell_reflection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_improved_spell_reflection_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                if (GetCaster())
                    unitList.remove(GetCaster());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_improved_spell_reflection_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_improved_spell_reflection_SpellScript();
        }
};

// 5246 - Intimidating Shout
class spell_warr_intimidating_shout : public SpellScriptLoader
{
    public:
        spell_warr_intimidating_shout() : SpellScriptLoader("spell_warr_intimidating_shout") { }

        class spell_warr_intimidating_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_intimidating_shout_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove(GetExplTargetWorldObject());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_intimidating_shout_SpellScript();
        }
};

// 7384, 7887, 11584, 11585 - Overpower
class spell_warr_overpower : public SpellScriptLoader
{
    public:
        spell_warr_overpower() : SpellScriptLoader("spell_warr_overpower") { }

        class spell_warr_overpower_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_overpower_SpellScript);

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId = 0;
                if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1))
                    spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1;
                else if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2))
                    spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2;

                if (!spellId)
                    return;

                if (Player* target = GetHitPlayer())
                    if (target->HasUnitState(UNIT_STATE_CASTING))
                        target->CastSpell(target, spellId, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_overpower_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_ANY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_overpower_SpellScript();
        }
};

// -772 - Rend
class spell_warr_rend : public SpellScriptLoader
{
    public:
        spell_warr_rend() : SpellScriptLoader("spell_warr_rend") { }

        class spell_warr_rend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_rend_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
            {
                if (Unit* caster = GetCaster())
                {
                    canBeRecalculated = false;

                    // $0.2 * (($MWB + $mwb) / 2 + $AP / 14 * $MWS) bonus per tick
                    float ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
                    int32 mws = caster->GetAttackTime(BASE_ATTACK);
                    float mwbMin = caster->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
                    float mwbMax = caster->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE);
                    float mwb = ((mwbMin + mwbMax) / 2 + ap * mws / 14000) * 0.2f;
                    amount += int32(caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), mwb));

                    // "If used while your target is above 75% health, Rend does 35% more damage."
                    // as for 3.1.3 only ranks above 9 (wrong tooltip?)
                    if (GetSpellInfo()->GetRank() >= 9)
                    {
                        if (GetUnitOwner()->HasAuraState(AURA_STATE_HEALTH_ABOVE_75_PERCENT, GetSpellInfo(), caster))
                            AddPct(amount, GetSpellInfo()->Effects[EFFECT_2].CalcValue(caster));
                    }
                }
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_rend_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_rend_AuraScript();
        }
};

// 64380, 65941 - Shattering Throw
class spell_warr_shattering_throw : public SpellScriptLoader
{
    public:
        spell_warr_shattering_throw() : SpellScriptLoader("spell_warr_shattering_throw") { }

        class spell_warr_shattering_throw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shattering_throw_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                // remove shields, will still display immune to damage part
                if (Unit* target = GetHitUnit())
                    target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_shattering_throw_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shattering_throw_SpellScript();
        }
};

/// Updated 4.3.4
class spell_warr_slam : public SpellScriptLoader
{
    public:
        spell_warr_slam() : SpellScriptLoader("spell_warr_slam") { }

        class spell_warr_slam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_slam_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SLAM))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 bp0 = GetEffectValue();
                if (GetHitUnit())
                    GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_WARRIOR_SLAM, &bp0, NULL, NULL, true, 0);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_slam_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_slam_SpellScript();
        }
};

// 12328, 18765, 35429 - Sweeping Strikes
class spell_warr_sweeping_strikes : public SpellScriptLoader
{
    public:
        spell_warr_sweeping_strikes() : SpellScriptLoader("spell_warr_sweeping_strikes") { }

        class spell_warr_sweeping_strikes_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_sweeping_strikes_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK))
                    return false;
                return true;
            }

            bool Load()
            {
                _procTarget = NULL;
                return true;
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                _procTarget = eventInfo.GetActor()->SelectNearbyTarget(eventInfo.GetProcTarget());
                return _procTarget;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(_procTarget, SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK, true, NULL, aurEff);
            }

            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_warr_sweeping_strikes_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_warr_sweeping_strikes_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }

        private:
            Unit* _procTarget;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_sweeping_strikes_AuraScript();
        }
};

// 50720 - Vigilance
class spell_warr_vigilance : public SpellScriptLoader
{
    public:
        spell_warr_vigilance() : SpellScriptLoader("spell_warr_vigilance") { }

        class spell_warr_vigilance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_vigilance_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_GLYPH_OF_VIGILANCE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_VIGILANCE_PROC))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_GEN_DAMAGE_REDUCTION_AURA))
                    return false;
                return true;
            }

            bool Load()
            {
                _procTarget = NULL;
                return true;
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->CastSpell(target, SPELL_GEN_DAMAGE_REDUCTION_AURA, true);

                if (Unit* caster = GetCaster())
                    target->CastSpell(caster, SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT, true);
            }

            void HandleAfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                //! WORKAROUND
                //! this glyph is a proc
                if (Unit* caster = GetCaster())
                {
                    if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_VIGILANCE, EFFECT_0))
                        GetTarget()->ModifyRedirectThreat(glyph->GetAmount());
                }
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (target->HasAura(SPELL_GEN_DAMAGE_REDUCTION_AURA) &&
                    !(target->HasAura(SPELL_PALADIN_BLESSING_OF_SANCTUARY) ||
                    target->HasAura(SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY) ||
                    target->HasAura(SPELL_PRIEST_RENEWED_HOPE)))
                {
                    target->RemoveAurasDueToSpell(SPELL_GEN_DAMAGE_REDUCTION_AURA);
                }

                target->ResetRedirectThreat();
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                _procTarget = GetCaster();
                return _procTarget;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(_procTarget, SPELL_WARRIOR_VIGILANCE_PROC, true, NULL, aurEff);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_vigilance_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectApply += AuraEffectApplyFn(spell_warr_vigilance_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_warr_vigilance_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                DoCheckProc += AuraCheckProcFn(spell_warr_vigilance_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_warr_vigilance_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }

        private:
            Unit* _procTarget;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_vigilance_AuraScript();
        }
};

// 50725 Vigilance
class spell_warr_vigilance_trigger : public SpellScriptLoader
{
    public:
        spell_warr_vigilance_trigger() : SpellScriptLoader("spell_warr_vigilance_trigger") { }

        class spell_warr_vigilance_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_vigilance_trigger_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                // Remove Taunt cooldown
                if (Player* target = GetHitPlayer())
                    target->RemoveSpellCooldown(SPELL_WARRIOR_TAUNT, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_vigilance_trigger_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_vigilance_trigger_SpellScript();
        }
};

// 97462 - Rallying Cry
class spell_warr_rallying_cry : public SpellScriptLoader
{
    public:
        spell_warr_rallying_cry() : SpellScriptLoader("spell_warr_rallying_cry") { }

        class spell_warr_rallying_cry_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_rallying_cry_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RALLYING_CRY_TRIGGERED))
                    return false;
                return true;
            }

            bool Load()
            {
                return GetCaster()->GetTypeId() ==  TYPEID_PLAYER;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                int32 basePoints0 = int32(GetHitUnit()->CountPctFromMaxHealth(GetEffectValue()));

                GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_WARRIOR_RALLYING_CRY_TRIGGERED, &basePoints0, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_rallying_cry_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_rallying_cry_SpellScript();
        }
};

// Heroic leap - 6544
class spell_warr_heroic_leap : public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap() : SpellScriptLoader("spell_warr_heroic_leap") { }

        class spell_warr_heroic_leap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_SpellScript);

            std::list<Unit*> targetList;

            SpellCastResult CheckElevation()
            {
                Unit* caster = GetCaster();

                WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                if (!dest)
                    return SPELL_FAILED_DONT_REPORT;

                if (dest->GetPositionZ() > caster->GetPositionZ() + 5.0f)
                    return SPELL_FAILED_NOPATH;
                else if (caster->HasAuraType(SPELL_AURA_MOD_ROOT))
                    return SPELL_FAILED_ROOTED;

                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                // Item - Warrior PvP Set 4P Bonus
                if (GetCaster()->HasAura(SPELL_WARRIOR_ITEM_PVP_SET_4P_BONUS))
                    GetCaster()->CastSpell(GetCaster(), SPELL_WARRIOR_HEROIC_LEAP_SPEED, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warr_heroic_leap_SpellScript::CheckElevation);
                AfterCast += SpellCastFn(spell_warr_heroic_leap_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_SpellScript();
        }

};

// Heroic Leap (damage) - 52174
class spell_warr_heroic_leap_damage : public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap_damage() : SpellScriptLoader("spell_warr_heroic_leap_damage") { }

        class spell_warr_heroic_leap_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    SetHitDamage(int32(caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.5f));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_heroic_leap_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_damage_SpellScript();
        }
};


/// Updated 5.1.0 : 86346 - Colossus Smash
class spell_warr_colossus_smash : public SpellScriptLoader
{
    public:
        spell_warr_colossus_smash() : SpellScriptLoader("spell_warr_colossus_smash") { }

        class spell_warr_colossus_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_colossus_smash_SpellScript);

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
				if (Unit* caster = GetCaster())
				{
					int32 damage = GetHitDamage();

					SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 1.75f));
					caster->CastSpell(GetHitUnit(), SPELL_WARRIOR_PHYSICAL_VULNERABILITY, true);
				}
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_colossus_smash_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_NORMALIZED_WEAPON_DMG);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_colossus_smash_SpellScript();
        }
};

/// Updated 5.1.0 : 103840 - Impending victory
class spell_warr_impending_victory : public SpellScriptLoader
{
    public:
        spell_warr_impending_victory() : SpellScriptLoader("spell_warr_impending_victory") { }

        class spell_warr_impending_victory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_impending_victory_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
				Player* caster = GetCaster()->ToPlayer();
				int32 damage = GetHitDamage();

				if (caster->GetPrimaryTalentTree(caster->GetActiveSpec()) == TALENT_TREE_WARRIOR_ARMS)
					SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.7f));
				else if (caster->GetPrimaryTalentTree(caster->GetActiveSpec()) == TALENT_TREE_WARRIOR_FURY ||
					caster->GetPrimaryTalentTree(caster->GetActiveSpec()) == TALENT_TREE_WARRIOR_PROTECTION)
					SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.56f));
				else SetHitDamage(damage);

				caster->CastSpell(caster, SPELL_WARRIOR_IMPENDING_VICTORY);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_impending_victory_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_impending_victory_SpellScript();
        }
};

/// Updated 5.1.0 : 57755 - Heroic Throw
class spell_warr_heroic_throw : public SpellScriptLoader
{
    public:
        spell_warr_heroic_throw() : SpellScriptLoader("spell_warr_heroic_throw") { }

        class spell_warr_heroic_throw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_throw_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_HEROIC_THROW))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();

				int32 damage = GetHitDamage();
                SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.5f));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_heroic_throw_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_throw_SpellScript();
        }
};

/// Updated 5.1.0 : 100130 - Wild strike
class spell_warr_wild_strike : public SpellScriptLoader
{
    public:
        spell_warr_wild_strike() : SpellScriptLoader("spell_warr_wild_strike") { }

        class spell_warr_wild_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_wild_strike_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_WILD_STRIKE))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();

				int32 damage = GetHitDamage();
                SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 2.3f));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_wild_strike_SpellScript::HandleDamage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_wild_strike_SpellScript();
        }
};

class spell_warr_heroic_strike : public SpellScriptLoader
{
    public:
        spell_warr_heroic_strike() : SpellScriptLoader("spell_warr_heroic_strike") { }

        class spell_warr_heroic_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_strike_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_HEROIC_STRIKE))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();

				if (!caster->haveOffhandWeapon())
				{
					int32 damage = caster->GetTotalAttackPowerValue(BASE_ATTACK) * 1.1f;
					SetHitDamage(damage);
				}

				if (caster->haveOffhandWeapon())
				{
					int32 damage = caster->GetTotalAttackPowerValue(BASE_ATTACK) * 1.54f;
					SetHitDamage(damage);
				}
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_heroic_strike_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_strike_SpellScript();
        }
};

class spell_warr_raging_blow_main : public SpellScriptLoader
{
    public:
        spell_warr_raging_blow_main() : SpellScriptLoader("spell_warr_raging_blow_main") { }

        class spell_warr_raging_blow_main_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_main_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RAGING_BLOW_MAIN))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();

				int32 damage = caster->GetTotalAttackPowerValue(BASE_ATTACK) * 1.9f;
				SetHitDamage(damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_raging_blow_main_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_main_SpellScript();
        }
};

class spell_warr_raging_blow_off : public SpellScriptLoader
{
    public:
        spell_warr_raging_blow_off() : SpellScriptLoader("spell_warr_raging_blow_off") { }

        class spell_warr_raging_blow_off_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_off_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RAGING_BLOW_OFF))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();

				int32 damage = caster->GetTotalAttackPowerValue(OFF_ATTACK) * 1.9f;
				SetHitDamage(damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_raging_blow_off_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_off_SpellScript();
        }
};

// Mortal strike - 12294
class spell_warr_mortal_strike : public SpellScriptLoader
{
    public:
        spell_warr_mortal_strike() : SpellScriptLoader("spell_warr_mortal_strike") { }

        class spell_warr_mortal_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_mortal_strike_SpellScript);

            void HandleOnHit()
            {
                // Fix Apply Mortal strike buff on player only if he has the correct glyph
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(SPELL_WARRIOR_MORTAL_STRIKE_AURA))
                            if (!_player->HasAura(SPELL_WARRIOR_GLYPH_OF_MORTAL_STRIKE))
                                _player->RemoveAura(SPELL_WARRIOR_MORTAL_STRIKE_AURA);

                        if (_player->HasAura(SPELL_WARRIOR_TASTE_FOR_BLOOD))
                        {
                            _player->AddComboPoints(target, 1);
                            _player->StartReactiveTimer(REACTIVE_OVERPOWER);
                            _player->CastSpell(_player, SPELL_WARRIOR_ALLOW_OVERPOWER, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_mortal_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_mortal_strike_SpellScript();
        }
};

// 29838 - Second Wind
class spell_warr_second_wind : public SpellScriptLoader
{
    public:
        spell_warr_second_wind() : SpellScriptLoader("spell_warr_second_wind") { }

        class spell_warr_second_wind_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_second_wind_AuraScript);

            void HandleEffectPeriodic(AuraEffect const* aurEff)
            {
                if(Unit *caster = GetCaster())
				{
					if((caster->GetHealthPct() < 35.f)  && caster->isAlive())
					{
						if(!caster->HasAura(SPELL_WARRIOR_SECOND_WIND_REGEN))
						{
							caster->CastSpell(caster, SPELL_WARRIOR_SECOND_WIND_ICON, true);
							caster->CastSpell(caster, SPELL_WARRIOR_SECOND_WIND_REGEN, true);
						}
					}

					else
					{
						if(caster->HasAura(SPELL_WARRIOR_SECOND_WIND_REGEN))
						{
							caster->RemoveAura(SPELL_WARRIOR_SECOND_WIND_ICON);
							caster->RemoveAura(SPELL_WARRIOR_SECOND_WIND_REGEN);
						}
					}
				}
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_second_wind_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_second_wind_AuraScript();
        }
};

// 16491 - Second Wind aura
class spell_warr_second_wind_aura : public SpellScriptLoader
{
    public:
        spell_warr_second_wind_aura() : SpellScriptLoader("spell_warr_second_wind_aura") { }

        class spell_warr_second_wind_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_second_wind_aura_AuraScript);

            void HandleEffectPeriodic(AuraEffect const* aurEff)
            {
                if(Unit *caster = GetCaster())
				{
					if((caster->GetHealthPct() < 35.f)  && caster->isAlive())
							caster->CastSpell(caster, SPELL_WARRIOR_SECOND_WIND_ICON, true);
					else caster->RemoveAura(SPELL_WARRIOR_SECOND_WIND_ICON);
				}
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_second_wind_aura_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_second_wind_aura_AuraScript();
        }
};

// Called By Thunder Clap - 6343, Mortal Strike - 12294, Bloodthirst - 23881 and Devastate - 20243
// Deep Wounds - 115767
/// Updated 4.3.4
class spell_warr_deep_wounds : public SpellScriptLoader
{
    public:
        spell_warr_deep_wounds() : SpellScriptLoader("spell_warr_deep_wounds") { }

        class spell_warr_deep_wounds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_deep_wounds_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DEEP_WOUNDS_RANK_1) || !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DEEP_WOUNDS_RANK_2) || !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DEEP_WOUNDS_RANK_3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 damage = GetEffectValue();
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    // apply percent damage mods
                    damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), damage, SPELL_DIRECT_DAMAGE);

                    ApplyPct(damage, 16 * sSpellMgr->GetSpellRank(GetSpellInfo()->Id));

                    damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage, SPELL_DIRECT_DAMAGE);

                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC);
                    uint32 ticks = spellInfo->GetDuration() / spellInfo->Effects[EFFECT_0].Amplitude;

                    // Add remaining ticks to damage done
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC, EFFECT_0, caster->GetGUID()))
                        damage += aurEff->GetAmount() * (ticks - aurEff->GetTickNumber());

                    damage /= ticks;

                    caster->CastCustomSpell(target, SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC, &damage, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_deep_wounds_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_deep_wounds_SpellScript();
        }
};

// Thunder Clap - 6343
class spell_warr_thunder_clap : public SpellScriptLoader
{
    public:
        spell_warr_thunder_clap() : SpellScriptLoader("spell_warr_thunder_clap") { }

        class spell_warr_thunder_clap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_thunder_clap_SpellScript);

            void HandleOnHit()
            {
				Player* caster = GetCaster()->ToPlayer();

				uint32 damage = GetHitDamage();

				SetHitDamage(damage + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.5f));

				if (Unit* target = GetHitUnit())
					caster->CastSpell(target, SPELL_WARRIOR_WEAKENED_BLOWS, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_thunder_clap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_thunder_clap_SpellScript();
        }
};

// Called by Devastate - 20243
// Sword and Board - 46953
class spell_warr_sword_and_board : public SpellScriptLoader
{
    public:
        spell_warr_sword_and_board() : SpellScriptLoader("spell_warr_sword_and_board") { }

        class spell_warr_sword_and_board_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sword_and_board_SpellScript);

            void HandleOnHit()
            {
                // Fix Sword and Board
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (roll_chance_i(30))
                        {
                            _player->CastSpell(_player, SPELL_WARRIOR_SWORD_AND_BOARD, true);
                            _player->RemoveSpellCooldown(SPELL_WARRIOR_SHIELD_SLAM, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sword_and_board_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sword_and_board_SpellScript();
        }
};

// Sudden Death - 52437
class spell_warr_sudden_death : public SpellScriptLoader
{
    public:
        spell_warr_sudden_death() : SpellScriptLoader("spell_warr_sudden_death") { }

        class spell_warr_sudden_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sudden_death_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasSpellCooldown(SPELL_WARRIOR_COLOSSUS_SMASH))
                        _player->RemoveSpellCooldown(SPELL_WARRIOR_COLOSSUS_SMASH, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sudden_death_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sudden_death_SpellScript();
        }
};

// Called by Overpower - 7384
// Taste for Blood - 56638
class spell_warr_taste_for_blood : public SpellScriptLoader
{
    public:
        spell_warr_taste_for_blood() : SpellScriptLoader("spell_warr_taste_for_blood") { }

        class spell_warr_taste_for_blood_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_taste_for_blood_SpellScript);

            bool rolled;

            void HandleOnHit()
            {
                rolled = false;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(SPELL_WARRIOR_TASTE_FOR_BLOOD))
                        {
                            if (roll_chance_i(30))
                            {
                                rolled = true;
                                // Increase damage of next Heroic Strike or Slam
                                _player->CastSpell(_player, SPELL_WARRIOR_TASTE_FOR_BLOOD_DAMAGE_DONE, true);
                            }
                        }
                    }
                }
            }

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(SPELL_WARRIOR_TASTE_FOR_BLOOD))
                        {
                            if (rolled)
                            {
                                // Second chance to allow overpower !
                                _player->AddComboPoints(target, 1);
                                _player->StartReactiveTimer(REACTIVE_OVERPOWER);
                                _player->CastSpell(_player, SPELL_WARRIOR_ALLOW_OVERPOWER, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_taste_for_blood_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_warr_taste_for_blood_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_taste_for_blood_SpellScript();
        }
};

// Called by Raging Blow - 85288
// Meat Cleaver - 85739
class spell_warr_meat_cleaver : public SpellScriptLoader
{
    public:
        spell_warr_meat_cleaver() : SpellScriptLoader("spell_warr_meat_cleaver") { }

        class spell_warr_meat_cleaver_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_meat_cleaver_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_WARRIOR_MEAT_CLEAVER_PROC))
                        _player->RemoveAura(SPELL_WARRIOR_MEAT_CLEAVER_PROC);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_meat_cleaver_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_meat_cleaver_SpellScript();
        }
};

// Called by Heroic Strike - 78 and Cleave - 845
// Glyph of Hindering Strikes - 58366
class spell_warr_glyph_of_hindering_strikes : public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_hindering_strikes() : SpellScriptLoader("spell_warr_glyph_of_hindering_strikes") { }

        class spell_warr_glyph_of_hindering_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_hindering_strikes_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(SPELL_WARRIOR_GLYPH_OF_HINDERING_STRIKES))
                            _player->CastSpell(target, SPELL_WARRIOR_SLUGGISH, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_hindering_strikes_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_hindering_strikes_SpellScript();
        }
};

// Schockwave - 46968
class spell_warr_shockwave : public SpellScriptLoader
{
    public:
        spell_warr_shockwave() : SpellScriptLoader("spell_warr_shockwave") { }

        class spell_warr_shockwave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shockwave_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHOCKWAVE))
                    return false;

                return true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster()->ToPlayer();
				uint32 damage = GetHitDamage();

				SetHitDamage(damage + caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.22f);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_shockwave_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shockwave_SpellScript();
        }
};

uint64 summonerGUID;
bool cast;

class npc_heroic_leap_damage : public CreatureScript
{
public :
    npc_heroic_leap_damage() : CreatureScript("npc_heroic_leap_damage")
    {}

    struct npc_heroic_leap_damage_AIScript : public ScriptedAI
    {
    public :
        npc_heroic_leap_damage_AIScript(Creature *creature) : ScriptedAI(creature)
        {
            summonerGUID = 0;
            cast = false;
        }

        void IsSummonedBy(Unit* summoner)
        {
            if(summoner->GetTypeId() == TYPEID_PLAYER)
                summonerGUID = summoner->GetGUID();
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(summonerGUID == who->GetGUID())
                if(who->IsWithinDist(me, 0.5, true) && !cast)
                    who->CastSpell(who, SPELL_WARRIOR_HEROIC_LEAP_DAMAGE, true);
        }
    };



    CreatureAI* GetAI(Creature *creature) const
    {
        return new npc_heroic_leap_damage_AIScript(creature);
    }
};

class spell_warr_storm_bolt : public SpellScriptLoader
{
public:
    spell_warr_storm_bolt() : SpellScriptLoader("spell_warr_storm_bolt") { }

    class spell_warr_storm_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_storm_bolt_SpellScript);

        bool Validate (SpellInfo const*)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_STORM_BOLT))
                return false;

            return true;
        }

        bool Load()
        {
            if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                return false;

            return true;
        }

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    if (unitTarget->IsImmunedToSpellEffect(sSpellMgr->GetSpellInfo(WARRIOR_SPELL_STORM_BOLT_STUN), 0))
                        SetHitDamage(GetHitDamage() * 4);

                    _player->CastSpell(unitTarget, WARRIOR_SPELL_STORM_BOLT_STUN, true);
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warr_storm_bolt_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_storm_bolt_SpellScript();
    }
};

// Shield Block - 2565
class spell_warr_shield_block : public SpellScriptLoader
{
    public:
        spell_warr_shield_block() : SpellScriptLoader("spell_warr_shield_block") { }

        class spell_warr_shield_block_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shield_block_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_WARRIOR_SHIELD_BLOCK_TRIGGERED, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_shield_block_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shield_block_SpellScript();
        }
};

// Dragon Roar - 118000
class spell_warr_dragon_roar : public SpellScriptLoader
{
    public:
        spell_warr_dragon_roar() : SpellScriptLoader("spell_warr_dragon_roar") { }

        class spell_warr_dragon_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_dragon_roar_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_WARRIOR_DRAGON_ROAR_KNOCK_BACK, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_warr_dragon_roar_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_dragon_roar_SpellScript();
        }
};

void AddSC_warrior_spell_scripts()
{
    new spell_warr_bloodthirst();
    new spell_warr_charge();
    new spell_warr_concussion_blow();
    new spell_warr_deep_wounds();
    new spell_warr_execute();
    new spell_warr_improved_spell_reflection();
    new spell_warr_intimidating_shout();
    new spell_warr_overpower();
    new spell_warr_rend();
    new spell_warr_shattering_throw();
    new spell_warr_slam();
    new spell_warr_sweeping_strikes();
    new spell_warr_vigilance();
    new spell_warr_vigilance_trigger();
	new spell_warr_rallying_cry();
	new spell_warr_heroic_leap();
	new spell_warr_heroic_leap_damage();
	new spell_warr_colossus_smash();
	new spell_warr_heroic_throw();
	new spell_warr_wild_strike();
	new spell_warr_heroic_strike();
	new spell_warr_raging_blow_main();
	new spell_warr_raging_blow_off();
	new spell_warr_second_wind();
	new spell_warr_second_wind_aura();
	new spell_warr_thunder_clap();
	new spell_warr_mortal_strike();
	new spell_warr_sword_and_board();
	new spell_warr_sudden_death();
	new spell_warr_taste_for_blood();
	new spell_warr_dragon_roar();
	new spell_warr_meat_cleaver();
	new spell_warr_glyph_of_hindering_strikes();
	new spell_warr_shockwave();
    new npc_heroic_leap_damage();
    new spell_warr_storm_bolt();
}