#include "pch.h"
#include "ShipBrain.h"
#include "Ship.cpp"


ShipBrain::ShipBrain(CommandModule& _1, const GameReadable& _2, int _3) : iBrain(_1, _2), id(_3) {};
void ShipBrain::set_new_id(int _) { id = _; }
std::string ShipBrain::get_gun_name() { return gun_name; }
std::string ShipBrain::get_hull_name() { return hull_name; }
std::string ShipBrain::get_left_module_name() { return left_module_name; }
std::string ShipBrain::get_right_module_name() { return right_module_name; }


void ShipBrain::suggest_equip(Equip _) {
	gun_name = _.gun_name;
	hull_name = _.hull_name;
	left_module_name = _.left_module_name;
	right_module_name = _.right_module_name;
}





EdgarBrain::EdgarBrain(CommandModule& _1, const GameReadable& _2, int _3) :
    ShipBrain(_1, _2, _3) {

}


bool EdgarBrain::turn_to_angle(Ship* ship, float angle) {

	auto cur_angle = ship->get_body()->GetAngle();
	while (cur_angle > b2_pi)
		cur_angle -= 2.f * b2_pi;
	while (cur_angle < -b2_pi)
		cur_angle += 2.f * b2_pi;

	while (angle > b2_pi)
		angle -= 2.f * b2_pi;
	while (angle < -b2_pi)
		angle += 2.f * b2_pi;


	auto dif = cur_angle - angle;
	if (abs(dif) > b2_pi) {
		if (dif < 0.f) {
			dif += 2.f * b2_pi;
		}
		else {
			dif -= 2.f * b2_pi;
		}
	}

	auto dist = 2.f * b2_pi;
	auto point = 0.0f;
	if (abs(-2.f * b2_pi - dif) < dist) {
		dist = abs(-2.f * b2_pi - dif);
		point = -2.f * b2_pi - dif;
	}
	if (abs(dif) < dist) {
		dist = abs(dif);
		point = dif;
	}
	if (abs(2.f * b2_pi - dif) < dist) {
		dist = abs(2.f * b2_pi - dif);
		point = 2.f * b2_pi - dif;
	}
	auto speed = ship->get_body()->GetAngularVelocity();
	auto acceleration = ship->get_engine()->get_torque() / ship->get_body()->GetInertia();
	if (dist < 0.03 || speed > b2_pi) {
		command_module.set_command(CommandModule::STABILIZE_ROTATION, 1);
		return true;
	}
	else {
		if (point > 0) {
			if (abs(point / speed) > abs(speed / acceleration)) {
				command_module.set_command(CommandModule::ENGINE_ANG_LEFT, 1);
			}
			else {
				command_module.set_command(CommandModule::STABILIZE_ROTATION, 1);
			}
		}
		else {
			if (abs(point / speed) > abs(speed / acceleration)) {
				command_module.set_command(CommandModule::ENGINE_ANG_RIGHT, 1);
			}
			else {
				command_module.set_command(CommandModule::STABILIZE_ROTATION, 1);
			}
		}
		return false;
	}
}

void EdgarBrain::accelerate_towards_point(Ship* ship, b2Vec2 point) {
	auto pos = ship->get_body()->GetPosition();
	auto speed = ship->get_body()->GetLinearVelocity();
	auto force_linear = ship->get_engine()->get_force_linear();

	auto path = point - pos;

	auto cur_angle = ship->get_body()->GetAngle();
	while (cur_angle > 2 * b2_pi)
		cur_angle -= 2 * b2_pi;
	while (cur_angle < 0)
		cur_angle += 2 * b2_pi;

	auto point_angle = aux::vec_to_angle(path) + b2_pi;
	auto dir_angle = cur_angle - point_angle;

	auto rand = aux::random_float(0, 1, 2);



	if (rand < abs(sin(dir_angle))) {
		if (sin(dir_angle) < 0) {
			command_module.set_command(CommandModule::ENGINE_LIN_LEFT, 1);
		}
		else {
			command_module.set_command(CommandModule::ENGINE_LIN_RIGHT, 1);
		}
	}

	if (rand < abs(cos(dir_angle))) {
		if (cos(dir_angle) < 0) {
			command_module.set_command(CommandModule::ENGINE_LIN_FORWARD, 1);
		}
		else {
			command_module.set_command(CommandModule::ENGINE_LIN_BACKWARD, 1);
		}
	}
}

void EdgarBrain::attack(Ship* _ship, Ship* my_ship) {
	if ((my_ship->get_bonus_slot()->get_current_bonus() == Bonus::CHARGE
		|| my_ship->get_effects()->get_effect(Effects::CHARGE)->get_counter()->get() > b2_epsilon)
		&& b2Distance(_ship->get_body()->GetPosition(), my_ship->get_body()->GetPosition()) < 7.f) {
		command_module.set_command(CommandModule::BONUS_ACTIVATION, 1);
		command_module.set_command(CommandModule::BOOST, 1);
		accelerate_towards_point(my_ship, _ship->get_body()->GetPosition());
		return;
	}
	if (my_ship->get_bonus_slot()->get_current_bonus() == Bonus::LASER
		|| my_ship->get_effects()->get_effect(Effects::LASER)->get_counter()->get() > b2_epsilon) {
		shoot_laser(_ship, my_ship);
		return;
	}
	auto speed = _ship->get_body()->GetLinearVelocity() - my_ship->get_body()->GetLinearVelocity();
	auto bullet_speed = my_ship->get_gun()->get_projectile_vel();
	auto b = aux::vec_to_angle(_ship->get_body()->GetPosition() - my_ship->get_body()->GetPosition()) - aux::vec_to_angle(speed);

	auto sina = sin(b) * speed.Length() / bullet_speed;
	auto angle = aux::vec_to_angle(_ship->get_body()->GetPosition() - my_ship->get_body()->GetPosition()) - asin(sina);


	if (turn_to_angle(my_ship, angle)) {
		if (is_target_visible(_ship, my_ship->get_body())) {
			if (my_ship->get_bonus_slot()->get_current_bonus() == Bonus::BERSERK) {
				command_module.set_command(CommandModule::BONUS_ACTIVATION, 1);
			}
			command_module.set_command(CommandModule::SHOOT, 1);
		}
	}
}

void EdgarBrain::shoot_laser(Ship* _ship, Ship* ship) {
	auto angle = aux::vec_to_angle(_ship->get_body()->GetPosition() - ship->get_body()->GetPosition());

	if (turn_to_angle(ship, angle)) {
		if (is_target_visible(_ship, ship->get_body())) {
			command_module.set_command(CommandModule::BONUS_ACTIVATION, 1);
		}
	}
}

b2Vec2 EdgarBrain::calc_volumetric_intersection(b2Vec2 cur_point, float dir_angle, float radius) {
	auto dir = aux::angle_to_vec(dir_angle);

	b2Vec2 body_pos = cur_point;
	b2Vec2 right_pos = (radius + 0.09) * dir;
	right_pos = body_pos + aux::rotate(right_pos, b2_pi / 2);
	b2Vec2 left_pos = (radius + 0.09) * dir;
	left_pos = body_pos + aux::rotate(left_pos, -b2_pi / 2);

	

	b2Vec2 intersection_right = environment.calc_intersection_(right_pos, dir_angle);
	b2Vec2 intersection_left = environment.calc_intersection_(left_pos, dir_angle);
	b2Vec2 intersection = environment.calc_intersection_(body_pos, dir_angle);

	intersection = std::min(std::min(b2Distance(body_pos, intersection_right), b2Distance(body_pos, intersection_left)),
		b2Distance(body_pos, intersection)) * dir;

	return cur_point + intersection;
}

void EdgarBrain::safety_flight(Ship* ship) {
	float max_dist = 0;
	b2Vec2 flight_point;
	auto cur_point = ship->get_body()->GetPosition();
	int beams_num = 12;
	for (int i = 0; i < beams_num; i++) {
		auto radius = ship->get_body()->GetFixtureList()->GetShape()->m_radius;
		auto intersect = calc_volumetric_intersection(cur_point, (2.0 * b2_pi / float(beams_num)) * float(i), radius);
		if (max_dist < (intersect - cur_point).Length()) {
			max_dist = (intersect - cur_point).Length();
			flight_point = intersect;
		}
	}
	accelerate_towards_point(ship, flight_point);
}

bool EdgarBrain::is_target_visible(Ship* my_ship, b2Body* target) {
	auto beam_intersection = environment.calc_intersection_(target->GetPosition(),
		aux::vec_to_angle(my_ship->get_body()->GetPosition() - target->GetPosition()));
	auto a = (my_ship->get_body()->GetPosition() - target->GetPosition()).Length();
	auto b = (beam_intersection - target->GetPosition()).Length();
	return a < b;
}


bool EdgarBrain::is_target_reachable(Ship* my_ship, b2Body* target) {
	auto beam_intersection = calc_volumetric_intersection(target->GetPosition(),
		aux::vec_to_angle(my_ship->get_body()->GetPosition() - target->GetPosition()),
		my_ship->get_body()->GetFixtureList()->GetShape()->m_radius);
	auto a = (my_ship->get_body()->GetPosition() - target->GetPosition()).Length();
	auto b = (beam_intersection - target->GetPosition()).Length();
	return a < b;
}


Ship* EdgarBrain::get_enemy(Ship* my_ship) {
	std::vector<Ship*> targets;
	
	Ship* ship_closest = nullptr;
	float dist = 1e5;
	

	for (auto ship : environment.ships) {
		if (ship->get_player()->is_hostile_to(*my_ship->get_player()) && ship->is_visible()) {
			auto cur_dist = b2Distance(ship->get_body()->GetPosition(), my_ship->get_body()->GetPosition());
			if (cur_dist < dist && is_target_visible(my_ship, ship->get_body())) {
				dist = cur_dist;
				ship_closest = ship;
			}
		}
	}
	return ship_closest;

}

bool EdgarBrain::get_bonuses(Ship* my_ship) {
	if (my_ship->get_body()->GetLinearVelocity().Length() > 3) {
		return false;
	}
	float pickup_dist = 5.f; 
	for (auto bonus : environment.bonuses) {
		auto body = bonus->get_body();
		if (b2Distance(my_ship->get_body()->GetPosition(), bonus->get_body()->GetPosition()) < pickup_dist
			&& is_target_reachable(my_ship, body)) {
			accelerate_towards_point(my_ship, body->GetPosition());
			return true;
		}
	}
	return false;
}
void EdgarBrain::compute_action() {
    Ship* my_ship = nullptr;
    for (auto ship : environment.ships) {
        if (ship->get_id() == id) {
            my_ship = ship;
        }
    }
	if (my_ship) {
		/*
		* AI code goes here
		*/
		command_module.set_command(CommandModule::ENGINE_ANG_RIGHT, 0);
		command_module.set_command(CommandModule::SHOOT, 0);
		command_module.set_command(CommandModule::ENGINE_ANG_LEFT, 0);
		command_module.set_command(CommandModule::ENGINE_LIN_LEFT, 0);
		command_module.set_command(CommandModule::ENGINE_LIN_RIGHT, 0);
		command_module.set_command(CommandModule::ENGINE_LIN_FORWARD, 0);
		command_module.set_command(CommandModule::ENGINE_LIN_BACKWARD, 0);
		command_module.set_command(CommandModule::BONUS_ACTIVATION, 0);
		command_module.set_command(CommandModule::BOOST, 0);


		if (my_ship->get_bonus_slot()->get_current_bonus() == Bonus::IMMORTALITY) {

			for (auto bullet : environment.projectiles) {


				auto bullet_ = bullet->get_body();
				auto bullet_r = bullet_->GetFixtureList()->GetShape()->m_radius;
				auto ship_ = my_ship->get_body();
				auto ship_r = ship_->GetFixtureList()->GetShape()->m_radius;



				if (bullet->get_player()->is_hostile_to(*my_ship->get_player())
					&& b2Distance(bullet_->GetPosition(), ship_->GetPosition()) < bullet_r + ship_r + 0.5f) {
					command_module.set_command(CommandModule::BONUS_ACTIVATION, 1);
					break;
				}
			}
		}

		Ship* enemy = get_enemy(my_ship);
		if (enemy != nullptr) {
			attack(enemy, my_ship);
		}
		else if (!get_bonuses(my_ship)) {
			safety_flight(my_ship);
		}


    }
    else {
        command_module.set_command(CommandModule::RESPAWN, 1);
    }
}