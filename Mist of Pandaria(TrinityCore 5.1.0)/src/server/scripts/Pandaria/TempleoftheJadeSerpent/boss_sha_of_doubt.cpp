/* # Script de Tydrheal & Sungis : Sha of Doubt # */

/*
	Notes : What is missing ? - Figment beam visual
*/

#include "ScriptPCH.h"
#include "AchievementMgr.h"
#include "temple_of_the_jade_serpent.h"

enum Spells
{
	/* Sha of Doubt */
	SPELL_WITHER_WILL				= 106736,
	SPELL_BOUNDS_OF_REALITY			= 117665,
	SPELL_TOUCH_OF_NOTHINGNESS		= 106113,
	SPELL_FIGMENT_OF_DOUBT_CLONE	= 106935,
	SPELL_FIGMENT_OF_DOUBT			= 106936,
	SPELL_SEED_OF_DOUBT				= 123916,

	/* Figment of Doubt */
	SPELL_SHADOWFORM				= 107903,
	SPELL_RELEASE_DOUBT				= 106112,
	SPELL_GATHERING_DOUBT			= 117570,
	SPELL_DRAW_DOUBT				= 106290,
};

enum Events
{
	/* Sha of Doubt */
	EVENT_WITHER_WILL				= 1,
	EVENT_TOUCH_OF_NOTHINGNESS		= 2,
	EVENT_MOVE_TO_THE_CENTER		= 3,
	EVENT_BOUNDS_OF_REALITY			= 4,
	EVENT_SUMMON_FIGMENT_OF_DOUBT	= 5,
	EVENT_BOUNDS_DONE				= 6,

	/* Figment of Doubt */
	EVENT_ATTACK_PLAYERS			= 1,
	EVENT_RELEASE_DOUBT				= 2,
};

enum Texts
{
	SAY_AGGRO		= 0,
	SAY_DEATH		= 1,
	SAY_FIGMENT_1	= 2,
	SAY_FIGMENT_2	= 3,
	SAY_RESET		= 4,
	SAY_SLAY_1		= 5,
	SAY_SLAY_2		= 6
};

enum Phases
{
	PHASE_NULL				= 0,
	PHASE_COMBAT			= 1,
	PHASE_BOUNDS_OF_REALITY	= 2
};

enum Actions
{
	ACTION_SHA_OF_DOUBT_PHASE_COMBAT,
	ACTION_FIGMENT_ATTACK
};

enum Npcs
{
	NPC_FIGMENT_OF_DOUBT	= 56792,
	NPC_SHA_TRIGGER			= 400453,
};

enum Achievements
{
// Defeat the Sha of Doubt in Temple of the Jade Serpent while under the effects of Purified Water.
	ACHI_CLEANING_UP						= 6475,
// Defeat the Sha of Doubt in Temple of the Jade Serpent while under the effect of 4 Seeds of Doubt on Heroic difficulty.
	ACHI_SEEDS_OF_DOUBT						= 6671,
// Defeat the Sha of Doubt in Temple of the Jade Serpent.
	ACHI_TEMPLE_OF_THE_JADE_SERPENT			= 6757,
// Defeat the Sha of Doubt in Temple of the Jade Serpent on Heroic difficulty.
	ACHI_TEMPLE_OF_THE_JADE_SERPENT_HEROIC	= 6758
};

class boss_sha_of_doubt : public CreatureScript
{
public:
	boss_sha_of_doubt() : CreatureScript("boss_sha_of_doubt") { }

	CreatureAI* GetAI(Creature* creature) const 
	{
		return new boss_sha_of_doubtAI(creature);
	}

	struct boss_sha_of_doubtAI : public BossAI
	{
		boss_sha_of_doubtAI(Creature *creature) : BossAI(creature, DATA_BOSS_SHA_OF_DOUBT)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		Map* map;
		bool boundsOfReality;
		bool seventyFivePct;
		bool fiftyPct;

		void Reset()
		{
			_Reset();

			events.Reset();

			if (instance)
			{
				boundsOfReality = false;
				seventyFivePct = false;
				fiftyPct = false;

				events.SetPhase(PHASE_NULL);
				instance->SetBossState(DATA_BOSS_SHA_OF_DOUBT, NOT_STARTED);
			}
		}

		void JustDied(Unit *pWho)
		{
			_JustDied();

			if (instance)
			{
				instance->SetBossState(DATA_BOSS_SHA_OF_DOUBT, DONE);

				map = me->GetMap();

				if (GameObject* go = me->FindNearestGameObject(GO_SHA_OF_DOUBT_GATE, 99999.0f))
					go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

				if (map && map->IsDungeon())
				{
					Map::PlayerList const &PlayerList = map->GetPlayers();

					if (!PlayerList.isEmpty())
						for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
							if (Player* player = i->getSource())
							{
								if (player->HasAura(SPELL_TOUCH_OF_NOTHINGNESS))
									player->RemoveAurasDueToSpell(SPELL_TOUCH_OF_NOTHINGNESS, player->GetGUID());

								if (map->IsHeroic())
								{
									if (player->HasAura(118714)) // Spell Purified Water
										player->CompletedAchievement(sAchievementMgr->GetAchievement(ACHI_CLEANING_UP));

									if (Aura* seed = player->GetAura(SPELL_SEED_OF_DOUBT))
										if (seed->GetStackAmount() == 4)
											player->CompletedAchievement(sAchievementMgr->GetAchievement(ACHI_SEEDS_OF_DOUBT));

									player->CompletedAchievement(sAchievementMgr->GetAchievement(ACHI_TEMPLE_OF_THE_JADE_SERPENT_HEROIC));
								}
								
								player->CompletedAchievement(sAchievementMgr->GetAchievement(ACHI_TEMPLE_OF_THE_JADE_SERPENT));
							}
				}
			}

			Talk(SAY_DEATH);
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_SHA_OF_DOUBT_PHASE_COMBAT:
					if (me->HasAura(SPELL_BOUNDS_OF_REALITY))
						me->RemoveAurasDueToSpell(SPELL_BOUNDS_OF_REALITY, me->GetGUID());
					me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
					events.SetPhase(PHASE_COMBAT);
					events.CancelEvent(EVENT_WITHER_WILL);
					events.CancelEvent(EVENT_TOUCH_OF_NOTHINGNESS);
					events.ScheduleEvent(EVENT_WITHER_WILL, 7*IN_MILLISECONDS, 0, PHASE_COMBAT);
					events.ScheduleEvent(EVENT_TOUCH_OF_NOTHINGNESS, 2*IN_MILLISECONDS, 0, PHASE_COMBAT);
					me->GetMotionMaster()->MoveChase(me->getVictim());
					boundsOfReality = false;
					break;
            }
        }

		void KilledUnit(Unit *pWho)
		{
			Talk(irand(SAY_SLAY_1, SAY_SLAY_2));
		}
		
		void EnterEvadeMode()
		{
			_EnterEvadeMode();

			events.Reset();

			boundsOfReality = false;
			seventyFivePct = false;
			fiftyPct = false;

			events.SetPhase(PHASE_NULL);

			Talk(SAY_RESET);

			me->CombatStop(true);
			me->DeleteThreatList();

			if (me->HasAura(SPELL_BOUNDS_OF_REALITY))
				me->RemoveAurasDueToSpell(SPELL_BOUNDS_OF_REALITY, me->GetGUID());

			if (GameObject* go = me->FindNearestGameObject(GO_SHA_OF_DOUBT_GATE, 9999.0f))
				go->UseDoorOrButton();

			if (instance)
			{
				instance->SetBossState(DATA_BOSS_SHA_OF_DOUBT, FAIL);

				std::list<Creature*> figments;
				me->GetCreatureListWithEntryInGrid(figments, NPC_FIGMENT_OF_DOUBT, 99999.0f);
				if (!figments.empty())
				{
					for (std::list<Creature*>::iterator itr = figments.begin(); itr != figments.end(); ++itr)
						(*itr)->DespawnOrUnsummon();
				}

				if (GameObject* go = me->FindNearestGameObject(GO_SHA_OF_DOUBT_GATE, 99999.0f))
					go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
			}
		}

		void EnterCombat(Unit* /*who*/)
		{
			_EnterCombat();

			if (instance)
			{
				instance->SetBossState(DATA_BOSS_SHA_OF_DOUBT, IN_PROGRESS);
				me->SetInCombatWithZone();

				if (GameObject* go = me->FindNearestGameObject(GO_SHA_OF_DOUBT_GATE, 9999.0f))
					go->SetGoState(GO_STATE_READY);
			}

			events.SetPhase(PHASE_COMBAT);
			events.ScheduleEvent(EVENT_WITHER_WILL, 2*IN_MILLISECONDS, 0, PHASE_COMBAT);
			events.ScheduleEvent(EVENT_TOUCH_OF_NOTHINGNESS, 8*IN_MILLISECONDS, 0, PHASE_COMBAT);

			Talk(SAY_AGGRO);
		}
		
		void UpdateAI(uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (!HealthBelowPct(75) && seventyFivePct)
				seventyFivePct = false;

			if (!HealthBelowPct(50) && fiftyPct)
				fiftyPct = false;

			if (HealthBelowPct(75) && !seventyFivePct)
			{
				events.SetPhase(PHASE_BOUNDS_OF_REALITY);
				events.ScheduleEvent(EVENT_MOVE_TO_THE_CENTER, 0, 0, PHASE_BOUNDS_OF_REALITY);
				events.ScheduleEvent(EVENT_SUMMON_FIGMENT_OF_DOUBT, 0, 0, PHASE_BOUNDS_OF_REALITY);
				seventyFivePct = true;
			}

			if (HealthBelowPct(50) && !fiftyPct)
			{
				events.SetPhase(PHASE_BOUNDS_OF_REALITY);
				events.ScheduleEvent(EVENT_MOVE_TO_THE_CENTER, 0, 0, PHASE_BOUNDS_OF_REALITY);
				events.ScheduleEvent(EVENT_SUMMON_FIGMENT_OF_DOUBT, 0, 0, PHASE_BOUNDS_OF_REALITY);
				fiftyPct = true;
			}

			if (events.IsInPhase(PHASE_BOUNDS_OF_REALITY) && !me->HasAura(SPELL_BOUNDS_OF_REALITY))
				if (me->FindNearestCreature(NPC_SHA_TRIGGER, 0.1f, true))
					events.ScheduleEvent(EVENT_BOUNDS_OF_REALITY, 2, 0, PHASE_BOUNDS_OF_REALITY);

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
					if (instance)
					{
						case EVENT_WITHER_WILL:
							me->CastSpell(me, SPELL_WITHER_WILL);

							events.ScheduleEvent(EVENT_WITHER_WILL, 7*IN_MILLISECONDS, 0, PHASE_COMBAT);
							break;

						case EVENT_TOUCH_OF_NOTHINGNESS:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if (target && target->GetTypeId() == TYPEID_PLAYER)
									target->CastSpell(target, SPELL_TOUCH_OF_NOTHINGNESS);

							events.ScheduleEvent(EVENT_TOUCH_OF_NOTHINGNESS, 18*IN_MILLISECONDS, 0, PHASE_COMBAT);
							break;

						case EVENT_MOVE_TO_THE_CENTER:
							if (Creature* trigger = me->FindNearestCreature(NPC_SHA_TRIGGER, 99999.0f, true))
								me->GetMotionMaster()->MovePoint(0, trigger->GetHomePosition());

							events.CancelEvent(EVENT_MOVE_TO_THE_CENTER);
							break;

						case EVENT_BOUNDS_OF_REALITY:
						{
							if (Creature* trigger = me->FindNearestCreature(NPC_SHA_TRIGGER, 99999.0f, true))
							{
								me->CastSpell(me, SPELL_BOUNDS_OF_REALITY);
								me->Relocate(trigger->GetHomePosition());
								me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
								me->SetOrientation(4.410300f);
								me->GetMotionMaster()->Clear(true);
							}

							std::list<Creature*> figments;
							me->GetCreatureListWithEntryInGrid(figments, NPC_FIGMENT_OF_DOUBT, 99999.0f);
							if (!figments.empty())
							{
								for (std::list<Creature*>::iterator itr = figments.begin(); itr != figments.end(); ++itr)
									(*itr)->AI()->DoAction(ACTION_FIGMENT_ATTACK);
							}

							events.ScheduleEvent(EVENT_BOUNDS_DONE, 1, 0, PHASE_BOUNDS_OF_REALITY);
							events.CancelEvent(EVENT_BOUNDS_OF_REALITY);
							break;
						}

						case EVENT_SUMMON_FIGMENT_OF_DOUBT:
						{
							map = me->GetMap();

							if (map && map->IsDungeon())
							{
								Map::PlayerList const &PlayerList = map->GetPlayers();

								if (!PlayerList.isEmpty())
									for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
										if (Unit* player = i->getSource()->ToPlayer())
											player->CastSpell(player, SPELL_FIGMENT_OF_DOUBT);
							}

							Talk(irand(SAY_FIGMENT_1, SAY_FIGMENT_2));

							events.CancelEvent(EVENT_SUMMON_FIGMENT_OF_DOUBT);
							break;
						}
						
						case EVENT_BOUNDS_DONE:
							boundsOfReality = true;

							events.CancelEvent(EVENT_BOUNDS_DONE);
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

class npc_figment_of_doubt : public CreatureScript 
{
public:
    npc_figment_of_doubt() : CreatureScript("npc_figment_of_doubt") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_figment_of_doubtAI(creature);
    }

    struct npc_figment_of_doubtAI : public ScriptedAI
    {
        npc_figment_of_doubtAI(Creature *creature) : ScriptedAI(creature)
        {
        }

		InstanceScript* instance;
        EventMap events;
		Player* player;
		Map* map;
		bool emote;
		bool aggro;

        void Reset()
		{
			events.Reset();

			emote = false;
			aggro = false;

			me->SetReactState(REACT_PASSIVE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			me->CastSpell(me, SPELL_SHADOWFORM);

			if (instance)
			{
				if (me->isSummon())
				{
					player = me->ToTempSummon()->GetSummoner()->ToPlayer();
					me->SetDisplayId(player->GetDisplayId());
				}
			}
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_FIGMENT_ATTACK:
					me->CastSpell(me, SPELL_GATHERING_DOUBT);
					events.ScheduleEvent(EVENT_ATTACK_PLAYERS, 2*IN_MILLISECONDS);
					events.ScheduleEvent(EVENT_RELEASE_DOUBT, 30*IN_MILLISECONDS);
					break;
            }
        }

		void EnterEvadeMode()
		{
			if (instance)
				me->DespawnOrUnsummon();
		}

        void JustDied(Unit *pWho)
        {
			if (instance)
			{
				if (!me->FindNearestCreature(NPC_FIGMENT_OF_DOUBT, 99999.0f, true))
				{
					if (Creature* sha = me->FindNearestCreature(BOSS_SHA_OF_DOUBT, 99999.0f, true))
					{
						if (sha->HasAura(SPELL_BOUNDS_OF_REALITY))
							sha->RemoveAurasDueToSpell(SPELL_BOUNDS_OF_REALITY, me->GetGUID());

						sha->AI()->DoAction(ACTION_SHA_OF_DOUBT_PHASE_COMBAT);
						me->DespawnOrUnsummon();
					}
				}

				if (me->FindNearestCreature(NPC_FIGMENT_OF_DOUBT, 99999.0f, true))
					me->DespawnOrUnsummon();
			}
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

			if (!emote)
			{
				me->HandleEmoteCommand(EMOTE_STATE_DROWNED);
				emote = true;
			}
			
			if (aggro)
			{
				if (me->getVictim() != player)
					me->AI()->AttackStart(player);

				if (!player->isAlive())
					me->DespawnOrUnsummon();
			}

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_ATTACK_PLAYERS:
							me->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
							me->setFaction(14);
							me->SetReactState(REACT_AGGRESSIVE);
							if (player)
							{
								me->AI()->AttackStart(player);
								me->AddThreat(player, 99999.0f);
								me->SetInCombatWithZone();
								me->CastSpell(player, SPELL_DRAW_DOUBT, true);
								aggro = true;
							}

							events.CancelEvent(EVENT_ATTACK_PLAYERS);
							break;

						case EVENT_RELEASE_DOUBT:
							me->CastSpell(me, SPELL_RELEASE_DOUBT);
							if (Creature* sha = me->FindNearestCreature(BOSS_SHA_OF_DOUBT, 99999.0f, true))
							{
								sha->ModifyHealth(int32(sha->GetMaxHealth() * 0.1f));
								me->Kill(me);
							}

							events.CancelEvent(EVENT_RELEASE_DOUBT);
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

class go_seed_of_doubt : public GameObjectScript
{
public:
    go_seed_of_doubt() : GameObjectScript("go_seed_of_doubt") { }

	InstanceScript* instance;

    bool OnGossipHello(Player* player, GameObject* go)
    {
		instance = go->GetInstanceScript();

		if (instance)
		{
			instance->DoCastSpellOnPlayers(SPELL_SEED_OF_DOUBT);
			go->Delete();
		}
		
        return true;
    }
};

void AddSC_boss_sha_of_doubt()
{
	new boss_sha_of_doubt();
	new npc_figment_of_doubt();
	new go_seed_of_doubt();
}