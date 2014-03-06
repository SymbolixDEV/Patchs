/* # Script de Sungis : Mogu'shan Vaults # */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "mogushan_vaults.h"


class instance_mogushan_vaults : public InstanceMapScript
{
    public:
        instance_mogushan_vaults() : InstanceMapScript("instance_mogushan_vaults", 1008) { }

        struct instance_mogushan_vaults_InstanceMapScript : public InstanceScript
        {
            instance_mogushan_vaults_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_mogushan_vaults_InstanceMapScript(map);
        }
};

void AddSC_instance_mogushan_vaults()
{
    new instance_mogushan_vaults();
}
