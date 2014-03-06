#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scholomance.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_instructor_chillheart : public CreatureScript
{
    public:
        boss_instructor_chillheart() : CreatureScript("boss_instructor_chillheart") { }

        struct boss_instructor_chillheartAI : public BossAI
        {
            boss_instructor_chillheartAI(Creature* creature) : BossAI(creature, DATA_INSTRUCTOR_CHILLHEART)
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
            return new boss_instructor_chillheartAI(creature);
        }
};

void AddSC_boss_instructor_chillheart()
{
    new boss_instructor_chillheart();
}
