#include "Cursor_input.h"

using namespace std;
using namespace sf;

Cursor_input::Cursor_input() {
    cursor_texture.loadFromFile("src/Textures/cursor.png");

    cursor_sprite.setTexture(cursor_texture);
    cursor_sprite.setScale(0.1, 0.1);
}

void Cursor_input::update(RenderWindow& window) {
    move(window);
    left_click();
    right_click();
}

void Cursor_input::move(RenderWindow& window) {
    cursor_sprite.setPosition(window.mapPixelToCoords(Vector2i(Mouse::getPosition(window))));
}

void Cursor_input::left_click() {
    //Click event
    if (Mouse::isButtonPressed(Mouse::Left) && !isLeftClicked && resetLeftClick) {
        isLeftClicked = true;
        resetLeftClick = false;
    }
    else if (!resetLeftClick) {
        isLeftClicked = false;
    }
    if (!Mouse::isButtonPressed(Mouse::Left) && !resetLeftClick) {
        resetLeftClick = true;
    }

    //Long Click event
    if (Mouse::isButtonPressed(Mouse::Left) && !isLeftLongClicked && resetLeftLongClick) {
        isLeftLongClicked = true;
        resetLeftLongClick = false;
    }
    else if (!Mouse::isButtonPressed(Mouse::Left) && !resetLeftLongClick) {
        isLeftLongClicked = false;
        resetLeftLongClick = true;
    }
}

void Cursor_input::right_click() {
    //Click event
    if (Mouse::isButtonPressed(Mouse::Right) && !isRightClicked && resetRightClick) {
        isRightClicked = true;
        resetRightClick = false;
    }
    else if (!resetRightClick) {
        isRightClicked = false;
    }
    if (!Mouse::isButtonPressed(Mouse::Right) && !resetRightClick) {
        resetRightClick = true;
    }

    //Long Click event
    if (Mouse::isButtonPressed(Mouse::Right) && !isRightLongClicked && resetRightLongClick) {
        isRightLongClicked = true;
        resetRightLongClick = false;
    }
    else if (!Mouse::isButtonPressed(Mouse::Right) && !resetRightLongClick) {
        isRightLongClicked = false;
        resetRightLongClick = true;
    }
}

void Cursor_input::draw_ui_layer(RenderTarget& window) {
    window.draw(cursor_sprite);
}