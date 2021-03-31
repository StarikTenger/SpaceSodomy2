#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
	draw->apply_camera({ 0, 0 }, 100, 0);
}
void Game_Client::set_audio(Audio* _audio) {
	audio = _audio;
}

Draw* Game_Client::get_draw() {
	return draw;
}
Audio* Game_Client::get_audio() {
	return audio;
}

std::string Game_Client::get_gun_name() {
	return gun_name;
}

std::string Game_Client::get_hull_name() {
	return hull_name;
}

void Game_Client::set_gun_name(std::string val) {
	gun_name = val;
}

void Game_Client::set_hull_name(std::string val) {
	hull_name = val;
}

void Game_Client::display(int id) {
	// Finding cam target
	auto ship = get_ship(id);
	if (ship) {
		draw->get_camera()->set_pos(ship->get_body()->GetPosition());
		draw->get_camera()->set_angle(ship->get_body()->GetAngle());
		draw->apply_camera();
	}

	// Clear scene
	draw->clear();

	// Background
	Camera camera_backup = *draw->get_camera();
	b2Vec2 mid = aux::to_b2Vec2(sf::Vector2f(draw->get_window()->getSize()));
	draw->apply_camera(b2Vec2(0, 0), mid.Normalize() / draw->get_texture("background")->getSize().x, camera_backup.get_angle());
	draw->image("background", b2Vec2(0, 0),
		aux::to_b2Vec2(sf::Vector2f(draw->get_texture("background")->getSize())), 0);
	draw->set_camera(camera_backup);
	draw->apply_camera();

	// Walls
	for (auto wall : walls) {
		auto color = sf::Color(0, 151, 255);
		auto vertices = wall->get_vertices();
		draw->image("wall " + std::to_string(wall->get_id()), 
			wall->get_origin_pos() + b2Vec2(wall->get_box_size().x/2, wall->get_box_size().y /2) , 
			wall->get_box_size(), 0, color);
		for (int i = 0; i < vertices.size(); i++) {
			int j = (i + 1) % vertices.size();
			draw->line(vertices[i], vertices[j], color);
		}
	}

	// Ships
	for (auto ship : ships) {
		if (!ship->get_body() ||
			!ship->get_body()->GetFixtureList() ||
			!ship->get_body()->GetFixtureList()->GetShape())
			continue;

		// HP_bar & name
		if (ship->get_player()->get_id() != id) {// && !object.effects[Bonus::INVISIBILITY]) {
			// hp
			{
				auto shift = b2Vec2(0, 0) - 0.5 * aux::direction(draw->get_camera()->get_angle());
				float l = ship->get_hp()->get() / ship->get_hp()->get_max();
				draw->image("box", ship->get_body()->GetPosition() + shift,	b2Vec2(1, 0.1),
					draw->get_camera()->get_angle() + b2_pi / 2, { 100, 20, 20, 255 });
				shift = b2Vec2(0, 0) - aux::rotate({ (1 - l) / 2, -0.5 },
					draw->get_camera()->get_angle() + b2_pi / 2);
				draw->image("box", ship->get_body()->GetPosition() + shift, b2Vec2(1*l, 0.1),
					draw->get_camera()->get_angle() + b2_pi / 2, { 255, 0, 0, 255 });
			}

			// Name
			{
				auto shift = b2Vec2(0, 0) - 0.7 * aux::direction(draw->get_camera()->get_angle());
				std::string str = ship->get_player()->get_name();
				if (str.size() > 18)
					str = str.substr(0, 18);
				draw->text(str, "font", ship->get_body()->GetPosition() + shift, 0.03 / 5,
					draw->get_camera()->get_angle() + b2_pi / 2, ship->get_player()->get_color());
			}
		}
		else {
			// Trace
			b2Vec2 dir = ship->get_body()->GetLinearVelocity() +
				guns[gun_name].projectile_vel * aux::direction(ship->get_body()->GetAngle());
			dir.Normalize();
			float lenstep = 0.5;
			int steps = 40;
			for (int i = 0; i < steps; i++) {
				auto col = sf::Color::Red;
				col.a = 255 / steps;
				draw->line(ship->get_body()->GetPosition(), 
					ship->get_body()->GetPosition() + (i * lenstep * dir), col);
			}
		}

		float radius = ship->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		auto color = ship->get_player()->get_color();
		draw->image("ship", ship->get_body()->GetPosition(), {radius, radius}, ship->get_body()->GetAngle());
		draw->image("ship_colors", ship->get_body()->GetPosition(), {radius, radius}, 
			ship->get_body()->GetAngle(), color);

		Camera camera_backup = *draw->get_camera();
		draw->apply_camera(b2Vec2(0, 0), 1, camera_backup.get_angle());
		draw->set_camera(camera_backup);
		draw->apply_camera();
		// Engines
		std::vector<std::string> textures = {
			"engine_lin_forward",
			"engine_lin_backward",
			"engine_lin_left",
			"engine_lin_right",
			"engine_turn_left",
			"engine_turn_right"
		};
		radius *= 2;
		for (int i = 0; i < textures.size(); i++) {
			if (ship->get_player()->get_command_module()->get_command(i))
				draw->image(textures[i], ship->get_body()->GetPosition(),
					{ radius, radius }, ship->get_body()->GetAngle(), color);
		}
	}

	// Animations
	draw->step(dt);
	draw->draw_animations();

	// Projectiles
	for (auto projectile : projectiles) {
		float radius = projectile->get_body()->GetFixtureList()->GetShape()->m_radius * 2 * 2;
		auto color = projectile->get_player()->get_color();
		draw->image("bullet", projectile->get_body()->GetPosition(), { radius, radius }, 
			projectile->get_body()->GetAngle(), color);
		radius *= 0.8;
		draw->image("bullet", projectile->get_body()->GetPosition(), { radius, radius },
			projectile->get_body()->GetAngle());
		// Animation
		Float_Animation::State state_begin;
		state_begin.pos = projectile->get_body()->GetPosition();
		state_begin.scale = 1. * b2Vec2(radius, radius);
		state_begin.angle = 0;
		state_begin.color = color;
		Float_Animation::State state_end = state_begin;
		state_end.scale = b2Vec2_zero;
		state_end.color.a = 0;
		state_end.pos += aux::rotate({ 0, 0.1 }, aux::random_float(0, 2, 2) * b2_pi);
		Float_Animation animation("bullet", state_begin, state_end, 0.15);
		draw->create_animation(animation);
	}

	
}

void Game_Client::decode(std::string source) {
	// First clear
	clear();

	// Creating stringstream
	//std::cout << source << "\n";
	std::stringstream stream;
	stream << source;

	std::string symbol;
	while (stream >> symbol) {
		// Map
		if (symbol == "M") {
			std::string path;
			stream >> path;
			if (map_path != path) {
				map_path = path;
				load_map(map_path);
				for (int i = 0; i < path.size(); i++) {
					if (path[i] == '/' || path[i] == '.') {
						path[i] = '_';
					}
				}
				aux::mk_dir("textures/walls/" + path);
				load_wall_textures();
			}
		}
		// Player
		if (symbol == "P") {
			// Id
			int id;
			stream >> id;
			// Color
			sf::Color color;
			int r, g, b;
			stream >> r >> g >> b;
			color = sf::Color(r, g, b);
			// Name
			std::string name;
			stream >> name;
			// Deaths, kills & etc
			int deaths, kills, time_to_respawn, is_alive;
			stream >> deaths >> kills >> time_to_respawn >> is_alive;
			//std::cout << float(time_to_respawn) << " ";
			// Creating player
			Player* player = create_player(id);
			player->set_color(color);
			player->set_name(name);
			player->set_deaths(deaths);
			player->set_kills(kills);
			player->set_is_alive(is_alive);
			player->get_time_to_respawn()->set(float(time_to_respawn));
			//std::cout << player->get_time_to_respawn()->get() << "\n";
		}
		// Ship
		if (symbol == "S") {
			// Ids
			int id, player_id;
			stream >> id >> player_id;
			// Pos
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			// Linear velocity
			b2Vec2 linvel;
			stream >> linvel.x >> linvel.y;
			// Angle
			float angle;
			stream >> angle;
			// Radius
			float radius;
			stream >> radius;
			// Commands
			std::string commands_stringed;
			stream >> commands_stringed;
			// Hp
			float hp;
			stream >> hp;
			float max_hp;
			stream >> max_hp;
			float stamina;
			stream >> stamina;
			float max_stamina;
			stream >> max_stamina;

			auto ship = create_ship(players[player_id], pos, angle);
			ship->get_body()->SetLinearVelocity(linvel);
			ship->get_body()->GetFixtureList()->GetShape()->m_radius = radius;
			ship->get_hp()->set(hp);
			ship->get_stamina()->set(stamina);
			ship->get_hp()->set_max(max_hp);
			ship->get_stamina()->set_max(max_stamina);

			// Decoding commands
			std::vector<int> commands = aux::string_to_mask(commands_stringed);
			for (int i = 0; i < commands.size(); i++)
				ship->get_player()->get_command_module()->set_command(i, commands[i]);
		}
		// Projectile
		if (symbol == "p") {
			// Ids
			int id, player_id;
			stream >> id >> player_id;
			// Pos
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			// Angle
			float angle;
			stream >> angle;
			// Radius
			float radius;
			stream >> radius;
			// Creating projectile_def
			Projectile_Def projectile_def;
			projectile_def.pos = pos;
			projectile_def.radius = radius;
			projectile_def.player = players[player_id];
			// Createing projectile
			auto projectile = create_projectile(projectile_def);
		}
		// Event
		if (symbol == "e") {
			int id;
			std::string name;
			b2Vec2 pos;
			
			stream >> id >> name >> pos.x >> pos.y;
			
			if (sound_volume != nullptr)
				audio->update_sound(id, name, pos, *sound_volume);
			else
				audio->update_sound(id, name, pos, 100);
		}
	}
}

Ship* Game_Client::get_ship(int id) {
	for (auto ship : ships)
		if (ship->get_player() == players[id])
			return ship;
	return nullptr;
}

std::map<int, Player*>* Game_Client::get_players() {
	return &players;
}

void Game_Client::load_setup(std::string path) {
	std::ifstream fileInput(path);
	std::stringstream file = aux::comment(fileInput);

	std::string command; // Current command
	while (file >> command) {
		if (command == "END") // End of file
			break;
		if (command == "GUN") {
			file >> gun_name;
		}
		if (command == "HULL") {
			file >> hull_name;
		}
	}

	std::cout << gun_name << "\n";
}

void Game_Client::save_setup(std::string path) {
	std::ofstream file(path);
	file << "GUN " << gun_name << "\n" << "HULL " << hull_name;
	file.close();
}

bool Game_Client::make_polygonal_texture(const std::vector<b2Vec2>& polygon, bool is_outer,
	sf::Vector2f scale, std::string base_texture, std::string result_texture,
	float wall_width) {

	if (draw->isTextureExist(result_texture)) {
		std::cout << "texture already found: " << result_texture << '\n';
		return false;
	}
	std::cout << "making texture of: " << result_texture << '\n';

	sf::Image base_image = draw->get_texture(base_texture)->copyToImage();
	sf::Image new_image;
	sf::Color::Transparent;

	sf::Vector2i image_size;
	b2Vec2 origin;
	b2Vec2 point;

	image_size.x = (aux::box_size(polygon).x + wall_width * 2) * scale.x;
	image_size.y = (aux::box_size(polygon).y + wall_width * 2) * scale.y;

	origin = aux::origin_pos(polygon) - b2Vec2(wall_width, wall_width);

	new_image.create(image_size.x, image_size.y, sf::Color::Transparent);

	for (int i = 0; i < image_size.x; i++) {
		for (int j = 0; j < image_size.y; j++) {

			point.x = origin.x + i / scale.x;
			point.y = origin.y + j / scale.y;

			if (aux::is_in_polygon(point, polygon, is_outer)) {
				auto base_color = base_image.getPixel(i % base_image.getSize().x,
					j % base_image.getSize().y);
				float transparency_modifier = 1 - (aux::dist_from_polygon(point, polygon) / wall_width);
				if (transparency_modifier < 0) {
					transparency_modifier = 0;
				}
				base_color.a *= transparency_modifier;
				new_image.setPixel(i, j, base_color);
			}
		}
	}
	sf::Texture* tex = new sf::Texture;
	draw->insert_texture(tex, result_texture);
	draw->get_texture(result_texture)->loadFromImage(new_image);

	std::cout << result_texture << " done\n";
	return true;
}


void Game_Client::load_wall_textures() {
	auto path = map_path;
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == '/' || path[i] == '.') {
			path[i] = '_';
		}
	}
	for (auto wall : walls) {

		auto wall_name = std::string("") + "wall" + " " + std::to_string(wall->get_id());

		draw->load_texture(wall_name,
			"textures/walls/" + path + '/' + wall_name + ".png");

		if (!draw->isTextureExist(wall_name)) {

			make_polygonal_texture(wall->get_vertices(), wall->get_orientation(), sf::Vector2f(100, 100),
				"wall", wall_name, 0.5);

			draw->export_texture(wall_name,
				"textures/walls/" + path + '/' + wall_name + ".png");
		}
		wall->init_drawing(0.5);
	}
}