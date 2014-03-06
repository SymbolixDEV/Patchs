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

class boss_vizier_jinbak : public CreatureScript
{
    public:
        boss_vizier_jinbak() : CreatureScript("boss_vizier_jinbak") { }

        struct boss_vizier_jinbakAI : public BossAI
        {
            boss_vizier_jinbakAI(Creature* creature) : BossAI(creature, DATA_VIZIER_JINBAK)
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
            return new boss_vizier_jinbakAI(creature);
        }
};

void AddSC_boss_vizier_jinbak()
{
    new boss_vizier_jinbak();
}
