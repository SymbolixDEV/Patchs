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

class boss_the_spirit_kings : public CreatureScript
{
    public:
        boss_the_spirit_kings() : CreatureScript("boss_the_spirit_kings") { }

        struct boss_the_spirit_kingsAI : public BossAI
        {
            boss_the_spirit_kingsAI(Creature* creature) : BossAI(creature, DATA_THE_SPIRIT_KINGS)
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
            return new boss_the_spirit_kingsAI(creature);
        }
};

void AddSC_boss_the_spirit_kings()
{
    new boss_the_spirit_kings();
}
