#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scarlet_halls.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_houndmaster_braun : public CreatureScript
{
    public:
        boss_houndmaster_braun() : CreatureScript("boss_houndmaster_braun") { }

        struct boss_houndmaster_braunAI : public BossAI
        {
            boss_houndmaster_braunAI(Creature* creature) : BossAI(creature, DATA_HOUNDMASTER_BRAUN)
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
            return new boss_houndmaster_braunAI(creature);
        }
};

void AddSC_boss_houndmaster_braun()
{
    new boss_houndmaster_braun();
}
