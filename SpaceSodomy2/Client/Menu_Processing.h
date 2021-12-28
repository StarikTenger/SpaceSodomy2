#pragma once
#include<Menu/Menu.h>
#include<AuxLib/AuxLib.h>
#include<Client/Game_Client.h>
#include<string>
#include<Client/Replay.h>
#include<Network/Client_Network.h>

class Menu_Processing {
private:
	Menu main_menu; // main menu object
	Menu config_menu; // config menu object
	Menu keys_menu; // keys menu object
	Menu settings_menu; // settings menu object
	Menu sound_menu; // sound menu object
	Menu gun_menu; // gun menu object
	std::map <std::string, Menu> guns; // guns
	Menu hull_menu; // hull menu objects
	std::map <std::string, Menu> hulls; // hulls
	Menu modules_menu;
	std::map <std::string, Menu> modules; // modules
	Menu HUD_menu;
	Menu replay_menu;
	Menu replay_setup_menu;

	std::vector<Menu*> menus, settings_menus;
	aux::Keyboard* keyboard;
	b2Vec2* mouse_pos;
	int current_id = 1;
	Client_Network* network;
	std::map<int, Bar*> bars;
	std::map<int, Menu_Object*> images;
	std::map<int, Button*> buttons;
	std::map<int, Constant_Text*> constant_texts;
	std::map<int, Slider*> sliders;
	std::map<int, Text_Field*> text_fields;
	std::map<int, Keyboard_Field*> keyboard_fields;
	std::map<int, Check_Box*> check_boxes;
	std::queue<int> events; // menu events
	std::map<std::string, int> name_to_id;
	std::map<int, std::pair<int, int>> id_to_keyit;
	std::vector<std::vector<std::string*>> keys_menu_vec;
	void save_config(std::string path, std::string address_, int port_, int id_, std::string name_, std::string team_name_);
	void load_config(std::string path, std::string* address_, std::string* port_,
		std::string* id_, std::string* name_, std::string* team_name_);
	void save_keys(std::string path, std::vector<std::vector<std::string*>> keys);
	void load_keys(std::string path, std::vector<std::vector<std::string*>>* keys, Menu* menu,
		b2Vec2 pos, float name_indent, b2Vec2 indent, int character_size);
	void load_HUD_settings(std::string path);
	void save_HUD_settings(std::string path);
	void load_sound(std::string path);
	void save_sound(std::string path);
	void init_menu(std::string path_, Menu* object);
	void init_gun(std::string, int damage, float recharge, int stamina_cost, float projectile_mass,
		float projectile_radius, int projectile_vel, Menu* gun);
	void init_gun_menu(b2Vec2 pos, std::string path_to_guns_description);
	void close_settings_menus();
	void init_hull_menu(b2Vec2 pos, std::string path_to_hulls_description);
	void init_hull(std::string name, int hp, float mass, float radius,
		int stamina, int stamina_recovery, Menu* hull);
	void init_modules_menu(b2Vec2 pos, std::string path_to_modules_description);
	void init_module(std::string name, Menu* module, b2Vec2 add_pos);
	//void init_modules_menu(b2Vec2 pos, std::string path_to_modules_description);
	//voif init_module(std::string name);
	Draw* draw;
	Game_Client* game;
	Replay* replay;
	bool disactivated = 0;
	bool shader_active = 1;
	bool* reload;
	int module_num = 1;
public:
	Menu_Processing();
	bool active = 1;
	int text_field_active = 0;
	void init(Draw* draw, b2Vec2* mouse_pos_,
		aux::Keyboard* keyboard_, Client_Network* network_,
		Game_Client* game_, Replay* replay_,
		bool* reload_);
	void step();
};


