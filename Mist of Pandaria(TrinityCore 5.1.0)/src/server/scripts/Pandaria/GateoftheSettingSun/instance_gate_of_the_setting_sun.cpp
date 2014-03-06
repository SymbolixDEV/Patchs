#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "gate_of_the_setting_sun.h"


class instance_gate_of_the_setting_sun : public InstanceMapScript
{
    public:
        instance_gate_of_the_setting_sun() : InstanceMapScript("instance_gate_of_the_setting_sun", 859) { }

        struct instance_gate_of_the_setting_sun_InstanceMapScript : public InstanceScript
        {
            instance_gate_of_the_setting_sun_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_gate_of_the_setting_sun_InstanceMapScript(map);
        }
};

void AddSC_instance_gate_of_the_setting_sun()
{
    new instance_gate_of_the_setting_sun();
}
