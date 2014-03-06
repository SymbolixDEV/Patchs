#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "siege_of_niuzao_temple.h"


class instance_siege_of_niuzao_temple : public InstanceMapScript
{
    public:
        instance_siege_of_niuzao_temple() : InstanceMapScript("instance_siege_of_niuzao_temple", 859) { }

        struct instance_siege_of_niuzao_temple_InstanceMapScript : public InstanceScript
        {
            instance_siege_of_niuzao_temple_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_siege_of_niuzao_temple_InstanceMapScript(map);
        }
};

void AddSC_instance_siege_of_niuzao_temple()
{
    new instance_siege_of_niuzao_temple();
}
