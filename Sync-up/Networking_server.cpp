#include "Networking_server.h"

using namespace std;
using namespace sf;

Networking_server::Networking_server() {

}

void Networking_server::update() {
	if (!isRunning) {
		serverFuture = async(launch::async, &Networking_server::run, this);
	}
}

bool Networking_server::run() {
	if (listener.listen(port) != Socket::Done) {
		cout << "Failed to bind server on port: " << port << endl;
		return 0;
	}

	selector.add(listener);
	isRunning = true;
	cout << "Server started on port: " << port << endl;

	while (isRunning) {
		if (selector.wait(milliseconds(100))) {
			// Check for new connections
			if (selector.isReady(listener)) {
				auto client = make_unique<TcpSocket>();
				if (listener.accept(*client) == Socket::Done) {
					IpAddress clientIP = client->getRemoteAddress();
					cout << "New client added: " << clientIP.toString() << endl;
					selector.add(*client);
					clients.push_back(move(client));
					clientCount++;
				}
			}
			// Check existing clients for incoming data
			else {
				cout << "Active clients [" << clientCount << "]" << endl;
				for (auto it = clients.begin(); it != clients.end();) {
					TcpSocket& client = **it;
					if (selector.isReady(client)) {
						Packet packet;
						Socket::Status status = client.receive(packet);
						if (status == Socket::Done) {
							// Receive message
							string message;
							packet >> message;
							lock_guard<mutex>lock(serverMutex);
							clientMessageString += message;
							cout << "Received: " << message << endl;

							// Send message
							if (message.find("status") != string::npos) {
								Packet reply;
								string replyMessage = "Message received";
								reply << replyMessage;
								client.send(reply); 
								cout << "Server echo:  " << reply << endl;
							}
							it++;
						}
						else if (status == Socket::Disconnected || status == Socket::Error) {
							cout << "Client disconnected." << std::endl;
							selector.remove(client);
							it = clients.erase(it);
							clientCount--;
						}
						else {
							it++;
						}
					}
					else {
						it++;
					}
				}
			}
		}
	}
	// Cleanup on exit
	cleanup();
	return 1;
}

void Networking_server::stop() {
	isRunning = false;
}

void Networking_server::cleanup() {
	selector.clear();
	listener.close();
	for (auto& client : clients) {
		client->disconnect();
	}
	clients.clear();
}