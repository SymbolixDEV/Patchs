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
* Scripts for spells with SPELLFAMILY_WARLOCK and SPELLFAMILY_GENERIC spells used by warlock players.
* Ordered alphabetically using scriptname.
* Scriptnames of files in this file should be prefixed with "spell_warl_".
*/

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "Pet.h"

enum WarlockSpells
{
    SPELL_WARLOCK_BANE_OF_DOOM_EFFECT               = 18662,
    SPELL_WARLOCK_CURSE_OF_DOOM_EFFECT              = 18662,
    SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST         = 62388,
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON             = 48018,
    SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT           = 48020,
    SPELL_WARLOCK_SOULBURN_DEMONIC_CIRCLE_TELE      = 114794,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD      = 54508,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER     = 54509,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP           = 54444,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS      = 54435,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER    = 54443,
    SPELL_WARLOCK_DEMON_SOUL_IMP                    = 79459,
    SPELL_WARLOCK_DEMON_SOUL_FELHUNTER              = 79460,
    SPELL_WARLOCK_DEMON_SOUL_FELGUARD               = 79452,
    SPELL_WARLOCK_DEMON_SOUL_SUCCUBUS               = 79453,
    SPELL_WARLOCK_DEMON_SOUL_VOIDWALKER             = 79454,
    SPELL_WARLOCK_FEL_SYNERGY_HEAL                  = 54181,
    SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE              = 63106,
    SPELL_WARLOCK_HAUNT                             = 48181,
    SPELL_WARLOCK_HAUNT_HEAL                        = 48210,
    SPELL_WARLOCK_IMMOLATE                          = 348,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1    = 60955,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2    = 60956,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R1         = 18703,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R2         = 18704,
    SPELL_WARLOCK_LIFE_TAP_ENERGIZE                 = 31818,
    SPELL_WARLOCK_LIFE_TAP_ENERGIZE_2               = 32553,
    SPELL_WARLOCK_SIPHON_LIFE_HEAL                  = 63106,
    SPELL_WARLOCK_SOULSHATTER                       = 32835,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION               = 30108,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL        = 31117,
    WARLOCK_KIL_JAEDENS_CUNNING_PASSIVE             = 108507,
    WARLOCK_HARVEST_LIFE_HEAL                       = 125314,
    WARLOCK_SUPPLANT_DEMONIC_COMMAND                = 119904,
    WARLOCK_GRIMOIRE_OF_SACRIFICE                   = 108503,
    WARLOCK_DRAIN_LIFE_HEAL                         = 89653,
    WARLOCK_SOULBURN_AURA                           = 74434,
    WARLOCK_MOLTEN_CORE                             = 122355,
    WARLOCK_MOLTEN_CORE_AURA                        = 122351,
    WARLOCK_DECIMATE_AURA                           = 108869,
    WARLOCK_METAMORPHOSIS                           = 103958,
    WARLOCK_FIRE_AND_BRIMSTONE                      = 108683,
    WARLOCK_BACKDRAFT                               = 117828,
    WARLOCK_PYROCLASM                               = 123686,
    WARLOCK_GRIMOIRE_OF_SUPREMACY_TALENT            = 108499,
    WARLOCK_SUMMON_FEL_IMP                          = 112866,
    WARLOCK_SUMMON_VOIDLORD                         = 112867,
    WARLOCK_SUMMON_SHIVARRA                         = 112868,
    WARLOCK_SUMMON_OBSERVER                         = 112869,
    WARLOCK_SUMMON_WRATHGUARD                       = 112870,
    WARLOCK_SUMMON_ABYSSAL                          = 112921,
    WARLOCK_SUMMON_TERRORGUARD                      = 112927

};

enum WarlockMisc
{
    ENTRY_IMP								= 0,
    ENTRY_VOIDWALKER						= 1,
    ENTRY_SUCCUBUS							= 2,
    ENTRY_FELHUNTER							= 3,
    ENTRY_FELGUARD							= 4,

    SPEC_WARLOCK_AFFLICTION					= 5,
    SPEC_WARLOCK_DEMONOLOGY					= 6,
    SPEC_WARLOCK_DESTRUCTION				= 7,
    SPEC_NONE								= 8
};

enum WarlockSpellIcons
{
    WARLOCK_ICON_ID_IMPROVED_LIFE_TAP               = 208,
    WARLOCK_ICON_ID_MANA_FEED                       = 1982
};

// 710 - Banish
/// Updated 4.3.4
class spell_warl_banish : public SpellScriptLoader
{
public:
    spell_warl_banish() : SpellScriptLoader("spell_warl_banish") { }

    class spell_warl_banish_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_banish_SpellScript);

        bool Load()
        {
            _removed = false;
            return true;
        }

        void HandleBanish()
        {
            /// Casting Banish on a banished target will cancel the effect
            /// Check if the target already has Banish, if so, do nothing.
            if (Unit* target = GetHitUnit())
            {
                if (target->GetAuraEffect(SPELL_AURA_SCHOOL_IMMUNITY, SPELLFAMILY_WARLOCK, 0, 0x08000000, 0))
                {
                    // No need to remove old aura since its removed due to not stack by current Banish aura
                    PreventHitDefaultEffect(EFFECT_0);
                    PreventHitDefaultEffect(EFFECT_1);
                    PreventHitDefaultEffect(EFFECT_2);
                    _removed = true;
                }
            }
        }

        void RemoveAura()
        {
            if (_removed)
                PreventHitAura();
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_warl_banish_SpellScript::HandleBanish);
            AfterHit += SpellHitFn(spell_warl_banish_SpellScript::RemoveAura);
        }

        bool _removed;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_banish_SpellScript();
    }
};

// 17962 - Conflagrate - Updated to 4.3.4
class spell_warl_conflagrate : public SpellScriptLoader
{
public:
    spell_warl_conflagrate() : SpellScriptLoader("spell_warl_conflagrate") { }

    class spell_warl_conflagrate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_conflagrate_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_IMMOLATE))
                return false;
            return true;
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (AuraEffect const* aurEff = GetHitUnit()->GetAuraEffect(SPELL_WARLOCK_IMMOLATE, EFFECT_2, GetCaster()->GetGUID()))
                SetHitDamage(CalculatePct(aurEff->GetAmount(), GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster())));
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_conflagrate_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_conflagrate_SpellScript();
    }
};

// 6201 - Create Healthstone
class spell_warl_create_healthstone : public SpellScriptLoader
{
public:
    spell_warl_create_healthstone() : SpellScriptLoader("spell_warl_create_healthstone") { }

    class spell_warl_create_healthstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_create_healthstone_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            return true;
        }

        SpellCastResult CheckCast()
        {
            if (Player* caster = GetCaster()->ToPlayer())
            {
                ItemPosCountVec dest;
                InventoryResult msg = caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 5512, 1, NULL);
                if (msg != EQUIP_ERR_OK)
                    return SPELL_FAILED_TOO_MANY_OF_ITEM;
            }
            return SPELL_CAST_OK;
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (Unit* unitTarget = GetHitUnit())
            {
                CreateItem(effIndex, 5512);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_create_healthstone_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnCheckCast += SpellCheckCastFn(spell_warl_create_healthstone_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_create_healthstone_SpellScript();
    }
};

//6262 - Use HeathStone
class spell_warl_use_healthstone : public SpellScriptLoader
{
public:
    spell_warl_use_healthstone() : SpellScriptLoader("spell_warl_use_healthstone") { }

    class spell_warl_use_healthstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_use_healthstone_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            return true;
        }

        void HandleHeal()
        {
            if (Unit* unitTarget = GetHitUnit())
            {
                if(!unitTarget->HasAura(56224)) SetHitHeal(20 * unitTarget->GetMaxHealth() / 100);
                else PreventHitHeal();
                PreventHitDamage();
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_use_healthstone_SpellScript::HandleHeal);
        }
    };

    class spell_warl_use_healthstone_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_use_healthstone_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            return true;
        }

        void OnPeriodicHeal(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* unitTarget = GetTarget())
            {
                if(unitTarget->HasAura(56224)) GetAura()->GetEffect(EFFECT_1)->SetAmount(40 * unitTarget->GetMaxHealth() / 100 * (GetAura()->GetEffect(EFFECT_1)->GetAmplitude()) / GetAura()->GetMaxDuration());
                else GetAura()->GetEffect(EFFECT_1)->SetAmount(0);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectRemoveFn(spell_warl_use_healthstone_AuraScript::OnPeriodicHeal, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_use_healthstone_AuraScript();
    }

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_use_healthstone_SpellScript();
    }
};

// 603 - Bane of Doom
/// Updated 4.3.4
class spell_warl_bane_of_doom : public SpellScriptLoader
{
public:
    spell_warl_bane_of_doom() : SpellScriptLoader("spell_warl_bane_of_doom") { }

    class spell_warl_curse_of_doom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_curse_of_doom_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_BANE_OF_DOOM_EFFECT))
                return false;
            return true;
        }

        bool Load()
        {
            return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster())
                return;

            AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
            if (removeMode != AURA_REMOVE_BY_DEATH || !IsExpired())
                return;

            if (GetCaster()->ToPlayer()->isHonorOrXPTarget(GetTarget()))
                GetCaster()->CastSpell(GetTarget(), SPELL_WARLOCK_BANE_OF_DOOM_EFFECT, true, NULL, aurEff);
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_warl_curse_of_doom_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_curse_of_doom_AuraScript();
    }
};

// 48018 - Demonic Circle: Summon
/// Updated 4.3.4
class spell_warl_demonic_circle_summon : public SpellScriptLoader
{
public:
    spell_warl_demonic_circle_summon() : SpellScriptLoader("spell_warl_demonic_circle_summon") { }

    class spell_warl_demonic_circle_summon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_circle_summon_AuraScript);

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            // If effect is removed by expire remove the summoned demonic circle too.
            if (!(mode & AURA_EFFECT_HANDLE_REAPPLY))
                GetTarget()->RemoveGameObject(GetId(), true);

            if (GetTarget()->GetAuraApplication(aurEff->GetSpellInfo()->Id, GetTarget()->GetGUID()))
                GetTarget()->GetAuraApplication(aurEff->GetSpellInfo()->Id, GetTarget()->GetGUID())->SendFakeAuraUpdate(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
        }

        void HandleDummyTick(AuraEffect const* aurEff)
        {
            if (GameObject* circle = GetTarget()->GetGameObject(GetId()))
            {
                // Here we check if player is in demonic circle teleport range, if so add
                // WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST; allowing him to cast the WARLOCK_DEMONIC_CIRCLE_TELEPORT.
                // If not in range remove the WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST.
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT);

                if (GetTarget()->IsWithinDist(circle, spellInfo->GetMaxRange(true)))
                    GetTarget()->GetAuraApplication(aurEff->GetSpellInfo()->Id, GetTarget()->GetGUID())->SendFakeAuraUpdate(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, false);
                else
                    GetTarget()->GetAuraApplication(aurEff->GetSpellInfo()->Id, GetTarget()->GetGUID())->SendFakeAuraUpdate(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
            }
        }

        void Register()
        {
            OnEffectRemove += AuraEffectApplyFn(spell_warl_demonic_circle_summon_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_circle_summon_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_demonic_circle_summon_AuraScript();
    }
};

// 48020 - Demonic Circle: Teleport
/// Updated 4.3.4
class spell_warl_demonic_circle_teleport : public SpellScriptLoader
{
public:
    spell_warl_demonic_circle_teleport() : SpellScriptLoader("spell_warl_demonic_circle_teleport") { }

    class spell_warl_demonic_circle_teleport_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_circle_teleport_AuraScript);

        void HandleTeleport(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
            {
                if (GameObject* circle = player->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON))
                {

                    player->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());
                    player->RemoveMovementImpairingAuras();

                    if (aurEff->GetSpellInfo()->Id == SPELL_WARLOCK_SOULBURN_DEMONIC_CIRCLE_TELE)
                        if (player->HasAura(WARLOCK_SOULBURN_AURA))
                            player->RemoveAurasDueToSpell(WARLOCK_SOULBURN_AURA);
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_circle_teleport_AuraScript::HandleTeleport, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_demonic_circle_teleport_AuraScript();
    }
};

// 77801 - Demon Soul - Updated to 4.3.4
class spell_warl_demon_soul : public SpellScriptLoader
{
public:
    spell_warl_demon_soul() : SpellScriptLoader("spell_warl_demon_soul") { }

    class spell_warl_demon_soul_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demon_soul_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMON_SOUL_IMP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMON_SOUL_FELHUNTER))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMON_SOUL_FELGUARD))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMON_SOUL_SUCCUBUS))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMON_SOUL_VOIDWALKER))
                return false;
            return true;
        }

        void OnHitTarget(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Creature* targetCreature = GetHitCreature())
            {
                if (targetCreature->isPet())
                {
                    CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(targetCreature->GetEntry());
                    switch (ci->family)
                    {
                    case CREATURE_FAMILY_SUCCUBUS:
                        caster->CastSpell(caster, SPELL_WARLOCK_DEMON_SOUL_SUCCUBUS);
                        break;
                    case CREATURE_FAMILY_VOIDWALKER:
                        caster->CastSpell(caster, SPELL_WARLOCK_DEMON_SOUL_VOIDWALKER);
                        break;
                    case CREATURE_FAMILY_FELGUARD:
                        caster->CastSpell(caster, SPELL_WARLOCK_DEMON_SOUL_FELGUARD);
                        break;
                    case CREATURE_FAMILY_FELHUNTER:
                        caster->CastSpell(caster, SPELL_WARLOCK_DEMON_SOUL_FELHUNTER);
                        break;
                    case CREATURE_FAMILY_IMP:
                        caster->CastSpell(caster, SPELL_WARLOCK_DEMON_SOUL_IMP);
                        break;
                    }
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_demon_soul_SpellScript::OnHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_demon_soul_SpellScript;
    }
};

// 47193 - Demonic Empowerment
/// Updated 4.3.4
class spell_warl_demonic_empowerment : public SpellScriptLoader
{
public:
    spell_warl_demonic_empowerment() : SpellScriptLoader("spell_warl_demonic_empowerment") { }

    class spell_warl_demonic_empowerment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demonic_empowerment_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS) || !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER) || !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD) || !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER) || !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Creature* targetCreature = GetHitCreature())
            {
                if (targetCreature->isPet())
                {
                    CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(targetCreature->GetEntry());
                    switch (ci->family)
                    {
                    case CREATURE_FAMILY_SUCCUBUS:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS, true);
                        break;
                    case CREATURE_FAMILY_VOIDWALKER:
                        {
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER);
                            int32 hp = int32(targetCreature->CountPctFromMaxHealth(GetCaster()->CalculateSpellDamage(targetCreature, spellInfo, 0)));
                            targetCreature->CastCustomSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER, &hp, NULL, NULL, true);
                            break;
                        }
                    case CREATURE_FAMILY_FELGUARD:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD, true);
                        break;
                    case CREATURE_FAMILY_FELHUNTER:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER, true);
                        break;
                    case CREATURE_FAMILY_IMP:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP, true);
                        break;
                    }
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_demonic_empowerment_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_demonic_empowerment_SpellScript();
    }
};

// 47422 - Everlasting Affliction
/// Updated 4.3.4
class spell_warl_everlasting_affliction : public SpellScriptLoader
{
public:
    spell_warl_everlasting_affliction() : SpellScriptLoader("spell_warl_everlasting_affliction") { }

    class spell_warl_everlasting_affliction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_everlasting_affliction_SpellScript);

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* unitTarget = GetHitUnit())
                // Refresh corruption on target
                    if (AuraEffect* aur = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_WARLOCK, 0x2, 0, 0, GetCaster()->GetGUID()))
                        aur->GetBase()->RefreshDuration();
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_everlasting_affliction_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_everlasting_affliction_SpellScript();
    }
};

// 77799 - Fel Flame - Updated to 4.3.4
class spell_warl_fel_flame : public SpellScriptLoader
{
public:
    spell_warl_fel_flame() : SpellScriptLoader("spell_warl_fel_flame") { }

    class spell_warl_fel_flame_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fel_flame_SpellScript);

        void OnHitTarget(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            Aura* aura = target->GetAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION, caster->GetGUID());
            if (!aura)
                aura = target->GetAura(SPELL_WARLOCK_IMMOLATE, caster->GetGUID());

            if (!aura)
                return;

            int32 newDuration = aura->GetDuration() + GetSpellInfo()->Effects[EFFECT_1].CalcValue() * 1000;
            aura->SetDuration(std::min(newDuration, aura->GetMaxDuration()));
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_fel_flame_SpellScript::OnHitTarget, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_fel_flame_SpellScript;
    }
};

// -47230 - Fel Synergy
class spell_warl_fel_synergy : public SpellScriptLoader
{
public:
    spell_warl_fel_synergy() : SpellScriptLoader("spell_warl_fel_synergy") { }

    class spell_warl_fel_synergy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_fel_synergy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_FEL_SYNERGY_HEAL))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return GetTarget()->GetGuardianPet() && eventInfo.GetDamageInfo()->GetDamage();
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 heal = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(SPELL_WARLOCK_FEL_SYNERGY_HEAL, SPELLVALUE_BASE_POINT0, heal, (Unit*)NULL, true, NULL, aurEff); // TARGET_UNIT_PET
        }

        void Register()
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_fel_synergy_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warl_fel_synergy_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_fel_synergy_AuraScript();
    }
};

// 48181 - Haunt
/// Updated 4.3.4
class spell_warl_haunt : public SpellScriptLoader
{
public:
    spell_warl_haunt() : SpellScriptLoader("spell_warl_haunt") { }

    class spell_warl_haunt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_haunt_SpellScript);

        void HandleOnHit()
        {
            if (Aura* aura = GetHitAura())
                if (AuraEffect* aurEff = aura->GetEffect(EFFECT_1))
                    aurEff->SetAmount(CalculatePct(aurEff->GetAmount(), GetHitDamage()));
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_haunt_SpellScript::HandleOnHit);
        }
    };

    class spell_warl_haunt_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_haunt_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_HAUNT_HEAL))
                return false;
            return true;
        }

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                int32 amount = aurEff->GetAmount();
                GetTarget()->CastCustomSpell(caster, SPELL_WARLOCK_HAUNT_HEAL, &amount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
            }
        }

        void Register()
        {
            OnEffectRemove += AuraEffectApplyFn(spell_warl_haunt_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_haunt_SpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_haunt_AuraScript();
    }
};

// 755 - Health Funnel
/// Updated 4.3.4
class spell_warl_health_funnel : public SpellScriptLoader
{
public:
    spell_warl_health_funnel() : SpellScriptLoader("spell_warl_health_funnel") { }

    class spell_warl_health_funnel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_health_funnel_AuraScript);

        void ApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetTarget();
            if (caster->HasAura(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R2))
                target->CastSpell(target, SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2, true);
            else if (caster->HasAura(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R1))
                target->CastSpell(target, SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1, true);
        }

        void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->RemoveAurasDueToSpell(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1);
            target->RemoveAurasDueToSpell(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_health_funnel_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_warl_health_funnel_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_health_funnel_AuraScript();
    }
};

// Life Tap - 1454
class spell_warl_life_tap : public SpellScriptLoader
{
public:
    spell_warl_life_tap() : SpellScriptLoader("spell_warl_life_tap") { }

    class spell_warl_life_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_life_tap_SpellScript);

        SpellCastResult CheckLife()
        {
            if (GetCaster()->GetHealthPct() > 15.0f)
                return SPELL_CAST_OK;
            return SPELL_FAILED_FIZZLE;
        }

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                int32 healthCost = int32(_player->GetMaxHealth() * 0.15f);

                _player->SetHealth(_player->GetHealth() - healthCost);
                _player->EnergizeBySpell(_player, 1454, healthCost, POWER_MANA);
            }
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_life_tap_SpellScript::CheckLife);
            OnHit += SpellHitFn(spell_warl_life_tap_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_life_tap_SpellScript();
    }
};

// 18541 - Ritual of Doom Effect
class spell_warl_ritual_of_doom_effect : public SpellScriptLoader
{
public:
    spell_warl_ritual_of_doom_effect() : SpellScriptLoader("spell_warl_ritual_of_doom_effect") { }

    class spell_warl_ritual_of_doom_effect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_ritual_of_doom_effect_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            caster->CastSpell(caster, GetEffectValue(), true);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_warl_ritual_of_doom_effect_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_ritual_of_doom_effect_SpellScript();
    }
};

// 27285 - Seed of Corruption
/// Updated 4.3.4
class spell_warl_seed_of_corruption : public SpellScriptLoader
{
public:
    spell_warl_seed_of_corruption() : SpellScriptLoader("spell_warl_seed_of_corruption") { }

    class spell_warl_seed_of_corruption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_seed_of_corruption_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (GetExplTargetUnit())
                targets.remove(GetExplTargetUnit());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_seed_of_corruption_SpellScript();
    }
};

// -7235 - Shadow Ward
class spell_warl_shadow_ward : public SpellScriptLoader
{
public:
    spell_warl_shadow_ward() : SpellScriptLoader("spell_warl_shadow_ward") { }

    class spell_warl_shadow_ward_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadow_ward_AuraScript);

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

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_shadow_ward_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_shadow_ward_AuraScript();
    }
};

// 63108 - Siphon Life
class spell_warl_siphon_life : public SpellScriptLoader
{
public:
    spell_warl_siphon_life() : SpellScriptLoader("spell_warl_siphon_life") { }

    class spell_warl_siphon_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_siphon_life_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SIPHON_LIFE_HEAL))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetDamageInfo()->GetDamage();
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
            // Glyph of Siphon Life
            if (AuraEffect const* glyph = GetTarget()->GetAuraEffect(SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE, EFFECT_0))
                AddPct(amount, glyph->GetAmount());

            GetTarget()->CastCustomSpell(SPELL_WARLOCK_SIPHON_LIFE_HEAL, SPELLVALUE_BASE_POINT0, amount, GetTarget(), true, NULL, aurEff);
        }

        void Register()
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_siphon_life_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warl_siphon_life_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_siphon_life_AuraScript();
    }
};

// 29858 - Soulshatter
/// Updated 4.3.4
class spell_warl_soulshatter : public SpellScriptLoader
{
public:
    spell_warl_soulshatter() : SpellScriptLoader("spell_warl_soulshatter") { }

    class spell_warl_soulshatter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soulshatter_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOULSHATTER))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (target->CanHaveThreatList() && target->getThreatManager().getThreat(caster) > 0.0f)
                    caster->CastSpell(target, SPELL_WARLOCK_SOULSHATTER, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soulshatter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_soulshatter_SpellScript();
    }
};

// 30108, 34438, 34439, 35183 - Unstable Affliction
/// Updated 4.3.4
class spell_warl_unstable_affliction : public SpellScriptLoader
{
public:
    spell_warl_unstable_affliction() : SpellScriptLoader("spell_warl_unstable_affliction") { }

    class spell_warl_unstable_affliction_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_unstable_affliction_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL))
                return false;
            return true;
        }

        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect const* aurEff = GetEffect(EFFECT_0))
                {
                    int32 damage = aurEff->GetAmount() * 9;
                    // backfire damage and silence
                    caster->CastCustomSpell(dispelInfo->GetDispeller(), SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL, &damage, NULL, NULL, true, NULL, aurEff);
                }
        }

        void Register()
        {
            AfterDispel += AuraDispelFn(spell_warl_unstable_affliction_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_unstable_affliction_AuraScript();
    }
};

// Kil'Jaeden's Cunning (passive with cooldown) - 119048
class spell_warl_kil_jaedens_cunning : public SpellScriptLoader
{
public:
    spell_warl_kil_jaedens_cunning() : SpellScriptLoader("spell_warl_kil_jaedens_cunning") { }

    class spell_warl_kil_jaedens_cunning_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_kil_jaedens_cunning_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
        {
            if (GetCaster())
                GetCaster()->RemoveAura(WARLOCK_KIL_JAEDENS_CUNNING_PASSIVE);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
        {
            if (GetCaster())
                GetCaster()->CastSpell(GetCaster(), WARLOCK_KIL_JAEDENS_CUNNING_PASSIVE, true);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_kil_jaedens_cunning_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_kil_jaedens_cunning_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_kil_jaedens_cunning_AuraScript();
    }
};

// Burning Rush - 111400
class spell_warl_burning_rush : public SpellScriptLoader
{
public:
    spell_warl_burning_rush() : SpellScriptLoader("spell_warl_burning_rush") { }

    class spell_warl_burning_rush_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_burning_rush_AuraScript);

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            if (GetCaster())
            {
                // Drain 4% of health every second
                int32 basepoints = GetCaster()->CountPctFromMaxHealth(4);

                GetCaster()->DealDamage(GetCaster(), basepoints, NULL, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_burning_rush_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_burning_rush_AuraScript();
    }
};

// Harvest Life - 108371
class spell_warl_harvest_life : public SpellScriptLoader
{
public:
    spell_warl_harvest_life() : SpellScriptLoader("spell_warl_harvest_life") { }

    class spell_warl_harvest_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_harvest_life_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (!GetCaster())
                return;

            if (Player* _player = GetCaster()->ToPlayer())
            {
                // Restoring 3-4.5% of the caster's total health every 1s - With 33% bonus
                int32 basepoints = int32(frand(0.03f, 0.045f) * _player->GetMaxHealth());

                AddPct(basepoints, 33);

                if (!_player->HasSpellCooldown(WARLOCK_HARVEST_LIFE_HEAL))
                {
                    _player->CastCustomSpell(_player, WARLOCK_HARVEST_LIFE_HEAL, &basepoints, NULL, NULL, true);
                    // prevent the heal to proc off for each targets
                    _player->AddSpellCooldown(WARLOCK_HARVEST_LIFE_HEAL, 0, time(NULL) + 1);
                }

                _player->EnergizeBySpell(_player, aurEff->GetSpellInfo()->Id, 4, POWER_DEMONIC_FURY);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_harvest_life_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_harvest_life_AuraScript();
    }
};

// Grimoire of Sacrifice - 108503
class spell_warl_grimoire_of_sacrifice : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_sacrifice() : SpellScriptLoader("spell_warl_grimoire_of_sacrifice") { }

    class spell_warl_grimoire_of_sacrifice_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_grimoire_of_sacrifice_SpellScript);

        void HandleAfterCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (Pet* pet = player->GetPet())
                {
                    // Supplant Command Demon
                    if (player->getLevel() >= 56)
                    {
                        int32 bp = 0;

                        player->RemoveAura(WARLOCK_SUPPLANT_DEMONIC_COMMAND);

                        switch (pet->GetEntry())
                        {
                        case ENTRY_IMP:
                            bp = 132411;// Single Magic
                            break;
                        case ENTRY_VOIDWALKER:
                            bp = 132413;// Shadow Bulwark
                            break;
                        case ENTRY_SUCCUBUS:
                            bp = 137706;// Whiplash
                            break;
                        case ENTRY_FELHUNTER:
                            bp = 132409;// Spell Lock
                            break;
                        case ENTRY_FELGUARD:
                            bp = 132410;// Pursuit
                            break;
                        default:
                            break;
                        }

                        if (bp)
                            player->CastCustomSpell(player, WARLOCK_SUPPLANT_DEMONIC_COMMAND, &bp, NULL, NULL, true);
                    }
                }
            }
        }

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                // EFFECT_0 : Instakill
                // EFFECT_1 : 2% health every 5s
                // EFFECT_2 : +50% DOT damage for Malefic Grasp, Drain Life and Drain Soul
                // EFFECT_3 : +30% damage for Shadow Bolt, Hand of Gul'Dan, Soul Fire, Wild Imps and Fel Flame
                // EFFECT_4 : +25% damage for Incinerate, Conflagrate, Chaos Bolt, Shadowburn and Fel Flame
                // EFFECT_5 : +50% damage for Fel Flame
                // EFFECT_6 : +20% Health if Soul Link talent is also chosen
                // EFFECT_7 : +50% on EFFECT_2 of Malefic Grasp
                // EFFECT_8 : +50% on EFFECT_4 and EFFECT_5 of Drain Soul -> Always set to 0
                // EFFECT_9 : Always set to 0
                // EFFECT_10 : Always set to 0
                if (Aura* grimoireOfSacrifice = player->GetAura(WARLOCK_GRIMOIRE_OF_SACRIFICE))
                {
                    if (grimoireOfSacrifice->GetEffect(EFFECT_10))
                        grimoireOfSacrifice->GetEffect(EFFECT_10)->SetAmount(0);
                    if (grimoireOfSacrifice->GetEffect(EFFECT_9))
                        grimoireOfSacrifice->GetEffect(EFFECT_9)->SetAmount(0);
                    if (grimoireOfSacrifice->GetEffect(EFFECT_8))
                        grimoireOfSacrifice->GetEffect(EFFECT_8)->SetAmount(0);

                    switch (player->GetPrimaryTalentTree(player->GetActiveSpec()))
                    {
                    case SPEC_WARLOCK_AFFLICTION:
                        if (grimoireOfSacrifice->GetEffect(EFFECT_3))
                            grimoireOfSacrifice->GetEffect(EFFECT_3)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_4))
                            grimoireOfSacrifice->GetEffect(EFFECT_4)->SetAmount(0);
                        break;
                    case SPEC_WARLOCK_DEMONOLOGY:
                        if (grimoireOfSacrifice->GetEffect(EFFECT_2))
                            grimoireOfSacrifice->GetEffect(EFFECT_2)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_4))
                            grimoireOfSacrifice->GetEffect(EFFECT_4)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_5))
                            grimoireOfSacrifice->GetEffect(EFFECT_5)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_7))
                            grimoireOfSacrifice->GetEffect(EFFECT_7)->SetAmount(0);
                        break;
                    case SPEC_WARLOCK_DESTRUCTION:
                        if (grimoireOfSacrifice->GetEffect(EFFECT_2))
                            grimoireOfSacrifice->GetEffect(EFFECT_2)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_3))
                            grimoireOfSacrifice->GetEffect(EFFECT_3)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_5))
                            grimoireOfSacrifice->GetEffect(EFFECT_5)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_7))
                            grimoireOfSacrifice->GetEffect(EFFECT_7)->SetAmount(0);
                        break;
                    case SPEC_NONE:
                        if (grimoireOfSacrifice->GetEffect(EFFECT_2))
                            grimoireOfSacrifice->GetEffect(EFFECT_2)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_3))
                            grimoireOfSacrifice->GetEffect(EFFECT_3)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_4))
                            grimoireOfSacrifice->GetEffect(EFFECT_4)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_5))
                            grimoireOfSacrifice->GetEffect(EFFECT_5)->SetAmount(0);
                        if (grimoireOfSacrifice->GetEffect(EFFECT_7))
                            grimoireOfSacrifice->GetEffect(EFFECT_7)->SetAmount(0);
                    default:
                        break;
                    }
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_warl_grimoire_of_sacrifice_SpellScript::HandleAfterCast);
            OnHit += SpellHitFn(spell_warl_grimoire_of_sacrifice_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_grimoire_of_sacrifice_SpellScript();
    }

    class spell_warl_grimoire_of_sacrifice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_grimoire_of_sacrifice_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
        {
            if (!GetTarget())
                return;

            if (Player* _player = GetTarget()->ToPlayer())
                if (_player->HasAura(WARLOCK_SUPPLANT_DEMONIC_COMMAND))
                    _player->RemoveAura(WARLOCK_SUPPLANT_DEMONIC_COMMAND);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectApplyFn(spell_warl_grimoire_of_sacrifice_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_grimoire_of_sacrifice_AuraScript();
    }
};


// Burning Embers - 108647
class spell_warl_burning_embers : public SpellScriptLoader
{
public:
    spell_warl_burning_embers() : SpellScriptLoader("spell_warl_burning_embers") { }

    class spell_warl_burning_embers_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_burning_embers_AuraScript);

        void OnProcHandler(ProcEventInfo& eventInfo)
        {
            if (!GetOwner())
                return;

            uint32 spellId = eventInfo.GetDamageInfo()->GetSpellInfo()->Id;
            int32 embers = 0;
            bool crit = (eventInfo.GetHitMask() & PROC_HIT_CRITICAL);

            switch(spellId)
            {
            case 17962:
            case 77799:
            case 29722:
                embers = 1;
                if (crit)
                    embers *= 2;
                break;
            case 348:
                embers = crit ? 2 : 0;
                break;
            default:
                return;
            }

            if (Player* _player = GetOwner()->ToPlayer())
            {
                _player->ModifyPower(POWER_BURNING_EMBERS, embers);
            }
        }

        void Register()
        {
            OnProc += AuraProcFn(spell_warl_burning_embers_AuraScript::OnProcHandler);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_burning_embers_AuraScript();
    }
};

// Drain Life - 689
class spell_warl_drain_life : public SpellScriptLoader
{
public:
    spell_warl_drain_life() : SpellScriptLoader("spell_warl_drain_life") { }

    class spell_warl_drain_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_drain_life_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
            {
                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                // Restoring 2% of the caster's total health every 1s
                int32 basepoints = _player->GetMaxHealth() / 50;

                // In Demonology spec : Generates 10 Demonic Fury per second
                _player->EnergizeBySpell(_player, 689, 10, POWER_DEMONIC_FURY);

                _player->CastCustomSpell(_player, WARLOCK_DRAIN_LIFE_HEAL, &basepoints, NULL, NULL, true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_drain_life_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_drain_life_AuraScript();
    }
};

// Soulburn : Drain Life - 89420
class spell_warl_soulburn_drain_life : public SpellScriptLoader
{
public:
    spell_warl_soulburn_drain_life() : SpellScriptLoader("spell_warl_soulburn_drain_life") { }

    class spell_warl_soulburn_drain_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soulburn_drain_life_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetCaster())
                if (GetCaster()->HasAura(WARLOCK_SOULBURN_AURA))
                    GetCaster()->RemoveAura(WARLOCK_SOULBURN_AURA);
        }

        void OnTick(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
            {
                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                // Restoring 2% of the caster's total health every 1s
                int32 basepoints = _player->CountPctFromMaxHealth(3);

                // In Demonology spec : Generates 10 Demonic Fury per second
                _player->EnergizeBySpell(_player, 689, 10, POWER_DEMONIC_FURY);

                _player->CastCustomSpell(_player, WARLOCK_DRAIN_LIFE_HEAL, &basepoints, NULL, NULL, true);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_soulburn_drain_life_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_soulburn_drain_life_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_soulburn_drain_life_AuraScript();
    }
};

// Called by Shadowflame - 47960
// Molten Core - 122351
class spell_warl_molten_core_dot : public SpellScriptLoader
{
public:
    spell_warl_molten_core_dot() : SpellScriptLoader("spell_warl_molten_core_dot") { }

    class spell_warl_molten_core_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_molten_core_dot_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (GetCaster())
            {
                if (GetCaster()->HasAura(WARLOCK_MOLTEN_CORE_AURA) && GetCaster()->getLevel() >= 69)
                    if (roll_chance_i(8))
                        GetCaster()->CastSpell(GetCaster(), WARLOCK_MOLTEN_CORE, true);

                GetCaster()->EnergizeBySpell(GetCaster(), aurEff->GetSpellInfo()->Id, 2, POWER_DEMONIC_FURY);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_molten_core_dot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_molten_core_dot_AuraScript();
    }
};

// Called by Shadow Bolt - 686 and Soul Fire - 6353
// Decimate - 108869
class spell_warl_decimate : public SpellScriptLoader
{
public:
    spell_warl_decimate() : SpellScriptLoader("spell_warl_decimate") { }

    class spell_warl_decimate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_decimate_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                    if (_player->HasAura(WARLOCK_DECIMATE_AURA) && _player->getLevel() >= 73)
                        if (target->GetHealthPct() < 25.0f)
                            _player->CastSpell(_player, WARLOCK_MOLTEN_CORE, true);

                _player->EnergizeBySpell(_player, GetSpellInfo()->Id, GetSpellInfo()->Id == 686 ? 25 : 30, POWER_DEMONIC_FURY);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_decimate_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_decimate_SpellScript();
    }
};

// Metamorphosis - 103958
class spell_warl_metamorphosis_cost : public SpellScriptLoader
{
public:
    spell_warl_metamorphosis_cost() : SpellScriptLoader("spell_warl_metamorphosis_cost") { }

    class spell_warl_metamorphosis_cost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_metamorphosis_cost_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (GetCaster())
                GetCaster()->EnergizeBySpell(GetCaster(), WARLOCK_METAMORPHOSIS, -6, POWER_DEMONIC_FURY);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_metamorphosis_cost_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_metamorphosis_cost_AuraScript();
    }
};

// Immolation Aura - 104025
class spell_warl_immolation_aura : public SpellScriptLoader
{
public:
    spell_warl_immolation_aura() : SpellScriptLoader("spell_warl_immolation_aura") { }

    class spell_warl_immolation_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_immolation_aura_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (GetCaster())
                GetCaster()->EnergizeBySpell(GetCaster(), GetSpellInfo()->Id, -25, POWER_DEMONIC_FURY);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_immolation_aura_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_immolation_aura_AuraScript();
    }
};

// Hellfire - 5857
class spell_warl_hellfire : public SpellScriptLoader
{
public:
    spell_warl_hellfire() : SpellScriptLoader("spell_warl_hellfire") { }

    class spell_warl_hellfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_hellfire_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    _player->EnergizeBySpell(_player, GetSpellInfo()->Id, 4, POWER_DEMONIC_FURY);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_hellfire_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_hellfire_SpellScript();
    }
};

// 108686 - Immolate effect for WARLOCK_FIRE_AND_BRIMSTONE
class spell_warl_immolate : public SpellScriptLoader
{
public:
    spell_warl_immolate() : SpellScriptLoader("spell_warl_immolate") { }

    class spell_warl_immolate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_immolate_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (_player->HasAura(WARLOCK_FIRE_AND_BRIMSTONE))
                        _player->RemoveAura(WARLOCK_FIRE_AND_BRIMSTONE);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_immolate_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_immolate_SpellScript();
    }
};

// 114654 - Incinerate effect for WARLOCK_FIRE_AND_BRIMSTONE
class spell_warl_incinerate : public SpellScriptLoader
{
public:
    spell_warl_incinerate() : SpellScriptLoader("spell_warl_incinerate") { }

    class spell_warl_incinerate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_incinerate_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (_player->HasAura(WARLOCK_FIRE_AND_BRIMSTONE))
                        _player->RemoveAura(WARLOCK_FIRE_AND_BRIMSTONE);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_incinerate_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_incinerate_SpellScript();
    }
};

// 108685 - conflagrate effect for WARLOCK_FIRE_AND_BRIMSTONE
class spell_warl_conflagrate_and_brimstone : public SpellScriptLoader
{
public:
    spell_warl_conflagrate_and_brimstone() : SpellScriptLoader("spell_warl_conflagrate_and_brimstone") { }

    class spell_warl_conflagrate_and_brimstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_conflagrate_and_brimstone_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (_player->HasAura(WARLOCK_FIRE_AND_BRIMSTONE))
                        _player->RemoveAura(WARLOCK_FIRE_AND_BRIMSTONE);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_conflagrate_and_brimstone_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_conflagrate_and_brimstone_SpellScript();
    }
};

// Chaos Bolt - 116858
class spell_warl_chaos_bolt : public SpellScriptLoader
{
public:
    spell_warl_chaos_bolt() : SpellScriptLoader("spell_warl_chaos_bolt") { }

    class spell_warl_chaos_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_chaos_bolt_SpellScript);

        void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (_player->HasAura(WARLOCK_PYROCLASM))
                    if(_player->HasAura(WARLOCK_BACKDRAFT))
                        _player->RemoveAura(WARLOCK_BACKDRAFT);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_warl_chaos_bolt_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_chaos_bolt_SpellScript();
    }
};

// Grimoire of Supremacy - 108499
class spell_warl_grimoire_of_supremacy : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_supremacy() : SpellScriptLoader("spell_warl_grimoire_of_supremacy") { }

    class spell_warl_grimoire_of_supremacy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_grimoire_of_supremacy_AuraScript);

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* _player = GetTarget()->ToPlayer())
            {
                _player->learnSpell(WARLOCK_SUMMON_FEL_IMP, false);
                _player->learnSpell(WARLOCK_SUMMON_VOIDLORD, false);
                _player->learnSpell(WARLOCK_SUMMON_SHIVARRA, false);
                _player->learnSpell(WARLOCK_SUMMON_OBSERVER, false);
                _player->learnSpell(WARLOCK_SUMMON_ABYSSAL, false);
                _player->learnSpell(WARLOCK_SUMMON_TERRORGUARD, false);

                if (_player->GetPrimaryTalentTree(_player->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                    _player->learnSpell(WARLOCK_SUMMON_WRATHGUARD, false);
            }
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* _player = GetTarget()->ToPlayer())
            {
                if (_player->HasSpell(WARLOCK_SUMMON_FEL_IMP))
                    _player->removeSpell(WARLOCK_SUMMON_FEL_IMP, false, false);

                if (_player->HasSpell(WARLOCK_SUMMON_VOIDLORD))
                    _player->removeSpell(WARLOCK_SUMMON_VOIDLORD, false, false);

                if (_player->HasSpell(WARLOCK_SUMMON_SHIVARRA))
                    _player->removeSpell(WARLOCK_SUMMON_SHIVARRA, false, false);

                if (_player->HasSpell(WARLOCK_SUMMON_OBSERVER))
                    _player->removeSpell(WARLOCK_SUMMON_OBSERVER, false, false);

                if (_player->HasSpell(WARLOCK_SUMMON_WRATHGUARD))
                    _player->removeSpell(WARLOCK_SUMMON_WRATHGUARD, false, false);

                if (_player->HasSpell(WARLOCK_SUMMON_ABYSSAL))
                    _player->removeSpell(WARLOCK_SUMMON_ABYSSAL, false, false);

                if (_player->HasSpell(WARLOCK_SUMMON_TERRORGUARD))
                    _player->removeSpell(WARLOCK_SUMMON_TERRORGUARD, false, false);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_grimoire_of_supremacy_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_grimoire_of_supremacy_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_grimoire_of_supremacy_AuraScript();
    }
};

void AddSC_warlock_spell_scripts()
{
    new spell_warl_bane_of_doom();
    new spell_warl_banish();
    new spell_warl_conflagrate();
    new spell_warl_create_healthstone();
    new spell_warl_use_healthstone();
    new spell_warl_demonic_circle_summon();
    new spell_warl_demonic_circle_teleport();
    new spell_warl_demonic_empowerment();
    new spell_warl_demon_soul();
    new spell_warl_everlasting_affliction();
    new spell_warl_fel_flame();
    new spell_warl_fel_synergy();
    new spell_warl_haunt();
    new spell_warl_health_funnel();
    new spell_warl_life_tap();
    new spell_warl_ritual_of_doom_effect();
    new spell_warl_seed_of_corruption();
    new spell_warl_shadow_ward();
    new spell_warl_siphon_life();
    new spell_warl_soulshatter();
    new spell_warl_unstable_affliction();
    new spell_warl_kil_jaedens_cunning();
    new spell_warl_burning_rush();
    new spell_warl_harvest_life();
    new spell_warl_grimoire_of_sacrifice();
    new spell_warl_burning_embers();
    new spell_warl_drain_life();
    new spell_warl_soulburn_drain_life();
    new spell_warl_molten_core_dot();
    new spell_warl_decimate();
    new spell_warl_metamorphosis_cost();
    new spell_warl_immolation_aura();
    new spell_warl_hellfire();
    new spell_warl_immolate();
    new spell_warl_incinerate();
    new spell_warl_conflagrate_and_brimstone();
    new spell_warl_chaos_bolt();
    new spell_warl_grimoire_of_supremacy();
}
