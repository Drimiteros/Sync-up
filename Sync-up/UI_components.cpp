#include "UI_components.h"

using namespace std;
using namespace sf;

UI_components::UI_components() {
    font.loadFromFile("src/Fonts/font.ttf");
    sidebarTexture.loadFromFile("src/Textures/sidebar.png");
    sidebarTexture.setSmooth(true);
    buttonTexture0.loadFromFile("src/Textures/button0.png");
    buttonTexture0.setSmooth(true);
    buttonTexture1.loadFromFile("src/Textures/button1.png");
    buttonTexture1.setSmooth(true);
    folderIconTexture.loadFromFile("src/Textures/folder.png");
    folderIconTexture.setSmooth(true);
    fileIconTexture.loadFromFile("src/Textures/file.png");
    fileIconTexture.setSmooth(true);
    gearIconTexture.loadFromFile("src/Textures/gear.png");
    gearIconTexture.setSmooth(true);
    uploadIconTexture.loadFromFile("src/Textures/upload.png");
    uploadIconTexture.setSmooth(true);
    errorIconTexture.loadFromFile("src/Textures/error.png");
    errorIconTexture.setSmooth(true);
    downloadsIconTexture.loadFromFile("src/Textures/downloads.png");
    downloadsIconTexture.setSmooth(true);
    monitorIconTexture.loadFromFile("src/Textures/monitor.png");
    monitorIconTexture.setSmooth(true);
    serverIconTexture.loadFromFile("src/Textures/server.png");
    serverIconTexture.setSmooth(true);
}

void UI_components::clear() {
    buttons.clear(); 
    sideBars.clear();
}

void UI_components::addConnectionStatus(Vector2f boxPosition) {
    connectionStatus.push_back(ConnectionStatus());
    connectionStatus.back().statusCircle.setPosition(boxPosition);
    connectionStatus.back().statusText.setFont(font);
    connectionStatus.back().statusText.setCharacterSize(15);
    connectionStatus.back().statusText.setPosition(connectionStatus.back().statusCircle.getPosition().x + 20, connectionStatus.back().statusCircle.getPosition().y - 5);
}
void UI_components::updateConnectionStatus(Networking_client& networking_client) {
    for (int i = 0; i < connectionStatus.size(); i++) {
        if (networking_client.connection.isConnected) {
            connectionStatus[i].statusCircle.setFillColor(connectionStatus[i].activeColor);
            connectionStatus[i].statusText.setString("Connected");
        }
        else {
            connectionStatus[i].statusCircle.setFillColor(connectionStatus[i].inactiveColor);
            connectionStatus[i].statusText.setString("Disconnected");
        }
    }
}
void UI_components::drawConnectionStatus(RenderWindow& window) {
    for (int i = 0; i < connectionStatus.size(); i++) {
        window.draw(connectionStatus[i].statusCircle);
        window.draw(connectionStatus[i].statusText);
    }
}

void UI_components::addSidebar(string label, Vector2f position) {
    for (int i = 0; i < sideBars.size(); i++) {
        if (sideBars[i].label == label) {
            cout << "Sidebar with this id already exists." << endl;
            return;
        }
    }
    sideBars.push_back(SideBar());
    sideBars.back().label = label;
    sideBars.back().sidebar.setPosition(position);
    sideBars.back().sidebar.setTexture(sidebarTexture);
}
void UI_components::updateSidebar() {

}
void UI_components::drawSideBar(RenderWindow& window) {
    for (int i = 0; i < sideBars.size(); i++) {
        window.draw(sideBars[i].sidebar);
    }
}

void UI_components::addButton(
    string titleString, string descriptionString, string icon, Vector2f buttonPosition, Vector2f buttonScale, Vector2f iconScale,
    Vector2f titleOffset, Vector2f descriptionOffset, Vector2f iconOffset, Color titleColor, Color descriptionColor, int buttonTexture, int titleCharacterSize,
    int hoverStyle, int baseStyle, int descriptionCharacterSize, function<void()> onClick
) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].titleString == titleString) {
            cout << "Button with this id already exists." << endl;
            return;
        }
    }

    buttons.push_back(Button());
    buttons.back().button.setPosition(buttonPosition);
    buttons.back().button.setScale(buttonScale);
    buttons.back().button.setTexture(
        buttonTexture == 0 ? buttonTexture0 : 
        buttonTexture == 1 ? buttonTexture1 : 
        buttonTexture0
    );
    buttons.back().hoverStyle = hoverStyle;
    buttons.back().baseStyle = baseStyle;
    buttons.back().titleString = titleString;
    buttons.back().titleColor = titleColor;
    buttons.back().title.setFont(font);
    buttons.back().title.setString(titleString);
    buttons.back().title.setPosition(buttons.back().button.getPosition().x + titleOffset.x, buttons.back().button.getPosition().y + titleOffset.y);
    buttons.back().title.setCharacterSize(titleCharacterSize);
    buttons.back().title.setFillColor(titleColor);

    buttons.back().descriptionColor = descriptionColor;
    buttons.back().description.setFont(font);
    buttons.back().description.setString(descriptionString);
    buttons.back().description.setPosition(buttons.back().title.getPosition().x + descriptionOffset.x, buttons.back().title.getPosition().y + descriptionOffset.y);
    buttons.back().description.setCharacterSize(descriptionCharacterSize);
    buttons.back().description.setFillColor(descriptionColor);
    buttons.back().description.setLineSpacing(0.7);

    buttons.back().icon.setTexture(
        icon == "folder" ? folderIconTexture :
        icon == "file" ? fileIconTexture :
        icon == "gear" ? gearIconTexture :
        icon == "upload" ? uploadIconTexture :
        icon == "error" ? errorIconTexture :
        icon == "download" ? downloadsIconTexture :
        icon == "monitor" ? monitorIconTexture :
        icon == "server" ? serverIconTexture :
        errorIconTexture
    );
    buttons.back().icon.setScale(iconScale);
    buttons.back().icon.setPosition(buttons.back().button.getPosition().x + iconOffset.x, buttons.back().button.getPosition().y + iconOffset.y);

    buttons.back().onClick = move(onClick);
}
void UI_components::updateButtons(Cursor_input& cursor_input) {
    for (int i = 0; i < buttons.size(); i++) {
        // Hover state
        if (cursor_input.cursor_sprite.getGlobalBounds().intersects(buttons[i].button.getGlobalBounds())) {
            switch (buttons[i].hoverStyle) {
                case 0: {
                    buttons[i].button.setColor(Color(255, 255, 255, 255));
                    buttons[i].title.setFillColor(Color(buttons[i].titleColor.r, buttons[i].titleColor.g, buttons[i].titleColor.b, 255));
                    buttons[i].description.setFillColor(Color(buttons[i].descriptionColor.r, buttons[i].descriptionColor.g, buttons[i].descriptionColor.b, 155));
                    buttons[i].icon.setColor(Color(255, 255, 255, 255));
                    break;
                }
                case 1: {
                    buttons[i].button.setColor(Color(255, 255, 255, 255));
                    buttons[i].title.setFillColor(Color(buttons[i].titleColor.r, buttons[i].titleColor.g, buttons[i].titleColor.b, 255));
                    buttons[i].description.setFillColor(Color(buttons[i].descriptionColor.r, buttons[i].descriptionColor.g, buttons[i].descriptionColor.b, 155));
                    buttons[i].icon.setColor(Color(255, 255, 255, 255));
                    break;
                }
            }
            if (cursor_input.isLeftClicked && buttons[i].onClick) {
                buttons[i].onClick();
                return;
            }
        }
        else {
            switch (buttons[i].baseStyle) {
                case 0: {
                    buttons[i].button.setColor(Color(255, 255, 255, 4));
                    buttons[i].title.setFillColor(Color(buttons[i].titleColor.r, buttons[i].titleColor.g, buttons[i].titleColor.b, 155));
                    buttons[i].description.setFillColor(Color(buttons[i].descriptionColor.r, buttons[i].descriptionColor.g, buttons[i].descriptionColor.b, 100));
                    buttons[i].icon.setColor(Color(255, 255, 255, 200));
                    break;
                }
                case 1: {
                    buttons[i].button.setColor(Color(255, 255, 255, 100));
                    buttons[i].title.setFillColor(Color(buttons[i].titleColor.r, buttons[i].titleColor.g, buttons[i].titleColor.b, 155));
                    buttons[i].description.setFillColor(Color(buttons[i].descriptionColor.r, buttons[i].descriptionColor.g, buttons[i].descriptionColor.b, 100));
                    buttons[i].icon.setColor(Color(255, 255, 255, 200));
                    break;
                }
            }
        }
    }
}
void UI_components::drawButtons(RenderWindow& window) {
    for (int i = 0; i < buttons.size(); i++) {
        window.draw(buttons[i].button);
        window.draw(buttons[i].title);
        window.draw(buttons[i].description);
        window.draw(buttons[i].icon);
    }
}