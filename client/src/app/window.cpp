#include "app/window.h"

Window::Window(sf::VideoMode vm, std::string title, const sf::Font & font) :
    vm_(vm), title_(title), kFont(font) {

    }

void Window::Open() {
    window_.create(vm_, title_, sf::Style::Titlebar | sf::Style::Close);
}

void Window::Close() {
    window_.close();
}

bool Window::IsOpen() const {
    return window_.isOpen();
}