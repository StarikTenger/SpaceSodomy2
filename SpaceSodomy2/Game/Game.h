#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include "entities.h"
#include "Projectile_Manager.h"
#include "Event_Manager.h"
#include "Id_Manager.h"
#include "Collision_Filter.h"
#include "Contact_Table.h"
#include "Gun_Def.h"
#include "Hull_Def.h"
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <memory>

class Game {
protected:
	bool auto_damage = 0;
	b2Vec2 get_rand_respawn_pos();

	// Collision control
	Collision_Filter collision_filter;

	// Time step
	float dt = 0;

	// Objects' systems
	std::map<int, Player*> players;
	std::set<Ship*> ships;
	std::set<Engine*> engines;
	std::set<Command_Module*> command_modules;
	
	std::set<Active_Module*> active_modules;
	std::set<Projectile*> projectiles;
	std::set<Counter*> counters;
	std::set<Damage_Receiver*> damage_receivers;
	std::set<Sound*> sounds;
	std::set<Effects*> effects;
	
	// Walls
	std::set<Wall*> walls;
	b2Vec2 lower_left_corner, upper_right_corner;

	// Managers
	Projectile_Manager projectile_manager;
	Event_Manager sound_manager;
	Id_Manager id_manager;
	b2World physics = b2World(b2Vec2_zero);

	// Ship components
	std::map<std::string, Gun_Def> guns;
	std::map<std::string, Hull_Def> hulls;
	Effects_Def default_effects;
	Effects* null_effects;
	// int is Effect_Type;
	std::map<int, Effects::Algebraic_Type> types;
	// Contact table (stores pairs which are in contact)
	Contact_Table contact_table;

	// Path to the map
	std::string map_path = "";

	// Create functions
	Player*          create_player(int id, sf::Color color = {}, std::string name = "_");
	b2Body*          create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Gun*             create_gun(Gun_Def);
	Command_Module*  create_command_module();
	Engine*          create_engine(b2Body* = nullptr, Command_Module* = nullptr, Counter* = nullptr, Effects* = nullptr);
	Counter*         create_counter(float val = 0, float change_vel = 0);
	Damage_Receiver* create_damage_receiver(b2Body* = nullptr, Counter* = nullptr, Player* = nullptr, Effects* = nullptr);
	Ship*            create_ship(Player* player, b2Vec2 pos, float angle);
	Wall*            create_wall(std::vector<b2Vec2> vertices, int orientation = Wall::OUTER, float restitution = 0.5);
	Projectile*      create_projectile(Projectile_Def);
	Sound*           create_event(std::string name = "_", b2Body* body = nullptr, float playing_offset = 0);
	Effects*         create_effects(Effects_Def);

	// Delete functions
	void delete_body(b2Body*);
	void delete_projectile(Projectile*);
	void delete_engine(Engine*);
	void delete_active_module(Active_Module*);
	void delete_ship(Ship*);
	void delete_damage_receiver(Damage_Receiver*);
	void delete_counter(Counter*);
	void delete_sound(Sound*);
	void delete_effects(Effects*);

	 // Processing functions
	void process_players();
	void process_ships();
	void process_engines();
	void process_projectiles();
	void process_active_modules();
	void process_projectlie_manager();
	void process_sound_manager();
	void process_physics();
	void process_counters();
	void process_sounds();
	void process_effects();

public:
	Game();
	// Sets command to player with id=id
	void apply_command(int id, int command, int val);
	void step(float dt);
	// Set
	void set_dt(float);
	// Load funcions
	bool load_map(std::string path);
	bool load_parameters(std::string path);
	// Clears everything
	void clear();
	// Encodes class into string
	std::string encode();
	// Creates new player
	void new_player(int id, sf::Color color, std::string name, std::string gun_name, std::string hull_name);
	// Gets player by id
	Player* player_by_id(int id);
	// Deletes player
	void delete_player(int id);
};

