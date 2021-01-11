#pragma once
#include "app/window.h"

class WaitForConnectionWindow : public Window {
public:
    using Window::Window;
    void Update() override;
    void PoolEvents() override;
    bool IsReady() const;
    void SetNotReady(std::string message="");

    std::string GetIP() const;

private:
    std::string message_;
    std::string string_;
    sf::Text text;
    bool ready_ = false;
};