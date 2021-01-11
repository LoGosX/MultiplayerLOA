#include "app/waitforconnectionwindow.h"

using WFCW = WaitForConnectionWindow;

bool IsAlpha(sf::Keyboard::Key k) {
    return k >= sf::Keyboard::A && k <= sf::Keyboard::Z;
}

bool IsNumeric(sf::Keyboard::Key k) {
    return k >= sf::Keyboard::Num0 && k <= sf::Keyboard::Num9;
}

void WFCW::PoolEvents() {
    sf::Event ev;
    while(window_.pollEvent(ev)) {
        if(ev.type == sf::Event::Closed){
            window_.close();
            return;
        }
        if(ev.type == sf::Event::KeyPressed) {
            auto k = ev.key.code;
            if(k == sf::Keyboard::BackSpace && !string_.empty()){
                string_.pop_back();
            }else if(k == sf::Keyboard::Enter){
                ready_ = true;
            }else{
                if(k == sf::Keyboard::Key::Period){
                    string_.push_back('.');
                }else if(IsNumeric(k)){
                    string_.push_back(k - sf::Keyboard::Num0 + '0');
                }
            }
        }
    }
}

void WFCW::Update() {
    // select the font
    text.setFont(kFont); // font is a sf::Font

    // set the string to display
    text.setString(string_);

    // set the character size
    text.setCharacterSize(24); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::Red);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);


    window_.clear();
    window_.draw(text);
    window_.display();
}

bool WFCW::IsReady() const {
    return ready_;
}

void WFCW::SetNotReady(std::string message) {
    message_ = message;
    ready_ = false;
}