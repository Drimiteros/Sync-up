#include "Networking_server.h"

using namespace std;
using namespace sf;

Networking_server::Networking_server() : port(25565) {}

Networking_server::~Networking_server() {
    stop();
}

void Networking_server::start() {
    if (isRunning) return;
    isRunning = true;

    serverThread = thread(&Networking_server::run, this);
}

void Networking_server::run() {
    if (listener.listen(port) != Socket::Done) {
        cout << "Failed to bind server on port: " << port << endl;
        isRunning = false;
        return;
    }

    selector.add(listener);
    cout << "Server started on port: " << port << endl;

    while (isRunning) {
        if (selector.wait(milliseconds(100))) {
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
            else {
                for (auto it = clients.begin(); it != clients.end();) {
                    TcpSocket& client = **it;
                    if (selector.isReady(client)) {
                        Packet packet;
                        if (client.receive(packet) == Socket::Done) {
                            string message;
                            packet >> message;

                            // Restore message handling and thread safety
                            lock_guard<mutex> lock(serverMutex);
                            clientMessageString += message;
                            cout << "Received: " << message << endl;

                            // Echo reply back to the client so receiveMessage() picks it up
                            if (message.find("status") != string::npos) {
                                Packet reply;
                                string replyMessage = "Message received";
                                reply << replyMessage;
                                client.send(reply);
                                cout << "Server echo: " << replyMessage << endl;
                            }
                            it++;
                        }
                        else {
                            cout << "Client disconnected." << endl;
                            selector.remove(client);
                            it = clients.erase(it);
                            clientCount--;
                        }
                    }
                    else {
                        it++;
                    }
                }
            }
        }
    }

    selector.clear();
    listener.close();
    for (auto& client : clients) {
        client->disconnect();
    }
    clients.clear();
    cout << "Server stopped cleanly." << endl;
}

void Networking_server::stop() {
    if (!isRunning) return;
    isRunning = false;
    listener.close();

    if (serverThread.joinable()) {
        serverThread.join();
    }
}