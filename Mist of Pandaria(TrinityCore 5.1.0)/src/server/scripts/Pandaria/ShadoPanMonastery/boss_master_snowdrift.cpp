/* # Script de Tydrheal : Master Snowdrift # */

/* Notes : Tester -- Voir Spells -- voir pour spwan coffre à la fin du boss -- SOUNDID
Ajouter l'ID du NPC_TRIGGER

Master Snowdrift : Script 95% (terminé -- voir spells)	
Master Trigger	: Script 95% (terminé) -- ajouter le flag pour qu'il soit invisible => Modifier dans la db pour la visibilité du PNJ par les joueurs.
ShadoPan Novice : Script 100% 
Flying Snow : Script 100%
Fragrant Lotus : Script 100%
Miroir Master Snowdrift : Script 95% (terminé -- voir spells)



UPDATE creature_template SET ScriptName = 'boss_master_snowdrift' WHERE entry = 56541;
UPDATE creature_template SET ScriptName = 'npc_master_trigger' WHERE entry = ?????;
UPDATE creature_template SET ScriptName = 'npc_shadopan_novice' WHERE entry = 56395;
UPDATE creature_template SET ScriptName = 'npc_fragrant_lotus' WHERE entry = 56472;
UPDATE creature_template SET ScriptName = 'npc_flying_snow' WHERE entry = 56473;
UPDATE creature_template SET ScriptName = 'npc_miroir_master_snowdrift' WHERE entry = ?????;


INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(56541, 0, 0, "Très bien, étrangers. Découvrons votre vraie force.", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - EnterCombat1"),
(56541, 1, 0, "Savoir manier une arme n’est que la manifestation physique de la force martiale. Laissez-moi vous montrer ce qu’est la force spirituelle !", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Phase2"),
(56541, 2, 0, "Quand je n’étais qu’un jeune pandaren, mes poings étaient faibles, mais les années d’entraînement m’ont ouvert tant de possibilités !", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Phase3"),
(56541, 3, 0, "Vous ! Vous avez permis le réveil du sha après toutes ces années !", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro1"),
(56541, 4, 0, "Si vous voulez vraiment arrêter ce que les vôtres ont déchaîné sur nos terres, vous devez d’abord faire vos preuves dans notre école.", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro2BeforeSpawn"),
(56541, 5, 0, "Vous affronterez tout d’abord nos initiés…", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro3SecondSpawn"),
(56541, 6, 0, "Quand l'un mord la poussière, un autre se lève et prend sa place.", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro4AndIntro6"),
(56541, 7, 0, "Vous avez beaucoup à apprendre si vous voulez aider votre peuple à se défendre des initiés des sha. Placez-vous sur le côté !", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro5ThirdSpawn"),
(56541, 8, 0, "Vous avez vaincu nos plus jeunes élèves. Maintenant, vous allez affronter les plus aguerris.", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro7"),
(56541, 9, 0, "Neige Virevoltante ! Brise de Lotus ! Avancez !", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro8SpawnBestStudents"),
(56541, 10, 0, "Vous avez vaincu mes meilleurs disciples. Vous pourrez peut-être nous être utiles, finalement…", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro9DeathBestStudents"),
(56541, 11, 0, "Comme mon maître avait coutume de le dire, « on ne connaît vraiment quelqu'un qu’après l'avoir combattu. »", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro10OpenDoor"),
(56541, 12, 0, "Avancez, que nous fassions les présentations dans les formes.", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Intro11BeforeMove"),
(56541, 13, 0, "Votre technique est imparfaite.", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - Slay1"),
(56541, 14, 0, "Vous avez besoin de vous entraîner plus !", 14, 0, 100, 0, 0, ???????, "MasterSnowdrift - InPhase1");


*/

#include "ScriptPCH.h"
#include "shadopan_monastery.h"

enum Spells
{
	/* Master Snowdrift */
	SPELL_CHASE_DOWN = 118961,
	SPELL_FISTS_OF_FURY = 113656,
	SPELL_TORNADO_KICK = 106434,
	SPELL_BALL_OF_FIRE = 106533,
	SPELL_TORNADO_SLAM = 106352,
	SPELL_PARRY_STANCE = 106447,
	SPELL_QUIVERING_PALM = 106422,
	SPELL_UNLEASH_VIBRATIONS = 106423,
	SPELL_FLYING_KICK = 106439
};

enum Events
{
	/* Intro balcon -- trigger */
	EVENT_SPAWN_BALCON_NOVICE_STUDENTS,
	EVENT_SPAWN_BALCON_BEST_STUDENTS,
	EVENT_INTRO_2,
	EVENT_INTRO_3,
	

	/* Boss Master Snowdrift */
	EVENT_INTRO_8,
	EVENT_INTRO_10,
	EVENT_INTRO_11,
	EVENT_MOVE,
	EVENT_BEFORE_COMBAT,
	EVENT_MORE_TRAINING_TEXT,

	EVENT_CHASE_DOWN,
	EVENT_FISTS_OF_FURY,
	EVENT_TORNADO_KICK,
	EVENT_PHASE_2,
	EVENT_PHASE_2_PART_1,
	EVENT_PHASE_2_SPAWN_1,
	EVENT_PHASE_2_PART_2,
	EVENT_PHASE_2_SPAWN_2,
	EVENT_PHASE_2_PART_3,
	EVENT_PHASE_2_SPAWN_3,
	EVENT_BALL_OF_FIRE,
	EVENT_PHASE_3,
	EVENT_TORNADO_SLAM,
	EVENT_PARRY_STANCE,
	EVENT_QUIVERING_PALM,
	EVENT_FLYING_KICK,
	EVENT_UNLEASH_VIBRATIONS,
	EVENT_REMOVE_AURA,
	EVENT_END,

	/* Npc Miroir Master Snowdrift */
	EVENT_NPC_BALL_OF_FIRE
};

enum Phases
{
	PHASE_1,
	PHASE_2,
	PHASE_3,
	PHASE_END
};

enum Actions
{
	ACTION_BOSS_MASTER_SNOWDRIFT_RESET,
	ACTION_BOSS_MASTER_SNOWDRIFT_DIED,
	ACTION_FRAGRANT_LOTUS_DEATH,
	ACTION_FLYING_SNOW_DEATH,
	ACTION_INTRO_1,
	ACTION_INTRO_2,
	ACTION_INTRO_3,
	ACTION_INTRO_4,
	ACTION_INTRO_5,
	ACTION_INTRO_7,
	ACTION_SPAWN_SHADOPAN_NOVICE
};

enum Texts
{
	SAY_COMBAT = 0,
	SAY_PHASE_2 = 1,
	SAY_PHASE_3 = 2,
	SAY_INTRO_1 = 3,
	SAY_INTRO_2 = 4,
	SAY_INTRO_3 = 5,
	SAY_INTRO_4 = 6,
	SAY_INTRO_5 = 7,
	SAY_INTRO_7 = 8,
	SAY_INTRO_8 = 9,
	SAY_INTRO_9 = 10,
	SAY_INTRO_10 = 11,
	SAY_INTRO_11 = 12,
	SAY_SLAY = 13,
	SAY_MORE_TRAINING = 14
};

enum MovePoints
{
    POINT_SPAWN_1  = 0,
	POINT_SPAWN_2 = 0,
	POINT_SPAWN_3 = 0
};

enum SpawnIds
{
    MIROIR_MASTER_SNOWDRIFT  = 0000,
	/* SNOWDRIFTS_POSSESSIONS = 214518 */
	SHADOPAN_NOVICE = 56395,
	FRAGRANT_LOTUS = 56472,
	FLYING_SNOW = 56473
};

Position const spawnPos[6] =
{
	{0.0f, 0.0f, 0.0f, 0.0f}, // Spawn balcon gauche
	{0.0f, 0.0f, 0.0f, 0.0f}, // Spawn balcon droite
    {0.0f, 0.0f, 0.0f, 0.0f}, // Spawn 1
    {0.0f, 0.0f, 0.0f, 0.0f}, // Spawn 2
	{0.0f, 0.0f, 0.0f, 0.0f}, // Spawn 3
	{0.0f, 0.0f, 0.0f, 0.0f}
};

enum TriggerIds
{
	NPC_TRIGGER = 00000
};

enum goIds
{
	GO_SNOWDRIFTS_POSSESIONS = 214518
};

class boss_master_snowdrift : public CreatureScript
{
public:
	boss_master_snowdrift() : CreatureScript("boss_master_snowdrift") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_master_snowdriftAI(creature);
	}

	struct boss_master_snowdriftAI : public ScriptedAI
	{
		boss_master_snowdriftAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;
		
		bool checkMasterSnowdriftAlive;
		bool checkFlyingSnowDeath;
		bool checkFragrantLotusDeath;
		bool checkIntro9;
		bool checkBallOfFire;
		int introTerminee;
		int counterShadoPanNovice;
		
		
		void Reset()
		{
			checkBallOfFire = false;
			checkFlyingSnowDeath = false;
			checkIntro9 = false;
			checkFragrantLotusDeath = false;
			checkMasterSnowdriftAlive = true;
			checkMasterSnowdriftAlive = me->isAlive();
			counterShadoPanNovice = 4;
			
			events.Reset();

			if (introTerminee != 5)
			{
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
				if(Creature *triggers = me->FindNearestCreature(NPC_TRIGGER, 50000.0f))
					if(triggers->AI())
					   triggers->AI()->Reset();
			}
			
			if(Aura* aura = me->GetAura(SPELL_PARRY_STANCE))
				me->RemoveAura(aura);

			if (instance)
			{
				instance->SetBossState(DATA_BOSS_MASTER_SNOWDRIFT, NOT_STARTED);
				me->AI()->DoAction(ACTION_BOSS_MASTER_SNOWDRIFT_RESET);
			}
		
		}

		void DoAction(int32 action)
        {
            switch (action)
            {
				case ACTION_BOSS_MASTER_SNOWDRIFT_RESET:
					checkMasterSnowdriftAlive = true;
					break;
				case ACTION_SPAWN_SHADOPAN_NOVICE:
					if (counterShadoPanNovice < 30)
					{
						me->SummonCreature(SHADOPAN_NOVICE, spawnPos[urand(0,1)], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
						counterShadoPanNovice++;
						if (counterShadoPanNovice == 8)
							DoAction(ACTION_INTRO_4);
						if (counterShadoPanNovice == 12)
							DoAction(ACTION_INTRO_5);
						if (counterShadoPanNovice == 16)
							DoAction(ACTION_INTRO_4);
					}
					if (counterShadoPanNovice >= 30)
					{
						DoAction(ACTION_INTRO_7);
					}
					break;
				case ACTION_BOSS_MASTER_SNOWDRIFT_DIED:
					checkMasterSnowdriftAlive = false;
					break;
				case ACTION_INTRO_1:
					if(introTerminee != 5)
						Talk(SAY_INTRO_1);
					break;
				case ACTION_INTRO_2:
					if(introTerminee != 5)
					{
						Talk(SAY_INTRO_2);
						me->SummonCreature(SHADOPAN_NOVICE, spawnPos[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
						me->SummonCreature(SHADOPAN_NOVICE, spawnPos[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
						me->SummonCreature(SHADOPAN_NOVICE, spawnPos[1], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
						me->SummonCreature(SHADOPAN_NOVICE, spawnPos[1], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
					}
					break;
				case ACTION_INTRO_3:
					Talk(SAY_INTRO_3);
					break;
				case ACTION_INTRO_4:
					Talk(SAY_INTRO_4);
					break;
				case ACTION_INTRO_5:
					Talk(SAY_INTRO_5);
					break;
				case ACTION_INTRO_7:
					Talk(SAY_INTRO_7);
					events.ScheduleEvent(EVENT_INTRO_8, 10*IN_MILLISECONDS);
					break;
				case ACTION_FRAGRANT_LOTUS_DEATH:
					checkFragrantLotusDeath = true;
					break;
				case ACTION_FLYING_SNOW_DEATH:
					checkFlyingSnowDeath = true;
					break;
			}
        }

		void JustDied(Unit *pWho)
		{
			if (instance)
			{
				instance->SetBossState(DATA_BOSS_MASTER_SNOWDRIFT, DONE);
			}
			introTerminee = 0;
			//au cas ou
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->RemoveCorpse(); 
			me->Respawn();
			me->GetMotionMaster()->MoveJump(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY(), spawnPos[2].GetPositionZ(), me->GetExactDist2d(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY()) * 10.0f / 1.0f, 1.0f); //voir pour le speedZ 1.0f
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			me->SummonGameObject(GO_SNOWDRIFTS_POSSESIONS, spawnPos[5].GetPositionX(), spawnPos[5].GetPositionY(), spawnPos[5].GetPositionZ(), 0, 0, 0, 0, 0, 9000000);
		}

		void KilledUnit(Unit *pWho)
		{
			Talk(SAY_SLAY);
		}
		
		void EnterEvadeMode()
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_MASTER_SNOWDRIFT, FAIL);
			introTerminee = 5;
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
			me->GetMotionMaster()->MoveJump(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY(), spawnPos[2].GetPositionZ(), me->GetExactDist2d(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY()) * 10.0f / 1.0f, 1.0f); //voir pour le speedZ 1.0f
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (instance)
				instance->SetBossState(DATA_BOSS_MASTER_SNOWDRIFT, IN_PROGRESS);

			me->SetInCombatWithZone();
			events.SetPhase(PHASE_1);
			Talk(SAY_COMBAT);
			events.ScheduleEvent(EVENT_FISTS_OF_FURY, 5*IN_MILLISECONDS, 0, PHASE_1);
			events.ScheduleEvent(EVENT_MORE_TRAINING_TEXT, 10*IN_MILLISECONDS, 0, PHASE_1);
			events.ScheduleEvent(EVENT_TORNADO_KICK, 15*IN_MILLISECONDS, 0, PHASE_1);
			
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
						case EVENT_INTRO_8:
							Talk(SAY_INTRO_8);
							me->SummonCreature(FLYING_SNOW, spawnPos[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 180*IN_MILLISECONDS);
							me->SummonCreature(FRAGRANT_LOTUS, spawnPos[1], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 180*IN_MILLISECONDS);
							events.CancelEvent(EVENT_INTRO_8);
							break;
						
						case EVENT_INTRO_10:
							Talk(SAY_INTRO_10);
							events.ScheduleEvent(EVENT_INTRO_11, 10*IN_MILLISECONDS);
							events.CancelEvent(EVENT_INTRO_10);
							break;
						
						case EVENT_INTRO_11:
							Talk(SAY_INTRO_11);
							events.ScheduleEvent(EVENT_MOVE, 3*IN_MILLISECONDS);
							events.CancelEvent(EVENT_INTRO_11);
							break;
						
						case EVENT_MOVE:
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
							me->GetMotionMaster()->MoveJump(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY(), spawnPos[2].GetPositionZ(), me->GetExactDist2d(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY()) * 10.0f / 1.0f, 1.0f); //voir pour le speedZ 1.0f
							events.ScheduleEvent(EVENT_BEFORE_COMBAT, 4*IN_MILLISECONDS);
							events.CancelEvent(EVENT_MOVE);
							break;

						case EVENT_BEFORE_COMBAT:
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
							events.CancelEvent(EVENT_BEFORE_COMBAT);
							break;

						case EVENT_FISTS_OF_FURY:
							DoCast(me, SPELL_FISTS_OF_FURY);
							events.CancelEvent(EVENT_FISTS_OF_FURY);
							break;

						case EVENT_TORNADO_KICK:
							DoCast(me, SPELL_TORNADO_KICK);
							events.CancelEvent(EVENT_TORNADO_KICK);
							break;

						case EVENT_MORE_TRAINING_TEXT:
							Talk(SAY_MORE_TRAINING);
							events.ScheduleEvent(EVENT_MORE_TRAINING_TEXT, 13*IN_MILLISECONDS, 0 , PHASE_1);
							break;

						case EVENT_PHASE_2:
							Talk(SAY_PHASE_2);
							events.ScheduleEvent(EVENT_PHASE_2_PART_1, 0, 0 , PHASE_2);
							events.CancelEvent(EVENT_PHASE_2);
							break;

						case EVENT_PHASE_2_PART_1:
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->GetMotionMaster()->MovePoint(POINT_SPAWN_2, spawnPos[3]);
							events.ScheduleEvent(EVENT_PHASE_2_SPAWN_1, 5*IN_MILLISECONDS, 0 , PHASE_2);
							events.CancelEvent(EVENT_PHASE_2_PART_1);
							break;

						case EVENT_PHASE_2_SPAWN_1:
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
							me->SummonCreature(MIROIR_MASTER_SNOWDRIFT, spawnPos[2], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							me->SummonCreature(MIROIR_MASTER_SNOWDRIFT, spawnPos[4], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							checkBallOfFire = true;
							events.ScheduleEvent(EVENT_BALL_OF_FIRE, 2*IN_MILLISECONDS, 0, PHASE_2);
							events.ScheduleEvent(EVENT_PHASE_2_PART_2, 15*IN_MILLISECONDS, 0, PHASE_2);
							events.CancelEvent(EVENT_PHASE_2_SPAWN_1);
							break;

						case EVENT_BALL_OF_FIRE:
							if(checkBallOfFire)
							{
								DoCast(me, SPELL_BALL_OF_FIRE);
								events.ScheduleEvent(EVENT_BALL_OF_FIRE, 2*IN_MILLISECONDS, 0, PHASE_2);
							}
							else
								events.CancelEvent(EVENT_BALL_OF_FIRE);
							break;

						case EVENT_PHASE_2_PART_2:
							checkBallOfFire = false;
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->GetMotionMaster()->MovePoint(POINT_SPAWN_3, spawnPos[4]);
							events.ScheduleEvent(EVENT_PHASE_2_SPAWN_2, 5*IN_MILLISECONDS, 0 , PHASE_2);
							events.CancelEvent(EVENT_PHASE_2_PART_2);
							break;

						case EVENT_PHASE_2_SPAWN_2:
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
							me->SummonCreature(MIROIR_MASTER_SNOWDRIFT, spawnPos[2], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							me->SummonCreature(MIROIR_MASTER_SNOWDRIFT, spawnPos[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							checkBallOfFire = true;
							events.ScheduleEvent(EVENT_BALL_OF_FIRE, 2*IN_MILLISECONDS, 0, PHASE_2);
							events.ScheduleEvent(EVENT_PHASE_2_PART_3, 15*IN_MILLISECONDS, 0, PHASE_2);
							events.CancelEvent(EVENT_PHASE_2_SPAWN_2);
							break;

						case EVENT_PHASE_2_PART_3:
							checkBallOfFire = false;
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
							me->GetMotionMaster()->MovePoint(POINT_SPAWN_1, spawnPos[2]);
							events.ScheduleEvent(EVENT_PHASE_2_SPAWN_3, 5*IN_MILLISECONDS, 0 , PHASE_2);
							events.CancelEvent(EVENT_PHASE_2_PART_3);
							break;

						case EVENT_PHASE_2_SPAWN_3:
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); 
							me->SummonCreature(MIROIR_MASTER_SNOWDRIFT, spawnPos[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							me->SummonCreature(MIROIR_MASTER_SNOWDRIFT, spawnPos[4], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15*IN_MILLISECONDS);
							checkBallOfFire = true;
							events.ScheduleEvent(EVENT_BALL_OF_FIRE, 2*IN_MILLISECONDS, 0, PHASE_2);
							events.ScheduleEvent(EVENT_PHASE_3, 15*IN_MILLISECONDS, 0, PHASE_2);
							events.CancelEvent(EVENT_PHASE_2_SPAWN_3);
							break;

						case EVENT_PHASE_3:
							checkBallOfFire = false;
							me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
							Talk(SAY_PHASE_3);
							events.SetPhase(PHASE_3);
							events.ScheduleEvent(EVENT_CHASE_DOWN, 0, 0, PHASE_3);
							events.ScheduleEvent(EVENT_TORNADO_SLAM, 10*IN_MILLISECONDS, 0, PHASE_3);
							events.ScheduleEvent(EVENT_PARRY_STANCE, 20*IN_MILLISECONDS, 0, PHASE_3);
							events.CancelEvent(EVENT_PHASE_3);
							break;

						case EVENT_CHASE_DOWN:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_CHASE_DOWN);
								}
							events.CancelEvent(EVENT_CHASE_DOWN);
							break;

						case EVENT_TORNADO_SLAM:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_TORNADO_SLAM);
								}
							events.CancelEvent(EVENT_TORNADO_SLAM);
							break;

						case EVENT_PARRY_STANCE:
							DoCast(me, SPELL_PARRY_STANCE);
							events.ScheduleEvent(urand(EVENT_QUIVERING_PALM, EVENT_FLYING_KICK), 4*IN_MILLISECONDS, 0, PHASE_3);
							events.CancelEvent(EVENT_PARRY_STANCE);
							break;

						case EVENT_QUIVERING_PALM:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_QUIVERING_PALM);
								}
							events.ScheduleEvent(EVENT_UNLEASH_VIBRATIONS, 6*IN_MILLISECONDS, 0, PHASE_3);
							events.CancelEvent(EVENT_QUIVERING_PALM);
							break;

						case EVENT_UNLEASH_VIBRATIONS:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_UNLEASH_VIBRATIONS);
								}
							events.ScheduleEvent(EVENT_REMOVE_AURA, 4*IN_MILLISECONDS, 0, PHASE_3);
							events.CancelEvent(EVENT_UNLEASH_VIBRATIONS);
							break;

						case EVENT_FLYING_KICK:
							if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
								if(target->GetTypeId() == TYPEID_PLAYER)
								{
									DoCast(target, SPELL_FLYING_KICK);
								}
							events.ScheduleEvent(EVENT_REMOVE_AURA, 4*IN_MILLISECONDS, 0, PHASE_3);
							events.CancelEvent(EVENT_FLYING_KICK);
							break;

						case EVENT_REMOVE_AURA:
							if(Aura* aura = me->GetAura(SPELL_PARRY_STANCE))
								me->RemoveAura(aura);
							events.ScheduleEvent(EVENT_CHASE_DOWN, 2*IN_MILLISECONDS, 0, PHASE_3);
							events.CancelEvent(EVENT_REMOVE_AURA);
							break;

						case EVENT_END:
							introTerminee = 0;
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
							if (instance)
							{
								instance->SetBossState(DATA_BOSS_MASTER_SNOWDRIFT, DONE);
							}
							me->GetMotionMaster()->MoveJump(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY(), spawnPos[2].GetPositionZ(), me->GetExactDist2d(spawnPos[2].GetPositionX(), spawnPos[2].GetPositionY()) * 10.0f / 1.0f, 1.0f); //voir pour le speedZ 1.0f
							me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
							events.CancelEvent(EVENT_END);
							break;

						default:
							break;
					}
				}
			}

			if (checkFragrantLotusDeath && checkFlyingSnowDeath && !checkIntro9)
			{
				Talk(SAY_INTRO_9);
				events.ScheduleEvent(EVENT_INTRO_10, 10*IN_MILLISECONDS);
				checkIntro9 = true;
			}

			if (me->HealthBelowPct(75) && events.IsInPhase(PHASE_1))
			{
				
				events.ScheduleEvent(EVENT_PHASE_2, 0, 0, PHASE_2);
				events.SetPhase(PHASE_2);
			}

			if (me->HealthBelowPct(3) && events.IsInPhase(PHASE_3))
			{
				events.ScheduleEvent(EVENT_END, 0, 0, PHASE_END);
				events.SetPhase(PHASE_END);
			}
			

			DoMeleeAttackIfReady();
		}
	};
};

class npc_master_trigger : public CreatureScript 
{
public:
	npc_master_trigger() : CreatureScript("npc_master_trigger") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_master_triggerAI(creature);
	}

	struct npc_master_triggerAI : public ScriptedAI
	{
		npc_master_triggerAI(Creature *creature) : ScriptedAI(creature)
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
			events.Update(diff);

			while(uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					if (instance)
					{
						case EVENT_INTRO_2:
							if (instance)
								if (Creature* master = me->GetCreature(*me, instance->GetData64(DATA_BOSS_MASTER_SNOWDRIFT)))
									if (master->AI())
									{
										master->AI()->DoAction(ACTION_INTRO_2);
										events.ScheduleEvent(EVENT_INTRO_3, 10*IN_MILLISECONDS);
										events.CancelEvent(EVENT_INTRO_2);
									}
							break;

							case EVENT_INTRO_3:
								if (instance)
									if (Creature* master = me->GetCreature(*me, instance->GetData64(DATA_BOSS_MASTER_SNOWDRIFT)))
										if (master->AI())
										{
											master->AI()->DoAction(ACTION_INTRO_3);
										}
								events.CancelEvent(EVENT_INTRO_3);							
							break;

						default:
							break;
					}
				}
			}

			if(checkTrigger)
			{
				Map::PlayerList const &playerList = me->GetMap()->GetPlayers();
				if (!playerList.isEmpty())
				{
					for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
					{
						if (me->GetExactDist2d(i->getSource()->GetPositionX(),i->getSource()->GetPositionY()) < 12) 
						{
							if (instance)
								if (Creature* master = me->GetCreature(*me, instance->GetData64(DATA_BOSS_MASTER_SNOWDRIFT)))
									if (master->AI())
									{
										master->AI()->DoAction(ACTION_INTRO_1);
										events.ScheduleEvent(EVENT_INTRO_2, 8*IN_MILLISECONDS);
										checkTrigger = false;
									}
						}
					}
				}
			}
		}
	};
};

class npc_shadopan_novice : public CreatureScript 
{
public:
	npc_shadopan_novice() : CreatureScript("npc_shadopan_novice") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_shadopan_noviceAI(creature);
	}

	struct npc_shadopan_noviceAI : public ScriptedAI
	{
		npc_shadopan_noviceAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;


		void Reset()
		{
		}

		void JustDied(Unit *pWho)
		{
			if (instance)
				if (Creature* master = me->GetCreature(*me, instance->GetData64(DATA_BOSS_MASTER_SNOWDRIFT)))
						if (master->AI())
						{
							master->AI()->DoAction(ACTION_SPAWN_SHADOPAN_NOVICE);
						}
		}

		void EnterCombat(Unit* /*who*/)
		{

		}

		void UpdateAI(uint32 diff)
		{	
			if(!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;
			
			DoMeleeAttackIfReady();
		}
	};
};

class npc_fragrant_lotus : public CreatureScript 
{
public:
	npc_fragrant_lotus() : CreatureScript("npc_fragrant_lotus") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_fragrant_lotusAI(creature);
	}

	struct npc_fragrant_lotusAI : public ScriptedAI
	{
		npc_fragrant_lotusAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;


		void Reset()
		{

		}

		void JustDied(Unit *pWho)
		{
			if (instance)
					if (Creature* master = me->GetCreature(*me, instance->GetData64(DATA_BOSS_MASTER_SNOWDRIFT)))
							if (master->AI())
							{
								master->AI()->DoAction(ACTION_FRAGRANT_LOTUS_DEATH);
							}
		}

		void EnterCombat(Unit* /*who*/)
		{

		}

		void UpdateAI(uint32 diff)
		{	
			if(!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;
			
			DoMeleeAttackIfReady();
		}
	};
};

class npc_flying_snow : public CreatureScript 
{
public:
	npc_flying_snow() : CreatureScript("npc_flying_snow") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_flying_snowAI(creature);
	}

	struct npc_flying_snowAI : public ScriptedAI
	{
		npc_flying_snowAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		EventMap events;


		void Reset()
		{

		}

		void JustDied(Unit *pWho)
		{
			if (instance)
					if (Creature* master = me->GetCreature(*me, instance->GetData64(DATA_BOSS_MASTER_SNOWDRIFT)))
							if (master->AI())
							{
								master->AI()->DoAction(ACTION_FLYING_SNOW_DEATH);
							}
		}

		void EnterCombat(Unit* /*who*/)
		{

		}

		void UpdateAI(uint32 diff)
		{	
			if(!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;
			
			DoMeleeAttackIfReady();
		}
	};
};

class npc_miroir_master_snowdrift : public CreatureScript 
{
public:
	npc_miroir_master_snowdrift() : CreatureScript("npc_miroir_master_snowdrift") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_miroir_master_snowdriftAI(creature);
	}

	struct npc_miroir_master_snowdriftAI : public ScriptedAI
	{
		npc_miroir_master_snowdriftAI(Creature *creature) : ScriptedAI(creature)
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
			events.ScheduleEvent(EVENT_NPC_BALL_OF_FIRE, 2*IN_MILLISECONDS);
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
						case EVENT_NPC_BALL_OF_FIRE:
							DoCast(me, SPELL_BALL_OF_FIRE);
							events.ScheduleEvent(EVENT_NPC_BALL_OF_FIRE, 2*IN_MILLISECONDS);
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


void AddSC_boss_master_snowdrift()
{
	new boss_master_snowdrift();
	new npc_master_trigger();
	new npc_shadopan_novice();
	new npc_fragrant_lotus();
	new npc_flying_snow();
	new npc_miroir_master_snowdrift();
}