#include "Networking_client.h"
using namespace std;
using namespace sf;

Networking_client::Networking_client() {

}

bool Networking_client::checkConnectionTask() {
    Packet pingPacket;
    pingPacket << "PING";
    Socket::Status status = connection.socket.send(pingPacket);

    if (status == Socket::Done)
        return true;
    else
        return false;
}

bool Networking_client::sendMessage(string message) {
    Packet packet;
    packet << message;
    Socket::Status status = connection.socket.send(packet);

    if (status == Socket::Done)
        return true;
    else
        return false;
}

string Networking_client::receiveMessage() {
    return "test";
}

bool Networking_client::connect() {
    cout << "Connecting to server..." << endl;
    if (connection.socket.connect(connection.ip, connection.port) == Socket::Done) {
        cout << "Connected to server on port: " << connection.port << endl;
        connection.isConnected = true;
        return true;
    }
    else {
        cout << "Failed to connected to server..." << endl;
        connection.isConnected = false;
        return false;
    }
}

void Networking_client::update() {
    // Check connection status every 5 sec
    if (connection.isChecking && connection.checkFuture.valid()) {
        // wait_for(0) asks the future for its result with zero extra delay.
        // If the task has finished, the result is ready and we grab it; if
        // it hasn't finished yet this returns early and we simply wait for
        // the next frame to handle it.
        if (connection.checkFuture.wait_for(chrono::seconds(0)) == future_status::ready) {
            connection.isConnected = connection.checkFuture.get();
            connection.isChecking = false;
            connection.checkClock.restart();
            cout << "Result: " << connection.isConnected << endl;
        }
    }

    if (!connection.isChecking && connection.checkClock.getElapsedTime() >= connection.checkInterval) {
        cout << "Checking connection..." << endl;
        connection.isChecking = true;
        // Launch the next background check and replace the future handle
        // with the new task's handle, so the next frame's wait_for sees it.
        connection.checkFuture = async(launch::async, &Networking_client::checkConnectionTask, this);
    }
}
