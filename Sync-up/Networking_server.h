#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <future>

class Networking_server {
private:

protected:
	sf::TcpListener listener;
	sf::SocketSelector selector;
	unsigned short port;
	std::vector<std::unique_ptr<sf::TcpSocket>> clients;
	std::atomic<bool> isRunning{false};
	std::string clientMessageString;

public:
	Networking_server();
	void update();
	bool run();
	void stop();
};

