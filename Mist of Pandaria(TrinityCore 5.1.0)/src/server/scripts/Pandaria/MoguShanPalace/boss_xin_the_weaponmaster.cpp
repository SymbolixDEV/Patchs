/* # Script de Sungis : Xin le Maître d'armes # */

/* Notes :
Xin le Maître d'armes : Script 0%					A faire : vérifier si les sorts fonctionnent.

UPDATE creature_template SET ScriptName = 'boss_xin_the_weaponmaster' WHERE entry = 61398;

INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(61398, 0, 0, "", 14, 0, 100, 0, 0, 28356, "Xin the Weaponmaster - Reset"),
(61398, 1, 0, "Ah ! Qu'est-ce que c'est ... Encore des prétendants à ma couronne ! Ils sont tout petits et tout faibles !", 14, 0, 100, 0, 0, 28355, "Xin the Weaponmaster - Intro"),
(61398, 2, 0, "Vous n'êtes pas les premiers à me défier, pécores. Vous ne serez pas les derniers.", 14, 0, 100, 0, 0, 28349, "Xin the Weaponmaster - Aggro"),
(61398, 3, 0, "", 14, 0, 100, 0, 0, 28357, "Xin the Weaponmaster - Slay 1"),
(61398, 3, 0, "", 14, 0, 100, 0, 0, 28358, "Xin the Weaponmaster - Slay 2"),
(61398, 4, 0, "", 14, 0, 100, 0, 0, 28352, "Xin the Weaponmaster - Death"),
(61398, 5, 0, "", 14, 0, 100, 0, 0, 28350, "Xin the Weaponmaster - Axes"), Affrontez la puissance de mon arsenal, gringalets !
(61398, 6, 0, "", 14, 0, 100, 0, 0, 28351, "Xin the Weaponmaster - Blades"),
(61398, 7, 0, "", 14, 0, 100, 0, 0, 28359, "Xin the Weaponmaster - Staves"),
(61398, 8, 0, "", 14, 0, 100, 0, 0, 28360, "Xin the Weaponmaster - Bows"),
(61398, 9, 0, "MA PUISSANCE EST SANS LIMITE !", 14, 0, 100, 0, 0, 28353, "Xin the Weaponmaster - 66%"),
(61398, 10, 0, "ASSEZ JOUÉ LA COMÉDIE ! Vous allez connaître la douleur, pleutres !", 14, 0, 100, 0, 0, 28354, "Xin the Weaponmaster - 33%");
*/

#include "ScriptPCH.h"
#include "mogushan_palace.h"

enum SpellIds
{
	/* Xin the Weaponmaster */
	SPELL_GROUND_SLAM			= 119684,
	SPELL_INCITING_ROAR			= 122959,

	/* Axes */


	/* Blades */


	/* Staves */
	SPELL_CIRCLE_OF_FLAME		= 0,

	/* Bow */

};

enum Events
{
	EVENT_GROUND_SLAM		= 1,
};

enum Phases
{
	PHASE_1			= 1,
	PHASE_2			= 2,
	PHASE_3			= 3,
};

enum Texts
{
	SAY_RESET		= 0,
	SAY_INTRO		= 1,
	SAY_AGGRO		= 2,
	SAY_SLAY		= 3,
	SAY_DEATH		= 4,
	SAY_AXES		= 5,
	SAY_BLADES		= 6,
	SAY_STAVES		= 7,
	SAY_BOWS		= 8,
	SAY_EVENT_1		= 9,
	SAY_EVENT_2		= 10
};

class boss_xin_the_weaponmaster : public CreatureScript
{
public:
	boss_xin_the_weaponmaster() : CreatureScript("boss_xin_the_weaponmaster") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_xin_the_weaponmasterAI(creature);
	}

	struct boss_xin_the_weaponmasterAI : public ScriptedAI
	{
		boss_xin_the_weaponmasterAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;

		bool healthBelowSixtyPct;
		bool healthBelowThirtyPct;

		void Reset()
		{
			healthBelowSixtyPct = false;
			healthBelowThirtyPct = false;
			events.Reset();
		}

		void JustDied(Unit *pWho)
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_XIN_THE_WEAPONMASTER, DONE);

			Talk(SAY_DEATH);
		}

		void KilledUnit(Unit *pWho)
		{
			Talk(SAY_SLAY);
		}
		
		void EnterEvadeMode()
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_XIN_THE_WEAPONMASTER, FAIL);
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (instance)
				instance->SetBossState(DATA_GEKKAN, IN_PROGRESS);

			me->SetInCombatWithZone();
			Talk(SAY_AGGRO);

			events.ScheduleEvent(EVENT_GROUND_SLAM, 20*IN_MILLISECONDS);
		}

		void UpdateAI(uint32 diff)
		{
			if(!UpdateVictim())
				return;

			events.Update(diff);

			if (!healthBelowSixtyPct && HealthBelowPct(66))
			{
				DoCast(SPELL_INCITING_ROAR);
				healthBelowSixtyPct = true;
			}

			if (!healthBelowThirtyPct && HealthBelowPct(33))
			{
				DoCast(SPELL_INCITING_ROAR);
				healthBelowThirtyPct = true;
			}

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_GROUND_SLAM:
							DoCast(me->getVictim(), SPELL_GROUND_SLAM);

							events.ScheduleEvent(EVENT_GROUND_SLAM, 20*IN_MILLISECONDS);
							break;

						default:
							break;
					}
				}
			}

			DoMeleeAttackIfReady();
		}
	};
};
void AddSC_boss_xin_the_weaponmaster()
{
	new boss_xin_the_weaponmaster();
}
