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
        if(ev.type == sf::Event::Closed || ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape){
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
                if(string_.size() >= 15)
                    continue;
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
    text_.setFont(kFont); // font is a sf::Font

    // set the string to display
    text_.setString(string_);

    // set the character size
    text_.setCharacterSize(54); // in pixels, not points!

    // set the color
    text_.setFillColor(sf::Color::Red);

    // set the text style
    text_.setStyle(sf::Text::Bold);

    auto size = window_.getSize();
    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
    text_.setPosition(sf::Vector2f(size.x/2.0f,size.y/2.0f));

    window_.clear();
    window_.draw(text_);
    window_.display();
}

bool WFCW::IsReady() const {
    return ready_;
}

void WFCW::SetNotReady(std::string message) {
    message_ = message;
    ready_ = false;
}

std::string WFCW::GetIP() const {
    return string_;
}