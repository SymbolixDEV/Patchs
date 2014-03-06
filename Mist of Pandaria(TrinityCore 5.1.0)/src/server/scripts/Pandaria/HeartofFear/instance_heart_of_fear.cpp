#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "heart_of_fear.h"


class instance_heart_of_fear : public InstanceMapScript
{
    public:
        instance_heart_of_fear() : InstanceMapScript("instance_heart_of_fear", 859) { }

        struct instance_heart_of_fear_InstanceMapScript : public InstanceScript
        {
            instance_heart_of_fear_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_heart_of_fear_InstanceMapScript(map);
        }
};

void AddSC_instance_heart_of_fear()
{
    new instance_heart_of_fear();
}
