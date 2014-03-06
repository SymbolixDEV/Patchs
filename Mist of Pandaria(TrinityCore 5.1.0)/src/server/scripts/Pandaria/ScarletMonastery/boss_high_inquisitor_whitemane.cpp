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

class boss_high_inquisitor_whitemane : public CreatureScript
{
    public:
        boss_high_inquisitor_whitemane() : CreatureScript("boss_high_inquisitor_whitemane") { }

        struct boss_high_inquisitor_whitemaneAI : public BossAI
        {
            boss_high_inquisitor_whitemaneAI(Creature* creature) : BossAI(creature, DATA_HIGH_INQUISITOR_WHITEMANE)
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
            return new boss_high_inquisitor_whitemaneAI(creature);
        }
};

void AddSC_boss_high_inquisitor_whitemane()
{
    new boss_high_inquisitor_whitemane();
}
