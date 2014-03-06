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
 * Scripts for spells with SPELLFAMILY_PALADIN and SPELLFAMILY_GENERIC spells used by paladin players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pal_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Group.h"

enum PaladinSpells
{
    SPELL_PALADIN_DIVINE_PLEA                    = 54428,
    SPELL_PALADIN_BLESSING_OF_SANCTUARY_BUFF     = 67480,
    SPELL_PALADIN_BLESSING_OF_SANCTUARY_ENERGIZE = 57319,

    SPELL_PALADIN_HOLY_SHOCK_R1                  = 20473,
    SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE           = 25912,
    SPELL_PALADIN_HOLY_SHOCK_R1_HEALING          = 25914,

    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID   = 37878,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN = 37879,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST  = 37880,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN  = 37881,

    SPELL_PALADIN_DIVINE_STORM                   = 53385,
    SPELL_PALADIN_DIVINE_STORM_DUMMY             = 54171,
    SPELL_PALADIN_DIVINE_STORM_HEAL              = 54172,

    SPELL_PALADIN_EYE_FOR_AN_EYE_DAMAGE          = 25997,

    SPELL_PALADIN_FORBEARANCE                    = 25771,
    SPELL_PALADIN_AVENGING_WRATH_MARKER          = 61987,
    SPELL_PALADIN_IMMUNE_SHIELD_MARKER           = 61988,

    SPELL_PALADIN_HAND_OF_SACRIFICE              = 6940,
    SPELL_PALADIN_DIVINE_SACRIFICE               = 64205,

    SPELL_PALADIN_DIVINE_PURPOSE_PROC            = 90174,
    
    SPELL_PALADIN_GLYPH_OF_SALVATION             = 63225,

    SPELL_PALADIN_RIGHTEOUS_DEFENSE_TAUNT        = 31790,

    SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS          = 25742,

	SPELL_PALADIN_WORD_OF_GLORY                  = 85673,
    SPELL_PALADIN_WORD_OF_GLORY_HEAL             = 130551,
    SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY         = 54936,
    SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY_DAMAGE  = 115522,
	PALADIN_ITEM_PVP_HOLY_4P_BONUS               = 131665,
	SPELL_PALADIN_DIVINE_PURPOSE                 = 90174,

    SPELL_GENERIC_ARENA_DAMPENING                = 74410,
    SPELL_GENERIC_BATTLEGROUND_DAMPENING         = 74411,
	SPELL_EXORCISM								 = 879,
	SPELL_HAMMER_OF_WRATH						 = 24275,

	PALADIN_SPELL_EXECUTION_SENTENCE             = 114916,
    PALADIN_SPELL_STAY_OF_EXECUTION              = 114917,

	PALADIN_SPELL_HOLY_PRISM_ALLIES              = 114871,
    PALADIN_SPELL_HOLY_PRISM_ENNEMIES            = 114852,
    PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL       = 114862,
    PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2     = 114870,
    PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL         = 121551,
    PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2       = 121552,

	PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE     = 88852,
    PALADIN_SPELL_BEACON_OF_LIGHT                = 53563,
    PALADIN_SPELL_SELFLESS_HEALER_STACK          = 114250,
	PALADIN_SPELL_TOWER_OF_RADIANCE              = 85512
};

//24275 -- SPELL_HAMMER_OF_WRATH
class spell_pal_hammer_of_wrath : public SpellScriptLoader
{
    public:
        spell_pal_hammer_of_wrath() : SpellScriptLoader("spell_pal_hammer_of_wrath") { }

        class spell_pal_hammer_of_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_hammer_of_wrath_SpellScript);
			
            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
				Player* caster = GetCaster()->ToPlayer();

				int32 baseDamage = urand(1747, 1930);

				if (caster->GetPrimaryTalentTree(caster->GetActiveSpec()) == TALENT_TREE_PALADIN_HOLY)
				{
					int32 damage = baseDamage + 1.61f * GetCaster()->GetTotalSpellPowerValue(SPELL_SCHOOL_MASK_NORMAL, false);
					SetHitDamage(damage);
				}

				if (caster->GetPrimaryTalentTree(caster->GetActiveSpec()) == TALENT_TREE_PALADIN_PROTECTION || caster->GetPrimaryTalentTree(caster->GetActiveSpec()) == TALENT_TREE_PALADIN_RETRIBUTION)
				{
					int32 damage = baseDamage + 1.61f * GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK);
					SetHitDamage(damage);
				}

				if (caster->GetPrimaryTalentTree(caster->GetActiveSpec()) != TALENT_TREE_PALADIN_HOLY &&
					caster->GetPrimaryTalentTree(caster->GetActiveSpec()) != TALENT_TREE_PALADIN_PROTECTION &&
					caster->GetPrimaryTalentTree(caster->GetActiveSpec()) != TALENT_TREE_PALADIN_RETRIBUTION)
					SetHitDamage(baseDamage);
            }
			
            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_hammer_of_wrath_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
			}
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_hammer_of_wrath_SpellScript();
        }
};

//879 -- SPELL_EXORCISM 
class spell_pal_exorcism : public SpellScriptLoader
{
    public:
        spell_pal_exorcism() : SpellScriptLoader("spell_pal_exorcism") { }

        class spell_pal_exorcism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_exorcism_SpellScript);

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
			    uint8 level = caster->getLevel();
				int32 baseDamage = int32((level*(urand(200,222) + -1.56f*(level-1))) + (caster->GetTotalAttackPowerValue(BASE_ATTACK)*(67.7f/100.0f)));
				SetHitDamage(baseDamage);
            }
			
            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_exorcism_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
			}
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_exorcism_SpellScript();
        }
};


// 31850 - Ardent Defender
/*class spell_pal_ardent_defender : public SpellScriptLoader
{
    public:
        spell_pal_ardent_defender() : SpellScriptLoader("spell_pal_ardent_defender") { }

        class spell_pal_ardent_defender_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_ardent_defender_AuraScript);

            uint32 absorbPct, healPct;

            enum Spell
            {
                PAL_SPELL_ARDENT_DEFENDER_HEAL = 66235,
            };

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
                return GetUnitOwner()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* victim = GetTarget();
                int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
                uint32 allowedHealth = victim->CountPctFromMaxHealth(35);
                // If damage kills us
                if (remainingHealth <= 0 && !victim->ToPlayer()->HasSpellCooldown(PAL_SPELL_ARDENT_DEFENDER_HEAL))
                {
                    // Cast healing spell, completely avoid damage
                    absorbAmount = dmgInfo.GetDamage();

                    uint32 defenseSkillValue = victim->GetDefenseSkillValue();
                    // Max heal when defense skill denies critical hits from raid bosses
                    // Formula: max defense at level + 140 (raiting from gear)
                    uint32 reqDefForMaxHeal  = victim->getLevel() * 5 + 140;
                    float pctFromDefense = (defenseSkillValue >= reqDefForMaxHeal)
                        ? 1.0f
                        : float(defenseSkillValue) / float(reqDefForMaxHeal);

                    int32 healAmount = int32(victim->CountPctFromMaxHealth(uint32(healPct * pctFromDefense)));
                    victim->CastCustomSpell(victim, PAL_SPELL_ARDENT_DEFENDER_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff);
                    victim->ToPlayer()->AddSpellCooldown(PAL_SPELL_ARDENT_DEFENDER_HEAL, 0, time(NULL) + 120);
                }
                else if (remainingHealth < int32(allowedHealth))
                {
                    // Reduce damage that brings us under 35% (or full damage if we are already under 35%) by x%
                    uint32 damageToReduce = (victim->GetHealth() < allowedHealth)
                        ? dmgInfo.GetDamage()
                        : allowedHealth - remainingHealth;
                    absorbAmount = CalculatePct(damageToReduce, absorbPct);
                }
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_ardent_defender_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_ardent_defender_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_ardent_defender_AuraScript();
        }
};*/

// 37877 - Blessing of Faith
class spell_pal_blessing_of_faith : public SpellScriptLoader
{
    public:
        spell_pal_blessing_of_faith() : SpellScriptLoader("spell_pal_blessing_of_faith") { }

        class spell_pal_blessing_of_faith_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_blessing_of_faith_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID) || !sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN) || !sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST) || !sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    uint32 spell_id = 0;
                    switch (unitTarget->getClass())
                    {
                        case CLASS_DRUID:
                            spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID;
                            break;
                        case CLASS_PALADIN:
                            spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN;
                            break;
                        case CLASS_PRIEST:
                            spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST;
                            break;
                        case CLASS_SHAMAN:
                            spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN;
                            break;
                        default:
                            return; // ignore for non-healing classes
                    }
                    Unit* caster = GetCaster();
                    caster->CastSpell(caster, spell_id, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_blessing_of_faith_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_blessing_of_faith_SpellScript();
        }
};

// 20911 - Blessing of Sanctuary
// 25899 - Greater Blessing of Sanctuary
class spell_pal_blessing_of_sanctuary : public SpellScriptLoader
{
    public:
        spell_pal_blessing_of_sanctuary() : SpellScriptLoader("spell_pal_blessing_of_sanctuary") { }

        class spell_pal_blessing_of_sanctuary_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_blessing_of_sanctuary_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_SANCTUARY_BUFF))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_SANCTUARY_ENERGIZE))
                    return false;
                return true;
            }

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (Unit* caster = GetCaster())
                    caster->CastSpell(target, SPELL_PALADIN_BLESSING_OF_SANCTUARY_BUFF, true);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->RemoveAura(SPELL_PALADIN_BLESSING_OF_SANCTUARY_BUFF, GetCasterGUID());
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                return GetTarget()->getPowerType() == POWER_MANA;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(GetTarget(), SPELL_PALADIN_BLESSING_OF_SANCTUARY_ENERGIZE, true, NULL, aurEff);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_pal_blessing_of_sanctuary_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_pal_blessing_of_sanctuary_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                DoCheckProc += AuraCheckProcFn(spell_pal_blessing_of_sanctuary_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_pal_blessing_of_sanctuary_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_blessing_of_sanctuary_AuraScript();
        }
};

// 64205 - Divine Sacrifice
class spell_pal_divine_sacrifice : public SpellScriptLoader
{
    public:
        spell_pal_divine_sacrifice() : SpellScriptLoader("spell_pal_divine_sacrifice") { }

        class spell_pal_divine_sacrifice_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_divine_sacrifice_AuraScript);

            uint32 groupSize, minHpPct;
            int32 remainingAmount;

            bool Load()
            {

                if (Unit* caster = GetCaster())
                {
                    if (caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (caster->ToPlayer()->GetGroup())
                            groupSize = caster->ToPlayer()->GetGroup()->GetMembersCount();
                        else
                            groupSize = 1;
                    }
                    else
                        return false;

                    remainingAmount = (caster->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_2].CalcValue(caster)) * groupSize);
                    minHpPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);
                    return true;
                }
                return false;
            }

            void Split(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & splitAmount)
            {
                remainingAmount -= splitAmount;
                // break when absorbed everything it could, or if the casters hp drops below 20%
                if (Unit* caster = GetCaster())
                    if (remainingAmount <= 0 || (caster->GetHealthPct() < minHpPct))
                        caster->RemoveAura(SPELL_PALADIN_DIVINE_SACRIFICE);
            }

            void Register()
            {
                OnEffectSplit += AuraEffectSplitFn(spell_pal_divine_sacrifice_AuraScript::Split, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_divine_sacrifice_AuraScript();
        }
};

// 53385 - Divine Storm
class spell_pal_divine_storm : public SpellScriptLoader
{
    public:
        spell_pal_divine_storm() : SpellScriptLoader("spell_pal_divine_storm") { }

        class spell_pal_divine_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_divine_storm_SpellScript);

            uint32 healPct;

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_DIVINE_STORM_DUMMY))
                    return false;
                return true;
            }

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return true;
            }

            void TriggerHeal()
            {
                Unit* caster = GetCaster();
                caster->CastCustomSpell(SPELL_PALADIN_DIVINE_STORM_DUMMY, SPELLVALUE_BASE_POINT0, (GetHitDamage() * healPct) / 100, caster, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_pal_divine_storm_SpellScript::TriggerHeal);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_divine_storm_SpellScript();
        }
};

// 54171 - Divine Storm (Dummy)
class spell_pal_divine_storm_dummy : public SpellScriptLoader
{
    public:
        spell_pal_divine_storm_dummy() : SpellScriptLoader("spell_pal_divine_storm_dummy") { }

        class spell_pal_divine_storm_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_divine_storm_dummy_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_DIVINE_STORM_HEAL))
                    return false;
                return true;
            }

            void CountTargets(std::list<WorldObject*>& targetList)
            {
                _targetCount = targetList.size();
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!_targetCount || ! GetHitUnit())
                    return;

                int32 heal = GetEffectValue() / _targetCount;
                GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_PALADIN_DIVINE_STORM_HEAL, &heal, NULL, NULL, true);
            }
        private:
            uint32 _targetCount;

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_divine_storm_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_divine_storm_dummy_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_divine_storm_dummy_SpellScript();
        }
};

// 33695 - Exorcism and Holy Wrath Damage
class spell_pal_exorcism_and_holy_wrath_damage : public SpellScriptLoader
{
    public:
        spell_pal_exorcism_and_holy_wrath_damage() : SpellScriptLoader("spell_pal_exorcism_and_holy_wrath_damage") { }

        class spell_pal_exorcism_and_holy_wrath_damage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_exorcism_and_holy_wrath_damage_AuraScript);

            void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
            {
                if (!spellMod)
                {
                    spellMod = new SpellModifier(aurEff->GetBase());
                    spellMod->op = SPELLMOD_DAMAGE;
                    spellMod->type = SPELLMOD_FLAT;
                    spellMod->spellId = GetId();
                    spellMod->mask[1] = 0x200002;
                }

                spellMod->value = aurEff->GetAmount();
            }

            void Register()
            {
                DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_pal_exorcism_and_holy_wrath_damage_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_exorcism_and_holy_wrath_damage_AuraScript();
        }
};

// -9799 - Eye for an Eye
class spell_pal_eye_for_an_eye : public SpellScriptLoader
{
    public:
        spell_pal_eye_for_an_eye() : SpellScriptLoader("spell_pal_eye_for_an_eye") { }

        class spell_pal_eye_for_an_eye_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_eye_for_an_eye_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_EYE_FOR_AN_EYE_DAMAGE))
                    return false;
                return true;
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                // return damage % to attacker but < 50% own total health
                int32 damage = int32(std::min(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()), GetTarget()->GetMaxHealth() / 2));
                GetTarget()->CastCustomSpell(SPELL_PALADIN_EYE_FOR_AN_EYE_DAMAGE, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetProcTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pal_eye_for_an_eye_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_eye_for_an_eye_AuraScript();
        }
};

// 63521 - Guarded by The Light
class spell_pal_guarded_by_the_light : public SpellScriptLoader
{
    public:
        spell_pal_guarded_by_the_light() : SpellScriptLoader("spell_pal_guarded_by_the_light") { }

        class spell_pal_guarded_by_the_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_guarded_by_the_light_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_DIVINE_PLEA))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                // Divine Plea
                if (Aura* aura = GetCaster()->GetAura(SPELL_PALADIN_DIVINE_PLEA))
                    aura->RefreshDuration();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_guarded_by_the_light_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_guarded_by_the_light_SpellScript();
        }
};

// 6940 - Hand of Sacrifice
class spell_pal_hand_of_sacrifice : public SpellScriptLoader
{
    public:
        spell_pal_hand_of_sacrifice() : SpellScriptLoader("spell_pal_hand_of_sacrifice") { }

        class spell_pal_hand_of_sacrifice_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_hand_of_sacrifice_AuraScript);

            int32 remainingAmount;

            bool Load()
            {
                if (Unit* caster = GetCaster())
                {
                    remainingAmount = caster->GetMaxHealth();
                    return true;
                }
                return false;
            }

            void Split(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & splitAmount)
            {
                remainingAmount -= splitAmount;

                if (remainingAmount <= 0)
                {
                    GetTarget()->RemoveAura(SPELL_PALADIN_HAND_OF_SACRIFICE);
                }
            }

            void Register()
            {
                OnEffectSplit += AuraEffectSplitFn(spell_pal_hand_of_sacrifice_AuraScript::Split, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_hand_of_sacrifice_AuraScript();
        }
};

// 1038 - Hand of Salvation
class spell_pal_hand_of_salvation : public SpellScriptLoader
{
    public:
        spell_pal_hand_of_salvation() : SpellScriptLoader("spell_pal_hand_of_salvation") { }

        class spell_pal_hand_of_salvation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_hand_of_salvation_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Glyph of Salvation
                    if (caster->GetGUID() == GetUnitOwner()->GetGUID())
                        if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_SALVATION, EFFECT_0))
                            amount -= aurEff->GetAmount();
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_hand_of_salvation_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_hand_of_salvation_AuraScript();
        }
};

// -20473 - Holy Shock
class spell_pal_holy_shock : public SpellScriptLoader
{
    public:
        spell_pal_holy_shock() : SpellScriptLoader("spell_pal_holy_shock") { }

        class spell_pal_holy_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_SpellScript);

            bool Validate(SpellInfo const* spell)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_SHOCK_R1))
                    return false;

                // can't use other spell than holy shock due to spell_ranks dependency
                if (sSpellMgr->GetFirstSpellInChain(SPELL_PALADIN_HOLY_SHOCK_R1) != sSpellMgr->GetFirstSpellInChain(spell->Id))
                    return false;

                uint8 rank = sSpellMgr->GetSpellRank(spell->Id);
                if (!sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE, rank, true) || !sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_HEALING, rank, true))
                    return false;

                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    uint8 rank = sSpellMgr->GetSpellRank(GetSpellInfo()->Id);
                    if (caster->IsFriendlyTo(unitTarget))
                        caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_HEALING, rank), true, 0);
                    else
                        caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE, rank), true, 0);
                }
            }

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                {
                    if (!caster->IsFriendlyTo(target))
                    {
                        if (!caster->IsValidAttackTarget(target))
                            return SPELL_FAILED_BAD_TARGETS;

                        if (!caster->isInFront(target))
                            return SPELL_FAILED_UNIT_NOT_INFRONT;
                    }
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_holy_shock_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_pal_holy_shock_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_SpellScript();
        }
};

// 20425 - Judgement of Command
class spell_pal_judgement_of_command : public SpellScriptLoader
{
    public:
        spell_pal_judgement_of_command() : SpellScriptLoader("spell_pal_judgement_of_command") { }

        class spell_pal_judgement_of_command_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_judgement_of_command_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                    if (SpellInfo const* spell_proto = sSpellMgr->GetSpellInfo(GetEffectValue()))
                        GetCaster()->CastSpell(unitTarget, spell_proto, true, NULL);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_judgement_of_command_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_judgement_of_command_SpellScript();
        }
};

// -633 - Lay on Hands
class spell_pal_lay_on_hands : public SpellScriptLoader
{
    public:
        spell_pal_lay_on_hands() : SpellScriptLoader("spell_pal_lay_on_hands") { }

        class spell_pal_lay_on_hands_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_lay_on_hands_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_FORBEARANCE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_AVENGING_WRATH_MARKER))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_IMMUNE_SHIELD_MARKER))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                    if (caster == target)
                        if (target->HasAura(SPELL_PALADIN_FORBEARANCE) || target->HasAura(SPELL_PALADIN_AVENGING_WRATH_MARKER) || target->HasAura(SPELL_PALADIN_IMMUNE_SHIELD_MARKER))
                            return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleScript()
            {
                Unit* caster = GetCaster();
                if (caster == GetHitUnit())
                {
                    caster->CastSpell(caster, SPELL_PALADIN_FORBEARANCE, true);
                    caster->CastSpell(caster, SPELL_PALADIN_AVENGING_WRATH_MARKER, true);
                    caster->CastSpell(caster, SPELL_PALADIN_IMMUNE_SHIELD_MARKER, true);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_lay_on_hands_SpellScript::CheckCast);
                AfterHit += SpellHitFn(spell_pal_lay_on_hands_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_lay_on_hands_SpellScript();
        }
};

// 31789 - Righteous Defense
class spell_pal_righteous_defense : public SpellScriptLoader
{
    public:
        spell_pal_righteous_defense() : SpellScriptLoader("spell_pal_righteous_defense") { }

        class spell_pal_righteous_defense_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_righteous_defense_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_RIGHTEOUS_DEFENSE_TAUNT))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (Unit* target = GetExplTargetUnit())
                {
                    if (!target->IsFriendlyTo(caster) || target->getAttackers().empty())
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void HandleTriggerSpellLaunch(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
            }

            void HandleTriggerSpellHit(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, SPELL_PALADIN_RIGHTEOUS_DEFENSE_TAUNT, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_righteous_defense_SpellScript::CheckCast);
                //! WORKAROUND
                //! target select will be executed in hitphase of effect 0
                //! so we must handle trigger spell also in hit phase (default execution in launch phase)
                //! see issue #3718
                OnEffectLaunchTarget += SpellEffectFn(spell_pal_righteous_defense_SpellScript::HandleTriggerSpellLaunch, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_pal_righteous_defense_SpellScript::HandleTriggerSpellHit, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_righteous_defense_SpellScript();
        }
};

// 85285 - Sacred Shield
class spell_pal_sacred_shield : public SpellScriptLoader
{
    public:
        spell_pal_sacred_shield() : SpellScriptLoader("spell_pal_sacred_shield") { }

        class spell_pal_sacred_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_sacred_shield_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (!caster->HealthBelowPct(30))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_sacred_shield_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_sacred_shield_SpellScript();
        }
};

// 85256 - Templar's Verdict
/// Updated 5.1.0
class spell_pal_templar_s_verdict : public SpellScriptLoader
{
    public:
        spell_pal_templar_s_verdict() : SpellScriptLoader("spell_pal_templar_s_verdict") { }

        class spell_pal_templar_s_verdict_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_templar_s_verdict_SpellScript);

            bool Validate (SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_DIVINE_PURPOSE_PROC))
                    return false;

                return true;
            }

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                if (GetCaster()->ToPlayer()->getClass() != CLASS_PALADIN)
                    return false;

                return true;
            }

            void ChangeDamage(SpellEffIndex /*effIndex*/)
            {                
                Unit* caster = GetCaster();
				uint8 level = caster->getLevel();
				int32 damage = int32((level*(19 - 0.134f*(level-1))) + (caster->GetTotalAttackPowerValue(BASE_ATTACK) * (275.0f /100.0f)));
                SetHitDamage(damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_templar_s_verdict_SpellScript::ChangeDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_templar_s_verdict_SpellScript();
        }
};

// 20154, 21084 - Seal of Righteousness - melee proc dummy (addition ${$MWS*(0.022*$AP+0.044*$SPH)} damage)
class spell_pal_seal_of_righteousness : public SpellScriptLoader
{
    public:
        spell_pal_seal_of_righteousness() : SpellScriptLoader("spell_pal_seal_of_righteousness") { }

        class spell_pal_seal_of_righteousness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_seal_of_righteousness_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS))
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

                float ap = GetTarget()->GetTotalAttackPowerValue(BASE_ATTACK);
                int32 holy = GetTarget()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY);
                holy += eventInfo.GetProcTarget()->SpellBaseDamageBonusTaken(SPELL_SCHOOL_MASK_HOLY);
                int32 bp = int32((ap * 0.022f + 0.044f * holy) * GetTarget()->GetAttackTime(BASE_ATTACK) / 1000);
                GetTarget()->CastCustomSpell(SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS, SPELLVALUE_BASE_POINT0, bp, eventInfo.GetProcTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_pal_seal_of_righteousness_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_pal_seal_of_righteousness_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_seal_of_righteousness_AuraScript();
        }
};

/* MUST APPLY IN DATABASE :

DELETE FROM `spell_proc_event` WHERE `entry` IN (87172); 
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES (87172, 0, 10, 8388608, 0, 0, 272, 0, 0, 100, 0);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (20271, 'spell_pal_long_arm_of_the_law'); 
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (20271, 'spell_pal_judgment'); 

*/

// 20271 - judgment
class spell_pal_judgment : public SpellScriptLoader
{
    public:
        spell_pal_judgment() : SpellScriptLoader("spell_pal_judgment") { }

        class spell_pal_judgment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_judgment_SpellScript);
	
			SpellCastResult CheckCast()
			{
				if(Unit* caster = GetCaster())
				    if (Unit* target = GetHitUnit())
					{
						 if(caster->GetDistance(target) > 30.0f)
							return SPELL_FAILED_OUT_OF_RANGE; 
						else
							return SPELL_CAST_OK;
					}
				return SPELL_CAST_OK;
			}

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
			    uint8 level = caster->getLevel();
				int32 baseDamage = int32((level*(20 + 0.034*(level-5))) + (caster->GetTotalAttackPowerValue(BASE_ATTACK)*(38.0f/100.0f)) + (caster->GetTotalSpellPowerValue(SPELL_SCHOOL_MASK_NORMAL, false)*(54.6f/100.0f)));
				SetHitDamage(baseDamage);

				//Judgment of the Bold
				if (caster->HasAura(111529))
				{
					caster->CastSpell(caster, 111528, true);
					caster->CastSpell(GetHitUnit(), 81326, true);
				}

				//Long arm of the law
				if (caster->HasAura(87172))
					caster->CastSpell(caster, 87173, true);

				//Judgment of the Wise
				if (caster->HasAura(105424))
				{
					caster->CastSpell(caster, 105427, true);
				}
            }
			
            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_judgment_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
				OnCheckCast += SpellCheckCastFn(spell_pal_judgment_SpellScript::CheckCast);
			}
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_judgment_SpellScript();
        }
};


// Word of Glory ; id = 85673
// 4.3.4

/*

MUST APPLY THIS TO DATABASE :

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (85673, 'spell_pal_word_of_glory');

*/

// Word of Glory - 85673
class spell_pal_word_of_glory : public SpellScriptLoader
{
    public:
        spell_pal_word_of_glory() : SpellScriptLoader("spell_pal_word_of_glory") { }

        class spell_pal_word_of_glory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_word_of_glory_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_WORD_OF_GLORY))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if ((unitTarget->GetTypeId() != TYPEID_PLAYER && !unitTarget->isPet()) || unitTarget->IsHostileTo(_player))
                            unitTarget = _player;

                        int32 holyPower = _player->GetPower(POWER_HOLY_POWER);

                        if (holyPower > 2)
                            holyPower = 2;

                        _player->CastSpell(unitTarget, SPELL_PALADIN_WORD_OF_GLORY_HEAL, true);

                        if (_player->HasAura(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY))
                        {
                            Aura* aura = _player->AddAura(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY_DAMAGE, _player);

                            if (aura)
                            {
                                aura->GetEffect(0)->ChangeAmount(aura->GetEffect(0)->GetAmount() * (holyPower + 1));
                                aura->SetNeedClientUpdateForTargets();
                            }
                        }

                        if (!_player->HasAura(SPELL_PALADIN_DIVINE_PURPOSE))
                            _player->ModifyPower(POWER_HOLY_POWER, -holyPower);

                        // Item - Paladin PvP Set Holy 4P Bonus
                        if (_player->HasAura(PALADIN_ITEM_PVP_HOLY_4P_BONUS) && holyPower == 2)
                            _player->ModifyPower(POWER_HOLY_POWER, 1);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_word_of_glory_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_word_of_glory_SpellScript();
        }
};

// Inquisition - 84963
class spell_pal_inquisition : public SpellScriptLoader
{
    public:
        spell_pal_inquisition() : SpellScriptLoader("spell_pal_inquisition") { }

        class spell_pal_inquisition_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_inquisition_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* inquisition = _player->GetAura(84963))
                    {
                        int32 holyPower = _player->GetPower(POWER_HOLY_POWER);

                        if (holyPower > 2)
                            holyPower = 2;

                        if (_player->HasAura(90174))
                            holyPower = 2;

                        int32 maxDuration = inquisition->GetMaxDuration();
                        int32 newDuration = inquisition->GetDuration() + maxDuration * holyPower;
                        inquisition->SetDuration(newDuration);

                        if (newDuration > maxDuration)
                            inquisition->SetMaxDuration(newDuration);

                        if (!_player->HasAura(90174))
                            _player->SetPower(POWER_HOLY_POWER, _player->GetPower(POWER_HOLY_POWER) - holyPower);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_inquisition_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_inquisition_SpellScript();
        }
};

// Execution Sentence - 114157
class spell_pal_execution_sentence : public SpellScriptLoader
{
    public:
        spell_pal_execution_sentence() : SpellScriptLoader("spell_pal_execution_sentence") { }

        class spell_pal_execution_sentence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_execution_sentence_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                            _player->CastSpell(target, PALADIN_SPELL_EXECUTION_SENTENCE, true);
                        else if (_player->GetGUID() == target->GetGUID())
                            _player->CastSpell(_player, PALADIN_SPELL_STAY_OF_EXECUTION, true);
                        else
                            _player->CastSpell(target, PALADIN_SPELL_STAY_OF_EXECUTION, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_execution_sentence_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_execution_sentence_SpellScript();
        }
};

// called by Holy Prism (damage) - 114852 or Holy Prism (heal) - 114871
// Holy Prism visual for other targets
class spell_pal_holy_prism_visual : public SpellScriptLoader
{
    public:
        spell_pal_holy_prism_visual() : SpellScriptLoader("spell_pal_holy_prism_visual") { }

        class spell_pal_holy_prism_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_visual_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                        }
                        else
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_visual_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_visual_SpellScript();
        }
};

// called by Holy Prism (visual damage) - 114862 or Holy Prism (visual heal) - 121551
// Holy Prism (damage) - 114852 or Holy Prism (heal) - 114871
class spell_pal_holy_prism_effect : public SpellScriptLoader
{
    public:
        spell_pal_holy_prism_effect() : SpellScriptLoader("spell_pal_holy_prism_effect") { }

        class spell_pal_holy_prism_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_effect_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // damage
                        if (GetSpellInfo()->Id == 114862)
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_ENNEMIES, true);
                        // heal
                        else if (GetSpellInfo()->Id == 121551)
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_ALLIES, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_effect_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_effect_SpellScript();
        }
};

// Holy Prism - 114165
class spell_pal_holy_prism : public SpellScriptLoader
{
    public:
        spell_pal_holy_prism() : SpellScriptLoader("spell_pal_holy_prism") { }

        class spell_pal_holy_prism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                        }
                        else
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_SpellScript();
        }
};

// Selfless healer - 85804
// Called by flash of light - 19750
class spell_pal_selfless_healer : public SpellScriptLoader
{
    public:
        spell_pal_selfless_healer() : SpellScriptLoader("spell_pal_selfless_healer") { }

        class spell_pal_selfless_healer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_selfless_healer_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(PALADIN_SPELL_SELFLESS_HEALER_STACK))
                        {
                            int32 charges = _player->GetAura(PALADIN_SPELL_SELFLESS_HEALER_STACK)->GetStackAmount();

                            if (_player->IsValidAssistTarget(target) && target != _player)
                                SetHitHeal(int32(GetHitHeal() + ((GetHitHeal() * 0.35f) * charges)));
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_selfless_healer_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_selfless_healer_SpellScript();
        }
};

// Called by Flash of Light - 19750 and Divine Light - 82326
// Tower of Radiance - 85512
class spell_pal_tower_of_radiance : public SpellScriptLoader
{
    public:
        spell_pal_tower_of_radiance() : SpellScriptLoader("spell_pal_tower_of_radiance") { }

        class spell_pal_tower_of_radiance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_tower_of_radiance_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(PALADIN_SPELL_TOWER_OF_RADIANCE))
                            if (target->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT, _player->GetGUID()))
                                _player->EnergizeBySpell(_player, PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE, 1, POWER_HOLY_POWER);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_tower_of_radiance_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_tower_of_radiance_SpellScript();
        }
};

void AddSC_paladin_spell_scripts()
{
    //new spell_pal_ardent_defender();
    new spell_pal_blessing_of_faith();
    new spell_pal_blessing_of_sanctuary();
    new spell_pal_divine_sacrifice();
    new spell_pal_divine_storm();
    new spell_pal_divine_storm_dummy();
    new spell_pal_exorcism_and_holy_wrath_damage();
    new spell_pal_eye_for_an_eye();
    new spell_pal_guarded_by_the_light();
    new spell_pal_hand_of_sacrifice();
    new spell_pal_hand_of_salvation();
    new spell_pal_holy_shock();
    new spell_pal_judgement_of_command();
    new spell_pal_lay_on_hands();
    new spell_pal_righteous_defense();
    new spell_pal_sacred_shield();
    new spell_pal_templar_s_verdict();
    new spell_pal_seal_of_righteousness();
	new spell_pal_word_of_glory();
	new spell_pal_judgment();
	new spell_pal_exorcism();
	new spell_pal_hammer_of_wrath();
	new spell_pal_inquisition();
	new spell_pal_execution_sentence();
	new spell_pal_holy_prism();
	new spell_pal_holy_prism_visual();
	new spell_pal_holy_prism_effect();
	new spell_pal_selfless_healer();
	new spell_pal_tower_of_radiance();
}
