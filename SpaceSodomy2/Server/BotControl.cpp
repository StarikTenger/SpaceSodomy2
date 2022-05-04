#include "BotControl.h"

BotControl::BotControl(std::string bot_name, ShipBrain::Type type, const GameReadable& env) {
	name = bot_name;
	switch (type) {
	case ShipBrain::Type::EDGAR_BRAIN:
		brain = new EdgarBrain(cmd_module, env, id);
		break;
	}
}

void BotControl::set_equip(std::string bot_name, ShipBrain::Equip equipment) {
	name = bot_name;
	brain->suggest_equip(equipment);
}


std::string BotControl::get_message() {

	brain->compute_action();


	std::string message = "";
	message + brain->get_gun_name() + " ";
	message += brain->get_hull_name() + " ";
	message += brain->get_left_module_name() + " ";
	message += brain->get_right_module_name() + " ";
	message += "#";
	for (int i = 0; i < CommandModule::COMMAND_COUNT; i++) {
		if (cmd_module.get_command(i))
			message += "1";
		else
			message += "0";
	}

	message =  quasi_IP_addr + " " +  
		std::to_string(id) + " " +
		std::to_string(aux::get_milli_count()) + " " +
		name + " " + 
		std::to_string(token) + " " +
		message;

	return message;
}
