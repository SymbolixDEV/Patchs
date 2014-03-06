#include "ScriptPCH.h"
#include "SpellScript.h"
enum NPCS
{
    BOSS_YAN_ZHU,
    NPC_BUBULLE,
    NPC_SUD,
    NPC_YEASTY,
    NPC_FIZZY_BREW_ALAMENTAL
};

Position SudPositions[2][2] =
{
    {
        {0,0,0,0}, // Start
        {0,0,0,0} // End
    }, // Left Side
    {
        {0,0,0,0}, // Start
        {0,0,0,0} // End
    } // RightSide
};

class boss_yan_zhu : public CreatureScript
{
public :
    boss_yan_zhu() : CreatureScript("boss_yan_zhu")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new boss_yan_zhu_AI(pCreature);
    }

    struct boss_yan_zhu_AI : public ScriptedAI
    {
    public :
        boss_yan_zhu_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }

        void Reset()
        {
            RandomWheatAbility = RAND(SPELL_BLOAT, SPELL_BLACKOUT_BREW);
            RandomStoutAbility = RAND(SPELL_CARBONATION, SPELL_WALL_OF_SUDS);
            RandomAleAbility = RAND(SPELL_BUBBLE_SHIELD, SPELL_SUMMON_YEASY);
        }

        void CheckDist()
        {
            Map::PlayerList const& pl = me->GetMap()->GetPlayers();
            uint8 count = 0 ;
            uint8 playercount = me->GetMap()->GetPlayersCountExceptGMs();
            if(!pl.isEmpty())
            {
                for(Map::PlayerList::const_iterator c_iter = pl.begin() ; c_iter != pl.end() ; ++c_iter)
                {
                    Position pos ;
                    if(Player* p = c_iter->getSource())
                    {
                        p->GetPosition(&pos);
                        if(p->isGameMaster())
                            continue ;
                        if(me->GetExactDist(&pos) > MELEE_RANGE)
                            count++;
                    }
                }
            }

            if(count == playercount)
                DoCast(me, SPELL_BLOAT, true);
        }

        void SummonSud()
        {
            bool justOnce = false ;
            uint32 j = 4 ;

            for(uint8 i = 1 ; i < 41 ; i+=5)
            {
                if(!justOnce)
                {
                    justOnce = true ;
                    SpawnInLine(&(SudPositions[0][0]), &(SudPositions[0][1]), NPC_SUD, j);
                    SpawnInLine(&(SudPositions[1][0]), &(SudPositions[1][1]), NPC_SUD, j);
                }
                else
                {
                    justOnce = false ;
                    j += 2 ;
                    SpawnInLine(&(SudPositions[0][0]), &(SudPositions[0][1]), NPC_SUD, j);
                    SpawnInLine(&(SudPositions[1][0]), &(SudPositions[1][1]), NPC_SUD, j);
                }
            }
        }

        void UpdateAI(uint32 diff)
        {
            if(!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return ;

            events.Update(diff);

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                case EVENT_WHEAT :
                    if(RandomWheatAbility == SPELL_BLOAT)
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            DoCast(target, RandomWheatAbility, true);
                    else
                        DoCastToAllHostilePlayers(RandomWheatAbility, true);

                    events.ScheduleEvent(EVENT_WHEAT, 10000);
                    break ;

                case EVENT_ALE :
                    if(RandomAleAbility == SPELL_BUBBLE_SHIELD)
                        SpawnInCircle(1, 8, NPC_BUBULLE, TEMPSUMMON_CORPSE_DESPAWN, 60000);
                    else
                        for(uint8 i = 0 ; i < 4 ; ++i)
                            me->SummonCreature(NPC_YEASTY, me->GetPositionX() + urand(0,4), me->GetPositionY() + irand(-4, 0), me->GetPositionZ() + 2.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 60000);

                    events.ScheduleEvent(EVENT_ALE, 20000);
                    break ;

                case EVENT_STOUT :
                    if(RandomStoutAbility == SPELL_CARBONATION)
                        // Summon les NPCS (@todo)
                        break;
                    else
                        SummonSud();

                    events.ScheduleEvent(EVENT_STOUT, DUNGEON_MODE(45000, 37500));
                    break ;

                case EVENT_CHECK :
                    CheckDist();
                    events.ScheduleEvent(EVENT_CHECK, 1000);
                    break ;
                }
            }
            DoMeleeAttackIfReady();
        }

        void SpawnInCircle(float rayon, uint8 number, uint32 entry, TempSummonType summon = TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, uint32 time = 30000)
        {
            float teta = (2*M_PI) / number;

            for(uint8 i = 0 ; i < number ; i++)
            {
                float x = cos(i*teta) * rayon + me->GetPositionX();
                float y = sin(teta*i) * rayon + me->GetPositionY();

                me->SummonCreature(entry, x, y, me->GetPositionZ()+2.0f, 0, summon, time);

            }
        }

        void SpawnInLine(Position* posStart, Position* posEnd, uint32 entry, uint32 number)
        {
            float coeff = (posStart->GetPositionY() - posEnd->GetPositionY()) / (posStart->GetPositionX() - posEnd->GetPositionX()) ; // Coefficient directeur de la droite
            float ord = posStart->GetPositionY() - posStart->GetPositionX() * coeff ; // Ordonée
            // Expression : posStart->GetPositionY() = coeff * posStart->GetPositionX() + ord; y = coeff * x + ord

            float distBetween = (std::max(posStart->GetPositionX(), posEnd->GetPositionX()) - std::min(posStart->GetPositionX(), posEnd->GetPositionX()))/number ;

            for(float x = posStart->GetPositionX() ; x < posEnd->GetPositionX() ; x += distBetween)
                me->SummonCreature(entry, x, coeff * x + ord, me->GetPositionZ() + 2, 0);
        }

    private :
        enum Spells
        {
            SPELL_BREW_BOLT = 114548,
            SPELL_BLOAT = 106546,
            SPELL_BLACKOUT_BREW = 106851,
            SPELL_BUBBLE_SHIELD = 106563,
            SPELL_SUMMON_YEASY = /*NPC*/ 66413,
            SPELL_CARBONATION = 115003,
            SPELL_WALL_OF_SUDS = 114466
        };

        enum Events
        {
            EVENT_WHEAT = 1,
            EVENT_ALE,
            EVENT_STOUT,
            EVENT_CHECK
        };

        uint32 RandomWheatAbility ;
        uint32 RandomAleAbility ;
        uint32 RandomStoutAbility ;
        InstanceScript* p_instance ;
        EventMap events;
    };
};

class mob_yeasty_brew_elemental : public CreatureScript
{
public :
    mob_yeasty_brew_elemental() : CreatureScript("mob_yeast_brew_elemental")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new mob_yeasty_brew_elemental_AI(pCreature);
    }

    struct mob_yeasty_brew_elemental_AI : public ScriptedAI
    {
    public :
        mob_yeasty_brew_elemental_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }

        void JustSummoned(Creature *summon)
        {
            me->SetInCombatWithZone();
            events.ScheduleEvent(EVENT_FERMENT, DUNGEON_MODE(urand(10000, 12000), urand(7000, 8000)));
            events.ScheduleEvent(EVENT_BREW_BOLT, DUNGEON_MODE(7000, 5500));
        }

        void DoChoseFermentVictim()
        {
            Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
            if(!playerList.isEmpty())
            {
                for(Map::PlayerList::const_iterator c_iter = playerList.begin() ; c_iter != playerList.end() ; ++c_iter)
                {
                    boss = me->FindNearestCreature(BOSS_YAN_ZHU, 50000.0f, true);
                    Player* p = c_iter->getSource();
                    if(boss && p)
                        if(p->IsInBetween(me, boss))
                        {
                            DoCast(p, SPELL_FERMENT, true);
                            events.ScheduleEvent(EVENT_FERMENT, DUNGEON_MODE(10000, 7500));
                            return ;
                        }
                        else continue ;
                }
            }

            if(boss = me->FindNearestCreature(BOSS_YAN_ZHU, 50000.0f, true))
                DoCast(boss, SPELL_FERMENT, true);
            events.ScheduleEvent(EVENT_FERMENT, DUNGEON_MODE(10000, 7500));
        }

        void UpdateAI(uint32 diff)
        {
            if(!UpdateVictim())
                return ;

            events.Update(diff);

            if(me->HasUnitState(UNIT_STATE_CASTING))
                return ;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                case EVENT_BREW_BOLT :
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                        DoCast(target, SPELL_BREW_BOLT, true);
                    events.ScheduleEvent(EVENT_BREW_BOLT, DUNGEON_MODE(7500, 5000));
                    break ;

                case EVENT_FERMENT :
                    DoChoseFermentVictim();
                    break ;
                }
            }
            DoMeleeAttackIfReady();
        }

    private :
        enum Events
        {
            EVENT_FERMENT = 1,
            EVENT_BREW_BOLT
        };

        enum Spells
        {
            SPELL_FERMENT = 114451,
            SPELL_BREW_BOLT = 114548
        };

        EventMap events ;
        InstanceScript* p_instance;
        Creature* boss ;
    };
};

class mob_bubble : public CreatureScript
{
public :
    mob_bubble() : CreatureScript("mob_bubble")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new mob_bubble_AI(pCreature);
    }

    struct mob_bubble_AI : public ScriptedAI
    {
    public :
        mob_bubble_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }
 
        void JustSummoned(Creature *summon)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFacingTo(me->GetOwner()->GetOrientation());
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            if(Creature* pBoss = me->FindNearestCreature(p_instance->GetData(BOSS_YAN_ZHU), 50000.0f, true))
                DoCast(pBoss, SPELL_BUBBLE_SHIELD, true);
            else
                TryBubbleTimer = 1000 ;
        }

        void JustDied(Unit *killer)
        {
            if(Creature* pBoss = me->FindNearestCreature(p_instance->GetData(BOSS_YAN_ZHU), 50000.0f, true))
                if(Aura* bubble = pBoss->GetAura(SPELL_BUBBLE_SHIELD))
                    bubble->ModStackAmount(-1);
        }

        void UpdateAI(uint32 diff)
        {
            if(TryBubbleTimer <= diff)
                if(Creature* pBoss = me->FindNearestCreature(p_instance->GetData(BOSS_YAN_ZHU), 50000.0f, true))
                    DoCast(pBoss, SPELL_BUBBLE_SHIELD, true);
                else TryBubbleTimer = 1000 ;
            else
                TryBubbleTimer -= diff ;
        }

    private :
        enum Spells
        {
            SPELL_BUBBLE_SHIELD = 106563
        };

        InstanceScript* p_instance ;
        uint32 TryBubbleTimer;
    };
};

/*class mob_carbonation : public CreatureScript
{
public :
    mob_carbonation() : CreatureScript("mob_carbonation")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new mob_carbonation_AI(pCreature);
    }

    struct mob_carbonation_AI : public ScriptedAI
    {
    public :
        mob_carbonation_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }

    private :
        InstanceScript* p_instance;
        enum Spells
        {
            SPELL_CARBONATION = 0
        };
    };
};*/

class spell_blackout_brew : public SpellScriptLoader
{
public :
    spell_blackout_brew() : SpellScriptLoader("spell_yan_zhu_blackout_brew")
    {

    }

    AuraScript* GetAuraScript() const
    {
        return new spell_blackout_brew_AuraScript();
    }

    SpellScript* GetSpellScript() const
    {
        return new spell_blackout_brew_SpellScript();
    }

    class spell_blackout_brew_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_blackout_brew_SpellScript)

        bool Validate(const SpellInfo* spellInfo)
        {
            if(!sSpellMgr->GetSpellInfo(SPELL_BLACKOUT_AURA) || !sSpellMgr->GetSpellInfo(SPELL_BLACKOUT_SLEEP))
                return false ;

            return true ;
        }

        void HandleOnHit()
        {
            for(uint8 i = 0 ; i < 3 ; ++i) // Three stacks to be added
            {
                if(Aura* aura = GetHitUnit()->GetAura(SPELL_BLACKOUT_AURA))
                {
                    uint8 amount = aura->GetStackAmount();
                    if(amount >= 10)
                    {
                        GetHitUnit()->CastSpell(GetHitUnit(), SPELL_BLACKOUT_SLEEP, true); // It's time to sleep !
                        break ; // And to stop adding the aura of course :D
                    }
                }
                GetHitUnit()->AddAura(SPELL_BLACKOUT_AURA, GetHitUnit()); // We add the aura
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_blackout_brew_SpellScript::HandleOnHit) ;
        }

    private :
        enum Spells
        {
            SPELL_BLACKOUT_AURA = 114930,
            SPELL_BLACKOUT_SLEEP = 106857
        };
    };

    class spell_blackout_brew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_blackout_brew_AuraScript)

        void HandleRemoveByMove(AuraEffect const* aurEff) // Why do we call this a periodic dummy tick ? O.O
        {
            if(GetTarget()->isMoving())
            {
                if(Aura* aura = GetTarget()->GetAura(114930))
                    aura->ModStackAmount(-1, AURA_REMOVE_BY_DEFAULT);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_blackout_brew_AuraScript::HandleRemoveByMove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };
};

void AddSC_boss_yan_zhu()
{
	new boss_yan_zhu();
	new mob_yeasty_brew_elemental();
	new mob_bubble();
	/*new mob_carbonation();*/
	new spell_blackout_brew();
}