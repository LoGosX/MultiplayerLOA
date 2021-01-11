#pragma once

#include "SFML/Graphics.hpp"
#include <string>

/*
- input box
- clickable button
- selectable (for example, outline when selected)
- dragable (?) (probably not. Too much work)
*/

class WindowArea : public sf::Drawable {
public:
    void SetSelected(bool);
    void Select(); //SetSelected(true)
    void Unselect(); //SetSelected(false)
    bool IsSelected() const;

    void SetMouseAbove(bool);
    void MouseAbove(); //SetMouseAbove(true)
    void MouseNotAbove(); //SetMouseAbove(false)

    void OnKeyPress(sf::Keyboard::Key);

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

    sf::Vector2f GetSize() const;
    void SetSize(sf::Vector2f);
    void SetOutlineThickness(float);
    bool Contains(sf::Vector2f) const;

    bool IsSelectable() const;
    void SetSelectable(bool);
    void Update();

    void SetStoreKeys(bool);

    void SetPosition(sf::Vector2f);

    void SetFont(const sf::Font&);

    std::string GetText() const;
private:

    void AddCharacter(char c);
    void PopCharacter();
    void CenterText();

    float outline_thickness_ = 5;
    sf::Text text_;
    std::string text_text_;
    sf::RectangleShape shape_;

    bool selected_ = false;
    bool mouse_above_ = false;
    bool selectable_ = false;
    bool store_keys_ = false;
};