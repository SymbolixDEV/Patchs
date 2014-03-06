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

class boss_feng_the_accursed : public CreatureScript
{
    public:
        boss_feng_the_accursed() : CreatureScript("boss_feng_the_accursed") { }

        struct boss_feng_the_accursedAI : public BossAI
        {
            boss_feng_the_accursedAI(Creature* creature) : BossAI(creature, DATA_FENG_THE_ACCURSED)
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
            return new boss_feng_the_accursedAI(creature);
        }
};

void AddSC_boss_feng_the_accursed()
{
    new boss_feng_the_accursed();
}
