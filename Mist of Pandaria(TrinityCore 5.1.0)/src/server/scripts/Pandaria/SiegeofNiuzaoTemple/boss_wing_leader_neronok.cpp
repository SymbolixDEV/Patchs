#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_niuzao_temple.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_wing_leader_neronok : public CreatureScript
{
    public:
        boss_wing_leader_neronok() : CreatureScript("boss_wing_leader_neronok") { }

        struct boss_wing_leader_neronokAI : public BossAI
        {
            boss_wing_leader_neronokAI(Creature* creature) : BossAI(creature, DATA_WING_LEADER_NERONOK)
            {
            }

            void Reset()
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
            }

            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                /*
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }
                */

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_wing_leader_neronokAI(creature);
        }
};

void AddSC_boss_wing_leader_neronok()
{
    new boss_wing_leader_neronok();
}
