#include "pch.h"
#include "Team_Assigner.h"

int Team_Assigner::get_team_id(std::string name) { // TODO: fix team name collision
    std::hash<std::string> hash;
    auto val = hash(name);
    val %= 1'000'000'000;
    return static_cast<int>(val);
}

Team_Assigner::Team_Assignment FFA_Team_Assigner::assign_team(int player_id, std::string team_name) {
    return Team_Assignment({true, player_id, aux::gen_new_player_color(number_of_players++)});
}

FFA_Team_Assigner::FFA_Team_Assigner(int _val, Respawn_Def _def) : number_of_players(0), max_number_of_players(_val), def(_def) {
}

Respawn_Def FFA_Team_Assigner::get_respawn_def(int team_id) {
    return def;
}

Two_Team_Assigner::Two_Team_Assigner(int _first_team_max_size, int _second_team_max_size, Respawn_Def first, Respawn_Def second,
    sf::Color _first_color, sf::Color _second_color, std::string first_name, std::string second_name) 
    : 
    first_team_size(0), first_team_max_size(_first_team_max_size), first_base_color(_first_color), first_team_name(first_name), first_default_respawn(first),
    second_team_size(0), second_team_max_size(_second_team_max_size), second_base_color(_second_color), second_team_name(second_name), second_default_respawn(second) {
}

Two_Team_Assigner::Team_Assignment Two_Team_Assigner::assign_team(int player_id, std::string team_name) {
    Team_Assignment res;
    res.was_accepted = false;
    if (team_name == first_team_name) {
        if (first_team_size >= first_team_max_size) {
            return res;
        }
        sf::Color temp = aux::gen_new_player_color(first_team_size);
        temp *= sf::Color(50, 50, 50);
        res.player_color = first_base_color + temp;
        res.team_id = Team_Assigner::get_team_id(first_team_name);
        res.was_accepted = true;
        return res;
    }
    if (team_name == second_team_name) {
        if (second_team_size >= second_team_max_size) {
            return res;
        }
        sf::Color temp = aux::gen_new_player_color(second_team_size);
        temp *= sf::Color(50, 50, 50);
        res.player_color = second_base_color + temp;
        res.team_id = Team_Assigner::get_team_id(second_team_name);
        res.was_accepted = true;
        return res;
    }
    return res;
}

Respawn_Def Two_Team_Assigner::get_respawn_def(int team_id) {
    if (team_id == get_team_id(second_team_name)) {
        return second_default_respawn;
    }
    else {
        return first_default_respawn;
    }
}

