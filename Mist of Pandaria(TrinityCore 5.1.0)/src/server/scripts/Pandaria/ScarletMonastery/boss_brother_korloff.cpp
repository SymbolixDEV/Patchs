#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scarlet_monastery.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_brother_korloff : public CreatureScript
{
    public:
        boss_brother_korloff() : CreatureScript("boss_brother_korloff") { }

        struct boss_brother_korloffAI : public BossAI
        {
            boss_brother_korloffAI(Creature* creature) : BossAI(creature, DATA_BROTHER_KORLOFF)
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
            return new boss_brother_korloffAI(creature);
        }
};

void AddSC_boss_brother_korloff()
{
    new boss_brother_korloff();
}
