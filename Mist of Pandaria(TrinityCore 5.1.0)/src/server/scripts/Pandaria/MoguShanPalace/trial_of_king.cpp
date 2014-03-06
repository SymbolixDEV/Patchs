/* # Script de Sungis : Épreuve du roi # */

#include "ScriptPCH.h"
#include "SpellScript.h"
#include "mogushan_palace.h"

enum Spells
{
	/* Xin the Weaponmaster */
	SPELL_JUMP					= 116572, // Tester le sort.

	/* Mu'Shiba */
	SPELL_RAVAGE				= 119946,

	/* Kuai the Brute */
	SPELL_SHOCKWAVE				= 119922,

	/* Ming the Cunning */
	SPELL_LIGHTNING_BOLT		= 123654,
	SPELL_MAGNETIC_FIELD		= 120100,
	SPELL_WHIRLING_DERVISH		= 119981,	

	/* Haiyan the Unstoppable */
	SPELL_CONFLAGRATE			= 120160,
	SPELL_CONFLAGRATE_TRIGGER	= 120201,
	SPELL_CONFLAGRATE_INSTANT	= 120167,
	SPELL_METEOR				= 120195,
	SPELL_TRAUMATIC_BLOW		= 123655,
};

enum Actions
{
	ACTION_KUAI_THE_BRUTE_RESET,
	ACTION_KUAI_THE_BRUTE_DIED,
	ACTION_MING_THE_CUNNING_RESET,
	ACTION_MING_THE_CUNNING_DIED,
	ACTION_HAIYAN_THE_UNSTOPPABLE_RESET,
	ACTION_HAIYAN_THE_UNSTOPPABLE_DIED
};

enum NpcXinEvents
{
	EVENT_INTRO				= 1,
	EVENT_JUMP				= 2,
	EVENT_KUAI				= 3
};

enum KuaiEvents
{
	EVENT_SHOCKWAVE			= 1,
};

enum MingEvents
{
	EVENT_LIGHTNING_BOLT	= 1,
	EVENT_MAGNETIC_FIELD	= 2,
	EVENT_WHIRLING_DERVISH	= 3
};

enum WhirlingDervishEvents
{
	EVENT_WHIRLING_DERVISH_MOVE_RANDOM = 1,
};

enum HaiyanEvents
{
	EVENT_CONFLAGRATE		= 1,
	EVENT_METEOR			= 2,
	EVENT_TRAUMATIC_BLOW	= 3
};

enum XinTexts
{
	SAY_INTRO				= 0
};

enum KuaiTexts
{
	SAY_AGGRO_KUAI			= 0,
	SAY_SLAY_KUAI			= 1,
	SAY_DEATH_KUAI			= 2
};

enum MingTexts
{
	SAY_AGGRO_MING			= 0,
	SAY_SLAY_MING			= 1,
	SAY_DEATH_MING			= 2
};

enum HaiyanTexts
{
	SAY_AGGRO_HAIYAN		= 0,
	SAY_SLAY_HAIYAN			= 1,
	SAY_DEATH_HAIYAN		= 2
};

enum XinPhases
{
	PHASE_NULL = 0,
	PHASE_ONE = 1,
	PHASE_TWO = 2,
	PHASE_THREE = 3,
	PHASE_LAST = 4
};

enum KuaiPhases
{
	PHASE_NULL = 0,
	PHASE_KUAI_WAITING = 1,
	PHASE_KUAI_ATTACK = 2
};

class npc_xin_the_weaponmaster : public CreatureScript
{
public:
	npc_xin_the_weaponmaster() : CreatureScript("npc_xin_the_weaponmaster") { }

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_xin_the_weaponmasterAI (pCreature);
	}

	struct npc_xin_the_weaponmasterAI : public ScriptedAI
	{
		npc_xin_the_weaponmasterAI(Creature *c) : ScriptedAI(c)
		{
			instance = c->GetInstanceScript();
		}

		InstanceScript* instance;
		EventManager events;

		void Reset()
		{
			events.Reset();

			if (instance)
				instance->SetData(DATA_TRIAL_OF_KING, NOT_STARTED);

			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			events.SetPhase(PHASE_NULL);
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (instance)
				instance->SetData(DATA_TRIAL_OF_KING, IN_PROGRESS);

			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SetInCombatWithZone();

			events.IsInPhase(PHASE_ONE);
			events.ScheduleEvent(EVENT_INTRO, 3*IN_MILLISECONDS, 0);
			events.ScheduleEvent(EVENT_JUMP, 21*IN_MILLISECONDS, 0);
			events.ScheduleEvent(EVENT_KUAI, 26*IN_MILLISECONDS, 0);
		}

		void EnterEvadeMode()
		{
			if (instance)
				instance->SetData(DATA_TRIAL_OF_KING, FAIL);
		}

		void UpdateAI(const uint32 diff)
		{
			if(!UpdateVictim())
				return;

			events.Update(diff);

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						if (events.IsInPhase(PHASE_ONE))
						{
							case EVENT_INTRO:
								Talk(SAY_INTRO);

								events.SetPhase(PHASE_TWO);
								break;
						}

						if (events.IsInPhase(PHASE_TWO))
						{
							float x = 0.0f, y = 0.0f, z = 0.0f, o = 0.0f; // Ajouter les coordonnées
							case EVENT_JUMP:
								me->CastSpell(x, y, z, SPELL_JUMP, true);
								me->SetOrientation(o);
								events.SetPhase(PHASE_THREE);
						}

						if (events.IsInPhase(PHASE_THREE))
						{
							case EVENT_KUAI:
								if (Creature* kuai = me->GetCreature(*me, instance->GetData64(DATA_KUAI_THE_BRUTE)))
								{
									kuai->SetReactState(REACT_AGGRESSIVE);
									kuai->SetInCombatWithZone();
								}

								events.SetPhase(PHASE_LAST);
						}
								
						default:
							break;
					}
				}
			}
		}
	};
};

class boss_kuai_the_brute : public CreatureScript
{
public:
	boss_kuai_the_brute() : CreatureScript("boss_kuai_the_brute") { }

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new boss_kuai_the_bruteAI (pCreature);
	}

	struct boss_kuai_the_bruteAI : public ScriptedAI
	{
		boss_kuai_the_bruteAI(Creature *c) : ScriptedAI(c)
		{
			instance = c->GetInstanceScript();
		}

		InstanceScript* instance;
		EventManager events;

		bool checkKuaiTheBruteAlive;
		bool checkMingTheCunningAlive;
        bool checkHaiyanTheUnstoppableAlive;

		void Reset()
		{
			me->SetReactState(REACT_PASSIVE);
			checkKuaiTheBruteAlive = true;
			checkMingTheCunningAlive = true;
			checkHaiyanTheUnstoppableAlive = true;

			events.Reset();

			events.SetPhase(PHASE_KUAI_WAITING);
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_KUAI_THE_BRUTE_RESET:
                    checkKuaiTheBruteAlive = true;
                    break;
                case ACTION_KUAI_THE_BRUTE_DIED:
                    checkKuaiTheBruteAlive = false;
                    break;
                case ACTION_MING_THE_CUNNING_RESET:
                    checkMingTheCunningAlive = true;
                    break;
                case ACTION_MING_THE_CUNNING_DIED:
                    checkMingTheCunningAlive = false;
                    break;
                case ACTION_HAIYAN_THE_UNSTOPPABLE_RESET:
                    checkHaiyanTheUnstoppableAlive = true;
                    break;
                case ACTION_HAIYAN_THE_UNSTOPPABLE_DIED:
                    checkHaiyanTheUnstoppableAlive = false;
                    break;
			}
		}

		void EnterCombat(Unit* /*who*/)
		{
			Talk(SAY_AGGRO_KUAI);
			
			if (Creature* mushiba = me->GetCreature(*me, instance->GetData64(DATA_MUSHIBA)))
			{
				mushiba->SetReactState(REACT_AGGRESSIVE);
				mushiba->SetInCombatWithZone();
			}

			events.SetPhase(PHASE_KUAI_ATTACK);
			events.ScheduleEvent(EVENT_SHOCKWAVE, 17*IN_MILLISECONDS, 0);
		}

		void KilledUnit(Unit *pWho)
		{
			Talk(SAY_SLAY_KUAI);
		}
		
		void EnterEvadeMode()
		{
			if (instance)
				instance->SetData(DATA_TRIAL_OF_KING, FAIL);
		}

		void DamageTaken(Unit* who, uint32& damage)
		{
			if (damage >= me->GetHealth())
			{
				damage = 0;
				JustDied(who);
			}
		}

		void JustDied(Unit* /*killer*/)
		{
			if (Creature* ming = me->GetCreature(*me, instance->GetData64(DATA_MING_THE_CUNNING)))
			{
				ming->SetReactState(REACT_AGGRESSIVE);
				ming->SetInCombatWithZone();
			}

			if (Creature* mushiba = me->GetCreature(*me, instance->GetData64(DATA_MUSHIBA)))
			{
				mushiba->SetReactState(REACT_PASSIVE);
				mushiba->AttackStop();
				mushiba->DeleteThreatList();
				mushiba->CombatStop(true);
			}

			Talk(SAY_DEATH_KUAI);
			me->SetReactState(REACT_PASSIVE);
			me->AttackStop();
			me->DeleteThreatList();
			me->CombatStop(true);
		}

		void UpdateAI(const uint32 diff)
		{
			if(!UpdateVictim())
				return;

			events.Update(diff);
			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						if (events.IsInPhase(PHASE_KUAI_ATTACK))
						{
							case EVENT_SHOCKWAVE:
								DoCastVictim(SPELL_SHOCKWAVE);

							events.ScheduleEvent(EVENT_SHOCKWAVE, 17*IN_MILLISECONDS, 0);
							break;
						}
					}
				}
			}
			DoMeleeAttackIfReady();
		}
	};
};

class boss_kuai_the_brute : public CreatureScript
{
public:
	boss_kuai_the_brute() : CreatureScript("boss_kuai_the_brute") { }

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new boss_kuai_the_bruteAI (pCreature);
	}

	struct boss_kuai_the_bruteAI : public ScriptedAI
	{
		boss_kuai_the_bruteAI(Creature *c) : ScriptedAI(c)
		{
			instance = c->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;

		bool checkKuaiTheBruteAlive;
		bool checkMingTheCunningAlive;
        bool checkHaiyanTheUnstoppableAlive;

		void Reset()
		{
			me->SetReactState(REACT_PASSIVE);
			checkKuaiTheBruteAlive = true;
			checkMingTheCunningAlive = true;
			checkHaiyanTheUnstoppableAlive = true;

			events.Reset();

			events.SetPhase(PHASE_KUAI_WAITING);
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_KUAI_THE_BRUTE_RESET:
                    checkKuaiTheBruteAlive = true;
                    break;
                case ACTION_KUAI_THE_BRUTE_DIED:
                    checkKuaiTheBruteAlive = false;
                    break;
                case ACTION_MING_THE_CUNNING_RESET:
                    checkMingTheCunningAlive = true;
                    break;
                case ACTION_MING_THE_CUNNING_DIED:
                    checkMingTheCunningAlive = false;
                    break;
                case ACTION_HAIYAN_THE_UNSTOPPABLE_RESET:
                    checkHaiyanTheUnstoppableAlive = true;
                    break;
                case ACTION_HAIYAN_THE_UNSTOPPABLE_DIED:
                    checkHaiyanTheUnstoppableAlive = false;
                    break;
			}
		}

		void EnterCombat(Unit* /*who*/)
		{
			Talk(SAY_AGGRO_KUAI);
			
			if (Creature* mushiba = me->GetCreature(*me, instance->GetData64(DATA_MUSHIBA)))
			{
				mushiba->SetReactState(REACT_AGGRESSIVE);
				mushiba->SetInCombatWithZone();
			}

			events.SetPhase(PHASE_KUAI_ATTACK);
			events.ScheduleEvent(EVENT_SHOCKWAVE, 17*IN_MILLISECONDS, 0);
		}

		void KilledUnit(Unit *pWho)
		{
			Talk(SAY_SLAY_KUAI);
		}
		
		void EnterEvadeMode()
		{
			if (instance)
				instance->SetData(DATA_TRIAL_OF_KING, FAIL);
		}

		void DamageTaken(Unit* who, uint32& damage)
		{
			if (damage >= me->GetHealth())
			{
				damage = 0;
				JustDied(who);
			}
		}

		void JustDied(Unit* /*killer*/)
		{
			if (Creature* ming = me->GetCreature(*me, instance->GetData64(DATA_MING_THE_CUNNING)))
			{
				ming->SetReactState(REACT_AGGRESSIVE);
				ming->SetInCombatWithZone();
			}

			if (Creature* mushiba = me->GetCreature(*me, instance->GetData64(DATA_MUSHIBA)))
			{
				mushiba->SetReactState(REACT_PASSIVE);
				mushiba->AttackStop();
				mushiba->DeleteThreatList();
				mushiba->CombatStop(true);
			}

			Talk(SAY_DEATH_KUAI);
			me->SetReactState(REACT_PASSIVE);
			me->AttackStop();
			me->DeleteThreatList();
			me->CombatStop(true);
		}

		void UpdateAI(const uint32 diff)
		{
			if(!UpdateVictim())
				return;

			events.Update(diff);
			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						if (events.IsInPhase(PHASE_KUAI_ATTACK))
						{
							case EVENT_WHIRLING_DERVISH_MOVE_RANDOM:

							events.ScheduleEvent(EVENT_WHIRLING_DERVISH_MOVE_RANDOM, 2*IN_MILLISECONDS, 0);
							break;
						}
					}
				}
			}
		}
	};
};

void AddSC_trial_of_king()
{
	new npc_xin_the_weaponmaster;
	new boss_kuai_the_brute;
}