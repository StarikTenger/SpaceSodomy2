#pragma once
#include "iId.h"
#include "iTeamMember.h"
#include <string>
#include <array>
#include <optional>
#include <SFML/Graphics.hpp> 
#include <AuxLib/AuxLib.h>
#include <set>


class TeamManager {
protected:
    static const std::array<int, 12> max_distance_team_colors;
    int players = 0;
    std::set<int> player_ids;
    int max_players = 12;
public:
    struct TeamAssignment {
        int team_id;
        sf::Color team_color;
    };
    std::optional<TeamAssignment> assign_team(int id, const std::string& team_name_hint) {
        if (players >= max_players || player_ids.count(id) > 0) {
            return std::nullopt;
        }
        player_ids.insert(id);
        players++;
        return assign_team_impl(id, team_name_hint);
    }
    TeamManager(int max_players_ = 12) : max_players(max_players_) {};
protected:
    virtual std::optional<TeamAssignment> assign_team_impl(int id, const std::string& team_name_hint) = 0;
};

class FFATeamManager : public TeamManager {
protected:
    int color_shift;
public:
    FFATeamManager(int max_players_) : TeamManager(max_players_), color_shift(aux::random_int(0, 360)) {};
    std::optional<TeamAssignment> assign_team_impl(int id, const std::string& team_name_hint) final {
        TeamAssignment res;
        res.team_id = id;
        if (players >= 12) {
            res.team_color = aux::from_hsv(aux::random_int(0, 360), 1, 1);
        }
        else {
            res.team_color = aux::from_hsv((max_distance_team_colors[players - 1]) % 360, 1, 1);
        }
        return res;
    };
};