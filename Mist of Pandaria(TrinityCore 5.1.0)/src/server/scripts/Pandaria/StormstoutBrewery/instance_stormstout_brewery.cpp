#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "stormstout_brewery.h"


class instance_stormstout_brewery : public InstanceMapScript
{
    public:
        instance_stormstout_brewery() : InstanceMapScript("instance_stormstout_brewery", 859) { }

        struct instance_stormstout_brewery_InstanceMapScript : public InstanceScript
        {
            instance_stormstout_brewery_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_stormstout_brewery_InstanceMapScript(map);
        }
};

void AddSC_instance_stormstout_brewery()
{
    new instance_stormstout_brewery();
}
