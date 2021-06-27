#pragma once
#include <Game/Game.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include <Menu/Bar.h>
#include <Menu/Constant_Text.h>
#include <AuxLib/AuxLib.h>

class Game_Client : public Game {
private:
	std::map<int, std::string> bonus_textures = {
		{Bonus::COUNT, "bonusEmpty"},
		{Bonus::INSTANT_HP, "bonusHp"},
		{Bonus::INSTANT_ENERGY, "bonusEnergy"},
		{Bonus::BERSERK, "bonusBerserk"},
		{Bonus::IMMORTALITY, "bonusImmortal"},
		{Bonus::CHARGE, "bonusCharge"},
		{Bonus::LASER, "bonusLaser"}
	};
	Draw* draw = nullptr;
	Audio* audio = nullptr;
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "IMMORTALITY";
	std::string right_module_name = "DASH";
	int my_id = 0;
	int engine_active = 0;
	int aim_conf = 1;
	int aim_opacity = 150;
	bool network_information_active = false;

	// Some drawing parameters
	// box for drawing the global wall texture;
	std::string global_wall_name = "";
	b2Vec2 origin_pos = { 0,0 };
	b2Vec2 end_pos = { 0,0 };
	// Wall thickness
	float outer_wall_thickness = 0.05;

	std::vector <int> engine_commands = {
				Command_Module::ENGINE_ANG_LEFT,
				Command_Module::ENGINE_ANG_RIGHT,
				Command_Module::ENGINE_LIN_BACKWARD,
				Command_Module::ENGINE_LIN_FORWARD,
				Command_Module::ENGINE_LIN_LEFT,
				Command_Module::ENGINE_LIN_RIGHT
	};

public:
	enum Animation_Layers {
		GAME,
		HUD
	};

	// Get methods
	Draw* get_draw();
	Audio* get_audio();

	std::string get_gun_name();
	std::string get_hull_name();
	std::string get_left_module_name();
	std::string get_right_module_name();

	std::string get_bonus_texture_name(int val);

	int get_aim_conf();
	int get_aim_opacity();

	// Set methods
	void set_gun_name(std::string val);
	void set_hull_name(std::string val);
	void set_left_module_name(std::string val);
	void set_right_module_name(std::string val);

	void set_draw(Draw* _draw);
	void set_audio(Audio* _audio);

	void set_aim_conf(int _conf);
	void set_aim_opacity(int _opacity);
	void set_network_information_active(bool _active);

	void display(int id);

	// Decodes class from string
	void update_state(std::string source);

	// Get ship
	Ship* get_ship(int id);

	// Get players
	std::map<int, Player*>* get_players();

	// Get player id by name
	int get_player_id(std::string name);

	// Get network information active
	bool get_network_information_active();

	// Load setup
	void load_setup(std::string path);
	void save_setup(std::string path);

	sf::Texture* make_polygonal_texture(Wall* wall, sf::Vector2f scale, 
		std::string base_texture, float wall_width, sf::Color overlay_color, std::set<Wall*>& walls);
	// load wall textures
	void load_wall_textures(sf::Color overlay_color = sf::Color(0, 151, 255), 
		                    sf::Vector2f scale = {100, 100}, 
		                    float wall_width = 1.0,
		                    std::string wall_name = "wall");
};