#pragma once
#include "iId.h"

class iTeamMember : public iId {
private:
    int team_id = -1;
public:
    int get_team_id() const { return team_id; }
    
    // id = -1 is hostile to everyone 
    void set_team_id(int _ = -1) { team_id = _; }

    bool is_hostile_to(iTeamMember& target) {
        if (team_id == -1 || target.get_team_id() == -1) {
            return true;
        }
        return get_team_id() != target.get_team_id();
    };
};