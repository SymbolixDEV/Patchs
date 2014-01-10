#include "ScriptPCH.h"
 
class PVP_Killstreak : public PlayerScript
{
        public:
        PVP_Killstreak() : PlayerScript("PVP_Killstreak"){}
};
 
void AddSC_PVP_Killstreak()
{
    new PVP_Killstreak();
}