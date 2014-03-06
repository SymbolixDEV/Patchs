#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogushan_vaults.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_elegon : public CreatureScript
{
    public:
        boss_elegon() : CreatureScript("boss_elegon") { }

        struct boss_elegonAI : public BossAI
        {
            boss_elegonAI(Creature* creature) : BossAI(creature, DATA_ELEGON)
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
            return new boss_elegonAI(creature);
        }
};

void AddSC_boss_elegon()
{
    new boss_elegon();
}
