#include "text.h"

#define SPACE_BETWEEN_LINES_IN_PIXELS 35

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Chat : public PlacedText {
public:
    str buffer;
    bool inputted;
    sf::Time times[11];
    std::vector<str> lines;
    int start, len;
    size_t cursorPos;
    sf::RectangleShape rect, cursor;
    std::map<str, void (*)(void)> commands;
    bool ChatEnable;

    Chat();
    void draw(sf::RenderWindow&);
    bool InputText(sf::Event&);
    bool Entered();
    void addLine(str);
    str Last();

    void SetCommand(str, void (*)(void));
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Chat::Chat() : PlacedText(), inputted(false), start(0), len(11) {
    PosX = 150; PosY = sch - SPACE_BETWEEN_LINES_IN_PIXELS * (len + 2);
    Width = scw - PosX * 2; Height = 35;
    lines.assign(11, "");

    text.setCharacterSize(27);
    text.setFillColor(sf::Color(199, 199, 199, 180));
    text.setString("");

    rect.setFillColor(sf::Color(50, 50, 50, 100));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2);
    rect.setPosition(PosX, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * len);
    rect.setSize({Width, Height});

    cursor.setFillColor(sf::Color::White);
    cursor.setSize({3, Height});

    ChatEnable = true;
}

void Chat::draw(sf::RenderWindow& window) {
    if (!ChatEnable) return;
    if (inputted) {
        window.draw(rect);
        PlacedText tempText; tempText.text.setString(lines[start].substr(0, cursorPos));
        tempText.text.setCharacterSize(text.getCharacterSize());
        cursor.setPosition(PosX + 5 + tempText.text.getGlobalBounds().width, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * len);
        if (GlobalClock.getElapsedTime() % sf::seconds(1.f) > sf::seconds(1.f / 2))
            window.draw(cursor);
    }
    for (int i = 0; i < len; i++)
        if (GlobalClock.getElapsedTime() - times[(start - i + len) % len] < sf::seconds(5) || inputted) {
            text.setString(lines[(start - i + len) % len]);
            text.setPosition(PosX + 5, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * (len - i) + 4);
            window.draw(text);
        }
}

bool Chat::InputText(sf::Event& event) {
    if (!ChatEnable) return false;
    setlocale(LC_ALL, "rus");
    buffer.clear();
    if (inputted && event.type == sf::Event::TextEntered && 32 <= event.text.unicode) {
        if (event.text.unicode <= 127)  buffer.push_back(event.text.unicode);
        else  buffer.push_back(event.text.unicode - 1072 - 32);
        lines[start].insert(cursorPos, buffer);
        cursorPos++;
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape)
            inputted = false;
        else if (event.key.code == sf::Keyboard::Enter)
            Entered();

        if (event.key.code == sf::Keyboard::BackSpace && cursorPos > 0)
            lines[start].erase(lines[start].begin() + cursorPos-- - 1);
        else if (event.key.code == sf::Keyboard::Delete && cursorPos < lines[start].size())
            lines[start].erase(lines[start].begin() + cursorPos);
        else if (event.key.code == sf::Keyboard::Left && cursorPos > 0) cursorPos--;
        else if (event.key.code == sf::Keyboard::Right && cursorPos < lines[start].size()) cursorPos++;
        else if (event.key.code == sf::Keyboard::Home) cursorPos = 0;
        else if (event.key.code == sf::Keyboard::End) cursorPos = lines[start].size();
    }
    return inputted || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter);
}

bool Chat::Entered() {
    if (inputted && lines[start].size() > 0) {
        if (commands.count(lines[start]) != 0)
            commands[lines[start]]();
        else {
            times[start] = GlobalClock.getElapsedTime();
            start = (start + 1) % len;
        }
        lines[start].clear();
    }
    inputted = !inputted;
    cursorPos = 0;
    return !inputted;
}

void Chat::addLine(str word) {
    std::swap(lines[start], lines[(start + 1) % len]);
    times[start] = GlobalClock.getElapsedTime();
    lines[start] = word;
    start = (start + 1) % len;
}

str Chat::Last() { return lines[(start + len - 1) % len]; }

void Chat::SetCommand(str CommandString, void (*CommandFunction)(void)) {
    commands[CommandString] = CommandFunction;
}