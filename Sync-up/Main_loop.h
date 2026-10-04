#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "UI_components.h"
#include "Cursor_input.h"
#include "Networking_client.h"
#include "Networking_server.h"
#include "Enums.h"

class Main_loop {
private:
    UI_components ui_components;
    Cursor_input cursor_input;
    Networking_client networking_client;
    Networking_server networking_server;

    sf::RenderWindow window;
    unsigned int width = 1280;
    unsigned int height = 720;
    unsigned int MIN_WIDTH = 1280;
    unsigned int MIN_HEIGHT = 720;
    std::string title = "Sync-up";
    std::string version = "v1.0";
    unsigned short serverPort = 25565;

    void createWindow(unsigned int width, unsigned int height);
    void buildUI();
    void events();
    void update();
    void draw();

public:
    // Initialize mode
    Mode currentMode = Mode::Neutral;

    Main_loop();
    void loop();
};