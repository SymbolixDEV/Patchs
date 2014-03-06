#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "scarlet_halls.h"


class instance_scarlet_halls : public InstanceMapScript
{
    public:
        instance_scarlet_halls() : InstanceMapScript("instance_scarlet_halls", 859) { }

        struct instance_scarlet_halls_InstanceMapScript : public InstanceScript
        {
            instance_scarlet_halls_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_scarlet_halls_InstanceMapScript(map);
        }
};

void AddSC_instance_scarlet_halls()
{
    new instance_scarlet_halls();
}
