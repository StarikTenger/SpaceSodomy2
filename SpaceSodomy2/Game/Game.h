#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include "entities.h"
#include "ProjectileManager.h"
#include "EventManager.h"
#include "IdManager.h"
#include "CollisionFilter.h"
#include "ContactTable.h"
#include "HullPrototype.h"
#include "BonusSlot.h"
#include "BonusManager.h"
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <memory>
#include "ModuleManager.h"
#include "GameObjects.h"
#include "RocketBrain.h"
#include "Forcefield.h"
#include "GameReadable.h"


class Game : public GameReadable {
protected:
	bool auto_damage = 0;
	b2Vec2 get_rand_respawn_pos();


	// Time step
	//float dt = 0.02;
	//float time = 0;

	// Objects' systems
	std::map<int, Player*> players;
	std::map<int, int>* connection_time;
	std::set<int> id_list;
	// std::set<Ship*> ships;
	//std::set<Engine*> engines;
	//std::set<CommandModule*> command_modules;
	
	// std::set<ActiveModule*> active_modules;
	// std::set<Projectile*> projectiles;
	//std::set<Counter*> counters;
	//std::set<DamageReceiver*> damage_receivers;
	//std::set<Event*> events;
	//std::set<Effects*> effects;
	//std::set<Bonus*> bonuses;
	//std::set<Rocket*> rockets;
	std::set<RocketBrain*> rocket_brains;
	//std::set<Forcefield*> forcefields;
	
	// Walls
	// std::set<Wall*> walls;
	b2Vec2 lower_left_corner, upper_right_corner;
	Player* wall_player = nullptr; // id = -1;

	// Managers
	ProjectileManager projectile_manager;
	EventManager event_manager;
	IdManager id_manager;
	BonusManager bonus_manager;
	ModuleManager module_manager;
	RocketManager rocket_manager;
	
	// b2World physics = b2World(b2Vec2_zero);

	// Collision control
	CollisionFilter collision_filter;
	// Contact table (stores pairs which are in contact)
	ContactTable contact_table;

	// Ship components
	std::map<std::string, Gun_Prototype> guns;
	std::map<unsigned char, std::string> gun_by_alias;
	std::map<std::string, HullPrototype> hulls;
	std::map<unsigned char, std::string> hull_by_alias;

	// Stores effect Algebraic types and strengths
	Effects_Prototype effect_params;
	// Misc
	std::map<std::string, float> params;

	// Path to the map
	std::string map_path = "";

	// Create functions
	Player*          create_player(int id, sf::Color color = {}, std::string name = "_");
	b2Body*          create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Gun*             create_gun(Gun_Prototype);
	CommandModule*  create_command_module();
	Engine*          create_engine(b2Body* = nullptr, CommandModule* = nullptr, Counter* = nullptr, Effects* = nullptr, float stamina_delay_ = 0.7);
	Counter*         create_counter(float val = 0, float change_vel = 0);
	DamageReceiver* create_damage_receiver(b2Body* = nullptr, Counter* = nullptr, Player* = nullptr, Effects* = nullptr);
	Ship*            create_ship(Player* player, b2Vec2 pos, float angle);
	Wall*            create_wall(std::vector<b2Vec2> vertices, int orientation = Wall::OUTER, float restitution = 0.5, int type = Wall::STANDART);
	Projectile*      create_projectile(ProjectileDef);
	Event*           create_event(EventDef, float playing_offset = 0);
	Effects*         create_effects(Effects_Prototype*);
	Bonus*           create_bonus(Bonus_Def);
	BonusSlot*      create_bonus_slot();
	Module*          create_module(Module_Prototype*);
	Rocket*          create_rocket(Rocket_Def);
	Forcefield*      create_forcefield(std::vector<b2Vec2> vertices, b2Vec2 force);

	// Delete functions
	void delete_body(b2Body*);
	void delete_wall(Wall*);
	void delete_projectile(Projectile*);
	void delete_engine(Engine*);
	void delete_active_module(ActiveModule*);
	void delete_command_module(CommandModule*);
	void delete_ship(Ship*);
	void delete_damage_receiver(DamageReceiver*);
	void delete_counter(Counter*);
	void delete_event(Event*);
	void delete_effects(Effects*);
	void delete_bonus(Bonus*);
	void delete_rocket(Rocket*);
	void delete_forcefield(Forcefield*);

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
	GameReadable& get_readable();

	Game();
	// Sets command to player with id=id
	void apply_command(int id, int command, int val);
	void step(float dt);
	// Get
	float get_dt();
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
protected:
	void create_new_player(int id, sf::Color color, std::string name, std::string gun_name, std::string hull_name,
		std::string left_module, std::string right_module);
public:
	bool new_player(PlayerDef);
	// Gets player by id
	Player* player_by_id(int id);
	// Deletes player
	void delete_player(int id);
	~Game();
};

