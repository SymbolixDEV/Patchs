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
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "GridNotifiers.h"
#include "Unit.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ShamanSpells
{
    SPELL_HUNTER_INSANITY                       = 95809,
    SPELL_MAGE_TEMPORAL_DISPLACEMENT            = 80354,
    SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC       = 52752,
    SPELL_SHAMAN_BIND_SIGHT                     = 6277,
    SPELL_SHAMAN_EARTH_SHIELD_HEAL              = 379,
    SPELL_SHAMAN_EXHAUSTION                     = 57723,
    SPELL_SHAMAN_FIRE_NOVA_TRIGGERED_R1         = 8349,
    SPELL_SHAMAN_FLAME_SHOCK                    = 8050,
    SPELL_SHAMAN_GLYPH_OF_EARTH_SHIELD          = 63279,
    SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM_TOTEM  = 55456,
    SPELL_SHAMAN_GLYPH_OF_MANA_TIDE             = 55441,
    SPELL_SHAMAN_GLYPH_OF_THUNDERSTORM          = 62132,
    SPELL_SHAMAN_LAVA_FLOWS_R1                  = 51480,
    SPELL_SHAMAN_LAVA_FLOWS_TRIGGERED_R1        = 65264,
    SPELL_SHAMAN_SATED                          = 57724,
    SPELL_SHAMAN_STORM_EARTH_AND_FIRE           = 51483,
    SPELL_SHAMAN_TOTEM_EARTHBIND_EARTHGRAB      = 64695,
    SPELL_SHAMAN_TOTEM_EARTHBIND_TOTEM          = 6474,
    SPELL_SHAMAN_TOTEM_EARTHEN_POWER            = 59566,
    SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL      = 52042,
	SPELL_SHAMAN_ASCENDANCE						= 114049,
	SPELL_SHAMAN_ASCENDANCE_ELEMENTAL			= 114050,
	SPELL_SHAMAN_ASCENDANCE_ENHANCEMENT			= 114051,
	SPELL_SHAMAN_ASCENDANCE_RESTORATION			= 114052,
	SPELL_SHAMAN_LAVA_BURST						= 51505,
	SPELL_SHA_FROZEN_POWER                  = 63374,
	SPELL_SHA_FROST_SHOCK_FREEZE            = 63685,
	SPELL_SHA_ANCESTRAL_GUIDANCE            = 114911,
    SPELL_SHA_CONDUCTIVITY_TALENT           = 108282,
    SPELL_SHA_CONDUCTIVITY_HEAL             = 118800,
	SPELL_SHA_HEALING_RAIN                  = 73920,
	SPELL_SHA_ELEMENTAL_BLAST               = 117014,
    SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS  = 118522,
    SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL = 118517,
    SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL  = 118515,
	SPELL_SHA_EARTHQUAKE                    = 61882,
    SPELL_SHA_EARTHQUAKE_TICK               = 77478,
    SPELL_SHA_EARTHQUAKE_KNOCKING_DOWN      = 77505,
};

enum ShamanSpellIcons
{
    SHAMAN_ICON_ID_SOOTHING_RAIN                = 2011,
    SHAMAN_ICON_ID_SHAMAN_LAVA_FLOW             = 3087
};

// 52759 - Ancestral Awakening
/// Updated 4.3.4
class spell_sha_ancestral_awakening_proc : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_awakening_proc() : SpellScriptLoader("spell_sha_ancestral_awakening_proc") { }

        class spell_sha_ancestral_awakening_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ancestral_awakening_proc_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 damage = GetEffectValue();
                if (GetCaster() && GetHitUnit())
                    GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC, &damage, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_ancestral_awakening_proc_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ancestral_awakening_proc_SpellScript();
        }
};

// 2825 - Bloodlust
/// Updated 4.3.4
class spell_sha_bloodlust : public SpellScriptLoader
{
    public:
        spell_sha_bloodlust() : SpellScriptLoader("spell_sha_bloodlust") { }

        class spell_sha_bloodlust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_bloodlust_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_SATED))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_HUNTER_INSANITY));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, SPELL_SHAMAN_SATED, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_bloodlust_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_bloodlust_SpellScript();
        }
};

// 1064 - Chain Heal
/// Updated 4.3.4
class spell_sha_chain_heal : public SpellScriptLoader
{
    public:
        spell_sha_chain_heal() : SpellScriptLoader("spell_sha_chain_heal") { }

        class spell_sha_chain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_chain_heal_SpellScript);

            bool Load()
            {
                firstHeal = true;
                riptide = false;
                amount = 0;
                return true;
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (firstHeal)
                {
                    // Check if the target has Riptide
                    if (AuraEffect* aurEff = GetHitUnit()->GetAuraEffect(SPELL_AURA_PERIODIC_HEAL, SPELLFAMILY_SHAMAN, 0, 0, 0x10, GetCaster()->GetGUID()))
                    {
                        riptide = true;
                        amount = aurEff->GetSpellInfo()->Effects[EFFECT_2].CalcValue();
                        // Consume it
                        GetHitUnit()->RemoveAura(aurEff->GetBase());
                    }
                    firstHeal = false;
                }
                // Riptide increases the Chain Heal effect by 25%
                if (riptide)
                {
                    uint32 bonus = CalculatePct(GetHitHeal(), amount);
                    SetHitHeal(GetHitHeal() + bonus);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }

            bool firstHeal;
            bool riptide;
            uint32 amount;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_chain_heal_SpellScript();
        }
};

// -974 - Earth Shield
class spell_sha_earth_shield : public SpellScriptLoader
{
    public:
        spell_sha_earth_shield() : SpellScriptLoader("spell_sha_earth_shield") { }

        class spell_sha_earth_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earth_shield_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTH_SHIELD_HEAL))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_GLYPH_OF_EARTH_SHIELD))
                    return false;
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                {
                    amount = caster->SpellHealingBonusDone(GetUnitOwner(), GetSpellInfo(), amount, HEAL);
                    amount = GetUnitOwner()->SpellHealingBonusTaken(caster, GetSpellInfo(), amount, HEAL);

                    // Glyph of Earth Shield
                    //! WORKAROUND
                    //! this glyph is a proc
                    if (AuraEffect* glyph = caster->GetAuraEffect(SPELL_SHAMAN_GLYPH_OF_EARTH_SHIELD, EFFECT_0))
                        AddPct(amount, glyph->GetAmount());
                }
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();

                //! HACK due to currenct proc system implementation
                if (Player* player = GetTarget()->ToPlayer())
                    if (player->HasSpellCooldown(SPELL_SHAMAN_EARTH_SHIELD_HEAL))
                        return;

                GetTarget()->CastCustomSpell(SPELL_SHAMAN_EARTH_SHIELD_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true, NULL, aurEff, GetCasterGUID());

                if (Player* player = GetTarget()->ToPlayer())
                    player->AddSpellCooldown(SPELL_SHAMAN_EARTH_SHIELD_HEAL, 0, time(NULL) + 3);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_earth_shield_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_DUMMY);
                OnEffectProc += AuraEffectProcFn(spell_sha_earth_shield_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earth_shield_AuraScript();
        }
};

// 6474 - Earthbind Totem - Fix Talent:Earthen Power, Earth's Grasp
/// Updated 4.3.4
class spell_sha_earthbind_totem : public SpellScriptLoader
{
    public:
        spell_sha_earthbind_totem() : SpellScriptLoader("spell_sha_earthbind_totem") { }

        class spell_sha_earthbind_totem_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earthbind_totem_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_TOTEM_EARTHBIND_TOTEM) || !sSpellMgr->GetSpellInfo(SPELL_SHAMAN_TOTEM_EARTHEN_POWER))
                    return false;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (!GetCaster())
                    return;
                if (Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself())
                    if (AuraEffect* aur = owner->GetDummyAuraEffect(SPELLFAMILY_SHAMAN, 2289, 0))
                        if (roll_chance_i(aur->GetBaseAmount()))
                            GetTarget()->CastSpell((Unit*)NULL, SPELL_SHAMAN_TOTEM_EARTHEN_POWER, true);
            }

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;
                Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself();
                if (!owner)
                    return;
                // Earth's Grasp
                if (AuraEffect* aurEff = owner->GetAuraEffectOfRankedSpell(SPELL_SHAMAN_STORM_EARTH_AND_FIRE, EFFECT_1))
                {
                    if (roll_chance_i(aurEff->GetAmount()))
                        GetCaster()->CastSpell(GetCaster(), SPELL_SHAMAN_TOTEM_EARTHBIND_EARTHGRAB, false);
                }
            }

            void Register()
            {
                 OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthbind_totem_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                 OnEffectApply += AuraEffectApplyFn(spell_sha_earthbind_totem_AuraScript::Apply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earthbind_totem_AuraScript();
        }
};

class EarthenPowerTargetSelector
{
    public:
        EarthenPowerTargetSelector() { }

        bool operator() (WorldObject* target)
        {
            if (!target->ToUnit())
                return true;

            if (!target->ToUnit()->HasAuraWithMechanic(1 << MECHANIC_SNARE))
                return true;

            return false;
        }
};

// 59566 - Earthen Power
class spell_sha_earthen_power : public SpellScriptLoader
{
    public:
        spell_sha_earthen_power() : SpellScriptLoader("spell_sha_earthen_power") { }

        class spell_sha_earthen_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthen_power_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(EarthenPowerTargetSelector());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_earthen_power_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthen_power_SpellScript();
        }
};

// 1535 Fire Nova
/// Updated 4.3.4
class spell_sha_fire_nova : public SpellScriptLoader
{
    public:
        spell_sha_fire_nova() : SpellScriptLoader("spell_sha_fire_nova") { }

        class spell_sha_fire_nova_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fire_nova_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    if (target->HasAura(SPELL_SHAMAN_FLAME_SHOCK))
                    {
                        caster->CastSpell(target, SPELL_SHAMAN_FIRE_NOVA_TRIGGERED_R1, true);
                        target->RemoveAurasDueToSpell(SPELL_SHAMAN_FLAME_SHOCK);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_fire_nova_SpellScript();
        }
};

// 8050 -Flame Shock
/// Updated 4.3.4
class spell_sha_flame_shock : public SpellScriptLoader
{
    public:
        spell_sha_flame_shock() : SpellScriptLoader("spell_sha_flame_shock") { }

        class spell_sha_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_flame_shock_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LAVA_FLOWS_R1))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LAVA_FLOWS_TRIGGERED_R1))
                    return false;
                return true;
            }

            void HandleDispel(DispelInfo* /*dispelInfo*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Lava Flows
                    if (AuraEffect const* aurEff = caster->GetDummyAuraEffect(SPELLFAMILY_SHAMAN, SHAMAN_ICON_ID_SHAMAN_LAVA_FLOW, EFFECT_0))
                    {
                        if (sSpellMgr->GetFirstSpellInChain(SPELL_SHAMAN_LAVA_FLOWS_R1) != sSpellMgr->GetFirstSpellInChain(aurEff->GetId()))
                            return;

                        int32 basepoints = aurEff->GetAmount();
                        caster->CastCustomSpell(caster, SPELL_SHAMAN_LAVA_FLOWS_TRIGGERED_R1, &basepoints, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_sha_flame_shock_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_flame_shock_AuraScript();
        }
};

// 52041 - Healing Stream Totem
/// Updated 4.3.4
class spell_sha_healing_stream_totem : public SpellScriptLoader
{
    public:
        spell_sha_healing_stream_totem() : SpellScriptLoader("spell_sha_healing_stream_totem") { }

        class spell_sha_healing_stream_totem_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_stream_totem_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL);
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                int32 damage = GetEffectValue();
                SpellInfo const* triggeringSpell = GetTriggeringSpell();
                if (Unit* target = GetHitUnit())
                    if (Unit* caster = GetCaster())
                    {
                        if (Unit* owner = caster->GetOwner())
                        {
                            if (triggeringSpell)
                                damage = int32(owner->SpellHealingBonusDone(target, triggeringSpell, damage, HEAL));

                            // Soothing Rains
                            if (AuraEffect* dummy = owner->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, SHAMAN_ICON_ID_SOOTHING_RAIN, EFFECT_0))
                                AddPct(damage, dummy->GetAmount());

                            damage = int32(target->SpellHealingBonusTaken(owner, triggeringSpell, damage, HEAL));
                        }
                        caster->CastCustomSpell(target, SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL, &damage, 0, 0, true, 0, 0, GetOriginalCaster()->GetGUID());
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_healing_stream_totem_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_stream_totem_SpellScript();
        }
};

// 32182 - Heroism
/// Updated 4.3.4
class spell_sha_heroism : public SpellScriptLoader
{
    public:
        spell_sha_heroism() : SpellScriptLoader("spell_sha_heroism") { }

        class spell_sha_heroism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_heroism_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EXHAUSTION))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTION));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_HUNTER_INSANITY));
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, SPELL_SHAMAN_EXHAUSTION, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_heroism_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_heroism_SpellScript();
        }
};

// 60103 - Lava Lash
/// Updated 4.3.4
class spell_sha_lava_lash : public SpellScriptLoader
{
    public:
        spell_sha_lava_lash() : SpellScriptLoader("spell_sha_lava_lash") { }

        class spell_sha_lava_lash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_SpellScript)

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    int32 damage = GetEffectValue();
                    int32 hitDamage = GetHitDamage();
                    if (caster->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                    {
                        // Damage is increased by 25% if your off-hand weapon is enchanted with Flametongue.
                        if (caster->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, 0x200000, 0, 0))
                            AddPct(hitDamage, damage);
                        SetHitDamage(hitDamage);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_lash_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_SpellScript();
        }
};

// 16191 - Mana Tide
/// Updated 4.3.4
class spell_sha_mana_tide_totem : public SpellScriptLoader
{
    public:
        spell_sha_mana_tide_totem() : SpellScriptLoader("spell_sha_mana_tide_totem") { }

        class spell_sha_mana_tide_totem_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_mana_tide_totem_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                ///@TODO: Exclude the "short term" buffs from the stat value
                if (Unit* caster = GetCaster())
                    if (Unit* owner = caster->GetOwner())
                        amount = CalculatePct(owner->GetStat(STAT_SPIRIT), aurEff->GetAmount());
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_mana_tide_totem_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_mana_tide_totem_AuraScript();
        }
};

// -51490 - Thunderstorm
class spell_sha_thunderstorm : public SpellScriptLoader
{
    public:
        spell_sha_thunderstorm() : SpellScriptLoader("spell_sha_thunderstorm") { }

        class spell_sha_thunderstorm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_thunderstorm_SpellScript);

            void HandleKnockBack(SpellEffIndex effIndex)
            {
                // Glyph of Thunderstorm
                if (GetCaster()->HasAura(SPELL_SHAMAN_GLYPH_OF_THUNDERSTORM))
                    PreventHitDefaultEffect(effIndex);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_thunderstorm_SpellScript::HandleKnockBack, EFFECT_2, SPELL_EFFECT_KNOCK_BACK);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_thunderstorm_SpellScript();
        }
};

// Healing Rain
// Spell Id: 73920
class spell_sha_healing_rain : public SpellScriptLoader
{
public:
    spell_sha_healing_rain() : SpellScriptLoader("spell_sha_healing_rain") { }

    class spell_sha_healing_rain_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_healing_rain_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (DynamicObject* dynObj = GetCaster()->GetDynObject(73920))
            {
                GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), 73921, true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }

    public:
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_sha_healing_rain_AuraScript();
    }
};

// Unleash Elements
// Spell Id: 73680
class spell_sha_unleash_elements : public SpellScriptLoader
{
public:
    spell_sha_unleash_elements() : SpellScriptLoader("spell_sha_unleash_elements") { }

    class spell_sha_unleash_elements_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_unleash_elements_SpellScript)

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if(!caster)
                return;
            Player* plr = caster->ToPlayer();
            if(!plr)
                return;

            if(!GetHitUnit())
                return;

            Item *weapons[2];
            weapons[0] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            weapons[1] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            for(int i = 0; i < 2; i++)
            {
                if(!weapons[i])
                    continue;

                uint32 unleashSpell = 0;
                Unit *target = GetHitUnit();
                bool hostileTarget = plr->IsHostileTo(target);
                bool hostileSpell = true;

                switch (weapons[i]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
                {
                case 3345: // Earthliving Weapon
                    unleashSpell = 73685; //Unleash Life
                    hostileSpell = false;
                    break;
                case 5: // Flametongue Weapon
                    unleashSpell = 73683; // Unleash Flame
                    break;
                case 2: // Frostbrand Weapon
                    unleashSpell = 73682; // Unleash Frost
                    break;
                case 3021: // Rockbiter Weapon
                    unleashSpell = 73684; // Unleash Earth
                    break;
                case 283: // Windfury Weapon
                    unleashSpell = 73681; // Unleash Wind
                    break;
                }
                if(hostileSpell && !hostileTarget)
                    continue; // don't allow to attack non-hostile targets. TODO: check this before cast

                if(!hostileSpell && hostileTarget)
                    target = plr;   // heal ourselves instead of the enemy
                if(unleashSpell)
                {
                    plr->CastSpell(target, unleashSpell, true);
                }
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_sha_unleash_elements_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_sha_unleash_elements_SpellScript();
    }
};

// Updated 5.1 : Ascendance - 114049
class spell_sha_ascendance : public SpellScriptLoader
{
    public:
        spell_sha_ascendance() : SpellScriptLoader("spell_sha_ascendance") { }

        class spell_sha_ascendance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ASCENDANCE))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
				Player* caster = GetCaster()->ToPlayer();

				switch (caster->GetPrimaryTalentTree(caster->ToPlayer()->GetActiveSpec()))
				{
					case TALENT_TREE_SHAMAN_ELEMENTAL:
						caster->CastSpell(caster, SPELL_SHAMAN_ASCENDANCE_ELEMENTAL);
						break;

					case TALENT_TREE_SHAMAN_ENHANCEMENT:
						caster->CastSpell(caster, SPELL_SHAMAN_ASCENDANCE_ENHANCEMENT);
						break;

					case TALENT_TREE_SHAMAN_RESTORATION:
						caster->CastSpell(caster, SPELL_SHAMAN_ASCENDANCE_RESTORATION);
						break;

					default:
						break;
				}
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_ascendance_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_SpellScript();
        }
};

// Updated 5.1 : Ascendance elemental - 114050
class spell_sha_ascendance_elemental : public SpellScriptLoader
{
public:
    spell_sha_ascendance_elemental() : SpellScriptLoader("spell_sha_ascendance_elemental") { }

    class spell_sha_ascendance_elemental_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_ascendance_elemental_AuraScript);

        void EffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
			Player* caster = GetCaster()->ToPlayer();

			caster->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST, true);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_sha_ascendance_elemental_AuraScript::EffectRemove, EFFECT_0, SPELL_EFFECT_APPLY_AURA, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_sha_ascendance_elemental_AuraScript;
    }
};

// Frost Shock - 8056
class spell_sha_frozen_power : public SpellScriptLoader
{
    public:
        spell_sha_frozen_power() : SpellScriptLoader("spell_sha_frozen_power") { }

        class spell_sha_frozen_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_frozen_power_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(8056))
                    return false;
                return true;
            }

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(SPELL_SHA_FROZEN_POWER))
                            _player->CastSpell(target, SPELL_SHA_FROST_SHOCK_FREEZE, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_frozen_power_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_frozen_power_SpellScript();
        }
};

// Called by Healing Wave - 331, Greater Healing Wave - 77472 and Healing Surge - 8004
// Called by Lightning Bolt - 403, Chain Lightning - 421, Earth Shock - 8042 and Stormstrike - 17364
// Called by Lightning Bolt - 45284, Chain Lightning - 45297
// Conductivity - 108282
class spell_sha_conductivity : public SpellScriptLoader
{
    public:
        spell_sha_conductivity() : SpellScriptLoader("spell_sha_conductivity") { }

        class spell_sha_conductivity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_conductivity_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                        {
                            if (DynamicObject* dynObj = _player->GetDynObject(SPELL_SHA_HEALING_RAIN))
                            {
                                std::list<Unit*> tempList;
                                std::list<Unit*> memberList;

                                _player->GetPartyMembers(tempList);

								for (std::list<Unit*>::const_iterator i = tempList.begin(); i != tempList.end(); ++i)
                                    if ((*i)->GetDistance(dynObj) <= 10.0f)
                                        memberList.push_back((*i));

                                if (memberList.empty())
                                    return;

                                memberList.sort(Trinity::DistanceCompareOrderPred(dynObj));
                                memberList.resize(1);

                                // When you cast Healing Wave, Greater Healing Wave, or Healing Surge
                                // allies within your Healing Rain share healing equal to 30% of the initial healing done
                                if (GetSpellInfo()->IsPositive())
                                {
                                    int32 bp = int32(GetHitHeal() * 0.30f) / memberList.size();

									for (std::list<Unit*>::const_iterator i = memberList.begin(); i != memberList.end(); ++i)
                                    {
                                        _player->CastCustomSpell((*i), SPELL_SHA_CONDUCTIVITY_HEAL, &bp, NULL, NULL, true);
                                        break;
                                    }
                                }
                                // If your Lightning Bolt, Chain Lightning, Earth Shock, or Stormstrike damages an enemy
                                // allies within your Healing Rain share healing equal to 50% of the initial damage done
                                else
                                {
                                    int32 bp = int32(GetHitDamage() * 0.50f) / memberList.size();

									for (std::list<Unit*>::const_iterator i = memberList.begin(); i != memberList.end(); ++i)
                                    {
                                        _player->CastCustomSpell((*i), SPELL_SHA_CONDUCTIVITY_HEAL, &bp, NULL, NULL, true);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_conductivity_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_conductivity_SpellScript();
        }
};

// Ancestral Guidance - 108281
class spell_sha_ancestral_guidance : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance() : SpellScriptLoader("spell_sha_ancestral_guidance") { }

        class spell_sha_ancestral_guidance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ancestral_guidance_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE)
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    int32 bp = eventInfo.GetDamageInfo()->GetDamage() > eventInfo.GetHealInfo()->GetHeal() ? eventInfo.GetDamageInfo()->GetDamage() : eventInfo.GetHealInfo()->GetHeal();
                    if (!bp)
                        return;

                    bp = int32(bp * 0.40f);

                    _player->CastCustomSpell(target, SPELL_SHA_ANCESTRAL_GUIDANCE, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ancestral_guidance_AuraScript();
        }
};

// Elemental Blast - 117014
class spell_sha_elemental_blast : public SpellScriptLoader
{
    public:
        spell_sha_elemental_blast() : SpellScriptLoader("spell_sha_elemental_blast") { }

        class spell_sha_elemental_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_elemental_blast_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        _player->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL, true);
                        _player->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL, true);
                    }
                }
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    int32 randomEffect = irand(0, 2);

                    _player->CastSpell(_player, SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS, true);

                    AuraApplication* aura = _player->GetAuraApplication(SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS, _player->GetGUID());

                    if (aura)
                    {
                        switch (randomEffect)
                        {
                            case 0:
                            {
                                aura->GetBase()->GetEffect(1)->ChangeAmount(0);
                                aura->GetBase()->GetEffect(2)->ChangeAmount(0);
                                break;
                            }
                            case 1:
                            {
                                aura->GetBase()->GetEffect(0)->ChangeAmount(0);
                                aura->GetBase()->GetEffect(2)->ChangeAmount(0);
                                break;
                            }
                            case 2:
                            {
                                aura->GetBase()->GetEffect(0)->ChangeAmount(0);
                                aura->GetBase()->GetEffect(1)->ChangeAmount(0);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_elemental_blast_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_sha_elemental_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_elemental_blast_SpellScript();
        }
};

// Earthquake : Ticks - 77478
class spell_sha_earthquake_tick : public SpellScriptLoader
{
    public:
        spell_sha_earthquake_tick() : SpellScriptLoader("spell_sha_earthquake_tick") { }

        class spell_sha_earthquake_tick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthquake_tick_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_EARTHQUAKE_TICK))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                // With a 10% chance of knocking down affected targets
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (roll_chance_i(10))
                            _player->CastSpell(target, SPELL_SHA_EARTHQUAKE_KNOCKING_DOWN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_earthquake_tick_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthquake_tick_SpellScript();
        }
};

// Earthquake - 61882
class spell_sha_earthquake : public SpellScriptLoader
{
    public:
        spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

        class spell_sha_earthquake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earthquake_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_EARTHQUAKE))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_EARTHQUAKE_TICK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earthquake_AuraScript();
        }
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_ancestral_awakening_proc();
    new spell_sha_bloodlust();
    new spell_sha_chain_heal();
    new spell_sha_earth_shield();
    new spell_sha_earthbind_totem();
    new spell_sha_earthen_power();
    new spell_sha_fire_nova();
    new spell_sha_flame_shock();
    new spell_sha_healing_stream_totem();
    new spell_sha_heroism();
    new spell_sha_lava_lash();
    new spell_sha_mana_tide_totem();
    new spell_sha_thunderstorm();
    new spell_sha_healing_rain();
    new spell_sha_unleash_elements();
	new spell_sha_ascendance();
	new spell_sha_ascendance_elemental();
	new spell_sha_frozen_power();
	new spell_sha_conductivity();
    new spell_sha_ancestral_guidance();
	new spell_sha_elemental_blast();
	new spell_sha_earthquake_tick();
	new spell_sha_earthquake();
}
