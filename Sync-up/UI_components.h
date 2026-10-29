#pragma once
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include <climits>
#include "Cursor_input.h"
#include "Networking_client.h"
#include "Networking_server.h"

class Networking_client;

class UI_components {
private:
    sf::Font font;

public:
    // Sidebar
    struct SideBar {
        sf::Sprite sidebar;
        sf::Vector2f offset{ 0, 0 };
        sf::Vector2f size{ 200, 720 };
        sf::Color color{ 45, 45, 45 };
        std::string label = "sidebar";

        SideBar() {
            sidebar.setPosition(offset);
        }
    };
    std::vector<SideBar> sideBars;
    sf::Texture sidebarTexture;

    // Button
    struct Button {
        // Button
        sf::Sprite button;
        sf::Vector2f buttonOffset{ 0, 0 };
        sf::Vector2f buttonScale{ 1, 1 };
        sf::Color buttonColor{ 255, 255, 255, 4 };
        int hoverStyle = 0;
        int baseStyle = 0;

        // Title text
        sf::Text title;
        sf::Vector2f titlePosition;
        float titleSize = 10;
        sf::Vector2f titleOrigin{ 0, 0 };
        sf::Color titleColor{ 0, 0, 0 };
        sf::Vector2f titleScale = { 1, 1 };
        std::string titleString;

        // Description Text
        sf::Text description;
        sf::Vector2f descriptionPosition{ 0, 0 };
        float descriptionSize = 10;
        sf::Vector2f descriptionOrigin{ 0, 0 };
        sf::Font descriptionFont;
        sf::Color descriptionColor{ 0, 0, 0 };
        sf::Vector2f descriptionScale = { 1, 1 };
        std::string descriptionString;

        // Icon
        sf::Sprite icon;
        sf::Vector2f iconPosition{ 0, 0 };
        sf::Vector2f iconOrigin{ 0, 0 };
        float iconScale = 1;

        std::function<void()> onClick;

        Button() {
            button.setPosition(buttonOffset);
            button.setScale(buttonScale);
            button.setColor(buttonColor);

            title.setCharacterSize(titleSize);
            title.setScale(titleScale);
            title.setFillColor(titleColor);
            title.setString(titleString);
            title.setPosition(button.getPosition().x + 5, button.getPosition().y + 15);

            descriptionFont.loadFromFile("src/Fonts/font.ttf");
            description.setFont(descriptionFont);
            description.setCharacterSize(descriptionSize);
            description.setScale(descriptionScale);
            description.setFillColor(descriptionColor);
            description.setString(descriptionString);
            description.setPosition(button.getPosition().x + 5, button.getPosition().y + 35);
        }
    };
    std::vector<Button> buttons;
    sf::Texture buttonTexture0;
    sf::Texture buttonTexture1;
    sf::Texture buttonTexture2;
    sf::Texture folderIconTexture;
    sf::Texture fileIconTexture;
    sf::Texture gearIconTexture;
    sf::Texture uploadIconTexture;
    sf::Texture downloadsIconTexture;
    sf::Texture errorIconTexture;
    sf::Texture monitorIconTexture;
    sf::Texture serverIconTexture;
    sf::Texture sendIconTexture;
    sf::Texture checkIconTexture;
    sf::Texture driveIconTexture;

    // Connection status
    struct ConnectionStatus {
        sf::CircleShape statusCircle;
        float radius = 5;
        sf::Color activeColor;
        sf::Color inactiveColor;
        sf::Text statusText;

        ConnectionStatus() {
            statusCircle.setPointCount(40);
            statusCircle.setRadius(radius);
            activeColor = sf::Color(55, 255, 155);
            inactiveColor = sf::Color(255, 55, 55);
        }
    };
    std::vector<ConnectionStatus> connectionStatus;

    // Plaintext
    struct PlainText {
        std::string id;
        sf::Text text;
        std::string textString = "Text...";
        sf::Vector2f textPosition{1, 1};
        sf::Vector2f scale{1, 1};
        sf::Color color{ 255, 255, 255, 255 };
        int characterSize = 10;

        PlainText() {
            text.setCharacterSize(characterSize);
            text.setString(textString);
            text.setPosition(textPosition);
            text.setScale(scale);
            text.setFillColor(color);
        }
    };
    std::vector<PlainText> plainTexts;

    // Text field
    enum class TextFieldType { Text, Number };
    struct TextField {
        std::string id;
        sf::RectangleShape box;
        sf::RectangleShape caret;
        sf::Text label;
        sf::Text text;
        std::string value;
        std::string allowedChars;          
        TextFieldType type = TextFieldType::Text;
        std::size_t maxLength = 64;
        long long maxValue = INT_MAX;    
        std::size_t cursor = 0;
        bool focused = false;
        sf::Clock caretClock;
        std::function<void(const std::string&)> onChange;
    };
    std::vector<TextField> textFields;
    TextField* createTextField(
        const std::string& id, const std::string& labelString, const std::string& initial,
        sf::Vector2f position, sf::Vector2f size, int characterSize
    );

    UI_components();
    void clear();

    void addConnectionStatus(sf::Vector2f boxPosition);
    void updateConnectionStatus(Networking_client& networking_client);
    void drawConnectionStatus(sf::RenderWindow& window);

    void addSidebar(std::string label, sf::Vector2f position);
    void updateSidebar();
    void drawSideBar(sf::RenderWindow& window);

    void addButton(
        sf::String titleString, sf::String descriptionString, std::string icon, sf::Vector2f buttonPosition, sf::Vector2f buttonScale,
        sf::Vector2f iconScale, sf::Vector2f titleOffset, sf::Vector2f descriptionOffset, sf::Vector2f iconOffset, sf::Color titleColor, 
        sf::Color descriptionColor, int buttonTexture, int titleCharacterSize, int hoverStyle, int baseStyle, int descriptionCharacterSize,
        std::function<void()> onClick = nullptr
    );
    void updateButtons(Cursor_input& cursor_input);
    void drawButtons(sf::RenderWindow& window);

    void addPlainText(std::string id, std::string textString, int characterSize, sf::Vector2f position, sf::Vector2f scale, sf::Color color);
    void updatePlainText(Networking_server& networking_server, sf::RenderWindow& window);
    void drawPlainText(sf::RenderWindow& window);

    void addTextField(
        std::string id, std::string labelString, std::string& target, std::string defaultValue,
        sf::Vector2f position, sf::Vector2f size, int characterSize = 18,
        std::size_t maxLength = 64, std::string allowedChars = ""
    );
    void addTextField(
        std::string id, std::string labelString, int& target, int defaultValue,
        sf::Vector2f position, sf::Vector2f size, int characterSize = 18, int maxValue = INT_MAX
    );
    void updadteTextField(const sf::Event& event, sf::RenderWindow& window);
    void drawTextFields(sf::RenderWindow& window);
};

