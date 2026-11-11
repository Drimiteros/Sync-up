#pragma once
#include <iostream>
#include <future>
#include <chrono>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "UI_components.h"
#include "Enums.h"

class Networking_client {
private:

protected:
    int port = 25565;
    std::string ip = "127.0.0.1";
    sf::TcpSocket socket;
    std::string serverPass;

    struct AuthenticateStruct {
        std::future<bool> checkFuture;
        std::atomic<bool> isChecking = { false };
        std::atomic<bool> isAuthenticated = { false };
        sf::Clock checkClock;
        sf::Time checkInterval = sf::seconds(0.1f);
    };
    AuthenticateStruct authenticate;

    struct CheckConnectionStruct {
        std::future<bool> checkFuture;
        sf::Clock checkClock;
        sf::Time checkInterval = sf::seconds(5.0f);
        std::atomic<bool> isConnected { false };
        std::atomic<bool> isChecking { false };
    };
    CheckConnectionStruct connection;

    struct ReceiveMessageStruct {
        std::future<std::string> checkFuture;
        sf::Clock checkClock;
        sf::Time checkInterval = sf::seconds(1.0f);
        std::atomic<bool> isChecking{ false };
        std::atomic<bool> isReceived{ false };
        std::string receivedMessage;
    };
    ReceiveMessageStruct message;

    friend class UI_components;
    friend class Main_loop;

public:
    Networking_client();
    bool checkConnectionTask();
    bool authenticateUser();
    bool sendMessage(std::string message);
    std::string receiveMessage();
    bool connect();
    bool triggerUIRebuild(Mode& currentMode);
    void update();
};

