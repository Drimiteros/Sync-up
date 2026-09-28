#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Networking_server {
private:

protected:
	TcpListener listener;
	SocketSelector selector;
	std::vector<std::unique_ptr<sf::TcpSocket>> clients;

	unsigned short port;
	atomic<bool> isRunning{false};

public:
	Networking_server();
	void run();
	void stop();
};

