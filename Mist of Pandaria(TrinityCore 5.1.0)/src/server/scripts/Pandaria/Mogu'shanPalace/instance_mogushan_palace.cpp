#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "mogushan_palace.h"


class instance_mogushan_palace : public InstanceMapScript
{
    public:
        instance_mogushan_palace() : InstanceMapScript("instance_mogushan_palace", 859) { }

        struct instance_mogushan_palace_InstanceMapScript : public InstanceScript
        {
            instance_mogushan_palace_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
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
