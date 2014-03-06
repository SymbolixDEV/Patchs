#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "heart_of_fear.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_grand_empress_shekzeer : public CreatureScript
{
    public:
        boss_grand_empress_shekzeer() : CreatureScript("boss_grand_empress_shekzeer") { }

        struct boss_grand_empress_shekzeerAI : public BossAI
        {
            boss_grand_empress_shekzeerAI(Creature* creature) : BossAI(creature, DATA_GRAND_EMPRESS_SHEKZEER)
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
            return new boss_grand_empress_shekzeerAI(creature);
        }
};

void AddSC_boss_grand_empress_shekzeer()
{
    new boss_grand_empress_shekzeer();
}
