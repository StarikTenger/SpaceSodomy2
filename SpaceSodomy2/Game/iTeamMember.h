#pragma once
#include "iId.h"

class iTeamMember : public iId {
private:
    int team_id = -1;
public:
    int get_team_id() const { return team_id; }
    void set_team_id(int _) { team_id = _; }

    bool is_hostile_to(iTeamMember& target) {
        return get_team_id() != target.get_team_id();
    };
};