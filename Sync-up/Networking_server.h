#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <memory>

class Networking_server {
private:
    std::string password;
    std::string confirm_password;
    sf::TcpListener listener;
    sf::SocketSelector selector;
    int port;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::atomic<bool> isRunning{ false };
    std::thread serverThread;
    std::mutex serverMutex;
    int clientCount = 0;
    std::string clientMessageString;

    void run();

    friend class Main_loop;
    friend class UI_components;

public:
    Networking_server();
    ~Networking_server();

    void start();
    void stop();
};