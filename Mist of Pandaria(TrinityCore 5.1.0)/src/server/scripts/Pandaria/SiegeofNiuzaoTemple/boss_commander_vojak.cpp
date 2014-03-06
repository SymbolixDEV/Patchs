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

class boss_commander_vojak : public CreatureScript
{
    public:
        boss_commander_vojak() : CreatureScript("boss_commander_vojak") { }

        struct boss_commander_vojakAI : public BossAI
        {
            boss_commander_vojakAI(Creature* creature) : BossAI(creature, DATA_COMMANDER_VOJAK)
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
            return new boss_commander_vojakAI(creature);
        }
};

void AddSC_boss_commander_vojak()
{
    new boss_commander_vojak();
}
