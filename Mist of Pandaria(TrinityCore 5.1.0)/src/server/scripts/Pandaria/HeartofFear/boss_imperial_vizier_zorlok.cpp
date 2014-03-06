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

class boss_imperial_vizier_zorlok : public CreatureScript
{
    public:
        boss_imperial_vizier_zorlok() : CreatureScript("boss_imperial_vizier_zorlok") { }

        struct boss_imperial_vizier_zorlokAI : public BossAI
        {
            boss_imperial_vizier_zorlokAI(Creature* creature) : BossAI(creature, DATA_IMPERIAL_VIZIER_ZORLOK)
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
            return new boss_imperial_vizier_zorlokAI(creature);
        }
};

void AddSC_boss_imperial_vizier_zorlok()
{
    new boss_imperial_vizier_zorlok();
}
