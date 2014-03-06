/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MASTERY_MGR_H
#define _MASTERY_MGR_H

#include <ace/Singleton.h>

#include "Player.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCStructure.h"

struct Mastery {
	float base;
	float ratio;

    float getPercent(uint32 amount) { return base + ( (ratio == 0.0f) ? 0.0f : (float(amount) / ratio) ); };
};

class MasteryMgr
{
	friend class ACE_Singleton<MasteryMgr, ACE_Null_Mutex>; 

public:

	void LoadFromDB();
	Mastery getMastery(TalentTree spec);


private:
	MasteryMgr() {};
	~MasteryMgr() {};

	std::map<TalentTree, Mastery> masteriesMap;

};

#define sMasteryMgr ACE_Singleton<MasteryMgr, ACE_Null_Mutex>::instance()

#endif