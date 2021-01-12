#pragma once
#include "app/window.h"

class WaitForConnectionWindow : public Window {
public:
    using Window::Window;
    void Update() override;
    void PoolEvents() override;
    bool IsReady() const;
    void Reset(std::string message="");

    std::string GetServerIP() const;
    std::string GetClientIP() const;

    void SetMessage(std::string);
private:
    std::string message_;
    std::string serverIpString_ = "127.0.0.1";
    std::string clientIpString_ = "127.0.0.1";
    sf::Text text_;
    bool ready_ = false;
    bool serverIpSelected_ = true;
};