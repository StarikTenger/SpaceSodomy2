#pragma once
#include "Respawn_Manager.h"
#include <AuxLib/AuxLib.h>

// TODO: this class has two responsibilities
struct Respawn_Def {
    int algo = Respawn_Manager::RANDOM;
    b2Vec2 vec;
    float time_to_respawn = 3.0f;
    float delay = 0.f;
};

class Team_Assigner {
public:
    static int get_team_id(std::string team_name);
    struct Team_Assignment {
        bool was_accepted;       // Team Assigner can refuse to accept a player
        int team_id;             // Player was assgined to this team
        sf::Color player_color;  // Color to set
    };
    // Interfaces Game with Server
    // Takes data coming from the player and makes it Game-formatted
    virtual Team_Assignment assign_team(int player_id, std::string team_name) = 0;
    // For player respawn initialization
    virtual Respawn_Def get_respawn_def(int team_id) = 0;
};


// Assign team = current id;
class FFA_Team_Assigner : public Team_Assigner {
public:
    FFA_Team_Assigner(int max_number_of_players, Respawn_Def def);
    Team_Assignment assign_team(int player_id, std::string team_name) override;
    Respawn_Def get_respawn_def(int team_id) override;
private:
    int number_of_players = 0;
    int max_number_of_players = 10;
    Respawn_Def def;
};
// TODO: rewrite this
class Two_Team_Assigner : public Team_Assigner {
public:
    Two_Team_Assigner(int first_team_max_size, 
        int second_team_max_size, 
        Respawn_Def first,
        Respawn_Def second,
        sf::Color first_color = sf::Color::Red,
        sf::Color second_color = sf::Color::Blue,
        std::string first_team_name = "red",
        std::string second_team_name = "blue");
    Team_Assignment assign_team(int player_id, std::string team_name) override;
    Respawn_Def get_respawn_def(int team_id) override;
private:
    int first_team_size = 0;
    int first_team_max_size = 0;
    sf::Color first_base_color = sf::Color::Red;
    std::string first_team_name = "red";
    Respawn_Def first_default_respawn;

    int second_team_size = 0;
    int second_team_max_size = 0;
    sf::Color second_base_color = sf::Color::Blue;
    std::string second_team_name = "blue";
    Respawn_Def second_default_respawn;

};
