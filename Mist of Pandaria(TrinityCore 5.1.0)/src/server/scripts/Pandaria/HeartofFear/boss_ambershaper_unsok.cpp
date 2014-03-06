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

class boss_ambershaper_unsok : public CreatureScript
{
    public:
        boss_ambershaper_unsok() : CreatureScript("boss_ambershaper_unsok") { }

        struct boss_ambershaper_unsokAI : public BossAI
        {
            boss_ambershaper_unsokAI(Creature* creature) : BossAI(creature, DATA_AMBERSHAPER_UNSOK)
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
            return new boss_ambershaper_unsokAI(creature);
        }
};

void AddSC_boss_ambershaper_unsok()
{
    new boss_ambershaper_unsok();
}
