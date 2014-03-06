#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogushan_palace.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_xin_the_weaponmaster : public CreatureScript
{
    public:
        boss_xin_the_weaponmaster() : CreatureScript("boss_xin_the_weaponmaster") { }

        struct boss_xin_the_weaponmasterAI : public BossAI
        {
            boss_xin_the_weaponmasterAI(Creature* creature) : BossAI(creature, DATA_XIN_THE_WEAPONMASTER)
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
            return new boss_xin_the_weaponmasterAI(creature);
        }
};

void AddSC_boss_xin_the_weaponmaster()
{
    new boss_xin_the_weaponmaster();
}
