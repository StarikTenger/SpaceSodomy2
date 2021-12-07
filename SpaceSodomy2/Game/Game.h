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
#include "Hull_Prototype.h"
#include "Bonus_Slot.h"
#include "Bonus_Manager.h"
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <memory>
#include "Module_Manager.h"
#include "Game_Objects.h"
#include "Rocket_Brain.h"
#include "Forcefield.h"
#include "Respawn_Manager.h"


class Game {
protected:
	bool auto_damage = 0;
	b2Vec2 get_rand_respawn_pos();


	// Time step
	float dt = 0;
	float time = 0;

	// Objects' systems
	Game_Objects game_objects;
	std::map<int, Player*> players;
	std::map<int, int>* connection_time;
	std::set<Ship*> ships;
	std::set<Engine*> engines;
	std::set<Command_Module*> command_modules;
	
	std::set<Active_Module*> active_modules;
	//std::set<Combatant*> combatants;
	std::set<Projectile*> projectiles;
	std::set<Counter*> counters;
	std::set<Damage_Receiver*> damage_receivers;
	std::set<Event*> events;
	std::set<Effects*> effects;
	std::set<Bonus*> bonuses;
	std::set<Rocket*> rockets;
	std::set<Rocket_Brain*> rocket_brains;
	std::set<Forcefield*> forcefields;
	
	// Walls
	Game_Map walls;
	Combatant* wall_combatant = nullptr;

	// Managers
	Projectile_Manager projectile_manager;
	Event_Manager event_manager;
	Id_Manager id_manager;
	Bonus_Manager bonus_manager;
	Module_Manager module_manager;
	Rocket_Manager rocket_manager;
	
	b2World physics = b2World(b2Vec2_zero);

	// Collision control
	Collision_Filter collision_filter;
	// Contact table (stores pairs which are in contact)
	Contact_Table contact_table;

	// Ship components
	std::map<std::string, Gun_Prototype> guns;
	std::map<unsigned char, std::string> gun_by_alias;
	std::map<std::string, Hull_Prototype> hulls;
	std::map<unsigned char, std::string> hull_by_alias;

	// Stores effect Algebraic types and strengths
	Effects_Prototype effect_params;
	// Misc
	std::map<std::string, float> params;

	// Path to the map
	std::string map_path = "";

	// Create functions
	Player*          create_player(int id, int team_id, sf::Color color = {}, std::string name = "_");
	//Combatant*       create_combatant(int id, int team_id,  sf::Color color = {}, std::string name = "_");
	b2Body*          create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Gun*             create_gun(Gun_Prototype);
	Command_Module*  create_command_module();
	Engine*          create_engine(b2Body* = nullptr, Command_Module* = nullptr, Counter* = nullptr, Effects* = nullptr);
	Counter*         create_counter(float val = 0, float change_vel = 0);
	Damage_Receiver* create_damage_receiver(b2Body* = nullptr, Counter* = nullptr, Combatant* = nullptr, Effects* = nullptr);
	Ship*            create_ship(Player* player, b2Vec2 pos, float angle);
	Wall*            create_wall(std::vector<b2Vec2> vertices, int orientation = Wall::OUTER, float restitution = 0.5, int type = Wall::STANDART);
	Projectile*      create_projectile(Projectile_Def);
	Event*           create_event(Event_Def, float playing_offset = 0);
	Effects*         create_effects(Effects_Prototype*);
	Bonus*           create_bonus(Bonus_Def);
	Bonus_Slot*      create_bonus_slot();
	Module*          create_module(Module_Prototype*);
	Rocket*          create_rocket(Rocket_Def);
	Rocket_Brain*    create_rocket_brain(Rocket_Prototype*);
	Forcefield*      create_forcefield(std::vector<b2Vec2> vertices, b2Vec2 force);

	// Delete functions
	void delete_body(b2Body*);
	void delete_wall(Wall*); // WARNING: Clear the map after calling to delete the wall from Game_Map
	void delete_projectile(Projectile*);
	void delete_engine(Engine*);
	void delete_active_module(Active_Module*);
	void delete_ship(Ship*);
	void delete_damage_receiver(Damage_Receiver*);
	void delete_counter(Counter*);
	void delete_event(Event*);
	void delete_effects(Effects*);
	void delete_bonus(Bonus*);
	void delete_rocket(Rocket*);
	void delete_rocket_brain(Rocket_Brain*);
	void delete_forcefield(Forcefield*);
	//void delete_combatant(Combatant*);

	 // Processing functions
	void process_players();
	void process_ships();
	void process_engines();
	void process_projectiles();
	void process_active_modules();
	void process_projectile_manager();
	void process_event_manager();
	void process_physics();
	void process_counters();
	void process_events();
	void process_effects();
	void process_bonuses();
	void process_bonus_manager();
	void process_rockets();
	void process_rocket_brains();
	void process_rocket_manager();
	void process_forcefields();

	// Misc
	// Calculates where beam intersects walls
	b2Vec2 get_beam_intersection(b2Vec2 start, float angle);

public:
	Game();
	// Sets command to player with id=id
	void apply_command(int id, int command, int val);
	void step(float dt);
	// Set
	void set_dt(float);
	void set_time(std::map<int, int>*);
	// Load funcions
	bool load_map(std::string path);
	bool load_parameters(std::string path);
	// Clears every temporary body
	void clear();
	// Clean all
	void wipe_map();
	// Encodes class into string
	std::string encode();
	// Creates new player
	void new_player(int id, int team_id, sf::Color color, std::string name, std::string gun_name, std::string hull_name,
		std::string left_module, std::string right_module);
	int count_players();
	// Gets player by id
	Player* player_by_id(int id);
	// Deletes player
	void delete_player(int id);
	~Game();
};

