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
    std::string string_ = "127.0.0.1";
    sf::Text text_;
    bool ready_ = false;
};