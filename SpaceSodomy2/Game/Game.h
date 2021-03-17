#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include "entities.h"
#include "Projectile_Manager.h"
#include "Id_Manager.h"
#include "Collision_Filter.h"
#include "Contact_Table.h"
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <memory>

class Game {
protected:
	// Collision control
	Collision_Filter collision_filter;

	// Time step
	float dt = 0;

	// Objects' systems
	std::map<int, Player*> players;
	std::set<Ship*> ships;
	std::set<Engine*> engines;
	std::set<Command_Module*> command_modules;
	std::set<Wall*> walls;
	std::set<Active_Module*> active_modules;
	std::set<Projectile*> projectiles;
	std::set<Counter*> counters;
	std::set<Damage_Receiver*> damage_receivers;
	std::set<Sound*> sounds;

	Projectile_Manager projectile_manager;
	Id_Manager id_manager;
	b2World physics = b2World(b2Vec2_zero);

	// Contact table (stores pairs which are in contact)
	Contact_Table contact_table;

	// Path to the map
	std::string map_path = "";

	// Create functions
	Player* create_player(int id, sf::Color color = {}, std::string name = "_");
	b2Body* create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Gun* create_gun();
	Command_Module* create_command_module();
	Engine* create_engine(b2Body* = nullptr, Command_Module* = nullptr);
	Counter* create_counter(float val = 0, float change_vel = 0);
	Damage_Receiver* create_damage_receiver(b2Body* = nullptr, Counter* = nullptr);
	Ship* create_ship(Player* player, b2Vec2 pos, float angle);
	Wall* create_wall(std::vector<b2Vec2> vertices, int orientation = Wall::OUTER, float restitution = 0.5);
	Projectile* create_projectile(Projectile_Def);
	Sound* create_sound(std::string name = "_", b2Body* body = nullptr, float playing_offset = 0);

	// Delete functions
	void delete_body(b2Body*);
	void delete_projectile(Projectile*);
	void delete_engine(Engine*);
	void delete_active_module(Active_Module*);
	void delete_ship(Ship*);
	void delete_damage_receiver(Damage_Receiver*);
	void delete_counter(Counter*);
	void delete_sound(Sound*);

	 // Processing functions
	void process_engines();
	void process_projectiles();
	void process_active_modules();
	void process_projectlie_manager();
	void process_physics();
	void process_counters();
	void process_sounds();

	// Misc
	//void deal_damage(); TODO: damage receiver
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
	// Creates new player
	Ship* new_player(int id, sf::Color color, std::string name, b2Vec2 pos, float angle);
	// Deletes player
	void delete_player(int id);
	// Get hp
	Ship* get_ship(int id);
};

