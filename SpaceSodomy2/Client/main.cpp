#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

int main()
{
    sf::RenderWindow window{ {800, 600}, "TGUI example - SFML_GRAPHICS backend" };
    tgui::Gui gui{ window };
    auto group1 = tgui::Group::create();
    tgui::setResourcePath("gui_resources");
    group1->loadWidgetsFromFile("form.txt");
    gui.add(group1);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        gui.draw();

        window.display();
    }
}