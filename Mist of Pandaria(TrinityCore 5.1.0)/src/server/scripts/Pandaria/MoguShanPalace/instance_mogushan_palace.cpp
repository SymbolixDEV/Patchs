/* # Script de Sungis : Mogu'shan Palace # */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "mogushan_palace.h"

/* Notes : What is missing ? : - ...
*/

class instance_mogushan_palace : public InstanceMapScript
{
	public:
		instance_mogushan_palace() : InstanceMapScript("instance_mogushan_palace", 994) { }

		struct instance_mogushan_palace_InstanceMapScript : public InstanceScript
		{
			instance_mogushan_palace_InstanceMapScript(Map* map) : InstanceScript(map)
			{
				NpcXinTheWeaponmasterGUID	= 0;
				MuShibaGUID					= 0;
				KuaiTheBruteGUID			= 0;
				MingTheCunningGUID			= 0;
				HaiyanTheUnstoppableGUID	= 0;
				GekkanGUID					= 0;
				GlintrokOracleGUID			= 0;
				GlintrokHexxerGUID			= 0;
				GlintrokSkulkerGUID			= 0;
				GlintrokIronhideGUID		= 0;
				BossXinTheWeaponmasterGUID	= 0;
			}

			void OnCreatureCreate(Creature* creature)
			{
				switch (creature->GetEntry())
				{
					case BOSS_MUSHIBA:
						MuShibaGUID = creature->GetGUID();
						break;
					case NPC_XIN_THE_WEAPONMASTER:
						NpcXinTheWeaponmasterGUID = creature->GetGUID();
						break;
					case BOSS_KUAI_THE_BRUTE:
						KuaiTheBruteGUID = creature->GetGUID();
						break;
					case BOSS_MING_THE_CUNNING:
						MingTheCunningGUID = creature->GetGUID();
						break;
					case BOSS_HAIYAN_THE_UNSTOPPABLE:
						HaiyanTheUnstoppableGUID = creature->GetGUID();
						break;
					case BOSS_GEKKAN:
						GekkanGUID = creature->GetGUID();
						break;
					case NPC_GLINTROK_ORACLE:
						GlintrokOracleGUID = creature->GetGUID();
						break;
					case NPC_GLINTROK_HEXXER:
						GlintrokHexxerGUID = creature->GetGUID();
						break;
					case NPC_GLINTROK_SKULKER:
						GlintrokSkulkerGUID = creature->GetGUID();
						break;
					case NPC_GLINTROK_IRONHIDE:
						GlintrokIronhideGUID = creature->GetGUID();
						break;
					case BOSS_XIN_THE_WEAPONMASTER:
						BossXinTheWeaponmasterGUID = creature->GetGUID();
						break;
					default:
						break;
				}
			}

			uint64 GetData64(uint32 id) const
			{
				switch (id)
				{
					case DATA_NPC_XIN_THE_WEAPONMASTER:
						return NpcXinTheWeaponmasterGUID;
					case DATA_KUAI_THE_BRUTE:
						return KuaiTheBruteGUID;
					case DATA_MUSHIBA:
						return MuShibaGUID;
					case DATA_MING_THE_CUNNING:
						return MingTheCunningGUID;
					case DATA_HAIYAN_THE_UNSTOPPABLE:
						return HaiyanTheUnstoppableGUID;
					case DATA_GEKKAN:
						return GekkanGUID;
					case DATA_GLINTROK_ORACLE:
						return GlintrokOracleGUID;
					case DATA_GLINTROK_HEXXER:
						return GlintrokHexxerGUID;
					case DATA_GLINTROK_SKULKER:
						return GlintrokSkulkerGUID;
					case DATA_GLINTROK_IRONHIDE:
						return GlintrokIronhideGUID;
					case DATA_BOSS_XIN_THE_WEAPONMASTER:
						return BossXinTheWeaponmasterGUID;
					default:
						break;
				}

				return 0;
			}

			protected:
				uint64 NpcXinTheWeaponmasterGUID;
				uint64 KuaiTheBruteGUID;
				uint64 MuShibaGUID;
				uint64 MingTheCunningGUID;
				uint64 HaiyanTheUnstoppableGUID;
				uint64 GekkanGUID;
				uint64 GlintrokOracleGUID;
				uint64 GlintrokHexxerGUID;
				uint64 GlintrokSkulkerGUID;
				uint64 GlintrokIronhideGUID;
				uint64 BossXinTheWeaponmasterGUID;
		};

		InstanceScript* GetInstanceScript(InstanceMap* map) const
		{
			return new instance_mogushan_palace_InstanceMapScript(map);
		}
};


void AddSC_instance_mogushan_palace()
{
   new instance_mogushan_palace();
}