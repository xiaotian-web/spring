/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef	_GROUP_HANDLER_H
#define	_GROUP_HANDLER_H

#include "System/creg/creg_cond.h"
#include "System/UnorderedMap.hpp"

#include <string>
#include <vector>

class CGroup;

/**
 * Handles all groups of a single team.
 * The default groups (aka hot-key groups) are the ones with
 * single digit IDs (1 - 9).
 * Groups with higher IDs are considered special.
 * Manual group creation/selection only works for the default groups.
 */
class CGroupHandler {
	CR_DECLARE_STRUCT(CGroupHandler)
public:
	CGroupHandler(int teamId);
	~CGroupHandler();

	/// lowest ID of the first group not reachable through a hot-key
	static constexpr size_t FIRST_SPECIAL_GROUP = 10;

	void Update();

	bool GroupCommand(int num);
	bool GroupCommand(int num, const std::string& cmd);

	CGroup* CreateNewGroup();
	CGroup* GetUnitGroup(int unitID) const {
		const auto iter = unitGroups.find(unitID);

		if (iter == unitGroups.end())
			return nullptr;

		return (iter->second);
	}

	bool SetUnitGroup(int unitID, CGroup* g) {
		unitGroups.erase(unitID);

		if (g == nullptr)
			return false;

		unitGroups.insert(unitID, g);
		return true;
	}

	void RemoveGroup(CGroup* group);

	void PushGroupChange(int id);

	int GetTeam() const { return team; }

public:
	std::vector<CGroup*> groups;

protected:
	std::vector<int> freeGroups;
	std::vector<int> changedGroups;

	spring::unsynced_map<int, CGroup*> unitGroups;

	int team = 0;
	/**
	 * The lowest ID not in use.
	 * This is always greater or equal FIRST_SPECIAL_GROUP.
	 */
	int firstUnusedGroup = FIRST_SPECIAL_GROUP;

};

extern std::vector<CGroupHandler> uiGroupHandlers;

#endif	// _GROUP_HANDLER_H
