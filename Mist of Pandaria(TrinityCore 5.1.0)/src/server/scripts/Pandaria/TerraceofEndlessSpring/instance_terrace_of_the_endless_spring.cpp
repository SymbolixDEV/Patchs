#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "terrace_of_the_endless_spring.h"


class instance_terrace_of_the_endless_spring : public InstanceMapScript
{
    public:
        instance_terrace_of_the_endless_spring() : InstanceMapScript("instance_terrace_of_the_endless_spring", 859) { }

        struct instance_terrace_of_the_endless_spring_InstanceMapScript : public InstanceScript
        {
            instance_terrace_of_the_endless_spring_InstanceMapScript(Map* map) : InstanceScript(map)
			{}
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_terrace_of_the_endless_spring_InstanceMapScript(map);
        }
};

void AddSC_instance_terrace_of_the_endless_spring()
{
    new instance_terrace_of_the_endless_spring();
}
