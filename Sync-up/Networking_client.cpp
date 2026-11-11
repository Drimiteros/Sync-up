#include "Networking_client.h"
using namespace std;
using namespace sf;

Networking_client::Networking_client() {

}

bool Networking_client::checkConnectionTask() {
    Packet pingPacket;
    pingPacket << "PING";
    Socket::Status status = socket.send(pingPacket);

    if (status == Socket::Done)
        return true;
    else
        return false;
}

bool Networking_client::authenticateUser() {
    Packet authenticationPacket;
    authenticationPacket << "password:" + serverPass;
    Socket::Status status = socket.send(authenticationPacket);

    if (status == Socket::Done)
        return true;
    else
        return false;
}

bool Networking_client::sendMessage(string message) {
    Packet packet;
    packet << message;
    Socket::Status status = socket.send(packet);

    if (status == Socket::Done)
        return true;
    else
        return false;
}

string Networking_client::receiveMessage() {
    Packet packet;
    Socket::Status status = socket.receive(packet);

    if (status == Socket::Done) {
        string receivedData;
        if (packet >> receivedData)
            return receivedData;
    }
    return "";
}

bool Networking_client::connect() {
    cout << "Connecting to server..." << endl;
    if (socket.connect(ip, port) == Socket::Done) {
        cout << "Connected to server on port: " << port << endl;
        connection.isConnected = true;
        return true;
    }
    else {
        cout << "Failed to connected to server..." << endl;
        socket.disconnect();
        connection.isConnected = false;
        return false;
    }
}

bool Networking_client::triggerUIRebuild(Mode& currentMode) {
    // Kick client
    if (message.receivedMessage == "Passwords do not match!" && authenticate.isAuthenticated) {
        authenticate.checkClock.restart();
        authenticate.isChecking = false;
        authenticate.isAuthenticated = false;
        connection.checkClock.restart();
        connection.isChecking = false;
        connection.isConnected = false;
        message.checkClock.restart();
        message.receivedMessage == "";
        message.isChecking = false;
        message.isReceived = false;
        currentMode = Mode::Neutral;
        return true;
    }
    else
        return false;
}

void Networking_client::update() {
    // One time authentication with the server
    if (!authenticate.isChecking && authenticate.checkClock.getElapsedTime() >= authenticate.checkInterval) {
        cout << "Authenticating..." << endl;
        authenticate.isChecking = true;
        authenticate.checkFuture = async(launch::async, &Networking_client::authenticateUser, this);
    }
    if (authenticate.isChecking && authenticate.checkFuture.valid()) {
        if (authenticate.checkFuture.wait_for(chrono::seconds(0)) == future_status::ready) {
            authenticate.isAuthenticated = authenticate.checkFuture.get();
            authenticate.isChecking = false;
            authenticate.checkClock.restart();
        }
    }

    // Check connection status every 5 sec
    if (!connection.isChecking && connection.checkClock.getElapsedTime() >= connection.checkInterval) {
        cout << "Checking connection..." << endl;
        connection.isChecking = true;
        // Launch the next background check and replace the future handle
        // with the new task's handle, so the next frame's wait_for sees it.
        connection.checkFuture = async(launch::async, &Networking_client::checkConnectionTask, this);
    }
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

    // Check for new messages
    if (!message.isChecking && message.checkClock.getElapsedTime() >= message.checkInterval) {
        cout << "Checking for messages..." << endl;
        message.isChecking = true;
        message.checkFuture = async(launch::async, &Networking_client::receiveMessage, this);
    }
    if (message.isChecking && message.checkFuture.valid()) {
        if (message.checkFuture.wait_for(chrono::seconds(0)) == future_status::ready) {
            message.receivedMessage = message.checkFuture.get();
            message.isChecking = false;
            message.checkClock.restart();
            cout << "Received message: " << message.receivedMessage << endl;
        }
    }
}
