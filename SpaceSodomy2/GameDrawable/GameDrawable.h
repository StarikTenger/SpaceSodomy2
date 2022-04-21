#pragma once
#include <Game/Game.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include <AuxLib/AuxLib.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>


class GameDrawable : public Game {
protected:
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

	// Preferences
	int my_id = 0;
	int engine_active = 0;
	int aim_conf = 1;
	int aim_opacity = 150;

	// Some drawing parameters
	// box for drawing the global wall texture;
	std::string global_wall_name = "";
	b2Vec2 origin_pos = { 0,0 };
	b2Vec2 end_pos = { 0,0 };
	// Wall thickness
	float outer_wall_thickness = 0.05;

	tgui::Gui* gui;
public:
	enum Animation_Layers {
		GAME,
		HUD
	};

	// Get methods
	Draw* get_draw();
	Audio* get_audio();


	// Set methods

	void set_draw(Draw* _draw);
	void set_audio(Audio* _audio);


	void set_gui(tgui::Gui* _gui);

	// Display scene relative to particular id
	void display(int id);

	// Get ship
	Ship* get_ship(int id);

	// Get players
	std::map<int, Player*>* get_players();

	// Get player id by name
	int get_player_id(std::string name);


	sf::Texture* make_polygonal_texture(Wall* wall, sf::Vector2f scale,
		std::string base_texture, float wall_width, sf::Color overlay_color);
	// load wall textures
	void load_wall_textures(sf::Color overlay_color = sf::Color(0, 151, 255),
		sf::Vector2f scale = { 100, 100 },
		float wall_width = 1.0,
		std::string wall_name = "wall");

	ModuleManager* get_module_manager();
};