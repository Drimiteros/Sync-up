#include "Main_loop.h"
#include <algorithm>

using namespace std;
using namespace sf;

Main_loop::Main_loop() {
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
                    networking_client.connect() ? currentMode = Mode::Client : currentMode = Mode::Neutral;
                    ui_components.clear();
                    buildUI();
                }
            );
            ui_components.addButton(
                "Server Mode", "Host storage, watch connected\nclients and live logs", "server", 
                { 390, 250 }, { 0.49, 0.49 }, { 0.115, 0.115 }, { 20, 92 }, { 0, 30 }, { 20, 20 }, { 255, 255, 255 }, { 255, 255, 255 }, 1, 20, 0, 1, 15,
                [this]() {
                    currentMode = Mode::Server;
                    ui_components.clear();
                    buildUI();
                }
            );
            break;
        }
        case Mode::Server: {
            break;
        }
        case Mode::Client: {
            ui_components.addSidebar("Sidebar", { 5, 5 });
            ui_components.addButton("Files", "", "file", { 15, 15 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0);
            ui_components.addButton("Downloads", "", "download", { 15, 65 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 10 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0);
            ui_components.addButton("Transfers", "", "upload", { 15, 115 }, { 0.49, 0.49 }, { 0.07, 0.07 }, { 50, 12 }, { 0, 0 }, { 10, 12 }, { 255, 255, 255, 155 }, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0);
            ui_components.addButton("Settings", "", "gear", {15, 165}, {0.49, 0.49}, {0.1, 0.1}, {50, 12}, { 0, 0 }, {5, 4}, {255, 255, 255, 155}, { 0, 0, 0, 0 }, 0, 20, 0, 0, 0);
            ui_components.addConnectionStatus({20, 690});
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

    // Networking
    switch (currentMode) {
        case Mode::Neutral: {
            break;
        }
        case Mode::Server: {
            break;
        }
        case Mode::Client: {
            networking_client.update();
            ui_components.updateSidebar();
            ui_components.updateConnectionStatus(networking_client);
        }
    }
}

void Main_loop::draw() {
    ui_components.drawSideBar(window);
    ui_components.drawButtons(window);
    ui_components.drawConnectionStatus(window);
    cursor_input.draw_ui_layer(window);
}