#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_the_endless_spring.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_protectors_of_the_endless : public CreatureScript
{
    public:
        boss_protectors_of_the_endless() : CreatureScript("boss_protectors_of_the_endless") { }

        struct boss_protectors_of_the_endlessAI : public BossAI
        {
            boss_protectors_of_the_endlessAI(Creature* creature) : BossAI(creature, DATA_PROTECTORS_OF_THE_ENDLESS)
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
            return new boss_protectors_of_the_endlessAI(creature);
        }
};

void AddSC_boss_protectors_of_the_endless()
{
    new boss_protectors_of_the_endless();
}
