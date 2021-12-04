#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

int main()
{
    sf::RenderWindow window{ {800, 600}, "TGUI example - SFML_GRAPHICS backend" };
    tgui::Gui gui{ window };
    tgui::Button::Ptr button = tgui::Button::create();
    auto x = tgui::BoxLayout::create();
    auto editBox = tgui::EditBox::create();
    editBox->setPosition(20, 0);
    x->setPosition(20, 20);
    x->add(button);
    x->add(editBox);
    gui.add(x);
    //gui.add(button);
    //gui.add(editBox, "MyWidgetName");
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