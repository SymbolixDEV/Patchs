/* # Script de Tydrheal & Sungis : Temple of the Jade Serpent # */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "temple_of_the_jade_serpent.h"

/* Notes : What is missing ? : - Some visuals and texts
*/


class instance_temple_of_the_jade_serpent : public InstanceMapScript
{
	public:
		instance_temple_of_the_jade_serpent() : InstanceMapScript("instance_temple_of_the_jade_serpent", 960) { }

		struct instance_temple_of_the_jade_serpent_InstanceMapScript : public InstanceScript
		{
			instance_temple_of_the_jade_serpent_InstanceMapScript(Map* map) : InstanceScript(map)
			{
				BossWiseMariGUID			= 0;
				WiseMariGateGUID			= 0;
				BossLorewalkerStonestepGUID	= 0;
				NpcZaoSunseekerGUID			= 0;
				BossLiuFlameheartGUID		= 0;
				BossShaOfDoubtGUID			= 0;
			}

			void OnCreatureCreate(Creature* creature)
			{
				switch (creature->GetEntry())
				{
					case BOSS_WISE_MARI:
						BossWiseMariGUID = creature->GetGUID();
						break;

					case BOSS_LOREWALKER_STONESTEP:
						BossLorewalkerStonestepGUID = creature->GetGUID();
						break;

					case NPC_ZAO_SUNSEEKER:
						NpcZaoSunseekerGUID = creature->GetGUID();
						break;
						
					case BOSS_LIU_FLAMEHEART:
						BossLiuFlameheartGUID = creature->GetGUID();
						break;
						
					case BOSS_SHA_OF_DOUBT:
						BossShaOfDoubtGUID = creature->GetGUID();
						break;
		
					default:
						break;
				}

				switch (creature->GetGUID())
				{
					default:
						break;
				}
			}
			
			void OnGameObjectCreate(GameObject* go) 
            {
                switch (go->GetEntry())
                {
					case GO_MARI_LOREWALKER_GATE:
						WiseMariGateGUID = go->GetGUID();
						break;
				}
			}
			
			void OnGameObjectRemove(GameObject* go) 
            {
                switch (go->GetEntry())
                {
				}
			}

			uint64 GetData64(uint32 id) const 
			{
				switch (id)
				{
					case DATA_BOSS_WISE_MARI:
						return BossWiseMariGUID;
					
					case DATA_WISE_MARI_GATE:
						return WiseMariGateGUID;

					case DATA_BOSS_LOREWALKER_STONESTEP:
						return BossLorewalkerStonestepGUID;

					case DATA_NPC_ZAO_SUNSEEKER:
						return NpcZaoSunseekerGUID;
					
					case DATA_BOSS_LIU_FLAMEHEART:
						return BossLiuFlameheartGUID;
						
					case DATA_BOSS_SHA_OF_DOUBT:
						return BossShaOfDoubtGUID;
						
					default:
						break;
				}

				return 0;
			}

			protected:
				uint64 BossWiseMariGUID;
				uint64 WiseMariGateGUID;
				uint64 BossLorewalkerStonestepGUID;
				uint64 NpcZaoSunseekerGUID;
				uint64 BossLiuFlameheartGUID;
				uint64 BossShaOfDoubtGUID;

		};

		InstanceScript* GetInstanceScript(InstanceMap* map) const 
		{
			return new instance_temple_of_the_jade_serpent_InstanceMapScript(map);
		}
};

/*class npc_instance_trigger_totjs: public CreatureScript
{
public:
	npc_instance_trigger_totjs() : CreatureScript("npc_instance_trigger_totjs") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_instance_trigger_totjsAI(creature);
	}

	struct npc_instance_trigger_totjsAI : public ScriptedAI
	{
		npc_instance_trigger_totjsAI(Creature *creature) : ScriptedAI(creature)
		{
			instance = creature->GetInstanceScript();
		}

		InstanceScript* instance;
		bool wiseMariAlive;
		bool lorewalkerAlive;
		bool middleDoor;
		bool liuAlive;
		bool wiseMariFirstDoor;
		bool lorewalkerFirstDoor;
		
		void Reset()
		{
			wiseMariFirstDoor = false;
			lorewalkerFirstDoor = false;
		}

		void UpdateAI(uint32 diff)
		{
			if (me->FindNearestCreature(BOSS_WISE_MARI, 999.0f, true))
				wiseMariAlive = true;

			if (me->FindNearestCreature(BOSS_WISE_MARI, 999.0f, false) && wiseMariAlive)
			{
				instance->DoUseDoorOrButton(428968, 999999);
				wiseMariAlive = false;
			}

			if (me->FindNearestCreature(BOSS_LOREWALKER_STONESTEP, 999.0f, true))
				lorewalkerAlive = true;

			if (me->FindNearestCreature(BOSS_LOREWALKER_STONESTEP, 999.0f, false) && lorewalkerAlive)
			{
				instance->DoUseDoorOrButton(433313, 999999);
				lorewalkerAlive = false;
			}

			if (me->FindNearestCreature(BOSS_LIU_FLAMEHEART, 999.0f, true))
				liuAlive = true;

			if (me->FindNearestCreature(BOSS_LIU_FLAMEHEART, 999.0f, false) && liuAlive)
			{
				instance->DoUseDoorOrButton(428964, 999999);
				liuAlive = false;
			}

			if (me->FindNearestCreature(BOSS_WISE_MARI, 999.0f, false) &&
				me->FindNearestCreature(BOSS_LOREWALKER_STONESTEP, 999.0f, false) &&
				me->FindNearestCreature(BOSS_LIU_FLAMEHEART, 999.0f, false))
				instance->DoUseDoorOrButton(433536, 999999);

			if (!wiseMariFirstDoor)
				instance->DoUseDoorOrButton(433323, 999999);

			if (!lorewalkerFirstDoor)
				instance->DoUseDoorOrButton(433325, 999999);
		}
	};
};*/

void AddSC_instance_temple_of_the_jade_serpent()
{
   new instance_temple_of_the_jade_serpent();
   //new npc_instance_trigger_totjs();
}

