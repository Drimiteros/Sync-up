#pragma once
#include <iostream>
#include <future>
#include <chrono>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "UI_components.h"

class Networking_client {
private:

protected:
    struct CheckConnectionStruct {
        sf::TcpSocket socket;
        int port = 25565;
        std::string ip = "127.0.0.1";

        std::future<bool> checkFuture;
        sf::Clock checkClock;
        sf::Time checkInterval = sf::seconds(5.0f);

        std::atomic<bool> isConnected { false };
        std::atomic<bool> isChecking { false };
    };
    CheckConnectionStruct connection;

    friend class UI_components;

public:
    Networking_client();
    bool checkConnectionTask();
    bool sendMessage(std::string message);
    std::string receiveMessage();
    bool connect();
    void update();
    bool getIsConnected() const { return connection.isConnected; }
};

