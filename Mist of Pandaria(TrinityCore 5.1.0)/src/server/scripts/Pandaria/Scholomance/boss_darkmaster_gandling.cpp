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

class boss_darkmaster_gandling : public CreatureScript
{
    public:
        boss_darkmaster_gandling() : CreatureScript("boss_darkmaster_gandling") { }

        struct boss_darkmaster_gandlingAI : public BossAI
        {
            boss_darkmaster_gandlingAI(Creature* creature) : BossAI(creature, DATA_DARKMASTER_GANDLING)
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
            return new boss_darkmaster_gandlingAI(creature);
        }
};

void AddSC_boss_darkmaster_gandling()
{
    new boss_darkmaster_gandling();
}
