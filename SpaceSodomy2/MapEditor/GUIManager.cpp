#include "GUIManager.h"


void GUIManager::init(tgui::Gui& gui) {
	auto load_widgets = [&gui](std::string file_name) {
		auto ans = tgui::Group::create();
		ans->loadWidgetsFromFile(file_name);
		ans->setVisible(false);
		gui.add(ans);
		ans->setWidgetName(file_name);
		return ans;
	};
	auto close_groups = [](tgui::Gui& gui) {
		auto wid = gui.getWidgets();
		for (int i = 0; i < wid.size(); i++) {
			wid[i]->setVisible(false);
		}
	};
	auto load_panel = [](tgui::Group::Ptr group, std::string file_name) {
		auto panel = tgui::Group::create();
		panel->loadWidgetsFromFile(file_name);
		panel->setVisible(false);
		group->add(panel);
		return panel;
	};
	enum ControlPanels { None, Polygon, Bonus, Tools };
	int cur_panel = 0;
	auto map_editor = load_widgets("editor.txt");
	map_editor->setVisible(true);

	auto load_group_buttons = [](std::vector<tgui::Widget::Ptr>& widgets) {
		for (int i = 0; i < widgets.size(); i++) {
			auto widget = widgets[i];
			if (widget->getWidgetType() == "Label")
				continue;
			widget->setMouseCursor(tgui::Cursor::Type::Hand);
			widget->onMouseEnter([=]() {
				tgui::PanelRenderer renderer(widget->getRenderer()->getData());
				renderer.getBorderColor() == tgui::Color::Black ? renderer.setBorderColor(tgui::Color::White) : void();
				});
			widget->onMouseLeave([=]() {
				tgui::PanelRenderer renderer(widget->getRenderer()->getData());
				renderer.getBorderColor() == tgui::Color::White ? renderer.setBorderColor(tgui::Color::Black) : void();
				});
			widget->onFocus([=]() {
				tgui::PanelRenderer renderer(widget->getRenderer()->getData());
				for (int j = 0; j < widgets.size(); j++) {
					tgui::PanelRenderer r(widgets[j]->getRenderer()->getData());
					if (r.getBorderColor() == tgui::Color("#66C0F4")) {
						r.setBorderColor(tgui::Color::Black);
					}
					widgets[j]->getRenderer()->setData(r.getData());
				}
				renderer.setBorderColor(tgui::Color("#66C0F4"));
				});
		}
	};
	std::vector<std::string> tools = { "Cursor", "Polygon", "Bonus" };
	for (int i = 0; i < tools.size(); i++) {
		auto widget = map_editor->get<tgui::Picture>(tools[i] + "Picture");
		widget->setMouseCursor(tgui::Cursor::Type::Hand);
		widget->onClick([=]() {
			for (int j = 0; j < tools.size(); j++) {
				tgui::PanelRenderer r(map_editor->get<tgui::Panel>(tools[j])->getRenderer()->getData());
				r.setBackgroundColor("#20283C");
				map_editor->get<tgui::Panel>(tools[j])->getRenderer()->setData(r.getData());
			}
			tgui::PanelRenderer renderer(map_editor->get<tgui::Panel>(tools[i])->getRenderer()->getData());
			renderer.setBackgroundColor(tgui::Color::Red);
			map_editor->get<tgui::Panel>(tools[i])->getRenderer()->setData(renderer.getData());
			});
	}
	// Initializing bonus panel
	auto bonus_panel = load_panel(map_editor->get<tgui::Group>("ToolSettings"), "BonusPanel.txt");
	auto load_sliders_vals([=](std::string name) {
		auto slider = map_editor->get<tgui::Slider>(name + "Slider");
		auto slider_val = map_editor->get<tgui::Label>(name + "SliderVal");
		slider_val->setText(std::to_string(int(slider->getValue())));
		slider->onValueChange([=]() {
			slider_val->setText(std::to_string(int(slider->getValue())));
			});
		});
	std::vector<std::string> bonuses = { "Berserk", "Charge", "Immortality" };
	for (int i = 0; i < bonuses.size(); i++) {
		load_sliders_vals(bonuses[i]);
	}
	auto bonus_panel_widgets = bonus_panel->getWidgets();
	load_group_buttons(bonus_panel_widgets);
	bonus_panel->setVisible(false);
	// Initializing polygon panel
	auto name_normailize = [](std::string name) {
		std::string new_name = "";
		new_name.push_back(std::toupper(name[0]));
		for (int i = 1; i < name.size(); i++) {
			if (name[i] != std::tolower(name[i])) {
				new_name.push_back(' ');
				new_name.push_back(std::tolower(name[i]));
			}
			else
				new_name.push_back(name[i]);
		}
		return new_name;
	};
	auto polygon_panel = load_panel(map_editor->get<tgui::Group>("ToolSettings"), "PolygonPanel.txt");
	auto polygon_panel_widgets = polygon_panel->getWidgets();
	auto walls_orientation = polygon_panel->get<tgui::Panel>("WallsOrientation");
	auto walls_orientation_widgets = walls_orientation->getWidgets();
	std::vector<std::string> walls_orientation_variants = { "innerWalls", "outerWalls" };
	for (int i = 0; i < walls_orientation_widgets.size(); i++) {
		if (walls_orientation_widgets[i]->getWidgetType() == "Label") {
			auto label = walls_orientation->get<tgui::Label>(walls_orientation_widgets[i]->getWidgetName());
			for (int j = 0; j < walls_orientation_variants.size(); j++) {
				std::cout << label->getText() << " " << name_normailize(walls_orientation_variants[j]) << "\n";
				if (label->getText() == name_normailize(walls_orientation_variants[j])) {
					label->setText(name_normailize(walls_orientation_variants[(j + 1) % walls_orientation_variants.size()]));
				}
			}
		}
		if (walls_orientation_widgets[i]->getWidgetType() == "Picture") {
			auto picture = walls_orientation->get<tgui::Picture>(walls_orientation_widgets[i]->getWidgetName());
			//std::cout << picture->getRenderer()->getTexture().getId();
			/*for (int j = 0; j < walls_orientation_variants.size(); j++) {
				if (label->getText() == name_normailize(walls_orientation_variants[j])) {
					label->setText(name_normailize(walls_orientation_variants[(j + 1) % walls_orientation_widgets.size()]));
				}
			}*/
		}
	}
	load_group_buttons(polygon_panel_widgets);
	polygon_panel->setVisible(true);
	// Initializing polygon union panel
	auto polygon_union_panel = load_panel(map_editor->get<tgui::Group>("ToolSettings"), "PolygonUnionPanel.txt");
	auto polygon_union_panel_widgets = polygon_union_panel->getWidgets();
	load_group_buttons(polygon_union_panel_widgets);
	polygon_union_panel->setVisible(false);
}