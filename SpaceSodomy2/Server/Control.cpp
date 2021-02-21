#include "Control.h"

Control::Control() {
	//load data from config
}

void Control::receive() {
	network.receive();
	std::stringstream message;
	message << network.get_last_message();
	network.del_last_message();
	std::string IP_adress_, local_, name_;
	message >> IP_adress_;
	message >> local_;
	int id_ = atoi(local_.c_str());
	message >> local_;
	message >> name_;
	message >> local_;
	if (!addresses.count(IP_by_id[id_])) {
		addresses.insert(IP_adress_);
		IP_by_id[id_] = IP_adress_;
		time_by_IP[IP_adress_] = aux::get_milli_count();
		//********************************
		//TODO: add new player to the game
		//********************************
	}
	if (IP_by_id[id_] == IP_adress_)
		for (int i = 0; message >> local_; i++) {
			game.apply_command(id_, i, atoi(local_.c_str()));
		}
}

void Control::step() {
	receive();
	if (last_step_time - aux::get_milli_count() >= delay) {
		game.step();
		network.send(game.encode());
	}
}