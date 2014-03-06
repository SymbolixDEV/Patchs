#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stormstout_brewery.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_ookook : public CreatureScript
{
    public:
        boss_ookook() : CreatureScript("boss_ookook") { }

        struct boss_ookookAI : public BossAI
        {
            boss_ookookAI(Creature* creature) : BossAI(creature, DATA_OOKOOK)
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
            return new boss_ookookAI(creature);
        }
};

void AddSC_boss_ookook()
{
    new boss_ookook();
}
