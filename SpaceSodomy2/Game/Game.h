#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "Ship.h"
#include "Engine.h"
#include "Player.h"
#include "Wall.h"
#include "Gun.h"
#include "Projectile_Manager.h"
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>

class Game {
protected:
	//friend class Projectile_Manager;

	// Objects' systems
	std::vector<Ship*> ships;
	std::vector<Player*> players;
	std::vector<Engine*> engines;
	std::map<int, Command_Module*> command_modules;
	std::vector<Wall*> walls;
	Projectile_Manager projectile_manager;
	b2World physics = b2World(b2Vec2_zero);

	// Path to the map
	std::string map_path = "";

	b2Body* create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Ship* create_ship(Player* player, b2Vec2 pos, float angle);
	Wall* create_wall(std::vector<b2Vec2> vertices, int orientation = Wall::OUTER, float restitution = 0.5);

	 // Processing functions
	void process_engines();
public:
	Game();
	// Sets command to player with id=id
	void apply_command(int id, int command, int val);
	void step(float dt);
	// Loads walls from file (returns 1 if everything is correct)
	int load_map(std::string path);
	// Clears everyrhing
	void clear();
	// Encodes class into string
	std::string encode();
	// Decodes class from string
	void decode(std::string source);
	// Creates new player
	Ship* create_player(int id, sf::Color color, std::string name, b2Vec2 pos, float angle);
	//Deletes player
	void del_player(int id);
};

