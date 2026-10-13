#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <future>

class Networking_server {
private:

protected:
	std::string password;
	std::string confirm_password;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	int port;
	std::vector<std::unique_ptr<sf::TcpSocket>> clients;
	std::atomic<bool> isRunning{false};
	std::string clientMessageString;
	std::future<bool> serverFuture;
	std::mutex serverMutex;
	int clientCount = 0;

	friend class Main_loop;
	friend class UI_components;

public:
	Networking_server();
	void update();
	bool run();
	void stop();
	void cleanup();
};

