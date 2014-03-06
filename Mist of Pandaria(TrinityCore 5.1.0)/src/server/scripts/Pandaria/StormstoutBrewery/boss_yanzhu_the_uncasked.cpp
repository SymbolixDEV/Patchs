#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stormstout_brewery.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_yanzhu_the_uncasked : public CreatureScript
{
    public:
        boss_yanzhu_the_uncasked() : CreatureScript("boss_yanzhu_the_uncasked") { }

        struct boss_yanzhu_the_uncaskedAI : public BossAI
        {
            boss_yanzhu_the_uncaskedAI(Creature* creature) : BossAI(creature, DATA_YANZHU_THE_UNCASKED)
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
            return new boss_yanzhu_the_uncaskedAI(creature);
        }
};

void AddSC_boss_yanzhu_the_uncasked()
{
    new boss_yanzhu_the_uncasked();
}
