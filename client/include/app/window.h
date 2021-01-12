#pragma once

#include "SFML/Graphics.hpp"
#include <string>

class Window {
public:
    Window(sf::VideoMode, std::string, const sf::Font &);
    virtual ~Window() = default;
    virtual void Open();
    virtual void Close();
    virtual bool IsOpen() const;
    virtual void Update() = 0;
    virtual void PoolEvents() = 0;
private:
    sf::VideoMode vm_;
    std::string title_;
protected:
    sf::RenderWindow window_;
    const sf::Font & kFont;
};