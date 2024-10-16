#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "../UI/text.h"

#define SPACE_BETWEEN_LINES_IN_PIXELS 35
#define CHARACTER_SIZE 27

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Chat : public sf::Drawable {
public:
    float PosX, PosY;
    sf::Vector2f size;
    std::vector<sf::Clock*> clocks;
    std::map<std::string, void (*)(void)> commands;
    PlacedText lines[11];
    int start, len;
    size_t cursorPos;
    sf::RectangleShape rect, cursor;
    sf::Clock* localClock = nullptr;
    bool inputted;
    bool ChatEnable;

    Chat(int, int);
    ~Chat();
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool InputText(sf::Event&);
    bool Entered();
    void addLine(std::string);
    void pushLine();
    std::string Last() { return lines[(start + len - 1) % len].getString(); }

    void SetCommand(std::string CommandString, void (*CommandFunction)(void)) { commands[CommandString] = CommandFunction; }
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Chat::Chat(int scw, int sch) : inputted(false), start(0), len(11) {
    PosX = 350; PosY = sch - SPACE_BETWEEN_LINES_IN_PIXELS * (len + 2);
    size.x = scw - PosX * 2; size.y = 35;

    for (int i = 0; i < len; i++) {
        lines[i].setCharacterSize(CHARACTER_SIZE);
        lines[i].setFillColor(sf::Color(199, 199, 199, 180));
        lines[i].setString("");
        lines[i].setPosition(PosX + 5, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * (len - i) + 4);

        clocks.push_back(new sf::Clock());
    }

    rect.setFillColor(sf::Color(50, 50, 50, 100));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2);
    rect.setPosition(PosX, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * len);
    rect.setSize(size);

    cursor.setFillColor(sf::Color::White);
    cursor.setSize({3, size.y});

    ChatEnable = true;
    localClock = new sf::Clock();
}

Chat::~Chat() {
    if (localClock) {
        delete localClock;
    }
}

void Chat::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!ChatEnable) return;
    if (inputted) {
        target.draw(rect, states);
        if (localClock->getElapsedTime() % sf::seconds(1.f) > sf::seconds(1.f / 2))
            target.draw(cursor, states);
    }
    for (int i = 0; i < len; i++)
        if (clocks[i]->getElapsedTime() < sf::seconds(5) || inputted)
            target.draw(lines[i], states);
}

bool Chat::InputText(sf::Event& event) {
    if (!ChatEnable) return false;
    setlocale(LC_ALL, "rus");

    std::string buffer;
    if (inputted && event.type == sf::Event::TextEntered && 32 <= event.text.unicode) {
        if (event.text.unicode <= 127) {
            buffer.push_back(event.text.unicode);
        } else {
            buffer.push_back(event.text.unicode - 1072 - 32);
        }
        lines[start].insert(cursorPos, buffer);
        cursorPos++;
    }

    if (event.type == sf::Event::KeyPressed) {
        if (inputted && event.key.code == sf::Keyboard::Escape) {
            inputted = false;
            return true;
        }
        if (event.key.code == sf::Keyboard::Enter)
            Entered();

        if (event.key.code == sf::Keyboard::BackSpace && cursorPos > 0) {
            lines[start].setString(lines[start].getString().substring(0, cursorPos - 1) +
                                   lines[start].getString().substring(cursorPos, lines[start].TextSize()));
            cursorPos--;
        }
        if (event.key.code == sf::Keyboard::Delete && cursorPos < lines[start].TextSize())
            lines[start].setString(lines[start].getString().substring(0, cursorPos) +
                                   lines[start].getString().substring(cursorPos + 1, lines[start].TextSize()));
        if (event.key.code == sf::Keyboard::Left && cursorPos > 0) cursorPos--;
        if (event.key.code == sf::Keyboard::Right && cursorPos < lines[start].TextSize()) cursorPos++;
        if (event.key.code == sf::Keyboard::Home) cursorPos = 0;
        if (event.key.code == sf::Keyboard::End) cursorPos = lines[start].TextSize();
    }
    if (inputted) {
        PlacedText tempText; tempText.setString(lines[start].getString().substring(0, cursorPos));
        tempText.setCharacterSize(CHARACTER_SIZE);
        cursor.setPosition(PosX + 5 + tempText.Width, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * len);
    }

    return inputted || (keyPressed(event, sf::Keyboard::Enter));
}

bool Chat::Entered() {
    if (inputted && lines[start].TextSize() > 0) {
        if (commands.count(lines[start].getString()) != 0) {
            commands[lines[start].getString()]();
        } else {
            pushLine();
        }
        lines[start].setString("");
    }
    inputted = !inputted;
    cursorPos = 0;
    return !inputted;
}

void Chat::addLine(std::string word) {
    std::swap(lines[start], lines[(start + 1) % len]);
    lines[start].setString(word);
    pushLine();
}

void Chat::pushLine() {
    clocks[start]->restart();
    start = (start + 1) % len;
    for (int i = 0; i < len; i++)
        lines[i].setPosition(PosX + 5, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * (len - ((start - i + len) % len)) + 4);
}