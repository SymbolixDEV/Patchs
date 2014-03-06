/* # Script de Tydrheal : Gu Cloudstrike # */

/* Notes : Il faudra tester les scripts et vérifier les sorts ainsi qu'ajouter les SoundID SAY_PHASE3 ("Puissance ! Glorieuse puissance !")
Ajouter l'ID du NPC_TRIGGER

Gu Cloudstrike : Script 95%		A test
Azure Serpent : Script 95%		A test
Gu Trigger : Script 95%         A test -- ajouter le flag pour qu'il soit invisible => Modifier dans la db pour la visibilité du PNJ par les joueurs.

UPDATE creature_template SET ScriptName = 'boss_gu_cloudstrike' WHERE entry = 56747;
UPDATE creature_template SET ScriptName = 'npc_azure_serpent' WHERE entry = 56754;
UPDATE creature_template SET ScriptName = 'npc_gu_intro_trigger' WHERE entry = ?????;

INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(56747, 0, 0, "Gringalets… faisons de la place pour nos invités.", 12, 0, 100, 0, 0, 28322, "GuCloudstrike - Intro1"),
(56747, 1, 0, "Approchez… votre trépas vous attend.", 12, 0, 100, 0, 0, 28323, "GuCloudstrike - Intro2"),
(56747, 2, 0, "Laissez-moi vous montrer ma puissance.", 12, 0, 100, 0, 0, 28317, "GuCloudstrike - EnterCombat1"),
(56747, 3, 0, "Viens à moi, serpent-dragon ! Ensemble, nous allons détruire ces intrus.", 12, 0, 100, 0, 0, 28318, "GuCloudstrike - EnterCombat2"),
(56747, 4, 0, "Serpent-dragon, que ta puissance me recharge !", 12, 0, 100, 0, 0, 28324, "GuCloudstrike - Phase2"),
(56747, 5, 0, "Leur énergie est puissante !", 14, 0, 100, 0, 0, 28320, "GuCloudstrike - HalfLifeSerpent"),
(56747, 6, 0, "Non, non ! Ne me laisse pas !", 14, 0, 100, 0, 0, 28321, "GuCloudstrike - SerpentDead"),
(56747, 7, 0, "Puissance ! Glorieuse puissance !", 12, 0, 100, 0, 0, ??????, "GuCloudstrike - Phase3"),
(56747, 8, 0, "Même… ensemble… c’est l’échec...", 14, 0, 100, 0, 0, 28319,, "GuCloudstrike - Death"),
(56747, 9, 0, "Gringalet !", 14, 0, 100, 0, 0, 28325, "GuCloudstrike - Slay1"),
(56747, 10, 0, "La foudre ignore la pitié !", 14, 0, 100, 0, 0, 28326, "GuCloudstrike - Slay2"),
(56747, 11, 0, "Nul ne peut défier les Pandashan !", 14, 0, 100, 0, 0, 28327, "GuCloudstrike - Slay3");

*/

#include "ScriptPCH.h"
#include "shadopan_monastery.h"

enum Spells
{
	/* Gu Cloudstrike */
	SPELL_CHARGING_SOUL			= 110945,
	SPELL_INVOKE_LIGHTNING		= 106984,
	SPELL_LIGHTNING_PURGE		= 114927,
	SPELL_OVERCHARGED_SOUL		= 110852,
	
	/* Azure Serpent */
	SPELL_LIGHTNING_BREATH		= 102573,
	SPELL_LIGHTNING_SHIELDED	= 123496,
	SPELL_MAGNETIC_SHROUD		= 107140,
	SPELL_STATIC_FIELD			= 106923
};

enum Events
{
	EVENT_INTRO_1,
	EVENT_INTRO_2,
	EVENT_AGGRO_PART_1,
	EVENT_AGGRO_PART_2, 
	EVENT_AGGRO_PART_3,
	EVENT_PHASE_2,
	EVENT_INVOKE_LIGHTNING,
	EVENT_AZURE_SERPENT,
	EVENT_LIGHTNING_BREATH,
	EVENT_STATIC_FIELD,
	EVENT_OVERCHARGED_SOUL,
	EVENT_LIGHTNING_PURGE,
	EVENT_MAGNETIC_SHROUD,
	EVENT_MOVE,
	EVENT_PHASE_3
};

enum Phases
{
	PHASE_INTRO,
	PHASE_COMBAT,
	PHASE_1,
	PHASE_2,
	PHASE_3
};

enum Actions
{
	ACTION_BOSS_GU_CLOUDSTRIKE_RESET,
	ACTION_BOSS_GU_CLOUDSTRIKE_DIED,
	ACTION_AZURE_SERPENT_RESET,
	ACTION_AZURE_SERPENT_DIED,
	ACTION_GU_INTRO,

	/* Azure Serpent */
	ACTION_VISIBLE
};

enum Texts
{
	SAY_INTRO_1 = 0, 
	SAY_INTRO_2 = 1, 
	SAY_ENTERCOMBAT_1 = 2,
	SAY_ENTERCOMBAT_2 = 3,
	SAY_PHASE_2 = 4,
	SAY_HALF_LIFE_SERPENT = 5,
	SAY_SERPENT_DEATH = 6,
	SAY_PHASE_3 = 7,
	SAY_DEATH = 8,
	SAY_SLAY1 = 9,
	SAY_SLAY2 = 10,
	SAY_SLAY3 = 11
};

enum TriggerIds
{
	NPC_TRIGGER = 00000
};

class boss_gu_cloudstrike : public CreatureScript
{
public:
	boss_gu_cloudstrike() : CreatureScript("boss_gu_cloudstrike") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_gu_cloudstrikeAI(creature);
	}

	struct boss_gu_cloudstrikeAI : public ScriptedAI
	{
		boss_gu_cloudstrikeAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		
		bool checkGuCloudstrikeAlive;
		bool checkAzureSerpentAlive;
		bool checkSaySerpentLife;
		
		void Reset()
		{
			checkGuCloudstrikeAlive = true;
			checkAzureSerpentAlive = true;
			checkGuCloudstrikeAlive = me->isAlive();
			checkSaySerpentLife = false;
			
			if(Aura* aura = me->GetAura(SPELL_CHARGING_SOUL))
				me->RemoveAura(aura);
				
			if(Aura* aura = me->GetAura(SPELL_INVOKE_LIGHTNING))
				me->RemoveAura(aura);

			events.Reset();

			if (instance)
			{
				if (Creature* serpent = me->GetCreature(*me, instance->GetData64(DATA_NPC_AZURE_SERPENT)))
						if (serpent->AI())
						{
							serpent->RemoveCorpse();
							serpent->Respawn();
						}
				instance->SetBossState(DATA_BOSS_GU_CLOUDSTRIKE, NOT_STARTED);
				me->AI()->DoAction(ACTION_BOSS_GU_CLOUDSTRIKE_RESET);
				if(Creature *triggers = me->FindNearestCreature(NPC_TRIGGER, 50000.0f))
					if(triggers->AI())
					   triggers->AI()->Reset();
			}
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_BOSS_GU_CLOUDSTRIKE_RESET:
					checkGuCloudstrikeAlive = true;
					break;
				case ACTION_BOSS_GU_CLOUDSTRIKE_DIED:
					checkGuCloudstrikeAlive = false;
					break;
				case ACTION_AZURE_SERPENT_RESET:
					checkAzureSerpentAlive = true;
					break;
				case ACTION_AZURE_SERPENT_DIED:
					checkAzureSerpentAlive = false;
					break;
				case ACTION_GU_INTRO:
					events.SetPhase(PHASE_INTRO);
					events.ScheduleEvent(EVENT_INTRO_1, 1*IN_MILLISECONDS, 0, PHASE_INTRO);
					events.ScheduleEvent(EVENT_INTRO_2, 4*IN_MILLISECONDS, 0, PHASE_INTRO);
					break;
			}
        }

		void JustDied(Unit *pWho)
		{
			if (instance)
			{
				me->AI()->DoAction(ACTION_BOSS_GU_CLOUDSTRIKE_DIED);
				me->AI()->DoAction(ACTION_AZURE_SERPENT_DIED);

				if (!checkGuCloudstrikeAlive)
					instance->SetBossState(DATA_BOSS_GU_CLOUDSTRIKE, DONE);

			}

			Talk(SAY_DEATH);
		}

		void KilledUnit(Unit *pWho)
		{
			Talk(urand(SAY_SLAY1, SAY_SLAY3));
		}
		
		void EnterEvadeMode()
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_GU_CLOUDSTRIKE, FAIL);
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_GU_CLOUDSTRIKE, IN_PROGRESS);

			me->SetInCombatWithZone();

			events.SetPhase(PHASE_COMBAT);
			events.ScheduleEvent(EVENT_AGGRO_PART_1, 2*IN_MILLISECONDS, 0, PHASE_COMBAT); 
			events.ScheduleEvent(EVENT_AGGRO_PART_2, 6*IN_MILLISECONDS, 0, PHASE_COMBAT);
			events.ScheduleEvent(EVENT_AGGRO_PART_3, 9*IN_MILLISECONDS, 0, PHASE_COMBAT);

		}

		void UpdateAI(uint32 diff)
		{
			if(!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_INTRO_1:
							Talk(SAY_INTRO_1);
							events.CancelEvent(EVENT_INTRO_1);
							break;

						case EVENT_INTRO_2:
							Talk(SAY_INTRO_2);
							events.CancelEvent(EVENT_INTRO_2);
							break;

						case EVENT_AGGRO_PART_1:
							Talk(SAY_ENTERCOMBAT_1);
							events.CancelEvent(EVENT_AGGRO_PART_1);
							break;
						
						case EVENT_AGGRO_PART_2:
							Talk(SAY_ENTERCOMBAT_2);
							events.CancelEvent(EVENT_AGGRO_PART_2);
							break;
						
						case EVENT_AGGRO_PART_3:
							if (Creature* serpent = me->GetCreature(*me, instance->GetData64(DATA_NPC_AZURE_SERPENT)))
								if (serpent->AI())
										serpent->AI()->DoAction(ACTION_VISIBLE);

							events.SetPhase(PHASE_1);
							events.ScheduleEvent(EVENT_INVOKE_LIGHTNING, 2*IN_MILLISECONDS, 0, PHASE_1);
							events.CancelEvent(EVENT_AGGRO_PART_3);
							break;
						
						case EVENT_INVOKE_LIGHTNING:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_INVOKE_LIGHTNING);
								}
							events.ScheduleEvent(EVENT_INVOKE_LIGHTNING, 16*IN_MILLISECONDS, 0, PHASE_1);
							break;
							
						case EVENT_PHASE_2:
							DoCast(me, SPELL_CHARGING_SOUL);
							events.CancelEvent(EVENT_PHASE_2);							
							break;
						
						case EVENT_OVERCHARGED_SOUL:
							DoCast(me, SPELL_OVERCHARGED_SOUL);
							events.CancelEvent(EVENT_OVERCHARGED_SOUL);	
							break;
						
						case EVENT_LIGHTNING_PURGE:
							DoCast(me, SPELL_LIGHTNING_PURGE);	
							events.CancelEvent(EVENT_LIGHTNING_PURGE);	
							break;

						case EVENT_PHASE_3:
							Talk(SAY_PHASE_3);
							events.CancelEvent(EVENT_PHASE_3);	
							break;

						default:
							break;
					}
				}
			}
			
			if(me->HealthBelowPct(50) && !events.IsInPhase(PHASE_2) && !events.IsInPhase(PHASE_3))
			{
				events.ScheduleEvent(EVENT_PHASE_2, 1*IN_MILLISECONDS, 0, PHASE_2);
				events.SetPhase(PHASE_2);
			}
			
			if (Creature* serpent = me->GetCreature(*me, instance->GetData64(DATA_NPC_AZURE_SERPENT)))
			{
				if(serpent->HealthBelowPct(55) && !checkSaySerpentLife)
				{
					Talk(SAY_HALF_LIFE_SERPENT);
					checkSaySerpentLife = true;
				}
		
				if(!checkAzureSerpentAlive)
				{
					Talk(SAY_SERPENT_DEATH);
					if(Aura* aura = me->GetAura(SPELL_CHARGING_SOUL))
						me->RemoveAura(aura);
					events.SetPhase(PHASE_3);
					events.ScheduleEvent(EVENT_PHASE_3, 2*IN_MILLISECONDS, 0, PHASE_3);
					events.ScheduleEvent(EVENT_INVOKE_LIGHTNING, 3*IN_MILLISECONDS, 0, PHASE_3);
					events.ScheduleEvent(EVENT_OVERCHARGED_SOUL, 25*IN_MILLISECONDS, 0, PHASE_3);
					events.ScheduleEvent(EVENT_LIGHTNING_PURGE, 60*IN_MILLISECONDS, 0, PHASE_3);
				}
			}

			DoMeleeAttackIfReady();
		}
	};
};

class npc_azure_serpent : public CreatureScript
{
public:
	npc_azure_serpent() : CreatureScript("npc_azure_serpent") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_azure_serpentAI(creature);
	}

	struct npc_azure_serpentAI : public ScriptedAI
	{
		npc_azure_serpentAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;

		void Reset()
		{
			if (instance)
				if (Creature* gu = me->GetCreature(*me, instance->GetData64(DATA_BOSS_GU_CLOUDSTRIKE)))
					if (gu->AI())
					    gu->AI()->DoAction(ACTION_AZURE_SERPENT_RESET);
						
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED); 
			me->SetVisible(false);
			
			events.Reset();
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_VISIBLE:
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
					me->SetVisible(true);
					break;
			}
        }
		void JustDied(Unit *pWho)
		{
			if (instance)
				if (Creature* gu = me->GetCreature(*me, instance->GetData64(DATA_BOSS_GU_CLOUDSTRIKE)))
					if (gu->AI())
					    gu->AI()->DoAction(ACTION_AZURE_SERPENT_DIED);
		}

		void EnterCombat(Unit* /*who*/)
		{
			me->SetInCombatWithZone();
			events.SetPhase(PHASE_1);
			events.ScheduleEvent(EVENT_STATIC_FIELD, 1*IN_MILLISECONDS, 0, PHASE_1);
		}

		void UpdateAI(uint32 diff)
		{
			if(!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_STATIC_FIELD:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_STATIC_FIELD);
								}
								
							events.ScheduleEvent(EVENT_STATIC_FIELD, 8*IN_MILLISECONDS, 0, PHASE_1);
							break;
							
						case EVENT_LIGHTNING_BREATH:
							DoCast(me, SPELL_LIGHTNING_BREATH);
							
							events.ScheduleEvent(EVENT_LIGHTNING_BREATH, 8*IN_MILLISECONDS, 0, PHASE_2);
							break;
							
						case EVENT_MAGNETIC_SHROUD:
							DoCast(me, SPELL_MAGNETIC_SHROUD);
							
							events.ScheduleEvent(EVENT_MAGNETIC_SHROUD, 12*IN_MILLISECONDS, 0, PHASE_2);
							break;

						case EVENT_MOVE:
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
							events.CancelEvent(EVENT_MOVE);
							break;

						default:
							break;
					}
				}
			}
			
			if (Creature* gu = me->GetCreature(*me, instance->GetData64(DATA_BOSS_GU_CLOUDSTRIKE)))
				if(gu->HealthBelowPct(50) && !events.IsInPhase(PHASE_2))
				{
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					events.ScheduleEvent(EVENT_MOVE, 3*IN_MILLISECONDS, 0, PHASE_2);
					events.ScheduleEvent(EVENT_LIGHTNING_BREATH, 5*IN_MILLISECONDS, 0, PHASE_2);
					events.ScheduleEvent(EVENT_MAGNETIC_SHROUD, 9*IN_MILLISECONDS, 0, PHASE_2);
					events.SetPhase(PHASE_2);
				}

			DoMeleeAttackIfReady();
		}
	};
};

class npc_gu_intro_trigger : public CreatureScript 
{
public:
	npc_gu_intro_trigger() : CreatureScript("npc_gu_intro_trigger") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_gu_intro_triggerAI(creature);
	}

	struct npc_gu_intro_triggerAI : public ScriptedAI
	{
		npc_gu_intro_triggerAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		
		bool checkTrigger; 

		void Reset()
		{
			checkTrigger = true;
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED); 
			//ajouter le flag pour qu'il soit invisible => Modifier dans la db pour la visibilité du PNJ par les joueurs.
		}

		void JustDied(Unit *pWho)
		{

		}

		void EnterCombat(Unit* /*who*/)
		{

		}

		void UpdateAI(uint32 diff)
		{	
			if(checkTrigger)
			{
				Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
				if (!PlayerList.isEmpty())
				{
					for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
					{
						if (me->GetExactDist2d(i->getSource()->GetPositionX(),i->getSource()->GetPositionY()) < 12) // A tester si cela fonctionne.
						{
							if (instance)
								if (Creature* gu = me->GetCreature(*me, instance->GetData64(DATA_BOSS_GU_CLOUDSTRIKE)))
									if (gu->AI())
									{
										gu->AI()->DoAction(ACTION_GU_INTRO);
										checkTrigger = false;
									}
						}
					}
				}
			}
		}
	};
};



void AddSC_boss_gu_cloudstrike()
{
	new boss_gu_cloudstrike();
	new npc_azure_serpent();
	new npc_gu_intro_trigger();
}