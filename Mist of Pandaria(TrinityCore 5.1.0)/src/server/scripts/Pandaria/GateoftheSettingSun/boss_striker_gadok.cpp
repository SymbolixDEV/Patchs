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

class boss_striker_gadok : public CreatureScript
{
    public:
        boss_striker_gadok() : CreatureScript("boss_striker_gadok") { }

        struct boss_striker_gadokAI : public BossAI
        {
            boss_striker_gadokAI(Creature* creature) : BossAI(creature, DATA_STRIKER_GADOK)
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
            return new boss_striker_gadokAI(creature);
        }
};

void AddSC_boss_striker_gadok()
{
    new boss_striker_gadok();
}
