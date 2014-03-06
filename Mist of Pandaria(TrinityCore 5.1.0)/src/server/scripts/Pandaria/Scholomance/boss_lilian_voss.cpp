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

class boss_lilian_voss : public CreatureScript
{
    public:
        boss_lilian_voss() : CreatureScript("boss_lilian_voss") { }

        struct boss_lilian_vossAI : public BossAI
        {
            boss_lilian_vossAI(Creature* creature) : BossAI(creature, DATA_LILIAN_VOSS)
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
            return new boss_lilian_vossAI(creature);
        }
};

void AddSC_boss_lilian_voss()
{
    new boss_lilian_voss();
}
