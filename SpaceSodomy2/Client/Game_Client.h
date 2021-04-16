#pragma once
#include <Game/Game.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include <Menu/Bar.h>
#include <Menu/Constant_Text.h>
#include <AuxLib/AuxLib.h>

class Game_Client : public Game {
private:
	enum Animation_Layers {
		GAME,
		HUD
	};
	std::map<int, std::string> bonus_textures = {
		{Bonus::COUNT, "bonusEmpty"},
		{Bonus::INSTANT_HP, "bonusHp"},
		{Bonus::INSTANT_STAMINA, "bonusEnergy"},
		{Bonus::BERSERK, "bonusBerserk"},
		{Bonus::IMMORTALITY, "bonusImmortal"},
		{Bonus::CHARGE, "bonusCharge"},
		{Bonus::LASER, "bonusLaser"}
	};
	Draw* draw = nullptr;
	Audio* audio = nullptr;
	std::string gun_name = "default";
	std::string hull_name = "default";
	int engine_active = 0;

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
	// Set methods
	void set_draw(Draw* _draw);
	void set_audio(Audio* _audio);
	
	// Get methods
	Draw* get_draw();
	Audio* get_audio();
	std::string get_gun_name();
	std::string get_hull_name();
	std::string get_bonus_texture_name(int val);

	// Set methods
	void set_gun_name(std::string val);
	void set_hull_name(std::string val);

	void display(int id);

	// Decodes class from string
	void decode(std::string source);

	// Get ship
	Ship* get_ship(int id);

	// Get players
	std::map<int, Player*>* get_players();

	// Load setup
	void load_setup(std::string path);
	void save_setup(std::string path);

	sf::Texture* make_polygonal_texture(Wall* wall, sf::Vector2f scale, 
		std::string base_texture, float wall_width);
	// load wall textures
	void load_wall_textures();


};