#include "ScriptPCH.h"

#define SAY_START_EVENT_1   "Who crashing Ook Party !?"
#define SAY_START_EVENT_2   "Who crashing Ook party!? Ook ook ook..."
#define SAY_AGGRO           "Me gonna ook you in the ooker !"
#define SAY_KILL            "In the ooker !"
#define SAY_BANANAS_1       "Get Ooking party started !"
#define SAY_BANANAS_2       "Come on and get your Ook on !"
#define SAY_BANANAS_3       "We're gonna Ook all night !"
#define SAY_DEATH           "Ook ! Oooook !"

#define BOSS_OOK_OOK 56637
#define NPC_WALL 0 /// TODO
#define NPC_BARREL 56682
#define SPELL_GOING_BANANA_DUMMY 115978
class boss_ook_ook : public CreatureScript
{
public :
    boss_ook_ook() : CreatureScript("boss_ook_ook")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new boss_ook_ook_AI(pCreature);
    }

    struct boss_ook_ook_AI : public ScriptedAI
    {
    public :
        boss_ook_ook_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }

        void Reset()
        {
            // if(p_instance)
            //    p_instance->SetData(DATA_OOK_OOK, NOT_STARTED);
            m_bIsLifeUnder30Pct = false ;
            m_bIsLifeUnder60Pct = false ;
            m_bIsLifeUnder90Pct = false ;
        }

        void EnterCombat(Unit *pWho)
        {
            // if(p_instance)
                // p_instance->SetData(DATA_OOK_OOK, IN_PROGRESS);

            me->MonsterYell(SAY_AGGRO, 0, 0);
            events.ScheduleEvent(EVENT_GROUND_POUND, 10000);
        }

        void EnterEvadeMode()
        {
            // if(p_instance)
                // p_instance->SetData(DATA_OOK_OOK, FAIL);
        }

        void JustDied(Unit *killer)
        {
            me->MonsterYell(SAY_DEATH, 0, 0);

            // if(p_instance)
                // p_instance->SetData(DATA_OOK_OOK, DONE);
        }

        void KilledUnit(Unit *pWho)
        {
            me->MonsterYell(SAY_KILL, 0, 0);
        }

        void DoAction(int32 action)
        {
            switch(action)
            {
            case ACTION_BANANA_90 :
                me->MonsterYell(SAY_BANANAS_1, 0, 0);
                m_bIsLifeUnder90Pct = true ;
                DoCast(SPELL_GOING_BANANA);
                break ;

            case ACTION_BANANA_60 :
                me->MonsterYell(SAY_BANANAS_2, 0, 0);
                m_bIsLifeUnder60Pct = true ;
                DoCast(SPELL_GOING_BANANA);
                break ;

            case ACTION_BANANA_30 :
                me->MonsterYell(SAY_BANANAS_3, 0, 0);
                m_bIsLifeUnder30Pct = true ;
                DoCast(SPELL_GOING_BANANA);
                break ;
            }
        }

        void UpdateAI(uint32 diff)
        {
            if(!UpdateVictim())
                return ;

            if(me->GetHealthPct() <= 90 && !m_bIsLifeUnder90Pct)
                DoAction(ACTION_BANANA_90);

            if(me->GetHealthPct() <= 60 && !m_bIsLifeUnder60Pct)
                DoAction(ACTION_BANANA_60);

            if(me->GetHealthPct() <= 30 && !m_bIsLifeUnder30Pct)
                DoAction(ACTION_BANANA_30);

            events.Update(diff);

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                case EVENT_GROUND_POUND :
                    DoCast(SPELL_GROUND_POUND);
                    events.ScheduleEvent(EVENT_GROUND_POUND, DUNGEON_MODE(10000, 7000));
                    break ;
                }
            }
        }

    private :
        InstanceScript* p_instance ;
        EventMap events ;
        bool m_bIsLifeUnder90Pct;
        bool m_bIsLifeUnder60Pct;
        bool m_bIsLifeUnder30Pct;

        enum Spells
        {
            SPELL_GROUND_POUND = 106807,
            SPELL_GROUND_POUND_TRIGGERED = 106808,
            SPELL_GOING_BANANA = 106651
        };

        enum Events
        {
            EVENT_GROUND_POUND = 1
        };

        enum Actions
        {
            ACTION_BANANA_90 = 1,
            ACTION_BANANA_60,
            ACTION_BANANA_30
        };
    };
};

// const Position SummonPositions[] =
// {

// };

// const Position DestPositions[] =
// {

// };

class npc_rolling_barrel : public CreatureScript
{
public :
    npc_rolling_barrel() : CreatureScript("npc_rolling_barrel")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new npc_rolling_barrel_AI(pCreature);
    }

    struct npc_rolling_barrel_AI : public ScriptedAI
    {
    public :
        npc_rolling_barrel_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }

        void JustSummoned(Creature *summon)
        {
            m_uiCheckTimer = 500 ;
            // switch(me->GetPositionX())
            // {
            // case SummonPositions[0] :
                // me->GetMotionMaster()->MovePoint(0, &(DestPositions[0]), false);
                // break ;

            // case SummonPositions[1] :
                // me->GetMotionMaster()->MovePoint(0, &(DestPositions[1]), false);
                // break ;

            // case SummonPositions[2] :
                // me->GetMotionMaster()->MovePoint(0, &(DestPositions[2]), false);
                // break ;

            // case SummonPositions[3] :
                // me->GetMotionMaster()->MovePoint(0, &(DestPositions[3]), false);
                // break ;

            // case SummonPositions[4] :
                // me->GetMotionMaster()->MovePoint(0, &(DestPositions[4]), false);
                // break ;

            // case SummonPositions[5] :
                // me->GetMotionMaster()->MovePoint(0, &(DestPositions[5]), false);
                // break ;
            // }
        }

        void Check() // Fonction nulle :x
        {
            // On stocke tout ce qui peut exploser : joueurs, murs et boss
            Map::PlayerList const& players = me->GetMap()->GetPlayers();

            Creature* OokOok;
            // if(p_instance)
                // OokOok = p_instance->GetData(BOSS_OOK_OOK); // C'est quoi cette connerie ? -_-'

            std::list<Creature*> walls ;
            GetCreatureListWithEntryInGrid(walls, me, NPC_WALL, 50000.0f);

            if(!players.isEmpty()) // Ensuite on parcourt les joueurs (PARCE QUE !)
            {
                for(Map::PlayerList::const_iterator citer = players.begin() ; citer != players.end() ; ++citer)
                {
                    Position pos ;
                    Player* p = citer->getSource();

                    p->GetPosition(&pos);


                    if(p && me->GetExactDist2d(&pos) <= 2.0f && !p->IsMounted()) // Et on check la distance
                    {
                        DoCast(p, SPELL_EXPLOSION, true);
                    }

                    me->DisappearAndDie();
                    m_uiCheckTimer = 100 ;
                    return ;
                }
            }

            if(OokOok) // On check OokOok
            {
                Position pos ;
                OokOok->GetPosition(&pos);

                if(me->GetExactDist2d(&pos) <= 2) // Distance
                    DoCast(OokOok, SPELL_EXPLOSION, true);

                me->DisappearAndDie();
                m_uiCheckTimer = 100 ;
                return ;
            }

            if(!walls.empty()) // Les murs
            {
                for(std::list<Creature*>::const_iterator citer = walls.begin() ; citer != walls.end() ; ++citer)
                {
                    Position pos ;
                    Creature* go = (*citer);

                    go->GetPosition(&pos);

                    if(me->GetExactDist2d(&pos) <= 2.0f) // Distance
                        DoCast(SPELL_EXPLOSION);

                    me->DisappearAndDie();
                    m_uiCheckTimer = 100 ;
                    return ;
                }
            }

            m_uiCheckTimer = 100 ;
        }

        void UpdateAI(uint32 diff)
        {
            if(m_uiCheckTimer <= diff)
                Check();
            else
                m_uiCheckTimer -= diff ;
        }

    private :
        InstanceScript* p_instance;
        uint32 m_uiCheckTimer;

        enum Spells
        {
            SPELL_EXPLOSION = 106648
        };
    };
};

class npc_barrel_threwer : public CreatureScript
{
public :
    npc_barrel_threwer() : CreatureScript("npc_barrel_threwer")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new npc_barrel_threwer_AI(pCreature);
    }

    struct npc_barrel_threwer_AI : public ScriptedAI
    {
    public :
        npc_barrel_threwer_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {

        }

        void Reset()
        {
            m_uiThrewTimer = 10000 ;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        }

        void CastBarrel()
        {
            Position pos ;
            me->GetPosition(&pos);
            me->SummonCreature(NPC_BARREL, pos, TEMPSUMMON_CORPSE_DESPAWN, 0, 0);
            m_uiThrewTimer = 10000 ;
        }

        void UpdateAI(uint32 diff)
        {
            if(m_uiThrewTimer <= diff)
                CastBarrel();
            else
                m_uiThrewTimer -= diff ;
        }

    private:
        uint32 m_uiThrewTimer;
    };
};

class spell_ook_ook_going_banana : public SpellScriptLoader
{
public :
    spell_ook_ook_going_banana() : SpellScriptLoader("spell_ook_ook_going_banana")
    {

    }

    SpellScript* GetSpellScript() const
    {
        return new spell_ook_ook_going_banana_SpellScript();
    }

    class spell_ook_ook_going_banana_SpellScript : public SpellScript
    {
    public :
        PrepareSpellScript(spell_ook_ook_going_banana_SpellScript)

        bool Validate(const SpellInfo *spellInfo)
        {
            if(!sSpellMgr->GetSpellInfo(SPELL_GOING_BANANA_DUMMY))
                return false ;

            return true ;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            // for(uint8 i = 0 ; i < 6 ; i++)
            // {
                // if(GetCaster() && GetCaster()->ToCreature() && GetCaster()->ToCreature()->AI())
                // {
                    // Creature* OokOok = GetCaster()->ToCreature();
                    // if(OokOok)
                        // OokOok->SummonCreature(NPC_BARREL, SummonPositions[i].GetPositionX(), SummonPositions[i].GetPositionY(), SummonPositions[i].GetPositionZ() + 1, 0, TEMPSUMMON_CORPSE_DESPAWN) ;
                // }
            // }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(spell_ook_ook_going_banana_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
};

void AddSC_boss_ook_ook()
{
	new boss_ook_ook;
	new spell_ook_ook_going_banana();
	new npc_rolling_barrel();
	new npc_barrel_threwer();
}