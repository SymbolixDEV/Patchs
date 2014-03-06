#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogushan_palace.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_trial_of_the_king : public CreatureScript
{
    public:
        boss_trial_of_the_king() : CreatureScript("boss_trial_of_the_king") { }

        struct boss_trial_of_the_kingAI : public BossAI
        {
            boss_trial_of_the_kingAI(Creature* creature) : BossAI(creature, DATA_TRIAL_OF_THE_KING)
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
            return new boss_trial_of_the_kingAI(creature);
        }
};

void AddSC_boss_trial_of_the_king()
{
    new boss_trial_of_the_king();
}
