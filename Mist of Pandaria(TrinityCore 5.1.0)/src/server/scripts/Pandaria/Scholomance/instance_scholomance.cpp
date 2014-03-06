#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "scholomance.h"


class instance_scholomance : public InstanceMapScript
{
    public:
        instance_scholomance() : InstanceMapScript("instance_scholomance", 859) { }

        struct instance_scholomance_InstanceMapScript : public InstanceScript
        {
            instance_scholomance_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_scholomance_InstanceMapScript(map);
        }
};

void AddSC_instance_scholomance()
{
    new instance_scholomance();
}
