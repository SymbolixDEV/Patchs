#include "ScriptPCH.h"
#include "SpellScript.h"

// Says
const char* SAY_FURLWIND = "Gonna spins around !" ;
const char* SAY_CARROT_BREATH = "Urp...eats too many carrots..." ;
const char* SAY_KILL_1 = "Oh yeah !";
const char* SAY_KILL_2 = "You die !";
const char* SAY_DIED = "You have... turnip...for a head";


enum NPCS
{
    BOSS_HOTPALLUS = 56717,
    NPC_HOOPER = 59464,
    NPC_HOPPLING = 60208,
    NPC_BOPPER = 59551,
    NPC_HAMMER = 59539
};

enum Actions
{
    ACTION_HOPPER_JUMP,
    ACTION_HOPPLING_JUMP,
    ACTION_BOPPER_JUMP
};

class boss_hoptallus : public CreatureScript
{
public :
    boss_hoptallus() : CreatureScript("boss_hoptallus")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new boss_hoptallus_AI(pCreature);
    }

    struct boss_hoptallus_AI : public ScriptedAI
    {
    public :
        boss_hoptallus_AI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            p_instance = pCreature->GetInstanceScript();
        }

        void EnterEvadeMode()
        {
            /*if(p_instance)
                p_instance->SetData(DATA_HOPTALLUS, FAIL);*/
        }

        void Reset()
        {
            /*if(p_instance)
                p_instance->SetData(DATA_HOPTALLUS, NOT_STARTED);*/
        }

        void EnterCombat(Unit *pWho)
        {
            /*if(p_instance)
                p_instance->SetData(DATA_HOPTALLUS, IN_PROGRESS);*/
            events.ScheduleEvent(EVENT_FURLWIND, DUNGEON_MODE(10000, 8000));
            events.ScheduleEvent(EVENT_CARROT_BREATH, DUNGEON_MODE(30000, 25000));
            events.ScheduleEvent(EVENT_SUMMON_NPCS, DUNGEON_MODE(15000, 12000));
        }

        //virtual void DoAction(int32 action)
        //{
        //
        //}

        void DoFurlwindText()
        {
            uint8 rand = urand(0, 9);
            if(rand <= 5)
                me->MonsterYell(SAY_FURLWIND, 0, 0);;
        }

        void DoCarrotBreathText()
        {
            uint8 rand = urand(0, 9);
            if(rand <= 5)
                me->MonsterYell(SAY_CARROT_BREATH, 0, 0);
        }

        void SubCallNPCS(std::list<Creature*>& creature_list, uint32 action)
        {
            if(!creature_list.empty())
            {
                for(std::list<Creature*>::const_iterator c_iter = creature_list.begin() ; c_iter != creature_list.end() ; ++c_iter)
                {
                    Creature* c = (*c_iter);
                    if(c->AI())
                        c->AI()->DoAction(action);
                }
            }
        }

        void CallNPCS()
        {
            std::list<Creature*> Hoppers, Hoppling, Bopper;
            GetCreatureListWithEntryInGrid(Hoppers, me, NPC_HOOPER, 100.0f);
            GetCreatureListWithEntryInGrid(Hoppling, me, NPC_HOPPLING,100.0f);
            GetCreatureListWithEntryInGrid(Bopper, me, NPC_BOPPER, 100.0f);

            SubCallNPCS(Hoppers, ACTION_HOPPER_JUMP);
            SubCallNPCS(Hoppling, ACTION_HOPPLING_JUMP);
            SubCallNPCS(Bopper, ACTION_BOPPER_JUMP);
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
                case EVENT_FURLWIND :
                    DoFurlwindText();
                    DoCast(SPELL_FURLWIND);
                    events.ScheduleEvent(EVENT_FURLWIND, DUNGEON_MODE(15000, 12000));
                    break ;

                case EVENT_CARROT_BREATH :
                    DoCarrotBreathText();
                    DoCast(SPELL_CARROT_BREATH);
                    events.ScheduleEvent(EVENT_CARROT_BREATH, DUNGEON_MODE(30000, 25000));
                    break ;

                case EVENT_SUMMON_NPCS :
                    CallNPCS();
                    events.ScheduleEvent(EVENT_SUMMON_NPCS, DUNGEON_MODE(10000, 8500));
                    break;
                }
            }
        }
        
        void KilledUnit(Unit *pWho)
        {
            me->MonsterYell(RAND(SAY_KILL_1, SAY_KILL_2), 0, 0);
        }

        void JustDied(Unit *pKiller)
        {
            /*if(p_instance)
                p_instance->SetData(DATA_HOPTALLUS, DONE);*/
            me->MonsterYell(SAY_DIED, 0, 0);
        }

    private :
        enum Spells
        {
            SPELL_CARROT_BREATH = 112944, // Apply Aura : Dummy 
            SPELL_CARROT_BREATH_TRIGGERED = 112945, // Apply Aura : Dummy 
            SPELL_FURLWIND = 112992
        };

        enum Events
        {
            EVENT_FURLWIND = 1,
            EVENT_CARROT_BREATH,
            EVENT_SUMMON_NPCS
        };

        EventMap events ;
        InstanceScript* p_instance;
    };
};

#define EVENT_ICE_TRAP 1
#define SPELL_ICE_TRAP 135382

class npc_vermin_AI : public ScriptedAI
{
public :
    npc_vermin_AI(Creature* pCreature) : ScriptedAI(pCreature)
    {

    }

    void UpdateAI(uint32 diff)
    {
        if(!UpdateVictim())
            return ;

        events.Update(diff);

        if(events.ExecuteEvent() == EVENT_ICE_TRAP)
        {
            DoCast(SPELL_ICE_TRAP);
        }
    }

protected :
    EventMap events;
};

class npc_hopper : public CreatureScript
{
public :
    npc_hopper() : CreatureScript("npc_hopper")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new npc_hopper_AI(pCreature);
    }

    struct npc_hopper_AI : public npc_vermin_AI
    {
    public :
        npc_hopper_AI(Creature* pCreature) : npc_vermin_AI(pCreature)
        {

        }

        void Reset()
        {
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        void DoAction(int32 action)
        {
            switch(action)
            {
            case ACTION_HOPPER_JUMP :
                Position pos ;
                Creature* boss ;
                boss = me->FindNearestCreature(BOSS_HOTPALLUS, 100.0f, true) ;
                if(boss)
                    boss->GetPosition(&pos);
                me->SetVisible(true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->GetMotionMaster()->MoveJump(pos, 1, 1, EVENT_JUMP);
                m_uiCheckTimer = 500 ;
                me->SetInCombatWithZone();
                events.ScheduleEvent(EVENT_ICE_TRAP, 10000);
                break ;
            }
        }

        void JustDied(Unit *pKiller)
        {
            me->GetMotionMaster()->Clear(true);
            me->SetFullHealth();
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        void CheckDist()
        {
            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            if(!players.isEmpty())
            {
                for(Map::PlayerList::const_iterator c_iter = players.begin() ; c_iter != players.end() ; ++c_iter)
                {
                    Position pos ;
                    Player* p = c_iter->getSource();
                    p->GetPosition(&pos);
                    if(me->GetExactDist(&pos) <= 1.0f)
                    {
                        DoCast(p, SPELL_EXPLOSE, true);
                        me->DisappearAndDie();
                        return ;
                    }
                    else continue ;
                }
            }

            m_uiCheckTimer = 500 ;
        }

        void UpdateAI(uint32 diff)
        {
            npc_vermin_AI::UpdateAI(diff);
            if(m_uiCheckTimer <= diff)
                CheckDist();
            else m_uiCheckTimer -= diff ;
        }

    private :
        enum Spells
        {
            SPELL_EXPLOSE = 114291
        };

        uint32 m_uiCheckTimer;
    };
};

class npc_bopper : public CreatureScript
{
public :
    npc_bopper() : CreatureScript("npc_bopper")
    {

    }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new npc_bopper_AI(pCreature);
    }

    struct npc_bopper_AI : public npc_vermin_AI
    {
    public :
        npc_bopper_AI(Creature* pCreature) : npc_vermin_AI(pCreature)
        {

        }

        void Reset()
        {
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        void DoAction(int32 action)
        {
            switch(action)
            {
            case ACTION_BOPPER_JUMP :
                Position pos ;
                Creature* boss ;
                boss = me->FindNearestCreature(BOSS_HOTPALLUS, 100.0f, true) ;
                if(boss)
                    boss->GetPosition(&pos);
                me->SetVisible(true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->GetMotionMaster()->MoveJump(pos, 1, 1, EVENT_JUMP);
                me->SetInCombatWithZone();
                events.ScheduleEvent(EVENT_ICE_TRAP, 10000);
                break ;
            }
        }

        void JustDied(Unit *pKiller)
        {
            Position pos ;
            me->GetPosition(&pos);
            me->SummonCreature(NPC_HAMMER, pos);
            me->GetMotionMaster()->Clear(true);
            me->SetFullHealth();
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        void UpdateAI(uint32 diff)
        {
            npc_vermin_AI::UpdateAI(diff);
        }

    };
};

void AddSC_boss_hoptallus()
{
	new boss_hoptallus();
	new npc_hopper();
	new npc_bopper();
}
