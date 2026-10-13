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
    buttonTexture2.loadFromFile("src/Textures/button2.png");
    buttonTexture2.setSmooth(true);
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
    sendIconTexture.loadFromFile("src/Textures/send.png");
    sendIconTexture.setSmooth(true);
    checkIconTexture.loadFromFile("src/Textures/check.png");
    checkIconTexture.setSmooth(true);
    driveIconTexture.loadFromFile("src/Textures/drive.png");
    driveIconTexture.setSmooth(true);
}

void UI_components::clear() {
    buttons.clear(); 
    sideBars.clear();
    connectionStatus.clear();
    plainTexts.clear();
    textFields.clear();
}

// ---------------------------------------------------------------- Connection status
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

// ---------------------------------------------------------------- Sidebars
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

// ---------------------------------------------------------------- Buttons
void UI_components::addButton(
    String titleString, String descriptionString, string icon, Vector2f buttonPosition, Vector2f buttonScale, Vector2f iconScale,
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
        buttonTexture == 2 ? buttonTexture2 :
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
        icon == "send" ? sendIconTexture :
        icon == "check" ? checkIconTexture :
        icon == "drive" ? driveIconTexture :
        errorIconTexture
    );
    buttons.back().icon.setScale(iconScale);
    buttons.back().icon.setPosition(buttons.back().button.getPosition().x + iconOffset.x, buttons.back().button.getPosition().y + iconOffset.y);
    buttons.back().onClick = move(onClick);
}
void UI_components::updateButtons(Cursor_input& cursor_input) {
    for (int i = 0; i < buttons.size(); i++) {
        //Base style
        if (!cursor_input.cursor_sprite.getGlobalBounds().intersects(buttons[i].button.getGlobalBounds())) {
            switch (buttons[i].baseStyle) {
                case 0: {
                    buttons[i].button.setColor(Color(255, 255, 255, 4));
                    break;
                }
                case 1: {
                    buttons[i].button.setColor(Color(255, 255, 255, 100));
                    break;
                }
            }
            buttons[i].title.setFillColor(Color(buttons[i].titleColor.r, buttons[i].titleColor.g, buttons[i].titleColor.b, 155));
            buttons[i].description.setFillColor(Color(buttons[i].descriptionColor.r, buttons[i].descriptionColor.g, buttons[i].descriptionColor.b, 100));
            buttons[i].icon.setColor(Color(255, 255, 255, 200));
        }
        // Hover state
        else {
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
            // Is clicked
            if (cursor_input.isLeftClicked && buttons[i].onClick) {
                buttons[i].onClick();
                return;
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

// ---------------------------------------------------------------- Plain Text
void UI_components::addPlainText(string id, string textString, int characterSize, Vector2f position, Vector2f scale, Color color) {
    for (int i = 0; i < plainTexts.size(); i++) {
        if (plainTexts[i].id == id) {
            cout << "Plain text with this id already exists." << endl;
            return;
        }
    }
    
    plainTexts.push_back(PlainText());
    plainTexts.back().id = id;
    plainTexts.back().text.setFont(font);
    plainTexts.back().text.setString(textString);
    plainTexts.back().text.setCharacterSize(characterSize);
    plainTexts.back().text.setPosition(position);
    plainTexts.back().text.setScale(scale);
    plainTexts.back().text.setFillColor(color);
}
void UI_components::updatePlainText(Networking_server& networking_server, RenderWindow& window) {
    for (int i = 0; i < plainTexts.size(); i++) {
        if (plainTexts[i].id == "passMatchText") {
            if (networking_server.password.size() > 0 || networking_server.confirm_password.size() > 0) {
                if (networking_server.password == networking_server.confirm_password) {
                    plainTexts[i].text.setString("Passwords match!");
                    plainTexts[i].text.setFillColor(Color(0, 255, 150, 255));
                    plainTexts[i].text.setPosition(window.getSize().x / 2 - plainTexts[i].text.getLocalBounds().width / 2, window.getSize().y / 2 + 130);
                }
                else {
                    plainTexts[i].text.setString("Passwords do not match!");
                    plainTexts[i].text.setFillColor(Color(255, 150, 150, 255));
                    plainTexts[i].text.setPosition(window.getSize().x / 2 - plainTexts[i].text.getLocalBounds().width / 2, window.getSize().y / 2 + 130);
                }
            }
            else
                plainTexts[i].text.setString("");
        }
    }
}
void UI_components::drawPlainText(RenderWindow& window) {
    for (int i = 0; i < plainTexts.size(); i++) {
        window.draw(plainTexts[i].text);
    }
}

// ---------------------------------------------------------------- Text fields
namespace {
    using Field = UI_components::TextField;
    using FieldType = UI_components::TextFieldType;

    void commit(Field& f) {
        f.text.setString(f.value);
        f.caretClock.restart();
        if (f.onChange) f.onChange(f.value);
    }

    bool acceptsChar(const Field& f, char c) {
        if (f.type == FieldType::Number) return c >= '0' && c <= '9';
        if (!f.allowedChars.empty()) return f.allowedChars.find(c) != string::npos;
        return c >= 32 && c < 127;
    }

    void insertText(Field& f, const string& s) {
        bool changed = false;
        for (char c : s) {
            if (!acceptsChar(f, c) || f.value.size() >= f.maxLength) continue;
            string next = f.value;
            next.insert(f.cursor, 1, c);
            if (f.type == FieldType::Number && stoll(next) > f.maxValue) continue;
            f.value = next;
            f.cursor++;
            changed = true;
        }
        if (changed) commit(f);
    }

    size_t cursorFromX(const Field& f, float x) {
        size_t best = 0;
        float bestDist = 1e9f;
        for (size_t i = 0; i <= f.value.size(); i++) {
            float d = abs(f.text.findCharacterPos(i).x - x);
            if (d < bestDist) { bestDist = d; best = i; }
        }
        return best;
    }

    void setFocus(Field& f, bool focus) {
        f.focused = focus;
        f.caretClock.restart();
    }
}
UI_components::TextField* UI_components::createTextField(
    const string& id, const string& labelString, const string& initial,
    Vector2f position, Vector2f size, int characterSize
) {
    for (auto& f : textFields) {
        if (f.id == id) {
            cout << "Text field with this id already exists." << endl;
            return nullptr;
        }
    }
    textFields.emplace_back();
    TextField& f = textFields.back();
    f.id = id;
    f.value = initial;
    f.cursor = initial.size();

    f.box.setPosition(position);
    f.box.setSize(size);
    f.box.setFillColor(Color(255, 255, 255, 10));
    f.box.setOutlineThickness(2);
    f.box.setOutlineColor(Color(255, 255, 255, 60));

    f.label.setFont(font);
    f.label.setString(labelString);
    f.label.setCharacterSize(characterSize - 3);
    f.label.setFillColor(Color(255, 255, 255, 155));
    f.label.setPosition(position.x, position.y - characterSize - 10);

    f.text.setFont(font);
    f.text.setString(initial);
    f.text.setCharacterSize(characterSize);
    f.text.setFillColor(Color::White);
    f.text.setPosition(position.x + 8, position.y + (size.y - characterSize * 1.2f) / 2.f);

    f.caret.setSize(Vector2f(1.5f, characterSize * 1.15f));
    f.caret.setFillColor(Color::White);
    return &f;
}
void UI_components::addTextField(
    string id, string labelString, string& target, string defaultValue,
    Vector2f position, Vector2f size, int characterSize, size_t maxLength, string allowedChars
) {
    TextField* f = createTextField(id, labelString, defaultValue, position, size, characterSize);
    if (!f) return;
    f->type = TextFieldType::Text;
    f->maxLength = maxLength;
    f->allowedChars = allowedChars;
    target = defaultValue;
    f->onChange = [&target](const string& v) { target = v; };
}
void UI_components::addTextField(
    string id, string labelString, int& target, int defaultValue,
    Vector2f position, Vector2f size, int characterSize, int maxValue
) {
    TextField* f = createTextField(id, labelString, to_string(defaultValue), position, size, characterSize);
    if (!f) return;
    f->type = TextFieldType::Number;
    f->maxValue = maxValue;
    f->maxLength = to_string(maxValue).size();
    target = defaultValue;
    f->onChange = [&target](const string& v) { if (!v.empty()) target = stoi(v); };
}
void UI_components::updadteTextField(const Event& event, RenderWindow& window) {
    if (textFields.empty()) return;

    switch (event.type) {
    case Event::MouseButtonPressed: {
        if (event.mouseButton.button != Mouse::Left) break;
        Vector2f p = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
        for (auto& f : textFields) {
            bool hit = f.box.getGlobalBounds().contains(p);
            setFocus(f, hit);
            if (hit) f.cursor = cursorFromX(f, p.x);
        }
        break;
    }
    case Event::TextEntered: {
        for (auto& f : textFields) {
            if (!f.focused) continue;
            if (event.text.unicode >= 32 && event.text.unicode < 127)
                insertText(f, string(1, static_cast<char>(event.text.unicode)));
        }
        break;
    }
    case Event::KeyPressed: {
        for (size_t i = 0; i < textFields.size(); i++) {
            Field& f = textFields[i];
            if (!f.focused) continue;

            switch (event.key.code) {
            case Keyboard::BackSpace:
                if (f.cursor > 0) { f.value.erase(--f.cursor, 1); commit(f); }
                break;
            case Keyboard::Delete:
                if (f.cursor < f.value.size()) { f.value.erase(f.cursor, 1); commit(f); }
                break;
            case Keyboard::Left:  if (f.cursor > 0) f.cursor--; f.caretClock.restart(); break;
            case Keyboard::Right: if (f.cursor < f.value.size()) f.cursor++; f.caretClock.restart(); break;
            case Keyboard::Home:  f.cursor = 0; f.caretClock.restart(); break;
            case Keyboard::End:   f.cursor = f.value.size(); f.caretClock.restart(); break;
            case Keyboard::Enter:
            case Keyboard::Escape:
                setFocus(f, false);
                break;
            case Keyboard::Tab:
                setFocus(f, false);
                setFocus(textFields[(i + 1) % textFields.size()], true);
                textFields[(i + 1) % textFields.size()].cursor = textFields[(i + 1) % textFields.size()].value.size();
                break;
            case Keyboard::V:
                if (event.key.control) insertText(f, Clipboard::getString().toAnsiString());
                break;
            default: break;
            }
            break; // only one focused field
        }
        break;
    }
    default: break;
    }
}
void UI_components::drawTextFields(RenderWindow& window) {
    for (auto& f : textFields) {
        f.box.setOutlineColor(f.focused ? Color(255, 255, 255, 200) : Color(255, 255, 255, 60));
        window.draw(f.box);
        window.draw(f.label);
        window.draw(f.text);

        if (f.focused && static_cast<int>(f.caretClock.getElapsedTime().asSeconds() * 2) % 2 == 0) {
            Vector2f p = f.text.findCharacterPos(f.cursor);
            f.caret.setPosition(p.x, p.y + 2);
            window.draw(f.caret);
        }
    }
}