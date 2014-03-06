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

class boss_flameweaver_koegler : public CreatureScript
{
    public:
        boss_flameweaver_koegler() : CreatureScript("boss_flameweaver_koegler") { }

        struct boss_flameweaver_koeglerAI : public BossAI
        {
            boss_flameweaver_koeglerAI(Creature* creature) : BossAI(creature, DATA_FLAMEWEAVER_KOEGLER)
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
            return new boss_flameweaver_koeglerAI(creature);
        }
};

void AddSC_boss_flameweaver_koegler()
{
    new boss_flameweaver_koegler();
}
