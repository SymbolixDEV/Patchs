/* # Script de Tydrheal & Sungis : Lorewalker Stonestep # */

/*
	Notes : What is missing ?	- Zao's & Sun's scripts
								- Some texts
*/

#include "ScriptPCH.h"
#include "temple_of_the_jade_serpent.h"

enum Spells
{
	/* Lorewalker Stonestep */
	SPELL_LOREWALKER_S_ALACRITY	= 122714,
	SPELL_SPINNING_CRANE_KICK	= 129003,
	SPELL_MEDITATE				= 121801,

	/* Corrupted Scroll */
	SPELL_FLOOR_SCROLL			= 107350,
	SPELL_CAMERA_SHAKE			= 106346,

	/* Strife & Peril */
	SPELL_AGONY					= 114571,
	SPELL_INTENSITY				= 113315,
	SPELL_DISSIPATION			= 113379,
	SPELL_ULTIMATE_POWER		= 113309,
	SPELL_SHA_CORRUPTION		= 115086,
};

enum Events
{
	/* Lorewalker Stonestep */
	EVENT_SPINNING_CRANE_KICK	= 1,
	EVENT_SAY_INTRO_2_3_4_5		= 2,
	EVENT_SAY_END_1				= 3,
	EVENT_SAY_END_2				= 4,

	/* Scroll */
	EVENT_SUMMON_BOSSES			= 1,

	/* Strife & Peril */
	EVENT_ATTACK_START			= 1,
	EVENT_AGONY					= 2,
	EVENT_DISSIPATION			= 3,
	
	/* Osong */
	EVENT_AGGRO					= 1,
	EVENT_ATTACK_PERIL			= 2,
	EVENT_ATTACK_STRIFE			= 3
};

enum Texts
{
	/* Lorewalker Stonestep */
	SAY_END_1			= 0,
	SAY_END_2			= 1,
	SAY_INTRO_1			= 2,
	SAY_INTRO_2			= 3,
	SAY_INTRO_3			= 4,
	SAY_INTRO_4			= 5,
	SAY_INTRO_5			= 6,
	SAY_RANDOM_1		= 7,
	SAY_RANDOM_2		= 8,
	SAY_RANDOM_3		= 9,
	SAY_RANDOM_4		= 10,

	/* Lorewalker => Osong */
	SAY_OSONG_INTRO_1	= 11,
	SAY_OSONG_INTRO_2	= 12,
	SAY_OSONG_INTRO_3	= 13,
	SAY_OSONG_INTRO_4	= 14,
	SAY_OSONG_INTRO_5	= 15,

	/* Lorewalker => Zao */
	SAY_ZAO_INTRO_1		= 16,
	SAY_ZAO_INTRO_2		= 17,
	SAY_ZAO_INTRO_3		= 18,
	SAY_ZAO_INTRO_4		= 19,
	SAY_ZAO_INTRO_5		= 20,
	SAY_ZAO_INTRO_6		= 21,
	SAY_ZAO_INTRO_7		= 22,

	/* Osong */
	SAY_OSONG_AGGRO		= 0
};

enum Phases
{
	PHASE_NULL			= 0,
	PHASE_ATTACK_SCROLL	= 1,
	PHASE_BOSSES		= 2,
};

enum Npcs
{
	NPC_LOREWALKER_STONESTEP	= 56843,
	NPC_CORRUPTED_SCROLL		= 57080,
	NPC_STRIFE					= 59051,
	NPC_PERIL					= 59726,
	NPC_OSONG					= 56872,
	NPC_LOREWALKER_TRIGGER		= 400449,
  //NPC_SUN						= 56915, Missing template
};

class boss_lorewalker_stonestep : public CreatureScript
{
public:
	boss_lorewalker_stonestep() : CreatureScript("boss_lorewalker_stonestep") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_lorewalker_stonestepAI(creature);
	}

	struct boss_lorewalker_stonestepAI : public ScriptedAI
	{
		boss_lorewalker_stonestepAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		bool emote;
		bool intro;
		bool end;

		void Reset()
		{
			events.Reset();

			if (instance)
				instance->SetBossState(DATA_BOSS_LOREWALKER_STONESTEP, NOT_STARTED);

			emote = false;
			intro = false;
			end = false;

			events.SetPhase(PHASE_ATTACK_SCROLL);
			events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 0, 0, PHASE_ATTACK_SCROLL);
		}

		void EnterEvadeMode()
		{
			if (instance)
			{
				events.Reset();

				instance->SetBossState(DATA_BOSS_LOREWALKER_STONESTEP, FAIL);

				emote = false;
				intro = false;
				end = false;
			}
		}

		void MoveInLineOfSight(Unit* who)
		{
			if (!me->IsWithinDistInMap(who, 40.0f) || intro)
				return;

			if (!who || !who->IsInWorld())
				return;

			if (who && who->GetTypeId() == TYPEID_PLAYER && !intro && !me->IsValidAttackTarget(who))
			{
				Talk(SAY_INTRO_1);
				events.ScheduleEvent(EVENT_SAY_INTRO_2_3_4_5, 20*IN_MILLISECONDS, 0, PHASE_ATTACK_SCROLL);
				intro = true;
			}
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_LOREWALKER_STONESTEP, IN_PROGRESS);
		}

		void UpdateAI(uint32 diff)
		{
			events.Update(diff);

			if (!emote)
			{
				me->HandleEmoteCommand(EMOTE_STATE_READY_UNARMED);
				emote = true;
			}

			if (Creature* trigger = me->FindNearestCreature(NPC_LOREWALKER_TRIGGER, 0.1f, true))
				if (!events.IsInPhase(PHASE_BOSSES))
				{
					me->Relocate(824.674438f, -2453.281738f, 176.302979f, 5.957958f);
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
					me->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
					me->SetFacingTo(5.957958f);
					events.CancelEvent(EVENT_SPINNING_CRANE_KICK);
					events.CancelEvent(EVENT_SAY_INTRO_2_3_4_5);
					events.SetPhase(PHASE_BOSSES);
				}

			if (me->FindNearestCreature(NPC_STRIFE, 99999.0f, false) && me->FindNearestCreature(NPC_PERIL, 99999.0f, false) && !end)
			{
				events.ScheduleEvent(EVENT_SAY_END_1, 0, 0, PHASE_BOSSES);
				end = true;
			}

			if (me->FindNearestCreature(NPC_LOREWALKER_TRIGGER, 99999.0f, false) && events.IsInPhase(PHASE_ATTACK_SCROLL))
				events.SetPhase(PHASE_NULL);

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_SPINNING_CRANE_KICK:
							me->CastSpell(me, SPELL_SPINNING_CRANE_KICK);

							events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 16*IN_MILLISECONDS, 0, PHASE_ATTACK_SCROLL);
							break;

						case EVENT_SAY_INTRO_2_3_4_5:
							Talk(irand(SAY_INTRO_2, SAY_INTRO_5));

							events.ScheduleEvent(EVENT_SAY_INTRO_2_3_4_5, 20*IN_MILLISECONDS, 0, PHASE_ATTACK_SCROLL);
							break;

						case EVENT_SAY_END_1:
							if (Creature* trigger = me->FindNearestCreature(NPC_LOREWALKER_TRIGGER, 99999.0f, true))
								trigger->Kill(trigger);

							if (Creature* wiseMari = me->FindNearestCreature(BOSS_WISE_MARI, 99999.0f, false))
							{
								if (GameObject* lorewalkerDoor = me->FindNearestGameObject(GO_MARI_LOREWALKER_GATE, 99999.0f))
									lorewalkerDoor->UseDoorOrButton();

								if (GameObject* wiseMariDoor = wiseMari->FindNearestGameObject(GO_MARI_LOREWALKER_GATE, 99999.0f))
									wiseMariDoor->UseDoorOrButton();

								if (GameObject* go = me->FindNearestGameObject(GO_LIU_GATE, 99999.0f))
									go->UseDoorOrButton();
							}

							Talk(SAY_END_1);

							me->SetWalk(true);
							me->GetMotionMaster()->MovePoint(0, me->GetHomePosition());
							
							events.ScheduleEvent(EVENT_SAY_END_2, 9*IN_MILLISECONDS, 0, PHASE_BOSSES);
							events.CancelEvent(EVENT_SAY_END_1);
							break;

						case EVENT_SAY_END_2:
							Talk(SAY_END_2);
							me->SetFacingTo(1.261891f);
							me->CastSpell(me, SPELL_MEDITATE);

							events.CancelEvent(EVENT_SAY_END_2); // End of the script
							break;

						default:
							break;
					}
				}
			}
		}
	};
};

class npc_corrupted_scroll : public CreatureScript 
{
public:
	npc_corrupted_scroll() : CreatureScript("npc_corrupted_scroll") { }

	CreatureAI* GetAI(Creature* creature) const 
	{
		return new npc_corrupted_scrollAI(creature);
	}

	struct npc_corrupted_scrollAI : public ScriptedAI
	{
		npc_corrupted_scrollAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		bool oneHp;
		bool corruptedScroll;

		void Reset() 
		{
			oneHp = false;
			corruptedScroll = false;
			me->setActive(false);
		}

		void DamageTaken(Unit* who, uint32& damage)
		{
			if (damage >= me->GetHealth())
			{
				damage = 0;
				me->SetHealth(1);

				if (!oneHp)
				{
					instance->DoCastSpellOnPlayers(SPELL_CAMERA_SHAKE);
					events.ScheduleEvent(EVENT_SUMMON_BOSSES, 4*IN_MILLISECONDS);
					oneHp = true;
				}
			}
		}

		void UpdateAI(uint32 diff) 
		{
			events.Update(diff);

			if (!corruptedScroll)
			{
				me->CastSpell(me, SPELL_FLOOR_SCROLL);
				corruptedScroll = true;
			}

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_SUMMON_BOSSES:
							me->SummonCreature(NPC_OSONG, 848.020325f, -2449.538818f, 174.961197f, 4.385465f, TEMPSUMMON_MANUAL_DESPAWN);
							me->SummonCreature(NPC_PERIL, 835.478394f, -2466.505859f, 174.961578f, 0.935758f, TEMPSUMMON_MANUAL_DESPAWN);
							me->SummonCreature(NPC_STRIFE, 848.223511f, -2470.850586f, 174.961578f, 1.537897f, TEMPSUMMON_MANUAL_DESPAWN);
							
							if (Creature* lorewalker = me->FindNearestCreature(NPC_LOREWALKER_STONESTEP, 99999.0f))
							{
								lorewalker->GetMotionMaster()->MovePoint(0, 824.674438f, -2453.281738f, 176.302979f);
								lorewalker->RemoveAurasDueToSpell(SPELL_SPINNING_CRANE_KICK, lorewalker->GetGUID());
								lorewalker->Say(SAY_OSONG_INTRO_1, 0, 0);
							}

							me->DespawnOrUnsummon();
							events.CancelEvent(EVENT_SUMMON_BOSSES);
							break;

						default:
							break;
					}
				}
			}
		}
	};
};

class npc_strife : public CreatureScript 
{
public:
	npc_strife() : CreatureScript("npc_strife") { }

	CreatureAI* GetAI(Creature* creature) const 
	{
		return new npc_strifeAI(creature);
	}

	struct npc_strifeAI : public ScriptedAI
	{
		npc_strifeAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		int32 damageDealt;
		int32 intensityStacks;
		bool eventAgony;

		void Reset()
		{
			events.Reset();

			damageDealt = 0;
			intensityStacks = 0;
			eventAgony = false;

			me->setActive(false);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

			if (!me->HasAura(SPELL_SHA_CORRUPTION, me->GetGUID()))
				me->CastSpell(me, SPELL_SHA_CORRUPTION);

			events.ScheduleEvent(EVENT_ATTACK_START, 5*IN_MILLISECONDS);
		}

		void JustSummoned(Creature* summoned)
        {
			events.Reset();

			damageDealt = 0;
			intensityStacks = 0;
			eventAgony = false;

			me->setActive(false);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

			if (!me->HasAura(SPELL_SHA_CORRUPTION, me->GetGUID()))
				me->CastSpell(me, SPELL_SHA_CORRUPTION);

			events.ScheduleEvent(EVENT_ATTACK_START, 5*IN_MILLISECONDS);
        }

		void JustDied(Unit *pWho)
		{
			if (instance)
				if (Creature* peril = me->FindNearestCreature(NPC_PERIL, 99999.0f, false))
					if (!peril->isAlive())
					{
						instance->SetBossState(DATA_BOSS_LOREWALKER_STONESTEP, DONE);
						instance->DoCastSpellOnPlayers(SPELL_LOREWALKER_S_ALACRITY);
					}
		}

		void DamageTaken(Unit* who, uint32& damage)
		{
			if (int32 dmg = damageDealt + damage)
				damageDealt = dmg;
		}

		void EnterEvadeMode()
		{
			if (instance)
			{
				me->CombatStop();
				me->DeleteThreatList();

				if (Creature* scroll = me->FindNearestCreature(NPC_CORRUPTED_SCROLL, 99999.0f, false))
					scroll->Respawn(true);

				if (Creature* lorewalker = me->FindNearestCreature(NPC_LOREWALKER_STONESTEP, 99999.0f, true))
				{
					me->Kill(lorewalker);
					lorewalker->Respawn(true);
					lorewalker->GetMotionMaster()->MovePoint(0, lorewalker->GetHomePosition());
					lorewalker->Relocate(lorewalker->GetHomePosition());
					lorewalker->SetFacingTo(1.252402f);
				}

				if (Creature* osong = me->FindNearestCreature(NPC_OSONG, 99999.0f, true))
					osong->DespawnOrUnsummon();

				me->DespawnOrUnsummon(1*IN_MILLISECONDS);
			}
		}

		void UpdateAI(uint32 diff)
		{
			events.Update(diff);

			if (!UpdateVictim())
			{
				while(uint32 eventId = events.ExecuteEvent())
				{
					switch(eventId)
					{
						if (instance)
						{
							case EVENT_ATTACK_START:
								me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
								me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
								me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
								events.ScheduleEvent(EVENT_DISSIPATION, 4*IN_MILLISECONDS);
								me->setActive(true);
								me->setFaction(14);
								me->SetInCombatWithZone();

								events.CancelEvent(EVENT_ATTACK_START);
								break;

							default:
								break;
						}
					}
				}
			}

			if (!me->HasAura(SPELL_INTENSITY) && intensityStacks != 0)
				intensityStacks = 0;

			if (damageDealt >= me->GetMaxHealth() * 0.02f)
			{
				me->RemoveAurasDueToSpell(SPELL_DISSIPATION);
				me->CastSpell(me, SPELL_INTENSITY);
				events.RescheduleEvent(EVENT_DISSIPATION, 4*IN_MILLISECONDS);
				intensityStacks++;
				damageDealt = 0;
			}

			if (intensityStacks == 10)
			{
				me->CastSpell(me, SPELL_ULTIMATE_POWER);
				me->RemoveAurasDueToSpell(SPELL_INTENSITY);
				intensityStacks = 0;
			}

			if (UpdateVictim())
			{
				if (!eventAgony)
				{
					events.ScheduleEvent(EVENT_AGONY, 0);
					eventAgony = true;
				}

				while(uint32 eventId = events.ExecuteEvent())
				{
					switch(eventId)
					{
						if (instance)
						{
							case EVENT_AGONY:
								me->CastSpell(me->getVictim(), SPELL_AGONY);

								events.RescheduleEvent(EVENT_AGONY, 2500);
								break;

							case EVENT_DISSIPATION:
								if (!me->HasAura(SPELL_ULTIMATE_POWER))
									me->CastSpell(me, SPELL_DISSIPATION);

								if (me->HasAura(SPELL_INTENSITY))
									me->RemoveAurasDueToSpell(SPELL_INTENSITY, me->GetGUID());

								intensityStacks = 0;

								events.RescheduleEvent(EVENT_DISSIPATION, 2*IN_MILLISECONDS);
								break;

							default:
								break;
						}
					}
				}
			}

			DoMeleeAttackIfReady();
		}
	};
};

class npc_peril : public CreatureScript 
{
public:
	npc_peril() : CreatureScript("npc_peril") { }

	CreatureAI* GetAI(Creature* creature) const 
	{
		return new npc_perilAI(creature);
	}

	struct npc_perilAI : public ScriptedAI
	{
		npc_perilAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		int32 damageDealt;
		int32 intensityStacks;
		bool eventAgony;

		void Reset()
		{
			events.Reset();

			damageDealt = 0;
			intensityStacks = 0;
			eventAgony = false;

			me->setActive(false);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

			if (!me->HasAura(SPELL_SHA_CORRUPTION, me->GetGUID()))
				me->CastSpell(me, SPELL_SHA_CORRUPTION);

			events.ScheduleEvent(EVENT_ATTACK_START, 5*IN_MILLISECONDS);
		}

		void JustSummoned(Creature* summoned)
        {
			events.Reset();

			damageDealt = 0;
			intensityStacks = 0;
			eventAgony = false;

			me->setActive(false);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

			if (!me->HasAura(SPELL_SHA_CORRUPTION, me->GetGUID()))
				me->CastSpell(me, SPELL_SHA_CORRUPTION);

			events.ScheduleEvent(EVENT_ATTACK_START, 5*IN_MILLISECONDS);
        }

		void JustDied(Unit *pWho)
		{
			if (instance)
				if (Creature* strife = me->FindNearestCreature(NPC_STRIFE, 99999.0f, false))
					if (!strife->isAlive())
					{
						instance->SetBossState(DATA_BOSS_LOREWALKER_STONESTEP, DONE);
						instance->DoCastSpellOnPlayers(SPELL_LOREWALKER_S_ALACRITY);
					}
		}

		void DamageTaken(Unit* who, uint32& damage)
		{
			if (int32 dmg = damageDealt + damage)
				damageDealt = dmg;
		}

		void EnterEvadeMode()
		{
			if (instance)
			{
				me->CombatStop();
				me->DeleteThreatList();

				if (Creature* scroll = me->FindNearestCreature(NPC_CORRUPTED_SCROLL, 99999.0f, false))
					scroll->Respawn(true);

				if (Creature* lorewalker = me->FindNearestCreature(NPC_LOREWALKER_STONESTEP, 99999.0f, true))
				{
					me->Kill(lorewalker);
					lorewalker->Respawn(true);
					lorewalker->GetMotionMaster()->MovePoint(0, lorewalker->GetHomePosition());
					lorewalker->Relocate(lorewalker->GetHomePosition());
					lorewalker->SetFacingTo(1.252402f);
				}

				if (Creature* osong = me->FindNearestCreature(NPC_OSONG, 99999.0f, true))
					osong->DespawnOrUnsummon();

				me->DespawnOrUnsummon(1*IN_MILLISECONDS);
			}
		}

		void UpdateAI(uint32 diff)
		{
			events.Update(diff);

			if (!UpdateVictim())
			{
				while(uint32 eventId = events.ExecuteEvent())
				{
					switch(eventId)
					{
						if (instance)
						{
							case EVENT_ATTACK_START:
								me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
								me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
								me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
								events.ScheduleEvent(EVENT_DISSIPATION, 4*IN_MILLISECONDS);
								me->setActive(true);
								me->setFaction(14);
								me->SetInCombatWithZone();

								events.CancelEvent(EVENT_ATTACK_START);
								break;

							default:
								break;
						}
					}
				}
			}

			if (!me->HasAura(SPELL_INTENSITY) && intensityStacks != 0)
				intensityStacks = 0;

			if (damageDealt >= me->GetMaxHealth() * 0.02f)
			{
				me->RemoveAurasDueToSpell(SPELL_DISSIPATION);
				me->CastSpell(me, SPELL_INTENSITY);
				events.RescheduleEvent(EVENT_DISSIPATION, 4*IN_MILLISECONDS);
				intensityStacks++;
				damageDealt = 0;
			}

			if (intensityStacks == 10)
			{
				me->CastSpell(me, SPELL_ULTIMATE_POWER);
				me->RemoveAurasDueToSpell(SPELL_INTENSITY);
				intensityStacks = 0;
			}

			if (UpdateVictim())
			{
				if (!eventAgony)
				{
					events.ScheduleEvent(EVENT_AGONY, 1*IN_MILLISECONDS);
					eventAgony = true;
				}

				while(uint32 eventId = events.ExecuteEvent())
				{
					switch(eventId)
					{
						if (instance)
						{
							case EVENT_AGONY:
								me->CastSpell(me->getVictim(), SPELL_AGONY);

								events.RescheduleEvent(EVENT_AGONY, 2500);
								break;

							case EVENT_DISSIPATION:
								if (!me->HasAura(SPELL_ULTIMATE_POWER))
									me->CastSpell(me, SPELL_DISSIPATION);

								if (me->HasAura(SPELL_INTENSITY))
									me->RemoveAurasDueToSpell(SPELL_INTENSITY, me->GetGUID());

								intensityStacks = 0;

								events.RescheduleEvent(EVENT_DISSIPATION, 2*IN_MILLISECONDS);
								break;

							default:
								break;
						}
					}
				}
			}

			DoMeleeAttackIfReady();
		}
	};
};

class npc_osong : public CreatureScript 
{
public:
	npc_osong() : CreatureScript("npc_osong") { }

	CreatureAI* GetAI(Creature* creature) const 
	{
		return new npc_osongAI(creature);
	}

	struct npc_osongAI : public ScriptedAI
	{
		npc_osongAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;

		void Reset()
		{
			events.Reset();

			events.ScheduleEvent(EVENT_AGGRO, 5500);
		}

		void EnterEvadeMode()
		{
			me->DespawnOrUnsummon();
		}

		void UpdateAI(uint32 diff)
		{
			events.Update(diff);

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_AGGRO:
							Talk(SAY_OSONG_AGGRO);

							events.ScheduleEvent(EVENT_ATTACK_STRIFE, 2*IN_MILLISECONDS);
							break;

						case EVENT_ATTACK_STRIFE:
							if (Creature* strife = me->FindNearestCreature(NPC_STRIFE, 99999.0f))
							{
								me->AI()->AttackStart(strife);
								me->AddThreat(strife, 99999.0f);
							}

							events.ScheduleEvent(EVENT_ATTACK_PERIL, 8*IN_MILLISECONDS);
							events.CancelEvent(EVENT_ATTACK_STRIFE);
							break;

						case EVENT_ATTACK_PERIL:
							if (Creature* peril = me->FindNearestCreature(NPC_PERIL, 99999.0f))
							{
								me->AI()->AttackStart(peril);
								me->AddThreat(peril, 99999.0f);
							}

							events.ScheduleEvent(EVENT_ATTACK_STRIFE, 8*IN_MILLISECONDS);
							events.CancelEvent(EVENT_ATTACK_PERIL);
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

void AddSC_boss_lorewalker_stonestep()
{
	new npc_corrupted_scroll();
	new boss_lorewalker_stonestep();
	new npc_strife();
	new npc_peril();
	new npc_osong();
}