#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogushan_vaults.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_garajal_the_spiritbinder : public CreatureScript
{
    public:
        boss_garajal_the_spiritbinder() : CreatureScript("boss_garajal_the_spiritbinder") { }

        struct boss_garajal_the_spiritbinderAI : public BossAI
        {
            boss_garajal_the_spiritbinderAI(Creature* creature) : BossAI(creature, DATA_GARAJAL_THE_SPIRITBINDER)
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
            return new boss_garajal_the_spiritbinderAI(creature);
        }
};

void AddSC_boss_garajal_the_spiritbinder()
{
    new boss_garajal_the_spiritbinder();
}
