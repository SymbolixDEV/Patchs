#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scholomance.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_jandice_barov : public CreatureScript
{
    public:
        boss_jandice_barov() : CreatureScript("boss_jandice_barov") { }

        struct boss_jandice_barovAI : public BossAI
        {
            boss_jandice_barovAI(Creature* creature) : BossAI(creature, DATA_JANDICE_BAROV)
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
            return new boss_jandice_barovAI(creature);
        }
};

void AddSC_boss_jandice_barov()
{
    new boss_jandice_barov();
}
