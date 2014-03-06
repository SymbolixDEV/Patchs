#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogushan_vaults.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_will_of_the_emperor : public CreatureScript
{
    public:
        boss_will_of_the_emperor() : CreatureScript("boss_will_of_the_emperor") { }

        struct boss_will_of_the_emperorAI : public BossAI
        {
            boss_will_of_the_emperorAI(Creature* creature) : BossAI(creature, DATA_WILL_OF_THE_EMPEROR)
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
            return new boss_will_of_the_emperorAI(creature);
        }
};

void AddSC_boss_will_of_the_emperor()
{
    new boss_will_of_the_emperor();
}
