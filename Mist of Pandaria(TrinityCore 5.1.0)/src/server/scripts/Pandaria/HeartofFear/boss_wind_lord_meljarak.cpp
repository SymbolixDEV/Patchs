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

class boss_wind_lord_meljarak : public CreatureScript
{
    public:
        boss_wind_lord_meljarak() : CreatureScript("boss_wind_lord_meljarak") { }

        struct boss_wind_lord_meljarakAI : public BossAI
        {
            boss_wind_lord_meljarakAI(Creature* creature) : BossAI(creature, DATA_WIND_LORD_MELJARAK)
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
            return new boss_wind_lord_meljarakAI(creature);
        }
};

void AddSC_boss_wind_lord_meljarak()
{
    new boss_wind_lord_meljarak();
}
