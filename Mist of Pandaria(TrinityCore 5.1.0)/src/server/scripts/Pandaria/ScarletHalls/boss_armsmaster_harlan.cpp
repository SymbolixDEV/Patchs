#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scarlet_halls.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_armsmaster_harlan : public CreatureScript
{
    public:
        boss_armsmaster_harlan() : CreatureScript("boss_armsmaster_harlan") { }

        struct boss_armsmaster_harlanAI : public BossAI
        {
            boss_armsmaster_harlanAI(Creature* creature) : BossAI(creature, DATA_ARMSMASTER_HARLAN)
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
            return new boss_armsmaster_harlanAI(creature);
        }
};

void AddSC_boss_armsmaster_harlan()
{
    new boss_armsmaster_harlan();
}
