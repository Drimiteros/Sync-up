#include "Networking_server.h"

using namespace std;
using namespace sf;

Networking_server::Networking_server() {

}

void Networking_server::update() {
	async(launch::async, &Networking_server::run, this);
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
					selector.add(*client);
					clients.push_back(move(client));
				}
			}
			// Check existing clients for incoming data
			else {
				for (auto it = clients.begin(); it != clients.end();) {
					TcpSocket& client = **it;
					if (selector.isReady(client)) {
						Packet packet;
						Socket::Status status = client.receive(packet);
						if (status == Socket::Done) {
							// Receive message
							string message;
							packet >> message;
							clientMessageString = clientMessageString + message;
							cout << "Received: " << packet << endl;

							// Send message
							if (message.find("status") != string::npos) {
								Packet reply;
								String replyMessage = "";
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

		// Cleanup on exit
		stop();
		return 1;
	}
}

void Networking_server::stop() {
	listener.close();
	for (auto& client : clients) { client->disconnect(); }
	clients.clear();
	isRunning = false;
}