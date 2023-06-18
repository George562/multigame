#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Chat : public PlaccedText {
public:
    str buffer;
    bool inputted;
    sf::Time times[11];
    int start, len, CharacterSize, cursorPos;
    sf::RectangleShape rect, cursor;
    float dy;

    sf::Clock* Clock;
    Chat();
    void draw(sf::RenderWindow&);
    void InputText(sf::Event&);
    bool Enterred();
    void addLine(str);
    str Last();
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Chat::Chat() : PlaccedText(), inputted(false), start(0), len(11), CharacterSize(27) {
    dy = 35;
    PosX = 150; PosY = sch - dy * (len + 2);
    Width = scw - PosX * 2; Height = 35;
    lines.assign(11, "");
    text.setCharacterSize(CharacterSize);
    text.setFillColor(sf::Color(199, 199, 199, 180));
    text.setString("");
    rect.setFillColor(sf::Color(50, 50, 50, 100));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2);
    rect.setPosition(PosX, PosY + dy * len);
    rect.setSize({Width, Height});
    cursor.setFillColor(sf::Color::White);
    cursor.setSize({3, Height});
}

void Chat::draw(sf::RenderWindow& window) {
    if (inputted) {
        window.draw(rect);
        PlaccedText tempText; tempText.text.setString(lines[start].substr(0, cursorPos));
        tempText.text.setCharacterSize(CharacterSize);
        cursor.setPosition(PosX + 5 + tempText.text.getLocalBounds().width, PosY + dy * len);
        if (Clock->getElapsedTime() % sf::seconds(1.f) > sf::seconds(1.f / 2))
            window.draw(cursor);
    }
    for (int i = 0; i < len; i++)
        if (Clock->getElapsedTime() - times[(start - i + len) % len] < sf::seconds(5) || inputted) {
            text.setString(lines[(start - i + len) % len]);
            text.setPosition(PosX + 5, PosY + dy * (len - i) + 4);
            window.draw(text);
        }
}

void Chat::InputText(sf::Event& event) {
    setlocale(LC_ALL, "rus");
    buffer.clear();
    if (inputted && event.type == sf::Event::TextEntered && 32 <= event.text.unicode) {
        if (event.text.unicode <= 127)  buffer.push_back(event.text.unicode);
        else  buffer.push_back(event.text.unicode - 1072 - 32);
        lines[start].insert(cursorPos, buffer);
        cursorPos++;
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace && lines[start].size() > 0 && cursorPos > 0)
            lines[start].erase(lines[start].begin() + cursorPos-- - 1);
        else if (event.key.code == sf::Keyboard::Left && cursorPos > 0) cursorPos--;
        else if (event.key.code == sf::Keyboard::Right && cursorPos < lines[start].length()) cursorPos++;
        else if (event.key.code == sf::Keyboard::Home) cursorPos = 0;
        else if (event.key.code == sf::Keyboard::End) cursorPos = lines[start].size();
        else if (event.key.code == sf::Keyboard::Delete && cursorPos < lines[start].size())
            lines[start].erase(lines[start].begin() + cursorPos);
    }
}

bool Chat::Enterred() {
    if (inputted && lines[start].size() > 0) {
        times[start] = Clock->getElapsedTime();
        start = (start + 1) % len;
        lines[start].clear();
    }
    inputted = !inputted;
    cursorPos = 0;
    return !inputted;
}

void Chat::addLine(str word) {
    std::swap(lines[start], lines[(start + 1) % len]);
    times[start] = Clock->getElapsedTime();
    lines[start] = word;
    start = (start + 1) % len;
}

str Chat::Last() { return lines[(start + len - 1) % len]; }
