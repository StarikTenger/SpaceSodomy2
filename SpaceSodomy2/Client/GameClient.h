#pragma once
#include <GameDrawable/GameDrawable.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include <AuxLib/AuxLib.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class GameClient : public GameDrawable {
private:
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "IMMORTALITY";
	std::string right_module_name = "DASH";
	
	bool network_information_active = false;

	std::vector <int> engine_commands = {
					CommandModule::ENGINE_ANG_LEFT,
					CommandModule::ENGINE_ANG_RIGHT,
					CommandModule::ENGINE_LIN_BACKWARD,
					CommandModule::ENGINE_LIN_FORWARD,
					CommandModule::ENGINE_LIN_LEFT,
					CommandModule::ENGINE_LIN_RIGHT
	};
public:
	// Get methods
	std::string get_gun_name();
	std::string get_hull_name();
	std::string get_left_module_name();
	std::string get_right_module_name();

	std::string get_bonus_texture_name(int val);

	// Set methods
	int get_aim_conf();
	int get_aim_opacity();
	bool get_network_information_active();

	void set_gun_name(std::string val);
	void set_hull_name(std::string val);
	void set_left_module_name(std::string val);
	void set_right_module_name(std::string val);

	void set_aim_conf(int _conf);
	void set_aim_opacity(int _opacity);
	void set_network_information_active(bool _active);

	// Display scene relative to particular id
	void display(int id);

	// Decodes class from string
	void update_state(std::string source);

	// Load setup
	void load_setup(std::string path);
	void save_setup(std::string path);
};


