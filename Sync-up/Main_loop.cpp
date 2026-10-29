#include "Main_loop.h"
#include <algorithm>

using namespace std;
using namespace sf;

Main_loop::Main_loop() {
    disk_operations.getDiskNames();
    createWindow(width, height);
    buildUI();
}

void Main_loop::createWindow(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    window.create(VideoMode(width, height), title + " " + version);
    window.setVerticalSyncEnabled(true);
}

void Main_loop::buildUI() {
    const float h = static_cast<float>(height);

    switch (currentMode) {
        case Mode::Neutral: {
            ui_components.addButton(
                "Client Mode", "Browse remote files, run\ntransfers and manage backups", "monitor", 
                { 650, 250 }, { 0.49, 0.49 }, { 0.115, 0.115 }, { 20, 92 }, { 0, 30 }, { 20, 20 }, { 255, 255, 255 }, { 255, 255, 255 }, 1, 20, 0, 1, 15,
                [this]() {
                    currentMode = Mode::ClientSetup;
                    ui_components.clear();
                    buildUI();
                }
            );
            ui_components.addButton(
                "Server Mode", "Host storage, watch connected\nclients and live logs", "server", 
                { 390, 250 }, { 0.49, 0.49 }, { 0.115, 0.115 }, { 20, 92 }, { 0, 30 }, { 20, 20 }, { 255, 255, 255 }, { 255, 255, 255 }, 1, 20, 0, 1, 15,
                [this]() {
                    currentMode = Mode::ServerSetup;
                    ui_components.clear();
                    buildUI();
                }
            );
            break;
        }
        case Mode::ServerSetup: {
            ui_components.addPlainText("passwordText", "        Let's setup your server!", 20, {float(width / 2) - 165, float(height / 2) - 300}, {1, 1}, {255, 255, 255, 255});
            ui_components.addPlainText("confirmPassText", "Please create a password to secure incoming connections.", 20, { float(width / 2) - 250, float(height / 2) - 270 }, { 1, 1 }, { 255, 255, 255, 155 });
            ui_components.addTextField(
                "port", "Server Port", networking_server.port, 25565,
                { float(width / 2) - 150, float(height / 2) - 150 }, { 300, 40 }, 20, 25
            );
            ui_components.addTextField(
                "pass", "Server Password", networking_server.password, "",
                { float(width / 2) - 150, float(height / 2) - 50 }, { 300, 40 }, 20, 25
            );
            ui_components.addTextField(
                "conf_pass", "Confirm Password", networking_server.confirm_password, "",
                { float(width / 2) - 150, float(height / 2) + 50 }, { 300, 40 }, 20, 25
            );
            ui_components.addPlainText("passMatchText", "a", 20, { float(width / 2) - 250, float(height / 2) + 120 }, { 1, 1 }, { 255, 255, 255, 155 });
            ui_components.addButton(
                "Finish", "", "check",
                { float(width / 2) - 110, float(height / 2) + 200 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 110, 12 }, { 0, 0 }, { 70, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 1, 0,
                [this]() {
                    currentMode = Mode::Server;
                    networking_server.start();
                    ui_components.clear();
                    buildUI();
                }
            );
            break;
        }
        case Mode::Server: {
            for (int i = 0; i < disk_operations.labels.size(); ++i) {
                ui_components.addButton(
                    wstring(1, disk_operations.drives[i]) + ":\\", disk_operations.labels[i], "drive",
                    { 10, 10 + float(i * 100) }, {0.49, 0.49}, {0.15, 0.15}, {50, 14}, {-40, 30}, {10, 7}, {255, 255, 255, 155}, {255, 255, 255, 100}, 2, 25, 0, 1, 15,
                    [this]() {}
                );
            }
            break;
        }
        case Mode::ClientSetup: {
            ui_components.addPlainText("0", "Please enter your credentials for the\n   current server you want to join", 20, { float(width / 2) - 165, float(height / 2) - 300 }, { 1, 1 }, { 255, 255, 255, 155 });
            ui_components.addButton(
                "Finish", "", "check",
                { float(width / 2) - 110, float(height / 2) + 200 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 110, 12 }, { 0, 0 }, { 70, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 1, 0,
                [this]() {
                    networking_client.connect() ? currentMode = Mode::Client : currentMode = Mode::Neutral;
                    ui_components.clear();
                    buildUI();
                }
            );
            ui_components.addTextField(
                "ip", "Server IP", networking_client.ip, "127.0.0.1",
                { float(width / 2) - 150, float(height / 2) - 100 }, { 300, 40 }, 20, 15, "0123456789."
            );
            ui_components.addTextField(
                "port", "Port", networking_client.port, 25565,
                { float(width / 2) - 150, float(height / 2) }, { 300, 40 }, 20, 65535
            );
            break;
        }
        case Mode::Client: {
            ui_components.addSidebar("Sidebar", { 5, 5 });
            ui_components.addConnectionStatus({ 20, 690 });
            ui_components.addButton(
                "Files", "", "file", 
                { 15, 15 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0,
                [this]() {
                    currentState = State::Files;
                    ui_components.clear();
                    buildUI();
                }
            );
            ui_components.addButton(
                "Downloads", "", "download", 
                { 15, 65 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0,
                [this]() {
                    currentState = State::Downloads;
                    ui_components.clear();
                    buildUI();
                }
            );
            ui_components.addButton(
                "Transfers", "", "upload", 
                { 15, 115 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 12 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0,
                [this]() {
                    currentState = State::Transfers;
                    ui_components.clear();
                    buildUI();
                }
            );
            ui_components.addButton(
                "Settings", "", "gear", 
                {15, 165}, {0.49, 0.49}, {0.1, 0.1}, {50, 12}, { 0, 0 }, {5, 4}, {255, 255, 255, 155}, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0,
                [this]() {
                    currentState = State::Settings;
                    ui_components.clear();
                    buildUI();
                }
            );
            
            if (currentState == State::Files) {
                ui_components.addButton(
                    "Send test message", "", "send",
                    { float(width / 2), float(height / 2) }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 1, 0,
                    [this]() {
                        networking_client.sendMessage("status");
                    }
                );
            }
            break;
        }
    }
}

void Main_loop::events() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
        if (event.type == Event::Resized) {
            createWindow(max(event.size.width, MIN_WIDTH), max(event.size.height, MIN_HEIGHT));
        }
        ui_components.updadteTextField(event, window);
    }
}

void Main_loop::loop() {
    while (window.isOpen()) {
        events();
        window.clear(Color(16, 12, 10));
        update();
        draw();
        window.display();
    }
}

void Main_loop::update() {
    // UIs
    cursor_input.update(window);
    ui_components.updateButtons(cursor_input);
    ui_components.updatePlainText(networking_server, window);

    // Networking
    switch (currentMode) {
        case Mode::Neutral: {
            break;
        }
        case Mode::ServerSetup: {

            break;
        }
        case Mode::Server: {
            break;
        }
        case Mode::Client: {
            networking_client.update();
            ui_components.updateSidebar();
            ui_components.updateConnectionStatus(networking_client);
            break;
        }
    }
}

void Main_loop::draw() {
    ui_components.drawSideBar(window);
    ui_components.drawButtons(window);
    ui_components.drawConnectionStatus(window);
    ui_components.drawPlainText(window);
    ui_components.drawTextFields(window);
    cursor_input.draw_ui_layer(window);
}