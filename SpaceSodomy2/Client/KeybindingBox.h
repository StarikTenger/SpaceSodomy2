#pragma once
#include <AuxLib/AuxLib.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
class KeybindingBox : public tgui::EditBox /* base type depends on approach */
{
public:
    // Add a Ptr typedef so that "MyCustomWidget::Ptr" has the correct type
    typedef std::shared_ptr<KeybindingBox> Ptr;
    typedef std::shared_ptr<const KeybindingBox> ConstPtr;

    // Add a constructor that sets the type name. This is the string that would be returned when calling getWidgetType().
    // We assume that we can keep the renderer of the base class in this code. See the "Changing renderer" section later
    // for an explanation on how to make changes to the renderer as well.
    KeybindingBox(const char* typeName = "KeybindingBox", bool initRenderer = true)
    {
        tgui::EditBox(typeName, initRenderer);
    }

    // Copy constructors, assignment operators and destructor can be added if required for the members that
    // you add to the custom widget. These functions are not required by default.

    // Every widget needs a static create function which creates the widget and returns a smart pointer to it.
    // This function can have any parameters you want.

    std::vector<std::vector<std::string*>>* keys;
    bool* reload;

    tgui::Signal onKeyPressed = { "keyPressed" };

    tgui::Signal& getSignal(tgui::String signalName) override
    {
        if (signalName == onKeyPressed.getName())
            return onKeyPressed;
        else
            return EditBox::getSignal(std::move(signalName));  // Replace "Widget" with the class you inherited from
    }

    static KeybindingBox::Ptr create()
    {
        return std::make_shared<KeybindingBox>();
    }

    // Every widget has a static method to copy it
    static KeybindingBox::Ptr copy(KeybindingBox::ConstPtr widget)
    {
        if (widget)
            return std::static_pointer_cast<KeybindingBox>(widget->clone());
        else
            return nullptr;
    }

    void textEntered(char32_t key) {}

    void keyPressed(const tgui::Event::KeyEvent& event) {
        std::cout << (int)event.code;
        aux::Keyboard keys;
        std::cout << "pressed " << keys.names[(int)event.code] << "\n";
        this->setText(keys.names[(int)event.code]);
        onKeyPressed.emit(this);
        onReturnKeyPress.emit(this, m_text);
        onReturnOrUnfocus.emit(this, m_text);
    }

protected:

    // Every widget needs a method to copy it
    Widget::Ptr clone() const override
    {
        return std::make_shared<KeybindingBox>(*this);
    }
};

