#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "gate_of_the_setting_sun.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_raigonn : public CreatureScript
{
    public:
        boss_raigonn() : CreatureScript("boss_raigonn") { }

        struct boss_raigonnAI : public BossAI
        {
            boss_raigonnAI(Creature* creature) : BossAI(creature, DATA_RAIGONN)
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
            return new boss_raigonnAI(creature);
        }
};

void AddSC_boss_raigonn()
{
    new boss_raigonn();
}
