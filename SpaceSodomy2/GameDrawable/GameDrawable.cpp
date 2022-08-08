// GameDrawable.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include "GameDrawable.h"
#include <iostream>
#include <thread>

Draw* GameDrawable::get_draw() {
	return draw;
}
Audio* GameDrawable::get_audio() {
	return audio;
}

void GameDrawable::set_draw(Draw* _draw) {
	draw = _draw;
	draw->apply_camera({ 0, 0 }, 100, 0);
}

void GameDrawable::set_audio(Audio* _audio) {
	audio = _audio;
}

void GameDrawable::set_gui(tgui::Gui* _gui) {
	gui = _gui;
}

void GameDrawable::display(int id) {
	// Setting id
	my_id = id;
    int my_team_id = (player_by_id(id) ? player_by_id(id)->get_team_id() : environment_team_id);
	

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
	draw->image(global_wall_name, 0.5 * (origin_pos + end_pos), end_pos - origin_pos);
	for (auto wall : walls) {
		auto color = sf::Color(0, 151, 255);
		if (wall->get_type() == Wall::SPIKED || wall->get_type() == Wall::GHOST) {
			color = sf::Color(255, 255, 255);
		}
		auto vertices = wall->get_vertices();
		for (int i = 0; i < vertices.size(); i++) {
			int j = (i + 1) % vertices.size();
			float thickness = 0.05;
			draw->thick_line(vertices[i], vertices[j], color, thickness);

			// Drawing wall connection
			b2Vec2 vec_a = vertices[(i + 1) % vertices.size()] - vertices[i];
			b2Vec2 vec_b = vertices[(vertices.size() + i - 1) % vertices.size()] - vertices[i];
			vec_a.Normalize();
			vec_b.Normalize();
			b2Vec2 vec_dir = vec_a + vec_b;
			vec_dir.Normalize();
			float cos_val = b2Dot(vec_a, vec_b);
			float sin_val = sqrt(abs(1 - cos_val) / 2);
			cos_val = sqrt(abs(1 + cos_val) / 2);
			float size_x = thickness * sin_val / 2;
			float size_y = thickness * cos_val;
			draw->thick_line(vertices[i] - size_x * vec_dir, vertices[i] + size_x * vec_dir, color, size_y);
		}
	}
	// Forcefields
	for (auto pt : ffield_spawnpoint_grid) {
		if (aux::random_int(0, 19) != 0) {
			continue;
		}
		float force_dir = aux::vec_to_angle(pt.force);
		FloatAnimation::State beg, end;
		beg.image = end.image = "particle_laser";
		beg.angle = end.angle = force_dir;
		
		beg.pos = pt.pos;
		end.pos = pt.pos + b2Vec2({pt.force.x * 0.3f, pt.force.y * 0.3f});
		beg.color = end.color = sf::Color::White;
		end.color.a = 0;
		beg.scale = { 0.1f, 0.1f };
		end.scale = { 0.3f, 0.1f };
		draw->create_animation(FloatAnimation("particle_laser", beg, end, 0.3f));
	}

	// Ships
	for (auto ship : ships) {
		float opacity = 1;
		if (ship->get_effects()->get_effect(Effects::INVISIBILITY)->get_counter()->get() > 0) {
			opacity = 0.3;  // TODO: configs
			if (ship->is_visible_to_enemies()) {
				opacity = 0.7;
			}
		}
		if (!ship->get_body() ||
			!ship->get_body()->GetFixtureList() ||
			!ship->get_body()->GetFixtureList()->GetShape())
			continue;

		// Some ship params
		float radius = ship->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		auto color = ship->get_player()->get_color();

		// HP_bar & name (others)
		if (ship->get_player()->get_id() != id) {
			if (ship->get_effects()->get_effect(Effects::INVISIBILITY)->get_counter()->get() < 0.01 || ship->get_player()->get_team_id() == my_team_id) {
				// hp
				{
					auto shift = b2Vec2(0, 0) - 0.5 * aux::direction(draw->get_camera()->get_angle());
					float l = ship->get_hp()->get() / ship->get_hp()->get_max();
					draw->image("box", ship->get_body()->GetPosition() + shift, b2Vec2(1, 0.1),
						draw->get_camera()->get_angle() + b2_pi / 2, { 100, 20, 20, 255 });
					shift = b2Vec2(0, 0) - aux::rotate({ (1 - l) / 2, -0.5 },
						draw->get_camera()->get_angle() + b2_pi / 2);
					draw->image("box", ship->get_body()->GetPosition() + shift, b2Vec2(1 * l, 0.1),
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
		}
		// Show aim lines (current player)
		else {
			// Direction
			b2Vec2 dir = ship->get_body()->GetLinearVelocity() +
				guns[ship->get_player()->get_gun_name()].projectile_vel * aux::direction(ship->get_body()->GetAngle());
			dir.Normalize();
			// Calculating 3 initial positions
			b2Vec2 body_pos = ship->get_body()->GetPosition();
			b2Vec2 right_pos = (guns[ship->get_player()->get_gun_name()].projectile_radius + 0.09) * dir;
			right_pos = body_pos + aux::rotate(right_pos, b2_pi / 2);
			b2Vec2 left_pos = (guns[ship->get_player()->get_gun_name()].projectile_radius + 0.09) * dir;
			left_pos = body_pos + aux::rotate(left_pos, -b2_pi / 2);
			// 3 intersections
			b2Vec2 intersection_right = get_beam_intersection(right_pos, aux::vec_to_angle(dir));
			b2Vec2 intersection_left = get_beam_intersection(left_pos, aux::vec_to_angle(dir));
			b2Vec2 intersection = get_beam_intersection(body_pos, aux::vec_to_angle(dir));
			intersection = std::min(std::min(b2Distance(body_pos, intersection_right), b2Distance(body_pos, intersection_left)),
				b2Distance(body_pos, intersection)) * dir;
			intersection += body_pos;
			color.a = aim_opacity * opacity;
			// Drawing lines in respond to aim config
			if (aim_conf % 2 == 1)
				draw->thin_line(ship->get_body()->GetPosition(), intersection, color);
			if (aim_conf > 1) {
				draw->thin_line(right_pos, intersection_right, color);
				draw->thin_line(left_pos, intersection_left, color);
			}

			// Blink target
			if (ship->get_left_module()->get_type() == Module::MODULE_BLINK ||
				ship->get_right_module()->get_type() == Module::MODULE_BLINK)
				draw->image("ship_aura_" + ship->get_player()->get_hull_name(),
					body_pos + module_manager.get_prototype(Module::MODULE_BLINK)->params["distance"] * aux::direction(ship->get_body()->GetAngle()),
					{ radius, radius }, ship->get_body()->GetAngle(), aux::set_opacity(color, 70));
		}

		// Hull
		color.a *= opacity;
		auto base_color = ship->get_player()->get_color();
		if (!(ship->get_player()->get_team_id() != my_team_id && ship->get_effects()->get_effect(Effects::INVISIBILITY)->get_counter()->get() > 0)) {
			draw->image("ship_" + ship->get_player()->get_hull_name(), ship->get_body()->GetPosition(), { radius, radius }, ship->get_body()->GetAngle());
			draw->image("ship_colors_" + ship->get_player()->get_hull_name(), ship->get_body()->GetPosition(), { radius, radius },
				ship->get_body()->GetAngle(), color);
		}

		// Engines
		std::vector<std::string> textures = {
			"engine_lin_forward",
			"engine_lin_backward",
			"engine_lin_left",
			"engine_lin_right",
			"engine_turn_left",
			"engine_turn_right"
		};
		std::vector<b2Vec2> directions = {
			{-1, 0},
			{1, 0},
			{0, 1},
			{0, -1},
			{0, 0},
			{0, 0}
		};
		radius *= 2;
		for (int i = 0; i < textures.size(); i++) {
			if (ship->get_player()->get_command_module()->get_command(i)) {
				if (!(ship->get_player()->get_team_id() != my_team_id && ship->get_effects()->get_effect(Effects::INVISIBILITY)->get_counter()->get() > 0.01)
					|| ship->is_visible_to_enemies()) {
					draw->image(textures[i], ship->get_body()->GetPosition(),
						{ radius, radius }, ship->get_body()->GetAngle(), color);
					// Animation
					float engine_len = 0.1;
					if (ship->get_player()->get_command_module()->get_command(CommandModule::BOOST)
						&& ship->get_stamina()->get() > 0)
						engine_len = 0.4;
					FloatAnimation::State state_a;
					state_a.pos = ship->get_body()->GetPosition();
					state_a.angle = ship->get_body()->GetAngle();
					state_a.scale = { radius, radius };
					state_a.color = color;
					float animation_time = 0.1;
					auto state_b = state_a;
					state_b.color.a = 0;
					state_b.pos += animation_time * ship->get_body()->GetLinearVelocity();
					state_b.pos += engine_len * aux::rotate(directions[i], ship->get_body()->GetAngle());
					state_b.pos += aux::rotate({ 0, 0.05 }, aux::random_float(0, 2, 2) * b2_pi);
					if (ship->get_player()->get_command_module()->get_command(CommandModule::BOOST))
						state_b.color = { 255, 255, 255, 0 };

					FloatAnimation animation(textures[i], state_a, state_b, animation_time, GAME);
					state_b.pos += aux::rotate({ 0, 0.05 }, aux::random_float(0, 2, 2) * b2_pi);
					FloatAnimation animation_1(textures[i], state_a, state_b, animation_time, GAME);
					draw->create_animation(animation);
					draw->create_animation(animation_1);
				}
			}

		}
		radius /= 2;

		// Effects

		// Charge
		if (ship->get_effects()->get_effect(Effects::CHARGE)->get_counter()->get() > 0) {
			draw->fadeout_animation("bullet",
				ship->get_body()->GetPosition(), // Position
				{ 0, radius / 4 }, // Shift
				{ radius * 2, 0 }, // Size
				{ 0, 0 }, // Angle
				{ base_color, aux::make_transparent(base_color) }, // Color
				0.4, GAME // Duration, layer
			);
		}
		// Immortality
		if (ship->get_effects()->get_effect(Effects::IMMORTALITY)->get_counter()->get() > 0 && (
			ship->get_effects()->get_effect(Effects::INVISIBILITY)->get_counter()->get() < 0.01 || ship->get_player()->get_id() == my_id)) {
			draw->fadeout_animation("ship_aura_" + ship->get_player()->get_hull_name(),
				ship->get_body()->GetPosition(), // Position
				{ 0, radius / 16 }, // Shift
				{ radius, radius }, // Size
				{ ship->get_body()->GetAngle(), ship->get_body()->GetAngle() }, // Angle
				{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
				0.15, GAME // Duration, layer
			);
		}
		// Berserk
		if (ship->get_effects()->get_effect(Effects::BERSERK)->get_counter()->get() > 0) {
			draw->image("effect", ship->get_body()->GetPosition(), b2Vec2(radius, radius), time * 10, sf::Color::Red);
		}
		// Laser
		if (ship->get_effects()->get_effect(Effects::LASER)->get_counter()->get() > 0) {
			b2Vec2 intersection = get_beam_intersection(ship->get_body()->GetPosition(), ship->get_body()->GetAngle());
			float thickness = 0.5;
			draw->textured_line("laser_aura", ship->get_body()->GetPosition(), intersection, color, thickness);
			draw->textured_line("laser_kernel", ship->get_body()->GetPosition(), intersection, sf::Color::White, thickness);
			draw->fadeout_animation("bullet",
				intersection, // Position
				{ 0, 0.3 }, // Shift
				{ thickness, 0 }, // Size
				{ 0, 0 }, // Angle
				{ sf::Color::White, color }, // Color
				0.15, GAME // Duration, layer
			);
		}
		if (ship->get_bonus_slot()->get_current_bonus() == Bonus::LASER) {
			b2Vec2 dir = aux::direction(ship->get_body()->GetAngle());
			b2Vec2 body_pos = ship->get_body()->GetPosition();
			b2Vec2 intersection = get_beam_intersection(body_pos, aux::vec_to_angle(dir));
			intersection = (b2Distance(body_pos, intersection)) * dir;
			intersection += body_pos;
			auto color = ship->get_player()->get_color();
			auto base_color = ship->get_player()->get_color();
			color.a = aim_opacity * opacity;
			draw->thin_line(ship->get_body()->GetPosition(), intersection, color);

		}
	}

	// Animations
	draw->draw_animations(GAME);
	draw->step(dt);
	time += dt;

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
		FloatAnimation::State state_begin;
		state_begin.pos = projectile->get_body()->GetPosition();
		state_begin.scale = 1. * b2Vec2(radius, radius);
		state_begin.angle = 0;
		state_begin.color = color;
		FloatAnimation::State state_end = state_begin;
		state_end.scale = b2Vec2_zero;
		state_end.color.a = 0;
		state_end.pos += aux::rotate({ 0, 0.1 }, aux::random_float(0, 2, 2) * b2_pi);
		FloatAnimation animation("bullet", state_begin, state_end, 0.15, GAME);
		draw->create_animation(animation);
	}

	// Rockets
	for (auto rocket : rockets) {
		float radius = rocket->get_body()->GetFixtureList()->GetShape()->m_radius * 8;
		auto color = rocket->get_player()->get_color();
		draw->image("rocket", rocket->get_body()->GetPosition(), { radius, radius },
			rocket->get_body()->GetAngle(), color);
		draw->fadeout_animation("bullet",
			rocket->get_body()->GetPosition(), // Position
			{ 0, 0.1 }, // Shift
			{ 0.3, 0.3 }, // Size
			{ 0, 0 }, // Angle
			{ color, aux::make_transparent(color) }, // Color
			0.15, GAME // Duration, layer
		);
	}

	// Bonuses
	for (auto bonus : bonuses) {
		float angle = draw->get_camera()->get_angle() + b2_pi / 2;
		draw->image(bonus_textures[bonus->get_type()],
			bonus->get_body()->GetPosition(), { 0.3, 0.3 },
			angle, sf::Color::White);
		draw->fadeout_animation(bonus_textures[bonus->get_type()],
			bonus->get_body()->GetPosition(), // Position
			{ 0, 0.1 }, // Shift
			{ 0.3, 0.3 }, // Size
			{ angle, angle }, // Angle
			{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
			0.15, GAME // Duration, layer
		);
	}
}


Ship* GameDrawable::get_ship(int id) {
	for (auto ship : ships)
		if (ship->get_player()->get_id() == id)
			return ship;
	return nullptr;
}

std::map<int, Player*>* GameDrawable::get_players() {
	return &players;
}

int GameDrawable::get_player_id(std::string name) {
	for (auto it = players.begin(); it != players.end(); it++)
		if (it->second != nullptr && it->second->get_name() == name)
			return it->second->get_id();
	return 0;
}

sf::Texture* GameDrawable::make_polygonal_texture(Wall* wall,
	sf::Vector2f scale, std::string base_texture,
	float wall_width, sf::Color overlay_color) {

	auto polygon = wall->get_vertices();
	bool is_outer = wall->get_orientation();

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

	if (wall->get_type() == Wall::GHOST) {
		sf::Texture* tex = new sf::Texture;
		tex->loadFromImage(new_image);
		return tex;
	}

	if (wall->get_type() == Wall::SPIKED) {
		for (int i = 0; i < image_size.x; i++) {
			for (int j = 0; j < image_size.y; j++) {

				point.x = origin.x + i / scale.x;
				point.y = origin.y + j / scale.y;

				if (aux::is_in_polygon(point, polygon, is_outer)) {
					auto base_color = base_image.getPixel(i % base_image.getSize().x,
						j % base_image.getSize().y);
					auto new_color = sf::Color(base_color.r * overlay_color.r / 255,
						base_color.g * overlay_color.g / 255,
						base_color.b * overlay_color.b / 255,
						std::max((1 - (aux::dist_from_polygon(point, polygon) / wall_width)) * 255, 0.f));
					new_image.setPixel(i, j, new_color);
				}
				else {
					// TODO: laser width parameter
					float laser_width = 0.4;
					auto base_color = sf::Color(255, 0, 0, std::max((1 - (aux::dist_from_polygon(point, polygon) / laser_width)) * 255, 0.f));
					new_image.setPixel(i, j, base_color);
				}
			}
		}
	}
	else {
		for (int i = 0; i < image_size.x; i++) {
			for (int j = 0; j < image_size.y; j++) {

				point.x = origin.x + i / scale.x;
				point.y = origin.y + j / scale.y;

				if (aux::is_in_polygon(point, polygon, is_outer)) {
					auto base_color = base_image.getPixel(i % base_image.getSize().x,
						j % base_image.getSize().y);
					auto new_color = sf::Color(base_color.r * overlay_color.r / 255,
						base_color.g * overlay_color.g / 255,
						base_color.b * overlay_color.b / 255,
						std::max((1 - (aux::dist_from_polygon(point, polygon) / wall_width)) * 255, 0.f));
					new_image.setPixel(i, j, new_color);
				}
			}
		}
	}

	sf::Texture* tex = new sf::Texture;
	tex->loadFromImage(new_image);
	return tex;
}


void GameDrawable::load_wall_textures(sf::Color overlay_color, sf::Vector2f scale, float wall_width, std::string wall_name) {
	auto path = map_path;
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == '/' || path[i] == '.') {
			path[i] = '_';
		}
	}

	global_wall_name = "wall_" + path;

	draw->load_texture(global_wall_name,
		"textures/walls/" + global_wall_name + ".png");

	for (auto wall : walls) {
		wall->init_drawing(wall_width);
		if (origin_pos.x > wall->get_origin_pos().x) {
			origin_pos.x = wall->get_origin_pos().x;
		}
		if (origin_pos.y > wall->get_origin_pos().y) {
			origin_pos.y = wall->get_origin_pos().y;
		}
		if (end_pos.x < wall->get_box_size().x + wall->get_origin_pos().x) {
			end_pos.x = wall->get_box_size().x + wall->get_origin_pos().x;
		}
		if (end_pos.y < wall->get_box_size().y + wall->get_origin_pos().y) {
			end_pos.y = wall->get_box_size().y + wall->get_origin_pos().y;
		}
	}


	if (!draw->is_texture_exist(global_wall_name)) {
		sf::RenderTexture base;
		base.create((end_pos.x - origin_pos.x) * scale.x, (end_pos.y - origin_pos.y) * scale.y);
		base.clear(sf::Color::Transparent);
		for (auto wall : walls) {
			std::cout << "making wall with id " << wall->get_id() << "\n";
			auto temp = make_polygonal_texture(wall, scale, wall_name, wall_width, overlay_color);
			draw->overlay_texture(base, temp, sf::Color::White,
				sf::Vector2i((wall->get_origin_pos().x - origin_pos.x) * scale.x,
					(wall->get_origin_pos().y - origin_pos.y) * scale.y));
			base.display();
			delete temp;
		}
		base.display();
		sf::Texture* tex = new sf::Texture;
		sf::Image ans = sf::Texture(base.getTexture()).copyToImage();
		draw->insert_texture(tex, global_wall_name);
		draw->get_texture(global_wall_name)->loadFromImage(ans);
		draw->export_texture(global_wall_name,
			"textures/walls/" + global_wall_name + ".png");
	}
}

ModuleManager* GameDrawable::get_module_manager() {
	return &module_manager;
}
