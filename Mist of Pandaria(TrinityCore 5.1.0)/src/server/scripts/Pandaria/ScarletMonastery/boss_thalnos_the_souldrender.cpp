#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scarlet_monastery.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_thalnos_the_souldrender : public CreatureScript
{
    public:
        boss_thalnos_the_souldrender() : CreatureScript("boss_thalnos_the_souldrender") { }

        struct boss_thalnos_the_souldrenderAI : public BossAI
        {
            boss_thalnos_the_souldrenderAI(Creature* creature) : BossAI(creature, DATA_THALNOS_THE_SOULDRENDER)
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
            return new boss_thalnos_the_souldrenderAI(creature);
        }
};

void AddSC_boss_thalnos_the_souldrender()
{
    new boss_thalnos_the_souldrender();
}
