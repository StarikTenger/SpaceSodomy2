#include "Notification.h"

Notification::Notification() {}

Notification::Notification(tgui::Gui* _gui, std::string _HUD_effect_name, Draw* _draw, Audio* _audio, std::string _audio_effect_name)  {
	gui = _gui;
	audio = _audio;
	HUD_effect_name = _HUD_effect_name;
	draw = _draw;
	audio_effect_name = _audio_effect_name;
}

void Notification::play() {
	audio->update_sound(aux::random_int(0, 1000), audio_effect_name, draw->get_camera()->get_pos(), 1, 0);

	auto ans = tgui::Group::create();
	ans->loadWidgetsFromFile(HUD_effect_name);
	
	gui->get<tgui::Group>("HUD.txt")->add(ans);
	ans->onAnimationFinish([&ans]() {
		ans->getParent()->remove(ans);
	});
	ans->showWithEffect(tgui::ShowAnimationType::Fade, 500);
	//ans->setVisible(true);
}