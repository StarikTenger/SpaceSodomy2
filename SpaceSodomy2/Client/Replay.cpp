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
	playback_time.set_max((frames.size() - 2) * dt); // -1-1 cause last replay string may be shchwached
	frame_number = frames.size() - 2;
	std::cout << playback_time.get_max() << " max\n";
}

void Replay::set_speed(float val) {
	playback_time.set_change_vel(val);
	float eps = 0.0001;
	if (val > eps || val < -eps)
		speed_backup = val;
}

void Replay::play_button(tgui::Gui &gui) {
	auto spin_control = gui.get<tgui::SpinControl>("SpinControl");
	is_paused = !is_paused;
}

void Replay::set_frame(int frame_number) {
	playback_time.set(frame_number * dt);
	// std::cout << replay_frame.get() << " set_frame\n";
}

void Replay::set_frame_number(int _frame_number) {
	frame_number = _frame_number;
}

void Replay::set_replay_active(bool val) {
	replay_active = val;
}

int Replay::get_cur_frame_number() {
	return (int)(playback_time.get() / dt);
}

std::string Replay::get_cur_frame() {	
	return (frames[get_cur_frame_number()]);
}

Counter* Replay::get_replay_frame() {
	return &playback_time;
}

bool Replay::get_replay_active() {
	return replay_active;
}

float Replay::get_seconds() {
	return playback_time.get();
}

int Replay::get_frame_number() {
	return frame_number;
}

void Replay::step(float _dt) {
	float real_dt = aux::get_milli_count() * 0.001 - last_frame_time;
	last_frame_time = aux::get_milli_count() * 0.001;
	dt = _dt;
	// Modify counter
	if (!is_paused)
		playback_time.step(real_dt);
}
