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

void Replay::set_speed(float val) {
	replay_frame.set_change_vel(val);
	float eps = 0.0001;
	if (val > eps || val < -eps)
		speed_backup = val;
}

void Replay::play_button(tgui::Gui &gui) {
	auto spin_control = gui.get<tgui::SpinControl>("SpinControl");
	float eps = 0.0001;
	if (spin_control->getValue() < eps && spin_control->getValue() > -eps) {
		if (speed_backup < eps && speed_backup > -eps) {
			spin_control->setValue(1);
		}
		else {
			spin_control->setValue(speed_backup);
		}
	}
	else {
		speed_backup = spin_control->getValue();
		spin_control->setValue(0);
	}
}

void Replay::set_replay_active(bool val) {
	replay_active = val;
}

std::string Replay::get_cur_frame() {
	return (frames[(int)(replay_frame.get() / dt)]);
}

Counter* Replay::get_replay_frame() {
	return &replay_frame;
}

bool Replay::get_replay_active() {
	return replay_active;
}

float Replay::get_seconds() {
	return replay_frame.get() * dt;
}

void Replay::step(float _dt) {
	dt = _dt;
	replay_frame.step(dt);
}
