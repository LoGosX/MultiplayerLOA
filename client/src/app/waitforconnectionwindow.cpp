#include "app/waitforconnectionwindow.h"
#include "common/utils/ip.h"

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
            auto & string_ = (serverIpSelected_ ? serverIpString_ : clientIpString_);
            if(k == sf::Keyboard::BackSpace && !string_.empty()){
                string_.pop_back();
            }else if(k == sf::Keyboard::Enter){
                if(!validateIP(serverIpString_)){
                    SetMessage("Invalid server IP address");
                }else if(!validateIP(clientIpString_)){
                    SetMessage("Invalid client IP address");
                }else
                    ready_ = true;
            }else if(k == sf::Keyboard::Up && !serverIpSelected_){
                serverIpSelected_ = true;
            }
            else if(k == sf::Keyboard::Down && serverIpSelected_){
                serverIpSelected_ = false;
            }
            else{
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

void CenterText(sf::Text & text) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
}

void WFCW::Update() {
    const auto size = window_.getSize();
    window_.clear();

    text_.setFont(kFont); // font is a sf::Font
    text_.setCharacterSize(54); // in pixels, not points!
    text_.setFillColor(sf::Color::Red);
    text_.setStyle(sf::Text::Bold);
    sf::RectangleShape outline;
    outline.setOutlineThickness(5);
    outline.setOutlineColor(sf::Color::Red);
    outline.setFillColor(sf::Color::Black);
    outline.setSize({(float)size.x, 60.f});
    outline.setOrigin(outline.getSize() / 2.f);

    text_.setString(message_);
    CenterText(text_);
    text_.setPosition(sf::Vector2f(size.x/2.0f,size.y/2.0f - 300));
    window_.draw(text_);

    text_.setString("Server address: " + serverIpString_);
    CenterText(text_);
    text_.setPosition(sf::Vector2f(size.x/2.0f,size.y/2.0f - 100));

    if(serverIpSelected_) {
        outline.setPosition(text_.getPosition());
        window_.draw(outline);
    }
    window_.draw(text_);

    text_.setString("Client address: " + clientIpString_);
    CenterText(text_);
    text_.setPosition(sf::Vector2f(size.x/2.0f,size.y/2.0f + 100));
    if(!serverIpSelected_) {
        outline.setPosition(text_.getPosition());
        window_.draw(outline);
    }
    window_.draw(text_);

    window_.display();
}

bool WFCW::IsReady() const {
    return ready_;
}


std::string WFCW::GetServerIP() const {
    return serverIpString_;
}

std::string WFCW::GetClientIP() const {
    return clientIpString_;
}

void WFCW::Reset(std::string message) {
    message_ = message;
    serverIpString_ = "127.0.0.1";
    clientIpString_ = "127.0.0.1";
    ready_ = false;
    serverIpSelected_ = true;
}

void WFCW::SetMessage(std::string n) {
    message_ = n;
}