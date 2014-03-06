#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogushan_palace.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_gekkan : public CreatureScript
{
    public:
        boss_gekkan() : CreatureScript("boss_gekkan") { }

        struct boss_gekkanAI : public BossAI
        {
            boss_gekkanAI(Creature* creature) : BossAI(creature, DATA_GEKKAN)
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
            return new boss_gekkanAI(creature);
        }
};

void AddSC_boss_gekkan()
{
    new boss_gekkan();
}
