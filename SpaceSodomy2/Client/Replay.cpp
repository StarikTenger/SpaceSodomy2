#include "Replay.h"

Replay::Replay() {}

Replay::Replay(std::string path) {
	set_replay_path(path);
}

void Replay::set_replay_path(std::string path) {
	frames.clear();
	std::ofstream end(path, std::ios::app | std::ios::binary);
	end << '\0';
	end.close();
	std::ifstream file(path, std::ios::binary);
	while (file.peek() != '\0') {
		std::string msg;
		int msg_size = aux::read_short(file);
		if (msg_size < 0) {
			break;
		}
		msg += aux::write_short(msg_size);
		for (int i = 0; i < msg_size; i++) {
			msg.push_back(file.get());
		}
		frames.push_back(msg);
	}
	replay_frame.set_max(frames.size() - 2);
}

void Replay::increase_speed() {
	if (replay_frame.get_change_vel() > -b2_epsilon && replay_frame.get_change_vel() < b2_epsilon) {
		replay_frame.set_change_vel(0.5);
		return;
	}
	if (replay_frame.get_change_vel() > b2_epsilon) {
		replay_frame.set_change_vel(replay_frame.get_change_vel() * 2);
		if (replay_frame.get_change_vel() > 64.0) {
			replay_frame.set_change_vel(64.0);
		}
		return;
	}
	if (replay_frame.get_change_vel() < -b2_epsilon) {
		if (replay_frame.get_change_vel() > -0.5 - b2_epsilon)
			replay_frame.set_change_vel(0);
		else
			replay_frame.set_change_vel(replay_frame.get_change_vel() / 2);
	}
}

void Replay::decrease_speed() {
	if (replay_frame.get_change_vel() > -b2_epsilon && replay_frame.get_change_vel() < b2_epsilon) {
		replay_frame.set_change_vel(-0.5);
		return;
	}
	if (replay_frame.get_change_vel() < -b2_epsilon) {
		replay_frame.set_change_vel(replay_frame.get_change_vel() * 2);
		if (replay_frame.get_change_vel() < -64.0) {
			replay_frame.set_change_vel(-64.0);
		}
		return;
	}
	if (replay_frame.get_change_vel() > b2_epsilon) {
		if (replay_frame.get_change_vel() < 0.5 + b2_epsilon)
			replay_frame.set_change_vel(0);
		else
			replay_frame.set_change_vel(replay_frame.get_change_vel() / 2);
	}
}

void Replay::set_replay_active(bool val) {
	replay_active = val;
}

std::string Replay::get_cur_frame() {
	return (frames[replay_frame.get()]);
}

Counter* Replay::get_replay_frame() {
	return &replay_frame;
}

bool Replay::get_replay_active() {
	return replay_active;
}