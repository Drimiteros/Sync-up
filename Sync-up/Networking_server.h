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
	std::future<bool> serverFuture;
	std::mutex serverMutex;
	int clientCount = 0;

public:
	Networking_server();
	void update(unsigned short customPort);
	bool run();
	void stop();
	void cleanup();
};

