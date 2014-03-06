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
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "WorldSession.h"
#include "MapManager.h"
#include "ScriptedCreature.h"

enum DruidSpells
{
    SPELL_DRUID_WRATH                       = 5176,
    SPELL_DRUID_STARFIRE                    = 2912,
    SPELL_DRUID_STARSURGE                   = 78674,
    SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE    = 89265,
    SPELL_DRUID_STARSURGE_ENERGIZE          = 86605,
    SPELL_DRUID_LUNAR_ECLIPSE_MARKER        = 67484, // Will make the yellow arrow on eclipse bar point to the blue side (lunar)
    SPELL_DRUID_SOLAR_ECLIPSE_MARKER        = 67483, // Will make the yellow arrow on eclipse bar point to the yellow side (solar)
    SPELL_DRUID_SOLAR_ECLIPSE               = 48517,
    SPELL_DRUID_LUNAR_ECLIPSE               = 48518,
	SPELL_DRUID_EUPHORIA					= 81062,
    SPELL_DRUID_ENRAGE_MOD_DAMAGE           = 51185,
    SPELL_DRUID_GLYPH_OF_TYPHOON            = 62135,
    SPELL_DRUID_IDOL_OF_FERAL_SHADOWS       = 34241,
    SPELL_DRUID_IDOL_OF_WORSHIP             = 60774,
    SPELL_DRUID_INCREASED_MOONFIRE_DURATION = 38414,
    SPELL_DRUID_KING_OF_THE_JUNGLE          = 48492,
    SPELL_DRUID_LIFEBLOOM_ENERGIZE          = 64372,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL        = 33778,
    SPELL_DRUID_LIVING_SEED_HEAL            = 48503,
    SPELL_DRUID_LIVING_SEED_PROC            = 48504,
    SPELL_DRUID_NATURES_SPLENDOR            = 57865,
    SPELL_DRUID_SAVAGE_ROAR                 = 62071,
    SPELL_DRUID_TIGER_S_FURY_ENERGIZE       = 51178,
    SPELL_DRUID_ITEM_T8_BALANCE_RELIC       = 64950,
    SPELL_DRUID_FUNGAL_GROWTH               = 81283,
    SPELL_DRUID_WILD_MUSHROOM_SUICIDE       = 92853,
    SPELL_DRUID_WILD_MUSHROOM_DAMAGE        = 78777,
	DRUID_SURVIVAL_INSTINCTS                = 50322,
	SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER  = 102793,
    SPELL_DRUID_URSOLS_VORTEX_SNARE         = 127797,
    SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST     = 118283,
	SPELL_DRUID_SOUL_OF_THE_FOREST          = 114107,
    SPELL_DRUID_SOUL_OF_THE_FOREST_HASTE    = 114108,
	
	SPELL_DRUID_FRENZIED_REGENERAION        = 22842,
    SPELL_DRUID_FRENZIED_REGENERATION_HEAL  = 122307
};

enum DruidCreatures
{
    DRUID_NPC_FUNGAL_GROWTH                 = 43484,
    DRUID_NPC_WILD_MUSHROOM                 = 47649
};

// Mangle - 33917
class spell_dru_mangle : public SpellScriptLoader
{
public:
    spell_dru_mangle() : SpellScriptLoader("spell_dru_mangle") { }

    class spell_dru_mangle_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_mangle_SpellScript);

        void Cast()
        {
            ShapeshiftForm casterForm =  GetCaster()->GetShapeshiftForm();
            if(casterForm == FORM_CAT)
            {
                GetCaster()->CastSpell(GetHitUnit(), 33876, false);
            }
            else if (casterForm == FORM_BEAR)
            {
                GetCaster()->CastSpell(GetHitUnit(), 33878, false);
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_dru_mangle_SpellScript::Cast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_mangle_SpellScript;
    }
};

// Prowl - 5215
class spell_dru_prowl : public SpellScriptLoader
{
    public:
        spell_dru_prowl() : SpellScriptLoader("spell_dru_prowl") { }

        class spell_dru_prowl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_prowl_SpellScript);

            void Cast()
            {
                Unit* caster = GetCaster();
                //activate cat form (768)
                caster->CastSpell(caster, 768, false);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_dru_prowl_SpellScript::Cast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_prowl_SpellScript;
        }
};

enum SPELL_DRU_MANGLE
{
    SPELL_DRU_MANGLE_GENERIC = 33917,
    SPELL_DRU_MANGLE_BEAR    = 33878,
    SPELL_DRU_MANGLE_CAT     = 33876,

    SPELL_DRU_SWIPE_GENERIC  = 106785,
    SPELL_DRU_SWIPE_CAT      = 62078,
    SPELL_DRU_SWIPE_BEAR     = 779
};

// Bear form - 5487
// Cat form - 768
class spell_dru_bear_cat : public SpellScriptLoader
{
public:
    spell_dru_bear_cat() : SpellScriptLoader("spell_dru_bear_cat") { }

    class spell_dru_bear_cat_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bear_cat_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            return true;
        }

        void OnShapeshiftApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* unitTarget = GetTarget();
            if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
            {
                //         new
                std::list<uint32> spellsToAdd;
                //         new       old
                std::map<uint32, uint32> spellsToChange;
                if(aurEff->GetMiscValue() == FORM_CAT)
                {
                    if(unitTarget->HasSpell(SPELL_DRU_MANGLE_GENERIC))
                    {
                        spellsToAdd.push_back(SPELL_DRU_MANGLE_GENERIC);
                        spellsToChange[SPELL_DRU_MANGLE_CAT] = SPELL_DRU_MANGLE_GENERIC;   
                    }
                    if(unitTarget->HasSpell(SPELL_DRU_SWIPE_GENERIC))
                    {
                        spellsToAdd.push_back(SPELL_DRU_SWIPE_GENERIC);
                        spellsToChange[SPELL_DRU_SWIPE_CAT] = SPELL_DRU_SWIPE_GENERIC;
                    }
                }
                else if(aurEff->GetMiscValue() == FORM_BEAR)
                {
                    if(unitTarget->HasSpell(SPELL_DRU_MANGLE_GENERIC))
                    {
                        spellsToAdd.push_back(SPELL_DRU_MANGLE_GENERIC);
                        spellsToChange[SPELL_DRU_MANGLE_BEAR] = SPELL_DRU_MANGLE_GENERIC;  
                    }
                    if(unitTarget->HasSpell(SPELL_DRU_SWIPE_GENERIC))
                    {
                        spellsToAdd.push_back(SPELL_DRU_SWIPE_GENERIC);
                        spellsToChange[SPELL_DRU_SWIPE_BEAR] = SPELL_DRU_SWIPE_GENERIC;
                    }
                }

                Player * player = unitTarget->ToPlayer();
                for(std::map<uint32, uint32>::iterator itr = spellsToChange.begin() ; itr != spellsToChange.end() ; itr++)
                {
                    WorldPacket data(SMSG_SUPERCEDED_SPELL);
                    data.WriteBits(1, 24);
                    data.WriteBits(1, 24);
                    player->AddTemporarySpell(itr->first);
                    data << uint32(itr->first);
                    data << uint32(itr->second);
                    player->GetSession()->SendPacket(&data);
                }

                WorldPacket data(SMSG_LEARNED_SPELL);
                data.WriteBit(1);
                data.WriteBits(spellsToAdd.size(), 24); // Spell Count
                data.FlushBits();
                for(std::list<uint32>::iterator itr = spellsToAdd.begin(); itr != spellsToAdd.end() ; itr++)
                    data << uint32(*itr);
                player->GetSession()->SendPacket(&data);
            }
        }

        void OnShapeshiftRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* unitTarget = GetTarget();
            if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
            {
                //         new       old
                std::map<uint32, uint32> spellsToChange;
                if(aurEff->GetMiscValue() == FORM_CAT)
                {
                    if(unitTarget->HasSpell(SPELL_DRU_MANGLE_GENERIC))
                    {
                        spellsToChange[SPELL_DRU_MANGLE_CAT] = SPELL_DRU_MANGLE_GENERIC;   
                    }
                    if(unitTarget->HasSpell(SPELL_DRU_SWIPE_GENERIC))
                    {
                        spellsToChange[SPELL_DRU_SWIPE_CAT] = SPELL_DRU_SWIPE_GENERIC;
                    }
                }
                else if(aurEff->GetMiscValue() == FORM_BEAR)
                {
                    if(unitTarget->HasSpell(SPELL_DRU_MANGLE_GENERIC))
                    {
                        spellsToChange[SPELL_DRU_MANGLE_BEAR] = SPELL_DRU_MANGLE_GENERIC;  
                    }
                    if(unitTarget->HasSpell(SPELL_DRU_SWIPE_GENERIC))
                    {
                        spellsToChange[SPELL_DRU_SWIPE_BEAR] = SPELL_DRU_SWIPE_GENERIC;
                    }
                }

                Player * player = unitTarget->ToPlayer();
                for(std::map<uint32, uint32>::iterator itr = spellsToChange.begin() ; itr != spellsToChange.end() ; itr++)
                {
                    WorldPacket data(SMSG_SUPERCEDED_SPELL);
                    data.WriteBits(1, 24);
                    data.WriteBits(1, 24);
                    player->RemoveTemporarySpell(itr->first);
                    data << uint32(itr->second);
                    data << uint32(itr->first);
                    player->GetSession()->SendPacket(&data);
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_bear_cat_AuraScript::OnShapeshiftApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_bear_cat_AuraScript::OnShapeshiftRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_bear_cat_AuraScript();
    }
};

// Growl - 6795
class spell_dru_growl : public SpellScriptLoader
{
public:
    spell_dru_growl() : SpellScriptLoader("spell_dru_growl") { }

    class spell_dru_growl_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_growl_SpellScript);

        void Cast()
        {
            Unit* caster = GetCaster();
            //activate bear form (5487)
            caster->CastSpell(caster, 5487, false);
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_dru_growl_SpellScript::Cast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_growl_SpellScript;
    }
};

// Might of Ursoc
// 106922 - SpellId
class spell_dru_might_of_ursoc : public SpellScriptLoader
{
public:
    spell_dru_might_of_ursoc() : SpellScriptLoader("spell_dru_might_of_ursoc") { }

    class spell_dru_might_of_ursoc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_might_of_ursoc_AuraScript);

        void EffectApply (AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            int32 amount = aurEff->GetAmount() * target->GetMaxHealth() / 100;
            target->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, float( aurEff->GetAmount() ), true);
            target->ModifyHealth( amount);
            //Cast bear form
            target->CastSpell(target, 5487, true);
            PreventDefaultAction();
        }

        void EffectRemove (AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            int32 amount = aurEff->GetAmount() * target->GetMaxHealth() / 100;
            
            
            if (int32(target->GetHealth()) >  amount)
                target->ModifyHealth(- amount);
            else
                target->SetHealth(1);

            target->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, float( aurEff->GetAmount() ), false);

            PreventDefaultAction();
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_might_of_ursoc_AuraScript::EffectApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_might_of_ursoc_AuraScript::EffectRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_might_of_ursoc_AuraScript;
    }
};

//Stampeding Roar
// SpellId : 106898
class spell_dru_stampeding_roar : public SpellScriptLoader
{
public:
    spell_dru_stampeding_roar() : SpellScriptLoader("spell_dru_stampeding_roar") { }

    class spell_dru_stampeding_roar_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_stampeding_roar_AuraScript);

        void EffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            ShapeshiftForm form = GetCaster()->GetShapeshiftForm();
            //Cast bear form
            if(form != FORM_BEAR && form != FORM_CAT)
                GetCaster()->CastSpell(GetCaster(), 5487, true);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_stampeding_roar_AuraScript::EffectApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_stampeding_roar_AuraScript;
    }
};


// 2912, 5176, 78674 - Starfire, Wrath, and Starsurge
class spell_dru_eclipse_energize : public SpellScriptLoader
{
    public:
        spell_dru_eclipse_energize() : SpellScriptLoader("spell_dru_eclipse_energize") { }

        class spell_dru_eclipse_energize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_eclipse_energize_SpellScript);


            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                if (GetCaster()->ToPlayer()->getClass() != CLASS_DRUID)
                    return false;

                return true;
            }

			void ModEclipsePower(int32 val)
			{
				Player* caster = GetCaster()->ToPlayer();

				if(!caster)
					return;

				//If the player has just logged in
				if(!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
				{
					caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
					if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
						caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE);
					if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
						caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
				}

				// Prevent eclipse to got in the wrong direction
				if(caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && val < 0)
					return;
				if(caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && val > 0)
					return;

				caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &val, 0, 0, true);
				int32 eclipse = caster->GetPower(POWER_ECLIPSE);

				//Removing eclipses auras when passing 0
				if (eclipse >= 0)
				{
					if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
					{
						caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE);
						if(caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST))
							ModEclipsePower(20);
					}
				}
				if (eclipse <= 0)
				{
					if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
					{
						caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
						if(caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST))
							ModEclipsePower(-20);
					}
				}

				//Check if player has reached an eclipse
				if (eclipse >= 100)
				{
					if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
						caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
					caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE, true);
					caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
				}
				if (eclipse <= -100)
				{
					if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
						caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
					caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE, true);
					caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
				}
				
			};

            void HandleEnergize(SpellEffIndex effIndex)
            {
                Player* caster = GetCaster()->ToPlayer();

                // No boomy, no deal.
                if (caster->GetPrimaryTalentTree(caster->GetActiveSpec()) != TALENT_TREE_DRUID_BALANCE)
                    return;

				int32 energizeAmount;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_DRUID_WRATH:
                    {
                        energizeAmount = -GetSpellInfo()->Effects[effIndex].BasePoints; // -15
                        break;
                    }
                    case SPELL_DRUID_STARFIRE:
                    {
                        energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // 20
                        break;
                    }
                    case SPELL_DRUID_STARSURGE:
                    {
                        // If we are set to fill the solar side or we've just logged in with 0 power (confirmed with sniffs)
                        if (!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                            energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // 20
                        else if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                            energizeAmount = -GetSpellInfo()->Effects[effIndex].BasePoints; // -20
						else
							energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // When just logged in
                        break;
                    }
                }

				if (caster->HasAura(SPELL_DRUID_EUPHORIA))
					if(!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE) && !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
						energizeAmount *= 2;

				ModEclipsePower(energizeAmount);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_eclipse_energize_SpellScript::HandleEnergize, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_eclipse_energize_SpellScript;
        }
};

// -1850 - Dash
class spell_dru_dash : public SpellScriptLoader
{
    public:
        spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

        class spell_dru_dash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_dash_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                // do not set speed if not in cat form
                if (GetUnitOwner()->GetShapeshiftForm() != FORM_CAT)
                    amount = 0;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_dash_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_dash_AuraScript();
        }
};

// -5229 - Enrage
class spell_dru_enrage : public SpellScriptLoader
{
    public:
        spell_dru_enrage() : SpellScriptLoader("spell_dru_enrage") { }

        class spell_dru_enrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_enrage_SpellScript);

            void OnHit()
            {
                if (AuraEffect const* aurEff = GetHitUnit()->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_JUNGLE, EFFECT_0))
                    GetHitUnit()->CastCustomSpell(SPELL_DRUID_ENRAGE_MOD_DAMAGE, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetHitUnit(), true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_dru_enrage_SpellScript::OnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_enrage_SpellScript();
        }
};

// 54846 - Glyph of Starfire
class spell_dru_glyph_of_starfire : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_starfire() : SpellScriptLoader("spell_dru_glyph_of_starfire") { }

        class spell_dru_glyph_of_starfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_glyph_of_starfire_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_INCREASED_MOONFIRE_DURATION) || !sSpellMgr->GetSpellInfo(SPELL_DRUID_NATURES_SPLENDOR))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                    if (AuraEffect const* aurEff = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_DRUID, 0x00000002, 0, 0, caster->GetGUID()))
                    {
                        Aura* aura = aurEff->GetBase();

                        uint32 countMin = aura->GetMaxDuration();
                        uint32 countMax = aura->GetSpellInfo()->GetMaxDuration() + 9000;
                        if (caster->HasAura(SPELL_DRUID_INCREASED_MOONFIRE_DURATION))
                            countMax += 3000;
                        if (caster->HasAura(SPELL_DRUID_NATURES_SPLENDOR))
                            countMax += 3000;

                        if (countMin < countMax)
                        {
                            aura->SetDuration(uint32(aura->GetDuration() + 3000));
                            aura->SetMaxDuration(countMin + 3000);
                        }
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_starfire_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_glyph_of_starfire_SpellScript();
        }
};

// 34246 - Idol of the Emerald Queen
// 60779 - Idol of Lush Moss
class spell_dru_idol_lifebloom : public SpellScriptLoader
{
    public:
        spell_dru_idol_lifebloom() : SpellScriptLoader("spell_dru_idol_lifebloom") { }

        class spell_dru_idol_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_idol_lifebloom_AuraScript);

            void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
            {
                if (!spellMod)
                {
                    spellMod = new SpellModifier(GetAura());
                    spellMod->op = SPELLMOD_DOT;
                    spellMod->type = SPELLMOD_FLAT;
                    spellMod->spellId = GetId();
                    spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
                }
                spellMod->value = aurEff->GetAmount() / 7;
            }

            void Register()
            {
                DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_idol_lifebloom_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_idol_lifebloom_AuraScript();
        }
};

// 29166 - Innervate
class spell_dru_innervate : public SpellScriptLoader
{
    public:
        spell_dru_innervate() : SpellScriptLoader("spell_dru_innervate") { }

        class spell_dru_innervate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_innervate_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = CalculatePct(int32(GetUnitOwner()->GetCreatePowers(POWER_MANA) / aurEff->GetTotalTicks()), amount);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_innervate_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_innervate_AuraScript();
        }
};

// -5570 - Insect Swarm
class spell_dru_insect_swarm : public SpellScriptLoader
{
    public:
        spell_dru_insect_swarm() : SpellScriptLoader("spell_dru_insect_swarm") { }

        class spell_dru_insect_swarm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_insect_swarm_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                    if (AuraEffect const* relicAurEff = caster->GetAuraEffect(SPELL_DRUID_ITEM_T8_BALANCE_RELIC, EFFECT_0))
                        amount += relicAurEff->GetAmount() / aurEff->GetTotalTicks();
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_insect_swarm_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_insect_swarm_AuraScript();
        }
};

// -33763 - Lifebloom
class spell_dru_lifebloom : public SpellScriptLoader
{
    public:
        spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

        class spell_dru_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lifebloom_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_ENERGIZE))
                    return false;
                return true;
            }

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                // final heal
                int32 stack = GetStackAmount();
                int32 healAmount = aurEff->GetAmount();
                if (Unit* caster = GetCaster())
                {
                    healAmount = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), healAmount, HEAL, stack);
                    healAmount = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, stack);

                    GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());

                    // restore mana
                    int32 returnMana = CalculatePct(caster->GetCreateMana(), GetSpellInfo()->GetPowerCostPercentage(caster)) * stack / 2;
                    caster->CastCustomSpell(caster, SPELL_DRUID_LIFEBLOOM_ENERGIZE, &returnMana, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
                    return;
                }

                GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* target = GetUnitOwner())
                {
                    if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                    {
                        // final heal
                        int32 healAmount = aurEff->GetAmount();
                        if (Unit* caster = GetCaster())
                        {
                            healAmount = caster->SpellHealingBonusDone(target, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());
                            healAmount = target->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());
                            target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, NULL, GetCasterGUID());

                            // restore mana
                            int32 returnMana = CalculatePct(caster->GetCreateMana(), GetSpellInfo()->GetPowerCostPercentage(caster)) * dispelInfo->GetRemovedCharges() / 2;
                            caster->CastCustomSpell(caster, SPELL_DRUID_LIFEBLOOM_ENERGIZE, &returnMana, NULL, NULL, true, NULL, NULL, GetCasterGUID());
                            return;
                        }

                        target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, NULL, GetCasterGUID());
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lifebloom_AuraScript();
        }
};

// -48496 - Living Seed
class spell_dru_living_seed : public SpellScriptLoader
{
    public:
        spell_dru_living_seed() : SpellScriptLoader("spell_dru_living_seed") { }

        class spell_dru_living_seed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_living_seed_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIVING_SEED_PROC))
                    return false;
                return true;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                int32 amount = CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount());
                GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_PROC, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_living_seed_AuraScript();
        }
};

// 48504 - Living Seed (Proc)
class spell_dru_living_seed_proc : public SpellScriptLoader
{
    public:
        spell_dru_living_seed_proc() : SpellScriptLoader("spell_dru_living_seed_proc") { }

        class spell_dru_living_seed_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_living_seed_proc_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIVING_SEED_HEAL))
                    return false;
                return true;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true, NULL, aurEff);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_living_seed_proc_AuraScript();
        }
};

// 69366 - Moonkin Form passive
class spell_dru_moonkin_form_passive : public SpellScriptLoader
{
    public:
        spell_dru_moonkin_form_passive() : SpellScriptLoader("spell_dru_moonkin_form_passive") { }

        class spell_dru_moonkin_form_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_moonkin_form_passive_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                // reduces all damage taken while Stunned in Moonkin Form
                if (GetTarget()->GetUInt32Value(UNIT_FIELD_FLAGS) & (UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1<<MECHANIC_STUN))
                    absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_moonkin_form_passive_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_moonkin_form_passive_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_moonkin_form_passive_AuraScript();
        }
};

// 48391 - Owlkin Frenzy
class spell_dru_owlkin_frenzy : public SpellScriptLoader
{
    public:
        spell_dru_owlkin_frenzy() : SpellScriptLoader("spell_dru_owlkin_frenzy") { }

        class spell_dru_owlkin_frenzy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_owlkin_frenzy_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = CalculatePct(GetUnitOwner()->GetCreatePowers(POWER_MANA), amount);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_owlkin_frenzy_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_PERIODIC_ENERGIZE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_owlkin_frenzy_AuraScript();
        }
};

// -16972 - Predatory Strikes
class spell_dru_predatory_strikes : public SpellScriptLoader
{
    public:
        spell_dru_predatory_strikes() : SpellScriptLoader("spell_dru_predatory_strikes") { }

        class spell_dru_predatory_strikes_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_predatory_strikes_AuraScript);

            void UpdateAmount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    target->UpdateAttackPowerAndDamage();
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_predatory_strikes_AuraScript();
        }
};

// 33851 - Primal Tenacity
class spell_dru_primal_tenacity : public SpellScriptLoader
{
    public:
        spell_dru_primal_tenacity() : SpellScriptLoader("spell_dru_primal_tenacity") { }

        class spell_dru_primal_tenacity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_primal_tenacity_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                // reduces all damage taken while Stunned in Cat Form
                if (GetTarget()->GetShapeshiftForm() == FORM_CAT && GetTarget()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1<<MECHANIC_STUN))
                    absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_primal_tenacity_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_primal_tenacity_AuraScript::Absorb, EFFECT_1);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_primal_tenacity_AuraScript();
        }
};

// -1079 - Rip
class spell_dru_rip : public SpellScriptLoader
{
    public:
        spell_dru_rip() : SpellScriptLoader("spell_dru_rip") { }

        class spell_dru_rip_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rip_AuraScript);

            bool Load()
            {
                Unit* caster = GetCaster();
                return caster && caster->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                canBeRecalculated = false;

                if (Unit* caster = GetCaster())
                {
                    // 0.01 * $AP * cp
                    uint8 cp = caster->ToPlayer()->GetComboPoints();

                    // Idol of Feral Shadows. Can't be handled as SpellMod due its dependency from CPs
                    if (AuraEffect const* idol = caster->GetAuraEffect(SPELL_DRUID_IDOL_OF_FERAL_SHADOWS, EFFECT_0))
                        amount += cp * idol->GetAmount();
                    // Idol of Worship. Can't be handled as SpellMod due its dependency from CPs
                    else if (AuraEffect const* idol = caster->GetAuraEffect(SPELL_DRUID_IDOL_OF_WORSHIP, EFFECT_0))
                        amount += cp * idol->GetAmount();

                    amount += int32(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), cp));
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rip_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_rip_AuraScript();
        }
};

// 62606 - Savage Defense
class spell_dru_savage_defense : public SpellScriptLoader
{
    public:
        spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

        class spell_dru_savage_defense_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_savage_defense_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
            {
                absorbAmount = uint32(CalculatePct(GetTarget()->GetTotalAttackPowerValue(BASE_ATTACK), absorbPct));
                aurEff->SetAmount(0);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_savage_defense_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_savage_defense_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_savage_defense_AuraScript();
        }
};

// 52610 - Savage Roar
class spell_dru_savage_roar : public SpellScriptLoader
{
    public:
        spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

        class spell_dru_savage_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_roar_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetShapeshiftForm() != FORM_CAT)
                    return SPELL_FAILED_ONLY_SHAPESHIFT;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
            }
        };

        class spell_dru_savage_roar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_savage_roar_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_ROAR))
                    return false;
                return true;
            }

            void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->CastSpell(target, SPELL_DRUID_SAVAGE_ROAR, true, NULL, aurEff, GetCasterGUID());
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_savage_roar_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_savage_roar_AuraScript();
        }
};

// -50294 - Starfall (AOE)
class spell_dru_starfall_aoe : public SpellScriptLoader
{
    public:
        spell_dru_starfall_aoe() : SpellScriptLoader("spell_dru_starfall_aoe") { }

        class spell_dru_starfall_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetExplTargetUnit());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_starfall_aoe_SpellScript();
        }
};

// -50286 - Starfall (Dummy)
class spell_dru_starfall_dummy : public SpellScriptLoader
{
    public:
        spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

        class spell_dru_starfall_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Trinity::Containers::RandomResizeList(targets, 2);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                // Shapeshifting into an animal form or mounting cancels the effect
                if (caster->GetCreatureType() == CREATURE_TYPE_BEAST || caster->IsMounted())
                {
                    if (SpellInfo const* spellInfo = GetTriggeringSpell())
                        caster->RemoveAurasDueToSpell(spellInfo->Id);
                    return;
                }

                // Any effect which causes you to lose control of your character will supress the starfall effect.
                if (caster->HasUnitState(UNIT_STATE_CONTROLLED))
                    return;

                caster->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_starfall_dummy_SpellScript();
        }
};

// Survival Instincts - 61336
class spell_dru_survival_instincts : public SpellScriptLoader
{
    public:
        spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

        class spell_dru_survival_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

            void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->CastSpell(GetTarget(), DRUID_SURVIVAL_INSTINCTS, true);
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(DRUID_SURVIVAL_INSTINCTS);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_survival_instincts_AuraScript();
        }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
    public:
        spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

        class spell_dru_swift_flight_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                        amount = 310;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_swift_flight_passive_AuraScript();
        }
};

// -5217 - Tiger's Fury
class spell_dru_tiger_s_fury : public SpellScriptLoader
{
    public:
        spell_dru_tiger_s_fury() : SpellScriptLoader("spell_dru_tiger_s_fury") { }

        class spell_dru_tiger_s_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_tiger_s_fury_SpellScript);

            void OnHit()
            {
                if (AuraEffect const* aurEff = GetHitUnit()->GetAuraEffectOfRankedSpell(SPELL_DRUID_KING_OF_THE_JUNGLE, EFFECT_1))
                    GetHitUnit()->CastCustomSpell(SPELL_DRUID_TIGER_S_FURY_ENERGIZE, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetHitUnit(), true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_dru_tiger_s_fury_SpellScript::OnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_tiger_s_fury_SpellScript();
        }
};

// -61391 - Typhoon
class spell_dru_typhoon : public SpellScriptLoader
{
    public:
        spell_dru_typhoon() : SpellScriptLoader("spell_dru_typhoon") { }

        class spell_dru_typhoon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_typhoon_SpellScript);

            void HandleKnockBack(SpellEffIndex effIndex)
            {
                // Glyph of Typhoon
                if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_TYPHOON))
                    PreventHitDefaultEffect(effIndex);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_typhoon_SpellScript::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_typhoon_SpellScript();
        }
};

// 70691 - Item T10 Restoration 4P Bonus
class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

        class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster()->ToPlayer()->GetGroup())
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
                else
                {
                    targets.remove(GetExplTargetUnit());
                    std::list<Unit*> tempTargets;
                    for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        if ((*itr)->GetTypeId() == TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                            tempTargets.push_back((*itr)->ToUnit());

                    if (tempTargets.empty())
                    {
                        targets.clear();
                        FinishCast(SPELL_FAILED_DONT_REPORT);
                        return;
                    }

                    Unit* target = Trinity::Containers::SelectRandomContainerElement(tempTargets);
                    targets.clear();
                    targets.push_back(target);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_t10_restoration_4p_bonus_SpellScript();
        }
};

/* 
EXECUTE THOSE SQL QUERIES: 

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (88747, 'spell_druid_wild_mushroom'); 
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (88751, 'spell_druid_wild_mushroom_detonate'); 
*/

// Wild mushroom, 88747
class spell_druid_wild_mushroom : public SpellScriptLoader
{
    public:
        spell_druid_wild_mushroom() : SpellScriptLoader("spell_druid_wild_mushroom") { }

        class spell_druid_wild_mushroom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_druid_wild_mushroom_SpellScript)

            void HandleSummon(SpellEffIndex effIndex)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    PreventHitDefaultEffect(effIndex);
                    SpellInfo const* spell = GetSpellInfo();

                    std::list<Creature*> list;
                    player->GetCreatureListWithEntryInGrid(list, DRUID_NPC_WILD_MUSHROOM, 500.0f);
                    for (std::list<Creature*>::iterator i = list.begin(); i != list.end(); ++i)
                    {
                        if ((*i)->isSummon() && (*i)->GetCharmerOrOwner() == player)
                            if (!player)
                                return;
                        continue;

                        list.remove((*i));
                    }

                    // Max 3 Wild Mushroom
                    if ((int32)list.size() >= spell->Effects[0].BasePoints)
                        list.front()->ToTempSummon()->UnSummon();

                    Position pos;
                    GetExplTargetDest()->GetPosition(&pos);
                    const SummonPropertiesEntry* properties = sSummonPropertiesStore.LookupEntry(spell->Effects[effIndex].MiscValueB);
                    TempSummon* summon = player->SummonCreature(spell->Effects[0].MiscValue, pos, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, spell->GetDuration());

                    if (!summon)
                        return;

                    summon->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, player->GetGUID());
                    summon->setFaction(player->getFaction());
                    summon->SetUInt32Value(UNIT_CREATED_BY_SPELL, GetSpellInfo()->Id);
                    summon->SetMaxHealth(5);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    summon->StopMoving();
                    summon->SetControlled(true, UNIT_STATE_STUNNED);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_druid_wild_mushroom_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_druid_wild_mushroom_SpellScript();
        }
};

/*
// Wild mushroom : Detonate, 88751
class spell_druid_wild_mushroom_detonate : public SpellScriptLoader
{
    public:
        spell_druid_wild_mushroom_detonate() : SpellScriptLoader("spell_druid_wild_mushroom_detonate") { }

        class spell_druid_wild_mushroom_detonate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_druid_wild_mushroom_detonate_SpellScript)

            // Globals variables
            float spellRange;
            std::list<TempSummon*> mushroomList;
            std::list<TempSummon*> fungal_List;

            bool Load()
            {
                spellRange = GetSpellInfo()->GetMaxRange();

                Player* player = GetCaster()->ToPlayer();

                if (!player)
                    return false;

                std::list<Creature*> list;
                std::list<TempSummon*> summonList;
                player->GetCreatureListWithEntryInGrid(list, DRUID_NPC_WILD_MUSHROOM, 500.0f);

                for (std::list<Creature*>::const_iterator i = list.begin(); i != list.end(); ++i)
                {
                    if ((*i)->isSummon() && (*i)->GetCharmerOrOwner() == player)
                    {
                        summonList.push_back((*i)->ToTempSummon());
                        continue;
                    }
                }
                mushroomList = summonList;

                if (!spellRange)
                    return false;

                return true;
            }

            SpellCastResult CheckCast()
            {
                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return SPELL_FAILED_CASTER_DEAD;

                if (mushroomList.empty())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                bool inRange = false;

                for (std::list<TempSummon*>::const_iterator i = mushroomList.begin(); i != mushroomList.end(); ++i)
                {
                    Position shroomPos;
                    (*i)->GetPosition(&shroomPos);

                    // Must have at least one mushroom within 40 yards
                    if (player->IsWithinDist3d(&shroomPos, spellRange))
                    {
                        inRange = true;
                        break;
                    }
                }

                if (!inRange)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_TARGET_TOO_FAR);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    uint32 fungal = NULL;
                    uint32 npcfungal = NULL;

                    fungal = SPELL_DRUID_FUNGAL_GROWTH;
                    npcfungal = DRUID_NPC_FUNGAL_GROWTH;


                    for (std::list<TempSummon*>::const_iterator i = mushroomList.begin(); i != mushroomList.end(); ++i)
                    {
                        Position shroomPos;

                        (*i)->GetPosition(&shroomPos);

                        if (!player->IsWithinDist3d(&shroomPos, spellRange))
                            continue;

                        // Explosion visual and suicide
                        (*i)->CastSpell((*i), SPELL_DRUID_WILD_MUSHROOM_SUICIDE, true);

                        // damage
                        player->CastSpell((*i)->GetPositionX(), (*i)->GetPositionY(), (*i)->GetPositionZ(), SPELL_DRUID_WILD_MUSHROOM_DAMAGE, true);

                        // Summoning fungal growth
                        if (fungal)
                            player->CastSpell((*i)->GetPositionX(), (*i)->GetPositionY(), (*i)->GetPositionZ(), fungal, true);
                    }

                    // Stop Moving on Fungal Growth
                    std::list<Creature*> fungal_list;
                    std::list<TempSummon*> fungalsummonList;
                    player->GetCreatureListWithEntryInGrid(fungal_list, npcfungal, 500.0f);

                    for (std::list<Creature*>::const_iterator i = fungal_list.begin(); i != fungal_list.end(); ++i)
                    {
                        if ((*i)->isSummon() && (*i)->GetCharmerOrOwner() == player)
                        {
                            fungalsummonList.push_back((*i)->ToTempSummon());
                            continue;
                        }
                    }
                    fungal_List = fungalsummonList;

                    if (!fungal_List.empty())
                    {
                        for (std::list<TempSummon*>::const_iterator iter = fungal_List.begin(); iter != fungal_List.end(); ++iter)
                        {
                            (*iter)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                            (*iter)->StopMoving();
                            (*iter)->SetControlled(true, UNIT_STATE_STUNNED);
                        }
                    }

                    // Cleanup Wild Mushroom
                    std::list<Creature*> list;
                    player->GetCreatureListWithEntryInGrid(list, DRUID_NPC_WILD_MUSHROOM, 500.0f);
                    for (std::list<Creature*>::iterator i = list.begin(); i != list.end(); ++i)
                    {
                        if ((*i)->isSummon() && (*i)->GetCharmerOrOwner() == player)
                            if (!player)
                                return;
                        continue;

                        list.remove((*i));
                    }

                    if ((int32)list.size() > 0)
                        list.front()->ToTempSummon()->UnSummon();
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_druid_wild_mushroom_detonate_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_druid_wild_mushroom_detonate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_druid_wild_mushroom_detonate_SpellScript();
        }
};
*/

// 102280 - Displacer Beast
class spell_dru_displacer_beast : public SpellScriptLoader
{
    public:
        spell_dru_displacer_beast() : SpellScriptLoader("spell_dru_displacer_beast") { }

        class spell_dru_displacer_beast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_displacer_beast_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

			void HandleCatForm(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetCaster(), 768, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_displacer_beast_SpellScript::HandleCatForm, EFFECT_0, SPELL_EFFECT_LEAP);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_displacer_beast_SpellScript();
        }
};

// 106737 Force of Nature
class spell_dru_force_of_nature : public SpellScriptLoader
{
    public:
        spell_dru_force_of_nature() : SpellScriptLoader("spell_dru_force_of_nature") { }

        class spell_dru_force_of_nature_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_force_of_nature_SpellScript);

            void Cast()
            {
				Position pos;
				GetExplTargetDest()->GetPosition(&pos);
				for(uint8 i=0; i<3; ++i)
				{
					TempSummon* summon = GetCaster()->SummonCreature(54983, pos, TEMPSUMMON_TIMED_DESPAWN, 15000);
				}

            }

            void Register()
            {
                OnCast += SpellCastFn(spell_dru_force_of_nature_SpellScript::Cast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_force_of_nature_SpellScript();
        }
};

// 54983 -- treant - Force of Nature
class npc_treant : public CreatureScript 
{
public:
	npc_treant() : CreatureScript("npc_treant") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_treantAI(creature);
	}

	struct npc_treantAI : public ScriptedAI
	{
		npc_treantAI(Creature *creature) : ScriptedAI(creature)
		{
		}

		EventMap events;
		bool needVictim;
		

		enum Events
		{
			EVENT_HEAL = 0,
			EVENT_BASH,
			EVENT_WRATH,
			EVENT_TAUNT,
			EVENT_ROOT
		};

		enum DisplayIDs
		{
			DISPLAY_HEAL = 18922,
			DISPLAY_FERAL = 40690,
			DISPLAY_CAST = 40688,
			DISPLAY_TANK = 40692
		};

		enum Spells
		{
			SPELL_HEAL = 113828,
			SPELL_TAUNT = 122719,
			SPELL_ROOT = 113770,
			SPELL_WRATH = 113769,
			SPELL_BASH = 113801
		};

		void Reset()
		{
			Player* owner = ((TempSummon*)me)->GetSummoner()->ToPlayer();
			me->SetMaxHealth(owner->GetMaxHealth()/10);
			me->SetHealth(owner->GetMaxHealth()/10);
					
			
			needVictim = true;

			switch(owner->GetPrimaryTalentTree(owner->GetActiveSpec()))
			{
			case TALENT_TREE_DRUID_RESTORATION:
				{
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
					me->SetDisplayId(DISPLAY_HEAL);
					events.ScheduleEvent(EVENT_HEAL, urand(500, 1500));
					events.ScheduleEvent(EVENT_ROOT, urand(1500, 3000));
					needVictim = false;
					break;
				}
			case TALENT_TREE_DRUID_FERAL:
				{
					me->SetDisplayId(DISPLAY_FERAL);
					me->SetStatFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER, (1 + (owner->GetTotalAttackPowerValue(BASE_ATTACK) / 14) * 2 * 0.75f + 1));
					events.ScheduleEvent(EVENT_BASH, urand(4000, 6000));
					break;
				}
			case TALENT_TREE_DRUID_GUARDIAN:
				{
					me->SetDisplayId(DISPLAY_TANK);
					me->SetStatFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER, ((1 + (owner->GetTotalAttackPowerValue(BASE_ATTACK) / 14) * 2 * 0.75f) * 0.2f + 1));
					events.ScheduleEvent(EVENT_TAUNT, urand(1000, 2000));
					break;
				}
			case TALENT_TREE_DRUID_BALANCE:
				{
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
					me->SetDisplayId(DISPLAY_CAST);
					events.ScheduleEvent(EVENT_WRATH, urand(500, 1500));
					events.ScheduleEvent(EVENT_ROOT, urand(1500, 3000));
					break;
				}
			default:
				break;
			}

			me->setFaction(owner->getFaction());
			me->SetReactState(REACT_AGGRESSIVE);
			DoZoneInCombat();			
		}

		void UpdateAI(uint32 diff)
		{	
			if(needVictim && !UpdateVictim())
				return;

			events.Update(diff);


			
			while(uint32 event = events.ExecuteEvent())
			{
				int32 bp0;
				switch(event)
				{
					case EVENT_BASH:
					{
						DoCast(SPELL_BASH);
						events.ScheduleEvent(EVENT_BASH, urand(8500, 10000));
						break;
					}
					case EVENT_TAUNT:
					{
						DoCast(SPELL_TAUNT);
						events.ScheduleEvent(EVENT_TAUNT, urand(8500, 10000));
						break;
					}
					case EVENT_WRATH:
					{
						if(Player* owner = ((TempSummon*)me)->GetSummoner()->ToPlayer())
						{
							bp0 = owner->GetSpellDamage(1, urand(30, 32), 90, urand(1930, 2176), 0.f, 37.5f);
							me->CastCustomSpell(me->getVictim(), SPELL_WRATH, &bp0, NULL, NULL, false);
						}
						events.ScheduleEvent(EVENT_WRATH, 2000);
						break;
					}
					case EVENT_ROOT:
					{
						if(UpdateVictim()) // Need to check for heal spec
							DoCast(SPELL_ROOT);
						events.ScheduleEvent(EVENT_ROOT, urand(8500, 10000));
						break;
					}
					case EVENT_HEAL:
					{
						if(Player* owner = ((TempSummon*)me)->GetSummoner()->ToPlayer())
						{
							Player* lowest = NULL;
							Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
							if (!PlayerList.isEmpty())
							{
								for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
								{
									if(Player *p = i->getSource())
									{
										if(me->GetDistance(p) < 35.f)
										{
											if(lowest)
											{
												if(p->GetHealth() < lowest->GetHealth())
													lowest = p;
											}
											else
												lowest = p;
										}
									}
								}
							}

							bp0 = owner->GetSpellDamage(1, urand(48, 56), 90, urand(3201, 3779), 0.f, 32.3f, true);
							if(lowest)
								me->CastCustomSpell(lowest, SPELL_HEAL, &bp0, NULL, NULL, false);
						}

						events.ScheduleEvent(EVENT_HEAL, 2500);
						break;
					}
				}
			}

			if(Player* owner = ((TempSummon*)me)->GetSummoner()->ToPlayer())
			{
				if (owner->GetPrimaryTalentTree(owner->GetActiveSpec()) == TALENT_TREE_DRUID_FERAL
					||(owner->GetPrimaryTalentTree(owner->GetActiveSpec())) == TALENT_TREE_DRUID_GUARDIAN)
					DoMeleeAttackIfReady();
			}

		}
	};
};

// Ursol's Vortex (snare) - 127797
/*
class spell_dru_ursols_vortex_snare : public SpellScriptLoader
{
    public:
        spell_dru_ursols_vortex_snare() : SpellScriptLoader("spell_dru_ursols_vortex_snare") { }

        class spell_dru_ursols_vortex_snare_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursols_vortex_snare_AuraScript);

            std::list<Unit*> targetList;

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                aurEff->GetTargetList(targetList);

				for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                {
                    if (Unit* caster = GetCaster())
                        if (AreaTrigger* areaTrigger = caster->GetAreaTrigger(SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER))
                            if ((*i)->GetDistance(areaTrigger) > 8.0f && !(*i)->HasAura(SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST))
                                (*i)->CastSpell(areaTrigger->GetPositionX(), areaTrigger->GetPositionY(), areaTrigger->GetPositionZ(), SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST, true);
                }

                targetList.clear();
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_ursols_vortex_snare_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ursols_vortex_snare_AuraScript();
        }
};

// Ursol's Vortex - 102793
class spell_dru_ursols_vortex : public SpellScriptLoader
{
    public:
        spell_dru_ursols_vortex() : SpellScriptLoader("spell_dru_ursols_vortex") { }

        class spell_dru_ursols_vortex_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_ursols_vortex_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (!target->HasAura(SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER))
                            _player->CastSpell(target, SPELL_DRUID_URSOLS_VORTEX_SNARE, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_ursols_vortex_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_ursols_vortex_SpellScript();
        }

        class spell_dru_ursols_vortex_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursols_vortex_AuraScript);

            std::list<Unit*> targetList;

            void OnUpdate(uint32 diff, AuraEffectPtr aurEff)
            {
                aurEff->GetTargetList(targetList);

				for (std::list<Unit*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                {
                    if (Unit* caster = GetCaster())
                        if (DynamicObject* dynObj = caster->GetDynObject(SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER))
                            if ((*i)->GetDistance(dynObj) > 8.0f && !(*i)->HasAura(SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST))
                                (*i)->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST, true);
                }

                targetList.clear();
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_ursols_vortex_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ursols_vortex_AuraScript();
        }
};
*/

// Called by Lunar Eclipse - 48518 and Solar Eclipse - 48517
// Soul of the Forest - 114107
/*
class spell_dru_soul_of_the_forest_eclipse : public SpellScriptLoader
{
    public:
        spell_dru_soul_of_the_forest_eclipse() : SpellScriptLoader("spell_dru_soul_of_the_forest_eclipse") { }

        class spell_dru_soul_of_the_forest_eclipse_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_soul_of_the_forest_eclipse_AuraScript);

            void HandleEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode)
            {
                if (!GetTarget())
                    return;

                if (Player* _player = GetTarget()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_HASTE))
                    {
                        if (aurEff->GetSpellInfo()->Id == SPELL_DRUID_SOLAR_ECLIPSE)
                            _player->SetEclipsePower(int32(_player->GetEclipsePower() - 20));
                        else if (aurEff->GetSpellInfo()->Id == SPELL_DRUID_LUNAR_ECLIPSE)
                            _player->SetEclipsePower(int32(_player->GetEclipsePower() + 20));
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_soul_of_the_forest_eclipse_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_soul_of_the_forest_eclipse_AuraScript();
        }
};
*/

// Called by Swiftmend - 18562 and Mangle (Bear) - 33878
// Soul of the Forest - 114107
class spell_dru_soul_of_the_forest : public SpellScriptLoader
{
    public:
        spell_dru_soul_of_the_forest() : SpellScriptLoader("spell_dru_soul_of_the_forest") { }

        class spell_dru_soul_of_the_forest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_soul_of_the_forest_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST))
                    {
                        if (GetSpellInfo()->Id == 18562)
                            _player->CastSpell(_player, SPELL_DRUID_SOUL_OF_THE_FOREST_HASTE, true);
                        else
                            _player->EnergizeBySpell(_player, SPELL_DRUID_SOUL_OF_THE_FOREST, 40, POWER_RAGE);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_soul_of_the_forest_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_soul_of_the_forest_SpellScript();
        }
};

class spell_dru_frenzied_regeneration : public SpellScriptLoader
{
public :
    spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration")
    {

    }

    class spell_dru_frenzied_regeneration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_frenzied_regeneration_SpellScript)

        bool Validate(const SpellInfo *spellInfo)
        {
            return true ;
        }

        bool Load()
        {
            return true ;
        }

        void handleHealOnEffectDummy(SpellEffIndex effectIndex)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : handleHealOnEffectDummy");
            if(Unit* caster = GetCaster())
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : caster as unit is not null");
                if(Player* p = GetCaster()->ToPlayer())
                {
                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : caster as player is not null");
                    if(p->HasAura(54810))
                        return ;

                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : caster has not the glyph, calculating amounts");
                    float maxHp = std::max((p->GetUInt32Value(UNIT_FIELD_ATTACK_POWER) - 2 * p->GetStat(STAT_AGILITY)) * 220 / 100,
                                        p->GetStat(STAT_STAMINA) * 250 / 100) ;

                    int32 maxRage = 0 ;
                    if(p->GetPower(POWER_RAGE) >= 60)
                        maxRage = 60 ;
                    else
                    {
                        maxRage = p->GetPower(POWER_RAGE);
                        maxHp = CalculatePct(maxHp, maxRage);
                    }

                    p->SetHealth(p->GetHealth() + maxHp);
                    p->ModifyPower(POWER_RAGE, -maxRage);
                }
            }
        }

        void handleModHealingDoneOnEffectDummy(SpellEffIndex effectIndex)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : handleModHealingDoneOnEffectDummy");
            if(GetCaster())
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : caster as unit not null");
                if(Player* caster = GetCaster()->ToPlayer())
                {
                    sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : caster as player not null ; casting the heal");

                    caster->CastSpell(caster, SPELL_DRUID_FRENZIED_REGENERATION_HEAL);

                    if(Aura* heal = caster->GetAura(SPELL_DRUID_FRENZIED_REGENERATION_HEAL))
                    {
                        sLog->outDebug(LOG_FILTER_NETWORKIO, "Frenzied Regeneration : caster has the aura ; checking glyph");
                        if(caster->HasAura(54810))
                        {
                            sLog->outDebug(LOG_FILTER_NETWORKIO,
                                           "Frenzied Regeneration : glyph found ; setting amount of the aura (actually : %f ; supposed to become %f)",
                                           heal->GetEffect(0)->GetAmount(), 1.4 * heal->GetEffect(0)->GetAmount());
                            heal->GetEffect(0)->SetAmount(1.4 * heal->GetEffect(0)->GetAmount());
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_frenzied_regeneration_SpellScript::handleHealOnEffectDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_dru_frenzied_regeneration_SpellScript::handleModHealingDoneOnEffectDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_frenzied_regeneration_SpellScript();
    }
};


void AddSC_druid_spell_scripts()
{
    new spell_dru_bear_cat();
    new spell_dru_mangle();
    new spell_dru_prowl();
    new spell_dru_growl();
    new spell_dru_might_of_ursoc();
    new spell_dru_stampeding_roar();
    new spell_dru_dash();
    new spell_dru_eclipse_energize();
    new spell_dru_enrage();
    new spell_dru_glyph_of_starfire();
    new spell_dru_idol_lifebloom();
    new spell_dru_innervate();
    new spell_dru_insect_swarm();
    new spell_dru_lifebloom();
    new spell_dru_living_seed();
    new spell_dru_living_seed_proc();
    new spell_dru_moonkin_form_passive();
    new spell_dru_owlkin_frenzy();
    new spell_dru_predatory_strikes();
    new spell_dru_primal_tenacity();
    new spell_dru_rip();
    new spell_dru_savage_defense();
    new spell_dru_savage_roar();
    new spell_dru_starfall_aoe();
    new spell_dru_starfall_dummy();
    new spell_dru_survival_instincts();
    new spell_dru_swift_flight_passive();
    new spell_dru_tiger_s_fury();
    new spell_dru_typhoon();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_druid_wild_mushroom();
    //new spell_druid_wild_mushroom_detonate();
	new spell_dru_displacer_beast();
	new spell_dru_force_of_nature();
	new npc_treant();
	/*new spell_dru_ursols_vortex_snare();
	new spell_dru_ursols_vortex();
	new spell_dru_soul_of_the_forest_eclipse();
	*/
	new spell_dru_soul_of_the_forest();
    new spell_dru_frenzied_regeneration();
}
