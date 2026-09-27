#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Gun;

class Cursor_input {
private:
    sf::Texture cursor_texture;

public:
    sf::Sprite cursor_sprite;
    sf::Vector2f position;
    bool resetLeftClick = true;
    bool resetLeftLongClick = true;
    bool isLeftClicked = false;
    bool isLeftLongClicked = false;
    bool resetRightClick = true;
    bool resetRightLongClick = true;
    bool isRightClicked = false;
    bool isRightLongClicked = false;

    Cursor_input();
    void update(sf::RenderWindow& window2);
    void move(sf::RenderWindow& window);
    void left_click();
    void right_click();
    void draw_ui_layer(sf::RenderTarget& window);
};

