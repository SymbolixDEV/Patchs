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

class boss_garalon : public CreatureScript
{
    public:
        boss_garalon() : CreatureScript("boss_garalon") { }

        struct boss_garalonAI : public BossAI
        {
            boss_garalonAI(Creature* creature) : BossAI(creature, DATA_GARALON)
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
            return new boss_garalonAI(creature);
        }
};

void AddSC_boss_garalon()
{
    new boss_garalon();
}
