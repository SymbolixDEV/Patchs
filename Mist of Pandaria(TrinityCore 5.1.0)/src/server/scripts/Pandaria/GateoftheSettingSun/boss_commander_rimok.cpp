#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "gate_of_the_setting_sun.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_commander_rimok : public CreatureScript
{
    public:
        boss_commander_rimok() : CreatureScript("boss_commander_rimok") { }

        struct boss_commander_rimokAI : public BossAI
        {
            boss_commander_rimokAI(Creature* creature) : BossAI(creature, DATA_COMMANDER_RIMOK)
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
            return new boss_commander_rimokAI(creature);
        }
};

void AddSC_boss_commander_rimok()
{
    new boss_commander_rimok();
}
