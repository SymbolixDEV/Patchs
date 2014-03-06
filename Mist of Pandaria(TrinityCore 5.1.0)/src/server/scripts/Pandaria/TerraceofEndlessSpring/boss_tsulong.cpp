#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_the_endless_spring.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_tsulong : public CreatureScript
{
    public:
        boss_tsulong() : CreatureScript("boss_tsulong") { }

        struct boss_tsulongAI : public BossAI
        {
            boss_tsulongAI(Creature* creature) : BossAI(creature, DATA_TSULONG)
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
            return new boss_tsulongAI(creature);
        }
};

void AddSC_boss_tsulong()
{
    new boss_tsulong();
}
