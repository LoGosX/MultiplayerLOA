#include "app/windowarea.h"

void WindowArea::SetSelected(bool v) {
    selected_ = v;
}

void WindowArea::Select() {
    SetSelected(true);
}

void WindowArea::Unselect() {
    SetSelected(false);
}

bool WindowArea::IsSelected() const {
    return selected_;
}

void WindowArea::SetMouseAbove(bool v) {
    mouse_above_ = v;
}

void WindowArea::MouseAbove() {
    SetMouseAbove(true);
}

void WindowArea::MouseNotAbove() {
    SetMouseAbove(false);
}

void WindowArea::AddCharacter(char c) {
    text_text_ += c;
}

void WindowArea::PopCharacter() {
    text_text_.pop_back();
}

void WindowArea::CenterText() {
    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
    text_.setPosition(500, 500);
}

sf::Vector2f WindowArea::GetSize() const {
    return shape_.getSize();
}

void WindowArea::SetSize(sf::Vector2f s) {
    shape_.setSize(s);
}

bool WindowArea::Contains(sf::Vector2f p) const {
    return shape_.getGlobalBounds().contains(p);
}

void WindowArea::OnKeyPress(sf::Keyboard::Key k) {
    if(!store_keys_)
        return;
    if(k == sf::Keyboard::Key::BackSpace || k == sf::Keyboard::Key::Delete){
        PopCharacter();
    }else {
        if(k >= sf::Keyboard::A && k <= sf::Keyboard::Num9){
            char c = k;
            if(k <= sf::Keyboard::Z){
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                c += 'A';
                }else{
                    c += 'a';
                }
            }else{
                c += '0';
            }
            AddCharacter(c);
        }
    }
}

void WindowArea::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape_);
    target.draw(text_);
}

void WindowArea::SetSelectable(bool b) {
    selectable_ = b;
}

bool WindowArea::IsSelectable() const {
    return selectable_;
}

void WindowArea::SetFont(const sf::Font & font) {
    text_.setFont(font);
}

void WindowArea::Update() {
    if(IsSelectable() && IsSelected()) {
        shape_.setOutlineColor(sf::Color::Black);
        shape_.setOutlineThickness(outline_thickness_);
    }
    text_.setString(text_text_);
    text_.setFillColor(sf::Color::White);
    text_.setCharacterSize(100);
    text_.setStyle(sf::Text::Bold);
    CenterText();
}

void WindowArea::SetOutlineThickness(float f) {
    outline_thickness_ = f;
}

void WindowArea::SetPosition(sf::Vector2f v) {
    shape_.setPosition(v);
}


std::string WindowArea::GetText() const {
    return text_text_;
}

void WindowArea::SetStoreKeys(bool v) {
    store_keys_ = v;
}
