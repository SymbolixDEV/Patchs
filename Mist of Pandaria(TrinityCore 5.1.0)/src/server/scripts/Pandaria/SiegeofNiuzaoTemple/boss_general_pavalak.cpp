#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_niuzao_temple.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_general_pavalak : public CreatureScript
{
    public:
        boss_general_pavalak() : CreatureScript("boss_general_pavalak") { }

        struct boss_general_pavalakAI : public BossAI
        {
            boss_general_pavalakAI(Creature* creature) : BossAI(creature, DATA_GENERAL_PAVALAK)
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
            return new boss_general_pavalakAI(creature);
        }
};

void AddSC_boss_general_pavalak()
{
    new boss_general_pavalak();
}
