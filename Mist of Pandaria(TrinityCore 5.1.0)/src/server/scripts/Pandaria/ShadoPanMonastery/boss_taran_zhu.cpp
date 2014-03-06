/* # Script de Tydrheal : Taran Zhu # */

/* Notes : Tester -- voir spells -- SoundID
Ajouter l'ID du NPC_TRIGGER

Taran Zhu : Script 95% (terminé -- voir spells)
Taran trigger : Script 95%	A test -- ajouter le flag pour qu'il soit invisible => Modifier dans la db pour la visibilité du PNJ par les joueurs.
Gripping hatred : Script 95% (terminé -- voir spells)


UPDATE creature_template SET ScriptName = 'boss_taran_zhu' WHERE entry = 56884;
UPDATE creature_template SET ScriptName = 'npc_taran_intro_trigger' WHERE entry = ?????;
UPDATE creature_template SET ScriptName = 'npc_gripping_hatred' WHERE entry = 59804;

INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(56884, 0, 0, "Taran Zhu et les Pandashan m’appartiennent. Avancez et mourez de ma main, comme tous ceux qui s’opposeront à moi à l’avenir !", 14, 0, 100, 0, 0, ??????, "TaranZhu - Intro"),
(56884, 1, 0, "La haine prévaudra sur tout le reste !", 14, 0, 100, 0, 0, ??????, "TaranZhu - EnterCombat"),
(56884, 2, 0, "Non… NON !", 14, 0, 100, 0, 0, ??????, "TaranZhu - Death"),
(56884, 3, 0, "Le sha de la haine a fui mon corps… et le monastère. Je vous remercie, étrangers. Les Pandashan vous sont redevables. Il y a beaucoup à faire, maintenant…", 14, 0, 100, 0, 0, ??????, "TaranZhu - Thanks"),
(56884, 4, 0, "Que votre défaite vous emplisse de rage.", 14, 0, 100, 0, 0, ??????, "TaranZhu - Evade"),
(56884, 5, 0, "Nourrissez-moi de votre haine !", 14, 0, 100, 0, 0, ??????, "TaranZhu - Slay");



*/

#include "ScriptPCH.h"
#include "shadopan_monastery.h"

enum Spells
{
	/* Taran Zhu */
	SPELL_HAZE_OF_HATE = 107087,
	SPELL_MEDIDATE = 107200,
	SPELL_RISING_HATE = 107357,
	SPELL_RING_OF_MALICE = 131521,
	SPELL_SHA_BLAST = 114999,
	SPELL_GRIP_OF_HATE = 115010,
	SPELL_POOL_OF_SHADOWS = 112918

};

enum Events
{
	EVENT_RING_OF_MALICE,
	EVENT_HAZE_OF_HATE,
	EVENT_MEDIDATE,
	EVENT_RISING_HATE,
	EVENT_SHA_BLAST,
	EVENT_GRIPPING_HATRED,
	EVENT_GRIP_OF_HATE,
	EVENT_POOL_OF_SHADOWS,
	EVENT_THANKS
};

enum Actions
{
	ACTION_BOSS_TARAN_ZHU_RESET,
	ACTION_BOSS_TARAN_ZHU_DIED,
	ACTION_TARAN_INTRO
};

enum Texts
{
	SAY_INTRO = 0,
	SAY_COMBAT = 1,
	SAY_DEATH = 2,
	SAY_THANKS = 3,
	SAY_EVADE = 4,
	SAY_SLAY = 5
};

enum Phases
{
	PHASE_COMBAT,
	PHASE_END
};

enum SpawnIds
{
	GRIPPING_HATRED = 59804,
	/* TARAN_ZHUS_PERSONAL_STASH = 213888 */ //coffre Taran Zhu
	NPC_TRIGGER = 0000
};

Position const spawnPos[4] =
{
	{0.0f, 0.0f, 0.0f, 0.0f}, // Spawn 1 gripping hatred
	{0.0f, 0.0f, 0.0f, 0.0f}, // Spawn 2 gripping hatred
    {0.0f, 0.0f, 0.0f, 0.0f}, // Spawn 3 gripping hatred
	{0.0f, 0.0f, 0.0f, 0.0f} // Spawn Master Snowdrift
};


class boss_taran_zhu : public CreatureScript
{
public:
	boss_taran_zhu() : CreatureScript("boss_taran_zhu") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_taran_zhuAI(creature);
	}

	struct boss_taran_zhuAI : public ScriptedAI
	{
		boss_taran_zhuAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		
		bool checkTaranZhuAlive;
		

		void Reset()
		{
			checkTaranZhuAlive = true;
			checkTaranZhuAlive = me->isAlive();
						
			events.Reset();

			if (instance)
			{
				if(Creature *triggers = me->FindNearestCreature(NPC_TRIGGER, 50000.0f))
					if(triggers->AI())
					   triggers->AI()->Reset();
				instance->SetBossState(DATA_BOSS_TARAN_ZHU, NOT_STARTED);
				me->AI()->DoAction(ACTION_BOSS_TARAN_ZHU_RESET);
			}
			
		}
		
		void CastSpellHazeOfHate()
		{
			Map::PlayerList const &playerList = me->GetMap()->GetPlayers();
			if (!playerList.isEmpty())
			{
				for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
				{
					DoCast(i->getSource(), SPELL_HAZE_OF_HATE); //A test
				}
			}
		}

		void CastSpellMedidate()
		{
			Map::PlayerList const &playerList = me->GetMap()->GetPlayers();
			if (!playerList.isEmpty())
			{
				for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
				{
					DoCast(i->getSource(), SPELL_MEDIDATE); //A test
				}
			}
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_BOSS_TARAN_ZHU_RESET:
					checkTaranZhuAlive = true;
					break;
				case ACTION_BOSS_TARAN_ZHU_DIED:
					checkTaranZhuAlive = false;
					break;
				case ACTION_TARAN_INTRO:
					Talk(SAY_INTRO);
					break;
				
			}
        }

		void JustDied(Unit *pWho)
		{
			if (instance)
			{
				events.SetPhase(PHASE_END);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
				me->Respawn();
				if (instance)
				{
					instance->SetBossState(DATA_BOSS_TARAN_ZHU, DONE);
				}
				Talk(SAY_DEATH);
				events.ScheduleEvent(EVENT_THANKS, 8*IN_MILLISECONDS, 0, PHASE_END);
				me->SummonCreature(BOSS_MASTER_SNOWDRIFT, spawnPos[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);

			}
			
		}

		void KilledUnit(Unit *pWho)
		{
			Talk(SAY_SLAY);
		}
		
		void EnterEvadeMode()
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_TARAN_ZHU, FAIL);	
			Talk(SAY_EVADE);
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_TARAN_ZHU, IN_PROGRESS);

			me->SetInCombatWithZone();
			events.SetPhase(PHASE_COMBAT);
			Talk(SAY_COMBAT);
			events.ScheduleEvent(EVENT_RING_OF_MALICE, 6*IN_MILLISECONDS, 0, PHASE_COMBAT);
			events.ScheduleEvent(EVENT_GRIPPING_HATRED, 8*IN_MILLISECONDS, 0, PHASE_COMBAT);
			events.ScheduleEvent(EVENT_HAZE_OF_HATE, 15*IN_MILLISECONDS, 0, PHASE_COMBAT);
			events.ScheduleEvent(EVENT_RISING_HATE, 90*IN_MILLISECONDS, 0, PHASE_COMBAT);	
			events.ScheduleEvent(EVENT_SHA_BLAST, 180*IN_MILLISECONDS, 0, PHASE_COMBAT);
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
						case EVENT_RING_OF_MALICE:
							DoCast(me, SPELL_RING_OF_MALICE);
							events.ScheduleEvent(EVENT_RING_OF_MALICE, 12*IN_MILLISECONDS, 0, PHASE_COMBAT);
							break;

						case EVENT_GRIPPING_HATRED:
							me->SummonCreature(GRIPPING_HATRED, spawnPos[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							me->SummonCreature(GRIPPING_HATRED, spawnPos[1], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							me->SummonCreature(GRIPPING_HATRED, spawnPos[2], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							events.ScheduleEvent(EVENT_GRIPPING_HATRED, 12*IN_MILLISECONDS, 0, PHASE_COMBAT);
							break;

						case EVENT_HAZE_OF_HATE:
							CastSpellHazeOfHate();
							events.ScheduleEvent(EVENT_MEDIDATE, 3*IN_MILLISECONDS, 0, PHASE_COMBAT);							
							events.ScheduleEvent(EVENT_HAZE_OF_HATE, 12*IN_MILLISECONDS, 0, PHASE_COMBAT);
							break;
							
						case EVENT_MEDIDATE:
							CastSpellMedidate();
							events.CancelEvent(EVENT_MEDIDATE);
							break;

						case EVENT_THANKS:
							Talk(SAY_THANKS);
							events.CancelEvent(EVENT_THANKS);
							break;

						case EVENT_SHA_BLAST:
							if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_SHA_BLAST);
								}
							events.CancelEvent(EVENT_SHA_BLAST);
							break;

						case EVENT_RISING_HATE:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_RISING_HATE);
								}
							events.CancelEvent(EVENT_RISING_HATE);
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

class npc_taran_intro_trigger : public CreatureScript 
{
public:
	npc_taran_intro_trigger() : CreatureScript("npc_taran_intro_trigger") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_taran_intro_triggerAI(creature);
	}

	struct npc_taran_intro_triggerAI : public ScriptedAI
	{
		npc_taran_intro_triggerAI(Creature *creature) : ScriptedAI(creature)
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
				Map::PlayerList const &playerList = me->GetMap()->GetPlayers();
				if (!playerList.isEmpty())
				{
					for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
					{
						if (me->GetExactDist2d(i->getSource()->GetPositionX(),i->getSource()->GetPositionY()) < 12) // A tester si cela fonctionne.
						{
							if (instance)
								if (Creature* taran = me->GetCreature(*me, instance->GetData64(DATA_BOSS_TARAN_ZHU)))
									if (taran->AI())
									{
										taran->AI()->DoAction(ACTION_TARAN_INTRO);
										checkTrigger = false;
									}
						}
					}
				}
			}
		}
	};
};

class npc_gripping_hatred : public CreatureScript 
{
public:
	npc_gripping_hatred() : CreatureScript("npc_gripping_hatred") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_gripping_hatredAI(creature);
	}

	struct npc_gripping_hatredAI : public ScriptedAI
	{
		npc_gripping_hatredAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		
		void Reset()
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);  
		}

		void JustSummoned(Creature* creature)
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);  
		}

		void JustDied(Unit *pWho)
		{

		}

		void EnterCombat(Unit* /*who*/)
		{
			events.ScheduleEvent(EVENT_POOL_OF_SHADOWS, 2*IN_MILLISECONDS);
			events.ScheduleEvent(EVENT_GRIP_OF_HATE, 10*IN_MILLISECONDS);
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
						case EVENT_GRIP_OF_HATE:
							DoCast(me, SPELL_GRIP_OF_HATE);
							events.CancelEvent(EVENT_GRIP_OF_HATE);
							break;

						case EVENT_POOL_OF_SHADOWS:
							DoCast(me, SPELL_POOL_OF_SHADOWS);
							events.CancelEvent(EVENT_POOL_OF_SHADOWS);
							break;

						default:
							break;
					}
				}
			}
		}
	};
};


void AddSC_boss_taran_zhu()
{
	new boss_taran_zhu();
	new npc_taran_intro_trigger();
	new npc_gripping_hatred();
}