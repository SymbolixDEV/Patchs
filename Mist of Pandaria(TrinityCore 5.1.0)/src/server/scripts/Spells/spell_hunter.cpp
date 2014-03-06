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
 * Scripts for spells with SPELLFAMILY_HUNTER, SPELLFAMILY_PET and SPELLFAMILY_GENERIC spells used by hunter players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_hun_".
 */

#include "Pet.h"
#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum HunterSpells
{
    SPELL_HUNTER_ASPECT_OF_THE_BEAST_PET            = 61669,
    SPELL_HUNTER_ASPECT_OF_THE_VIPER_ENERGIZE       = 34075,
    SPELL_HUNTER_BESTIAL_WRATH                      = 19574,
    SPELL_HUNTER_CHIMERA_SHOT                       = 53209,
    SPELL_HUNTER_CHIMERA_SHOT_HEAL                  = 53353,
    /*SPELL_HUNTER_CHIMERA_SHOT_SERPENT               = 53353,*/    
	SPELL_HUNTER_CHIMERA_SHOT_VIPER                 = 53358,
    SPELL_HUNTER_CHIMERA_SHOT_SCORPID               = 53359,
    SPELL_HUNTER_GLYPH_OF_ASPECT_OF_THE_VIPER       = 56851,
    SPELL_HUNTER_INVIGORATION_TRIGGERED             = 53398,
    SPELL_HUNTER_MASTERS_CALL_TRIGGERED             = 62305,
    SPELL_HUNTER_MISDIRECTION_PROC                  = 35079,
    SPELL_HUNTER_PET_LAST_STAND_TRIGGERED           = 53479,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX           = 55709,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED = 54114,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF    = 55711,
    SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED       = 54045,
    SPELL_HUNTER_READINESS                          = 23989,
    SPELL_HUNTER_SNIPER_TRAINING_R1                 = 53302,
    SPELL_HUNTER_SNIPER_TRAINING_BUFF_R1            = 64418,
	SPELL_HUNTER_SERPENT_STING                      = 1978,
	SPELL_HUNTER_STEADY_SHOT_FOCUS                  = 77443,
	SPELL_HUNTER_COBRA_FOCUS                        = 91954,
    SPELL_DRAENEI_GIFT_OF_THE_NAARU                 = 59543,
	HUNTER_SPELL_POSTHASTE                          = 109215,
    HUNTER_SPELL_POSTHASTE_INCREASE_SPEED           = 118922,
    HUNTER_SPELL_NARROW_ESCAPE                      = 109298,
    HUNTER_SPELL_NARROW_ESCAPE_RETS                 = 128405,
	HUNTER_SPELL_A_MURDER_OF_CROWS_SUMMON           = 129179,
    HUNTER_NPC_MURDER_OF_CROWS                      = 61994,
    HUNTER_SPELL_DIRE_BEAST                         = 120679,
    DIRE_BEAST_JADE_FOREST                          = 121118,
    DIRE_BEAST_KALIMDOR                             = 122802,
    DIRE_BEAST_EASTERN_KINGDOMS                     = 122804,
    DIRE_BEAST_OUTLAND                              = 122806,
    DIRE_BEAST_NORTHREND                            = 122807,
    DIRE_BEAST_KRASARANG_WILDS                      = 122809,
    DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS             = 122811,
    DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM          = 126213,
    DIRE_BEAST_KUN_LAI_SUMMIT                       = 126214,
    DIRE_BEAST_TOWNLONG_STEPPES                     = 126215,
    DIRE_BEAST_DREAD_WASTES                         = 126216,
    DIRE_BEAST_DUNGEONS                             = 132764,
	HUNTER_SPELL_POWERSHOT                          = 109259,
	HUNTER_SPELL_GLAIVE_TOSS_DAMAGES                = 121414,
    HUNTER_SPELL_GLAIVE_TOSS                        = 117050,
	HUNTER_SPELL_GLAIVE_TOSS_AURA                   = 117050,
    HUNTER_SPELL_GLAIVE_TOSS_RIGHT                  = 120755,
    HUNTER_SPELL_GLAIVE_TOSS_LEFT                   = 120756,
    HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT  = 120761,
    HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT = 121414,
	HUNTER_SPELL_BLINK_STRIKE                       = 130393,
	HUNTER_SPELL_BINDING_SHOT_AREA                  = 109248,
    HUNTER_SPELL_BINDING_SHOT_LINK                  = 117405,
    HUNTER_SPELL_BINDING_SHOT_STUN                  = 117526,
    HUNTER_SPELL_BINDING_SHOT_IMMUNE                = 117553,
	HUNTER_SPELL_LYNX_RUSH_AURA                     = 120697,
    HUNTER_SPELL_LYNX_CRUSH_DAMAGE                  = 120699,
};

// 13161 - Aspect of the Beast
class spell_hun_aspect_of_the_beast : public SpellScriptLoader
{
    public:
        spell_hun_aspect_of_the_beast() : SpellScriptLoader("spell_hun_aspect_of_the_beast") { }

        class spell_hun_aspect_of_the_beast_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_aspect_of_the_beast_AuraScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_ASPECT_OF_THE_BEAST_PET))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    if (Pet* pet = caster->GetPet())
                        pet->RemoveAurasDueToSpell(SPELL_HUNTER_ASPECT_OF_THE_BEAST_PET);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    if (caster->GetPet())
                        caster->CastSpell(caster, SPELL_HUNTER_ASPECT_OF_THE_BEAST_PET, true);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_hun_aspect_of_the_beast_AuraScript::OnApply, EFFECT_0, SPELL_AURA_UNTRACKABLE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_aspect_of_the_beast_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_UNTRACKABLE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_aspect_of_the_beast_AuraScript();
        }
};

// 34074 - Aspect of the Viper
class spell_hun_ascpect_of_the_viper : public SpellScriptLoader
{
    public:
        spell_hun_ascpect_of_the_viper() : SpellScriptLoader("spell_hun_ascpect_of_the_viper") { }

        class spell_hun_ascpect_of_the_viper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_ascpect_of_the_viper_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_ASPECT_OF_THE_VIPER_ENERGIZE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_GLYPH_OF_ASPECT_OF_THE_VIPER))
                    return false;
                return true;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();

                uint32 maxMana = GetTarget()->GetMaxPower(POWER_MANA);
                int32 mana = CalculatePct(maxMana, GetTarget()->GetAttackTime(RANGED_ATTACK) / 1000.0f);

                if (AuraEffect const* glyph = GetTarget()->GetAuraEffect(SPELL_HUNTER_GLYPH_OF_ASPECT_OF_THE_VIPER, EFFECT_0))
                    AddPct(mana, glyph->GetAmount());

                GetTarget()->CastCustomSpell(SPELL_HUNTER_ASPECT_OF_THE_VIPER_ENERGIZE, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_hun_ascpect_of_the_viper_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_OBS_MOD_POWER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_ascpect_of_the_viper_AuraScript();
        }
};

// 53209 - Chimera Shot
/*class spell_hun_chimera_shot : public SpellScriptLoader (deprecated, wrong script)
{
    public:
        spell_hun_chimera_shot() : SpellScriptLoader("spell_hun_chimera_shot") { }

        class spell_hun_chimera_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_chimera_shot_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_CHIMERA_SHOT_SERPENT) || !sSpellMgr->GetSpellInfo(SPELL_HUNTER_CHIMERA_SHOT_VIPER) || !sSpellMgr->GetSpellInfo(SPELL_HUNTER_CHIMERA_SHOT_SCORPID))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    uint32 spellId = 0;
                    int32 basePoint = 0;
                    Unit::AuraApplicationMap& Auras = unitTarget->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator i = Auras.begin(); i != Auras.end(); ++i)
                    {
                        Aura* aura = i->second->GetBase();
                        if (aura->GetCasterGUID() != caster->GetGUID())
                            continue;

                        // Search only Serpent Sting, Viper Sting, Scorpid Sting auras
                        flag96 familyFlag = aura->GetSpellInfo()->SpellFamilyFlags;
                        if (!(familyFlag[1] & 0x00000080 || familyFlag[0] & 0x0000C000))
                            continue;
                        if (AuraEffect const* aurEff = aura->GetEffect(0))
                        {
                            // Serpent Sting - Instantly deals 40% of the damage done by your Serpent Sting.
                            if (familyFlag[0] & 0x4000)
                            {
                                int32 TickCount = aurEff->GetTotalTicks();
                                spellId = SPELL_HUNTER_CHIMERA_SHOT_SERPENT;
                                basePoint = caster->SpellDamageBonusDone(unitTarget, aura->GetSpellInfo(), aurEff->GetAmount(), DOT, aura->GetStackAmount());
                                ApplyPct(basePoint, TickCount * 40);
                                basePoint = unitTarget->SpellDamageBonusTaken(caster, aura->GetSpellInfo(), basePoint, DOT, aura->GetStackAmount());
                            }
                            // Viper Sting - Instantly restores mana to you equal to 60% of the total amount drained by your Viper Sting.
                            else if (familyFlag[1] & 0x00000080)
                            {
                                int32 TickCount = aura->GetEffect(0)->GetTotalTicks();
                                spellId = SPELL_HUNTER_CHIMERA_SHOT_VIPER;

                                // Amount of one aura tick
                                basePoint = int32(CalculatePct(unitTarget->GetMaxPower(POWER_MANA), aurEff->GetAmount()));
                                int32 casterBasePoint = aurEff->GetAmount() * unitTarget->GetMaxPower(POWER_MANA) / 50; // TODO: WTF? caster uses unitTarget?
                                if (basePoint > casterBasePoint)
                                    basePoint = casterBasePoint;
                                ApplyPct(basePoint, TickCount * 60);
                            }
                            // Scorpid Sting - Attempts to Disarm the target for 10 sec. This effect cannot occur more than once per 1 minute.
                            else if (familyFlag[0] & 0x00008000)
                                spellId = SPELL_HUNTER_CHIMERA_SHOT_SCORPID;
                            // ?? nothing say in spell desc (possibly need addition check)
                            //if (familyFlag & 0x0000010000000000LL || // dot
                            //    familyFlag & 0x0000100000000000LL)   // stun
                            //{
                            //    spellId = 53366; // 53366 Chimera Shot - Wyvern
                            //}

                            // Refresh aura duration
                            aura->RefreshDuration();
                        }
                        break;
                    }
                    if (spellId)
                        caster->CastCustomSpell(unitTarget, spellId, &basePoint, 0, 0, true);
                    if (spellId == SPELL_HUNTER_CHIMERA_SHOT_SCORPID && caster->ToPlayer()) // Scorpid Sting - Add 1 minute cooldown
                        caster->ToPlayer()->AddSpellCooldown(spellId, 0, uint32(time(NULL) + 60));
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_chimera_shot_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_chimera_shot_SpellScript();
        }
};*/

class spell_hun_chimera_shot : public SpellScriptLoader
{
public :
    spell_hun_chimera_shot() : SpellScriptLoader("spell_hun_chimera_shot")
    {

    }

    class spell_hun_chimera_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_chimera_shot_SpellScript);

        bool Validate(const SpellInfo *spellInfo)
        {
            return true ;
        }

        bool Load()
        {
            return true ;
        }

        void handleMiscOnEffectScriptEffect(SpellEffIndex effIndex)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : handleMiscOnEffectScriptEffect");
            if(GetCaster())
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : caster as unit is not null");
                if(Player* caster = GetCaster()->ToPlayer())
                {
                    // Reset Serpent Sting duration && apply Mark of the Hunter
                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : caster as player is not null");
                    if(Unit* target = GetExplTargetUnit())
                    {
                        sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : target is not null");
                        if(Aura* serpentSting = target->GetAura(118253))
                        {
                            sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : target has aura serpent sting ; refreshing it");
                            serpentSting->RefreshDuration();
                        }

                        sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : casting mark of the hunter");
                        // Mark of the Hunter
                        caster->CastSpell(target, 1130, TRIGGERED_FULL_MASK);
                    }

                    // Heal the caster
                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : calculating hp");
                    float healthToRestore = caster->GetMaxHealth();
                    if(caster->HasAura(119447))
						healthToRestore = (healthToRestore / 100 * 5);
					else
						healthToRestore = (healthToRestore / 100 * 3);

					healthToRestore += caster->GetHealth();

                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Chimera Shot : life going to be set to %f", healthToRestore);
                    caster->SetHealth((uint32)healthToRestore);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_chimera_shot_SpellScript::handleMiscOnEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_hun_chimera_shot_SpellScript();
    }
};

// 781 - Disengage
class spell_hun_disengage : public SpellScriptLoader
{
    public:
        spell_hun_disengage() : SpellScriptLoader("spell_hun_disengage") { }

        class spell_hun_disengage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_disengage_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetTypeId() == TYPEID_PLAYER && !caster->isInCombat())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

			
            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(HUNTER_SPELL_POSTHASTE))
                    {
                        _player->RemoveMovementImpairingAuras();
                        _player->CastSpell(_player, HUNTER_SPELL_POSTHASTE_INCREASE_SPEED, true);
                    }
                    else if (_player->HasAura(HUNTER_SPELL_NARROW_ESCAPE))
                    {
                        std::list<Unit*> unitList;
                        std::list<Unit*> retsList;

                        _player->GetAttackableUnitListInRange(unitList, 8.0f);

						for (std::list<Unit*>::const_iterator i = unitList.begin(); i != unitList.end(); ++i)
                            if (_player->IsValidAttackTarget((*i)))
                                retsList.push_back((*i));

						for (std::list<Unit*>::const_iterator i = retsList.begin(); i != retsList.end(); ++i)
                            _player->CastSpell((*i), HUNTER_SPELL_NARROW_ESCAPE_RETS, true);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_disengage_SpellScript::CheckCast);
                AfterCast += SpellCastFn(spell_hun_disengage_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_disengage_SpellScript();
        }
};

// 53412 - Invigoration
class spell_hun_invigoration : public SpellScriptLoader
{
    public:
        spell_hun_invigoration() : SpellScriptLoader("spell_hun_invigoration") { }

        class spell_hun_invigoration_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_invigoration_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_INVIGORATION_TRIGGERED))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                    if (AuraEffect* aurEff = unitTarget->GetDummyAuraEffect(SPELLFAMILY_HUNTER, 3487, 0))
                        if (roll_chance_i(aurEff->GetAmount()))
                            unitTarget->CastSpell(unitTarget, SPELL_HUNTER_INVIGORATION_TRIGGERED, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_invigoration_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_invigoration_SpellScript();
        }
};

// 53478 - Last Stand Pet
class spell_hun_last_stand_pet : public SpellScriptLoader
{
    public:
        spell_hun_last_stand_pet() : SpellScriptLoader("spell_hun_last_stand_pet") { }

        class spell_hun_last_stand_pet_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_last_stand_pet_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_LAST_STAND_TRIGGERED))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(30));
                caster->CastCustomSpell(caster, SPELL_HUNTER_PET_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_last_stand_pet_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_last_stand_pet_SpellScript();
        }
};

// 53271 - Masters Call
class spell_hun_masters_call : public SpellScriptLoader
{
    public:
        spell_hun_masters_call() : SpellScriptLoader("spell_hun_masters_call") { }

        class spell_hun_masters_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_masters_call_SpellScript);

            bool Validate(SpellInfo const* spellInfo)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MASTERS_CALL_TRIGGERED) || !sSpellMgr->GetSpellInfo(spellInfo->Effects[EFFECT_0].CalcValue()) || !sSpellMgr->GetSpellInfo(spellInfo->Effects[EFFECT_1].CalcValue()))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* ally = GetHitUnit())
                    if (Player* caster = GetCaster()->ToPlayer())
                        if (Pet* target = caster->GetPet())
                        {
                            TriggerCastFlags castMask = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_CASTER_AURASTATE);
                            target->CastSpell(ally, GetEffectValue(), castMask);
                            target->CastSpell(ally, GetSpellInfo()->Effects[EFFECT_0].CalcValue(), castMask);
                        }
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    // Cannot be processed while pet is dead
                    TriggerCastFlags castMask = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_CASTER_AURASTATE);
                    target->CastSpell(target, SPELL_HUNTER_MASTERS_CALL_TRIGGERED, castMask);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_masters_call_SpellScript();
        }
};

// 34477 - Misdirection
class spell_hun_misdirection : public SpellScriptLoader
{
    public:
        spell_hun_misdirection() : SpellScriptLoader("spell_hun_misdirection") { }

        class spell_hun_misdirection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_misdirection_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MISDIRECTION_PROC))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEFAULT)
                    GetTarget()->ResetRedirectThreat();
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                return GetTarget()->GetRedirectThreatTarget();
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(GetTarget(), SPELL_HUNTER_MISDIRECTION_PROC, true, NULL, aurEff);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                DoCheckProc += AuraCheckProcFn(spell_hun_misdirection_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_hun_misdirection_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_misdirection_AuraScript();
        }
};

// 35079 - Misdirection (Proc)
class spell_hun_misdirection_proc : public SpellScriptLoader
{
    public:
        spell_hun_misdirection_proc() : SpellScriptLoader("spell_hun_misdirection_proc") { }

        class spell_hun_misdirection_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_misdirection_proc_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->ResetRedirectThreat();
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_proc_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_misdirection_proc_AuraScript();
        }
};

// 54044 - Pet Carrion Feeder
class spell_hun_pet_carrion_feeder : public SpellScriptLoader
{
    public:
        spell_hun_pet_carrion_feeder() : SpellScriptLoader("spell_hun_pet_carrion_feeder") { }

        class spell_hun_pet_carrion_feeder_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_pet_carrion_feeder_SpellScript);

            bool Load()
            {
                if (!GetCaster()->isPet())
                    return false;
                return true;
            }

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED))
                    return false;
                return true;
            }

            SpellCastResult CheckIfCorpseNear()
            {
                Unit* caster = GetCaster();
                float max_range = GetSpellInfo()->GetMaxRange(false);
                WorldObject* result = NULL;
                // search for nearby enemy corpse in range
                Trinity::AnyDeadUnitSpellTargetInRangeCheck check(caster, max_range, GetSpellInfo(), TARGET_CHECK_ENEMY);
                Trinity::WorldObjectSearcher<Trinity::AnyDeadUnitSpellTargetInRangeCheck> searcher(caster, result, check);
                caster->GetMap()->VisitFirstFound(caster->m_positionX, caster->m_positionY, max_range, searcher);
                if (!result)
                    return SPELL_FAILED_NO_EDIBLE_CORPSES;
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                caster->CastSpell(caster, SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED, false);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_hun_pet_carrion_feeder_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_hun_pet_carrion_feeder_SpellScript::CheckIfCorpseNear);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_pet_carrion_feeder_SpellScript();
        }
};

// 55709 - Pet Heart of the Phoenix
class spell_hun_pet_heart_of_the_phoenix : public SpellScriptLoader
{
    public:
        spell_hun_pet_heart_of_the_phoenix() : SpellScriptLoader("spell_hun_pet_heart_of_the_phoenix") { }

        class spell_hun_pet_heart_of_the_phoenix_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_pet_heart_of_the_phoenix_SpellScript);

            bool Load()
            {
                if (!GetCaster()->isPet())
                    return false;
                return true;
            }

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED) || !sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* owner = caster->GetOwner())
                    if (!caster->HasAura(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                    {
                        owner->CastCustomSpell(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED, SPELLVALUE_BASE_POINT0, 100, caster, true);
                        caster->CastSpell(caster, SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF, true);
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_pet_heart_of_the_phoenix_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_pet_heart_of_the_phoenix_SpellScript();
        }
};

// 23989 - Readiness
class spell_hun_readiness : public SpellScriptLoader
{
    public:
        spell_hun_readiness() : SpellScriptLoader("spell_hun_readiness") { }

        class spell_hun_readiness_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_readiness_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();
                // immediately finishes the cooldown on your other Hunter abilities except Bestial Wrath
                const SpellCooldowns& cm = caster->ToPlayer()->GetSpellCooldownMap();
                for (SpellCooldowns::const_iterator itr = cm.begin(); itr != cm.end();)
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);

                    ///! If spellId in cooldown map isn't valid, the above will return a null pointer.
                    if (spellInfo &&
                        spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER &&
                        spellInfo->Id != SPELL_HUNTER_READINESS &&
                        spellInfo->Id != SPELL_HUNTER_BESTIAL_WRATH &&
                        spellInfo->Id != SPELL_DRAENEI_GIFT_OF_THE_NAARU &&
                        spellInfo->GetRecoveryTime() > 0)
                        caster->RemoveSpellCooldown((itr++)->first, true);
                    else
                        ++itr;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_readiness_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_readiness_SpellScript();
        }
};

// 37506 - Scatter Shot
class spell_hun_scatter_shot : public SpellScriptLoader
{
    public:
        spell_hun_scatter_shot() : SpellScriptLoader("spell_hun_scatter_shot") { }

        class spell_hun_scatter_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_scatter_shot_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();
                // break Auto Shot and autohit
                caster->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
                caster->AttackStop();
                caster->SendAttackSwingCancelAttack();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_scatter_shot_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_scatter_shot_SpellScript();
        }
};

// -53302 - Sniper Training
class spell_hun_sniper_training : public SpellScriptLoader
{
    public:
        spell_hun_sniper_training() : SpellScriptLoader("spell_hun_sniper_training") { }

        class spell_hun_sniper_training_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_sniper_training_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_SNIPER_TRAINING_R1) || !sSpellMgr->GetSpellInfo(SPELL_HUNTER_SNIPER_TRAINING_BUFF_R1))
                    return false;
                return true;
            }

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if (aurEff->GetAmount() <= 0)
                {
                    Unit* caster = GetCaster();
                    uint32 spellId = SPELL_HUNTER_SNIPER_TRAINING_BUFF_R1 + GetId() - SPELL_HUNTER_SNIPER_TRAINING_R1;
                    if (Unit* target = GetTarget())
                        if (!target->HasAura(spellId))
                        {
                            SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(spellId);
                            Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster() ? caster : target;
                            triggerCaster->CastSpell(target, triggeredSpellInfo, true, 0, aurEff);
                        }
                }
            }

            void HandleUpdatePeriodic(AuraEffect* aurEff)
            {
                if (Player* playerTarget = GetUnitOwner()->ToPlayer())
                {
                    int32 baseAmount = aurEff->GetBaseAmount();
                    int32 amount = playerTarget->isMoving() ?
                    playerTarget->CalculateSpellDamage(playerTarget, GetSpellInfo(), aurEff->GetEffIndex(), &baseAmount) :
                    aurEff->GetAmount() - 1;
                    aurEff->SetAmount(amount);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_sniper_training_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_hun_sniper_training_AuraScript::HandleUpdatePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_sniper_training_AuraScript();
        }
};

// 1515 - Tame Beast
class spell_hun_tame_beast : public SpellScriptLoader
{
    public:
        spell_hun_tame_beast() : SpellScriptLoader("spell_hun_tame_beast") { }

        class spell_hun_tame_beast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_tame_beast_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (!GetExplTargetUnit())
                    return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                if (Creature* target = GetExplTargetUnit()->ToCreature())
                {
                    if (target->getLevel() > caster->getLevel())
                        return SPELL_FAILED_HIGHLEVEL;

                    // use SMSG_PET_TAME_FAILURE?
                    if (!target->GetCreatureTemplate()->isTameable(caster->ToPlayer()->CanTameExoticPets()))
                        return SPELL_FAILED_BAD_TARGETS;

                    if (caster->GetPetGUID())
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;

                    if (caster->GetCharmGUID())
                        return SPELL_FAILED_ALREADY_HAVE_CHARM;
                }
                else
                    return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_tame_beast_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_tame_beast_SpellScript();
        }
};

// -24604 - Furious Howl
//  53434 - Call of the Wild
class spell_hun_target_only_pet_and_owner : public SpellScriptLoader
{
    public:
        spell_hun_target_only_pet_and_owner() : SpellScriptLoader("spell_hun_target_only_pet_and_owner") { }

        class spell_hun_target_only_pet_and_owner_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_target_only_pet_and_owner_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();
                targets.push_back(GetCaster());
                if (Unit* owner = GetCaster()->GetOwner())
                    targets.push_back(owner);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_target_only_pet_and_owner_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_target_only_pet_and_owner_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_PARTY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_target_only_pet_and_owner_SpellScript();
        }
};

/* 
EXECUTE THOSE SQL QUERIES: 

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (77767, 'spell_hun_cobra_shot'); 
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (56641, 'spell_hun_steady_shot'); 

*/

// 77767 - Cobra Shot
class spell_hun_cobra_shot : public SpellScriptLoader
{
    public:
        spell_hun_cobra_shot() : SpellScriptLoader("spell_hun_cobra_shot") { }

        class spell_hun_cobra_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_cobra_shot_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_COBRA_FOCUS) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HUNTER_SERPENT_STING))
                    return false;
                return true;
            }

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_COBRA_FOCUS, true);

                if (Aura* aur = GetHitUnit()->GetAura(SPELL_HUNTER_SERPENT_STING, GetCaster()->GetGUID()))
                {
                    int32 newDuration = aur->GetDuration() + GetEffectValue() * IN_MILLISECONDS;
                    aur->SetDuration(std::min(newDuration, aur->GetMaxDuration()), true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_cobra_shot_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_cobra_shot_SpellScript();
        }
};

// 56641 - Steady Shot
class spell_hun_steady_shot : public SpellScriptLoader
{
    public:
        spell_hun_steady_shot() : SpellScriptLoader("spell_hun_steady_shot") { }

        class spell_hun_steady_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_steady_shot_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) 
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_STEADY_SHOT_FOCUS))
                    return false;
                return true;
            }

            bool Load() 
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleOnHit()
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_STEADY_SHOT_FOCUS, true);
            }

            void Register() 
            {
                OnHit += SpellHitFn(spell_hun_steady_shot_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const 
        {
            return new spell_hun_steady_shot_SpellScript();
        }
};

// Dire Beast - 120679
class spell_hun_dire_beast : public SpellScriptLoader
{
    public:
        spell_hun_dire_beast() : SpellScriptLoader("spell_hun_dire_beast") { }

        class spell_hun_dire_beast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_dire_beast_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Summon's skin is different function of Map or Zone ID
                        switch (_player->GetZoneId())
                        {
                            case 5785: // The Jade Forest
                                _player->CastSpell(target, DIRE_BEAST_JADE_FOREST, true);
                                break;
                            case 5805: // Valley of the Four Winds
                                _player->CastSpell(target, DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS, true);
                                break;
                            case 5840: // Vale of Eternal Blossoms
                                _player->CastSpell(target, DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM, true);
                                break;
                            case 5841: // Kun-Lai Summit
                                _player->CastSpell(target, DIRE_BEAST_KUN_LAI_SUMMIT, true);
                                break;
                            case 5842: // Townlong Steppes
                                _player->CastSpell(target, DIRE_BEAST_TOWNLONG_STEPPES, true);
                                break;
                            case 6134: // Krasarang Wilds
                                _player->CastSpell(target, DIRE_BEAST_KRASARANG_WILDS, true);
                                break;
                            case 6138: // Dread Wastes
                                _player->CastSpell(target, DIRE_BEAST_DREAD_WASTES, true);
                                break;
                            default:
                            {
                                switch (_player->GetMapId())
                                {
                                    case 0: // Eastern Kingdoms
                                        _player->CastSpell(target, DIRE_BEAST_EASTERN_KINGDOMS, true);
                                        break;
                                    case 1: // Kalimdor
                                        _player->CastSpell(target, DIRE_BEAST_KALIMDOR, true);
                                        break;
                                    case 8: // Outland
                                        _player->CastSpell(target, DIRE_BEAST_OUTLAND, true);
                                        break;
                                    case 10: // Northrend
                                        _player->CastSpell(target, DIRE_BEAST_NORTHREND, true);
                                        break;
                                    default:
                                        if (_player->GetMap()->IsDungeon())
                                            _player->CastSpell(target, DIRE_BEAST_DUNGEONS, true);
                                        break;
                                }
                                break;
                            }
                        }
                    }
                }
            }

            void Register()
            {
               OnHit += SpellHitFn(spell_hun_dire_beast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_dire_beast_SpellScript();
        }
};

// A Murder of Crows - 131894
class spell_hun_a_murder_of_crows : public SpellScriptLoader
{
    public:
        spell_hun_a_murder_of_crows() : SpellScriptLoader("spell_hun_a_murder_of_crows") { }

        class spell_hun_a_murder_of_crows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_a_murder_of_crows_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    if (!GetCaster())
                        return;

                    if (aurEff->GetTickNumber() > 15)
                        return;

                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        _player->CastSpell(target, HUNTER_SPELL_A_MURDER_OF_CROWS_SUMMON, true);

                        std::list<Creature*> tempList;
                        std::list<Creature*> crowsList;

                        _player->GetCreatureListWithEntryInGrid(tempList, HUNTER_NPC_MURDER_OF_CROWS, 100.0f);

						for (std::list<Creature*>::const_iterator i = tempList.begin(); i != tempList.end(); ++i)
                            crowsList.push_back((*i));

                        // Remove other players Crows
                        for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                        {
                            Unit* owner = (*i)->GetOwner();
                            if (owner && owner == _player && (*i)->isSummon())
                                continue;

                            crowsList.remove((*i));
                        }

						for (std::list<Creature*>::const_iterator i = crowsList.begin(); i != crowsList.end(); ++i)
                            (*i)->AI()->AttackStart(target);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_a_murder_of_crows_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_a_murder_of_crows_AuraScript();
        }
};

// Powershot - 109259
class spell_hun_powershot : public SpellScriptLoader
{
    public:
        spell_hun_powershot() : SpellScriptLoader("spell_hun_powershot") { }

        class spell_hun_powershot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_powershot_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> tempUnitMap;
                        _player->GetAttackableUnitListInRange(tempUnitMap, _player->GetDistance(target));

						for (std::list<Unit*>::const_iterator i = tempUnitMap.begin(); i != tempUnitMap.end(); ++i)
                        {
                            if (!(*i)->IsValidAttackTarget(_player))
                                continue;

                            if ((*i)->GetGUID() == _player->GetGUID())
                                continue;

                            if (!(*i)->IsInBetween(_player, target, 1.0f))
                                continue;

                            SpellNonMeleeDamage damageInfo(_player, (*i), GetSpellInfo()->Id, GetSpellInfo()->SchoolMask);
                            damageInfo.damage = int32(GetHitDamage() / 2);
                            _player->SendSpellNonMeleeDamageLog(&damageInfo);
                            _player->DealSpellDamage(&damageInfo, true);

                            if (Creature* creatureTarget = (*i)->ToCreature())
                                if (creatureTarget->isWorldBoss() || creatureTarget->IsDungeonBoss())
                                    continue;

                            if ((*i)->GetTypeId() == TYPEID_PLAYER)
                                if ((*i)->ToPlayer()->GetKnockBackTime())
                                    continue;

                            // Instantly interrupt non melee spells being casted
                            if ((*i)->IsNonMeleeSpellCasted(true))
                                (*i)->InterruptNonMeleeSpells(true);

                            float ratio = 0.1f;
                            float speedxy = float(GetSpellInfo()->Effects[EFFECT_1].MiscValue) * ratio;
                            float speedz = float(GetSpellInfo()->Effects[EFFECT_1].BasePoints) * ratio;
                            if (speedxy < 0.1f && speedz < 0.1f)
                                return;

                            float x, y;
                            _player->GetPosition(x, y);

                            (*i)->KnockbackFrom(x, y, speedxy, speedz);

                            if ((*i)->GetTypeId() == TYPEID_PLAYER)
                                (*i)->ToPlayer()->SetKnockBackTime(getMSTime());
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_hun_powershot_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_powershot_SpellScript();
        }
};

// Glaive Toss (damage) - 120761 and 121414
class spell_hun_glaive_toss_damage : public SpellScriptLoader
{
    public:
        spell_hun_glaive_toss_damage() : SpellScriptLoader("spell_hun_glaive_toss_damage") { }

        class spell_hun_glaive_toss_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glaive_toss_damage_SpellScript);

            uint64 mainTargetGUID;

            bool Load()
            {
                mainTargetGUID = 0;
                return true;
            }

            void CorrectDamageRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                std::list<Unit*> targetList;
                float radius = 50.0f;

                Trinity::NearestAttackableUnitInObjectRangeCheck u_check(GetCaster(), GetCaster(), radius);
                Trinity::UnitListSearcher<Trinity::NearestAttackableUnitInObjectRangeCheck> searcher(GetCaster(), targetList, u_check);
                GetCaster()->VisitNearbyObject(radius, searcher);

				for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                {
                    if ((*i)->HasAura(HUNTER_SPELL_GLAIVE_TOSS_AURA))
                    {
                        mainTargetGUID = (*i)->GetGUID();
                        break;
                    }
                }

                if (!mainTargetGUID)
                    return;

                Unit* target = ObjectAccessor::FindUnit(mainTargetGUID);
                if (!target)
                    return;

                targets.push_back(target);

                for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                    if ((*i)->IsInBetween(GetCaster(), target, 5.0f))
                        targets.push_back((*i));
            }

            void CorrectSnareRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                std::list<Unit*> targetList;
                float radius = 50.0f;

                Trinity::NearestAttackableUnitInObjectRangeCheck u_check(GetCaster(), GetCaster(), radius);
                Trinity::UnitListSearcher<Trinity::NearestAttackableUnitInObjectRangeCheck> searcher(GetCaster(), targetList, u_check);
                GetCaster()->VisitNearbyObject(radius, searcher);

                for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                {
                    if ((*i)->HasAura(HUNTER_SPELL_GLAIVE_TOSS_AURA))
                    {
                        mainTargetGUID = (*i)->GetGUID();
                        break;
                    }
                }

                if (!mainTargetGUID)
                    return;

                if (!mainTargetGUID)
                    return;

                Unit* target = ObjectAccessor::FindUnit(mainTargetGUID);
                if (!target)
                    return;

                targets.push_back(target);

                for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                    if ((*i)->IsInBetween(GetCaster(), target, 5.0f))
                        targets.push_back((*i));
            }

            void OnDamage()
            {
                if (!mainTargetGUID)
                    return;

                Unit* target = ObjectAccessor::FindUnit(mainTargetGUID);
                if (!target)
                    return;

                if (GetHitUnit())
                    if (GetHitUnit() == target)
                        SetHitDamage(GetHitDamage() * 4);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectDamageRange, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectSnareRange, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
                OnHit += SpellHitFn(spell_hun_glaive_toss_damage_SpellScript::OnDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_glaive_toss_damage_SpellScript();
        }
};

// Glaive Toss (Missile data) - 120755 and 120756
class spell_hun_glaive_toss_missile : public SpellScriptLoader
{
    public:
        spell_hun_glaive_toss_missile() : SpellScriptLoader("spell_hun_glaive_toss_missile") { }

        class spell_hun_glaive_toss_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glaive_toss_missile_SpellScript);

            void HandleAfterCast()
            {
                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_RIGHT)
                {
                    if (Player* plr = GetCaster()->ToPlayer())
                        plr->CastSpell(plr, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                    else if (GetOriginalCaster())
                    {
                        if (Player* caster = GetOriginalCaster()->ToPlayer())
                            caster->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                    }
                }
                else
                {
                    if (Player* plr = GetCaster()->ToPlayer())
                        plr->CastSpell(plr, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                    else if (GetOriginalCaster())
                    {
                        if (Player* caster = GetOriginalCaster()->ToPlayer())
                            caster->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                    }
                }

                if (Unit* target = GetExplTargetUnit())
                    if (GetCaster() == GetOriginalCaster())
                        GetCaster()->AddAura(HUNTER_SPELL_GLAIVE_TOSS_AURA, target);
            }

            void HandleOnHit()
            {
                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_RIGHT)
                {
                    if (Unit* caster = GetCaster())
                        if (Unit* target = GetHitUnit())
                            if (caster == GetOriginalCaster())
                                target->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_LEFT, true, NULL, NULL, caster->GetGUID());
                }
                else
                {
                    if (Unit* caster = GetCaster())
                        if (Unit* target = GetHitUnit())
                            if (caster == GetOriginalCaster())
                                target->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_RIGHT, true, NULL, NULL, caster->GetGUID());
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_hun_glaive_toss_missile_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_hun_glaive_toss_missile_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_glaive_toss_missile_SpellScript();
        }
};

// Blink Strike - 130392
class spell_hun_blink_strike : public SpellScriptLoader
{
    public:
        spell_hun_blink_strike() : SpellScriptLoader("spell_hun_blink_strike") { }

        class spell_hun_blink_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_blink_strike_SpellScript);

            SpellCastResult CheckPet()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (!_player->GetPet())
                        return SPELL_FAILED_NO_PET;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (Pet* pet = _player->GetPet())
                            pet->CastSpell(target, HUNTER_SPELL_BLINK_STRIKE, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_blink_strike_SpellScript::CheckPet);
                OnHit += SpellHitFn(spell_hun_blink_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_blink_strike_SpellScript();
        }
};

// Binding Shot - 117405
/*
class spell_hun_binding_shot : public SpellScriptLoader
{
    public:
        spell_hun_binding_shot() : SpellScriptLoader("spell_hun_binding_shot") { }

        class spell_hun_binding_shot_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_binding_shot_zone_AuraScript);

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    DynamicObject* dynObj = caster->GetDynObject(HUNTER_SPELL_BINDING_SHOT_AREA);

                    if (!dynObj)
                        return;

                    std::list<Unit*> bindedList;

                    CellCoord p(Trinity::ComputeCellCoord(dynObj->GetPositionX(), dynObj->GetPositionY()));
                    Cell cell(p);
                    cell.SetNoCreate();

                    Trinity::AnyUnitInObjectRangeCheck u_check(dynObj, 15.0f);
                    Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(dynObj, bindedList, u_check);

                    TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
                    TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

                    cell.Visit(p, world_unit_searcher, *dynObj->GetMap(), *dynObj, 15.0f);
                    cell.Visit(p, grid_unit_searcher, *dynObj->GetMap(), *dynObj, 15.0f);

                    bindedList.remove_if(Trinity::UnitAuraCheck(false, GetSpellInfo()->Id, caster->GetGUID()));

					for (std::list<Unit*>::const_iterator i = bindedList.begin(); i != bindedList.end(); ++i)
                    {
                        Unit* target = (*i)->ToUnit();
                        if (!target)
                            continue;

                        if (target->GetDistance(dynObj) > 5.0f)
                        {
                            if (!target->HasAura(HUNTER_SPELL_BINDING_SHOT_IMMUNE))
                            {
                                target->CastSpell(target, HUNTER_SPELL_BINDING_SHOT_STUN, true);
                                target->CastSpell(target, HUNTER_SPELL_BINDING_SHOT_IMMUNE, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_binding_shot_zone_AuraScript::OnUpdate, EFFECT_1, SPELL_AURA_MOD_DAMAGE_FROM_CASTER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_binding_shot_zone_AuraScript();
        }
};
*/

// Binding Shot - 109248
class spell_hun_binding_shot_zone : public SpellScriptLoader
{
    public:
        spell_hun_binding_shot_zone() : SpellScriptLoader("spell_hun_binding_shot_zone") { }

        class spell_hun_binding_shot_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_binding_shot_zone_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (DynamicObject* dynObj = GetCaster()->GetDynObject(HUNTER_SPELL_BINDING_SHOT_AREA))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), HUNTER_SPELL_BINDING_SHOT_LINK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_binding_shot_zone_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_binding_shot_zone_AuraScript();
        }
};


// Barrage damage - 120361
class spell_hun_barrage : public SpellScriptLoader
{
    public:
        spell_hun_barrage() : SpellScriptLoader("spell_hun_barrage") { }

        class spell_hun_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_barrage_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (!target->HasAura(120360))
                            SetHitDamage(GetHitDamage() / 2);
            }

            void Register()
            {
               OnHit += SpellHitFn(spell_hun_barrage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_barrage_SpellScript();
        }
};

// Lynx Rush - 120697
class spell_hun_lynx_rush : public SpellScriptLoader
{
    public:
        spell_hun_lynx_rush() : SpellScriptLoader("spell_hun_lynx_rush") { }

        class spell_hun_lynx_rush_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_lynx_rush_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                std::list<Unit*> tempList;
                std::list<Unit*> targetList;
                Unit* unitTarget = NULL;

                GetTarget()->GetAttackableUnitListInRange(tempList, 10.0f);

				for (std::list<Unit*>::const_iterator i = tempList.begin(); i != tempList.end(); ++i)
                {
                    if ((*i)->GetGUID() == GetTarget()->GetGUID())
                        continue;

                    if (GetTarget()->GetOwner() && GetTarget()->GetOwner()->GetGUID() == (*i)->GetGUID())
                        continue;

                    if (!GetTarget()->IsValidAttackTarget((*i)))
                        continue;

                    targetList.push_back((*i));
                }

                tempList.clear();

                if (targetList.empty())
                    return;

                Trinity::Containers::RandomResizeList(targetList, 1);

				for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                {
                    unitTarget = (*i);
                    break;
                }

                if (!unitTarget)
                    return;

                float angle = unitTarget->GetRelativeAngle(GetTarget());
                Position pos;

                unitTarget->GetContactPoint(GetTarget(), pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetObjectSize(), angle);
                GetTarget()->GetMotionMaster()->MoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ + unitTarget->GetObjectSize());

                GetTarget()->CastSpell(unitTarget, HUNTER_SPELL_LYNX_CRUSH_DAMAGE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_lynx_rush_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_lynx_rush_AuraScript();
        }

        class spell_hun_lynx_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_lynx_rush_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GetHitUnit())
                    {
                        if (Pet* pet = _player->GetPet())
                        {
                            if (pet->GetGUID() == GetHitUnit()->GetGUID())
                            {
                                std::list<Unit*> tempList;
                                std::list<Unit*> targetList;
                                Unit* unitTarget = NULL;

                                pet->GetAttackableUnitListInRange(tempList, 10.0f);

								for (std::list<Unit*>::const_iterator i = tempList.begin(); i != tempList.end(); ++i)
                                {
                                    if ((*i)->GetGUID() == pet->GetGUID())
                                        continue;

                                    if (_player->GetGUID() == (*i)->GetGUID())
                                        continue;

                                    if (!pet->IsValidAttackTarget((*i)))
                                        continue;

                                    targetList.push_back((*i));
                                }

                                tempList.clear();

                                if (targetList.empty())
                                    return;

                                Trinity::Containers::RandomResizeList(targetList, 1);

								for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                                {
                                    unitTarget = (*i);
                                    break;
                                }

                                if (!unitTarget)
                                    return;

                                float angle = unitTarget->GetRelativeAngle(pet);
                                Position pos;

                                unitTarget->GetContactPoint(pet, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                                unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetObjectSize(), angle);
                                pet->GetMotionMaster()->MoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ + unitTarget->GetObjectSize());

                                pet->CastSpell(unitTarget, HUNTER_SPELL_LYNX_CRUSH_DAMAGE, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_lynx_rush_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_lynx_rush_SpellScript();
        }
};

void AddSC_hunter_spell_scripts()
{
    new spell_hun_aspect_of_the_beast();
    new spell_hun_ascpect_of_the_viper();
    new spell_hun_chimera_shot();
    new spell_hun_disengage();
    new spell_hun_invigoration();
    new spell_hun_last_stand_pet();
    new spell_hun_masters_call();
    new spell_hun_misdirection();
    new spell_hun_misdirection_proc();
    new spell_hun_pet_carrion_feeder();
    new spell_hun_pet_heart_of_the_phoenix();
    new spell_hun_readiness();
    new spell_hun_scatter_shot();
    new spell_hun_sniper_training();
    new spell_hun_tame_beast();
    new spell_hun_target_only_pet_and_owner();
	new spell_hun_cobra_shot();
	new spell_hun_steady_shot();
	new spell_hun_dire_beast();
    new spell_hun_a_murder_of_crows();
	new spell_hun_powershot();
	new spell_hun_glaive_toss_damage();
	new spell_hun_glaive_toss_missile();
	new spell_hun_blink_strike();
	//new spell_hun_binding_shot();
	new spell_hun_binding_shot_zone();
	new spell_hun_barrage();
	new spell_hun_lynx_rush();
}
