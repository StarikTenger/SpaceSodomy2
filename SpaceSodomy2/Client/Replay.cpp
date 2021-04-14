#include "Replay.h"

Replay::Replay() {}

Replay::Replay(std::string path) {
	set_replay_path(path);
}

void Replay::set_replay_path(std::string path) {
	std::ifstream file(path);
	std::string k;
	while (!getline(file, k).eof()) {
		frames.push_back(k);
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
		return;
	}
	if (replay_frame.get_change_vel() > b2_epsilon) {
		if (replay_frame.get_change_vel() < 0.5 + b2_epsilon)
			replay_frame.set_change_vel(0);
		else
			replay_frame.set_change_vel(replay_frame.get_change_vel() / 2);
	}
}

std::string Replay::get_cur_frame() {
	return (frames[replay_frame.get()]);
}

Counter* Replay::get_replay_frame() {
	return &replay_frame;
}