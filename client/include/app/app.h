#include "SFML/Graphics.hpp"
#include <map>
#include <memory>

#include "networking/windowstcpclient.h"
#include "app/waitforconnectionwindow.h"
#include "app/playingwindow.h"
#include "common/utils/timer.h"

class App {
public:
    App();
    void Run();
private:
    enum class AppState {
        kWaitingForConnection,
        kTryingToConnect,
        kPlaying
    };
    void AppLoop();
    sf::Font font_;
    AppState state_;
    std::unique_ptr<WaitForConnectionWindow> connection_;
    std::unique_ptr<PlayingWindow> playing_;
    Window * current_window_ = nullptr;
    std::unique_ptr<TCPClient> client_;
    Timer timer_;
};