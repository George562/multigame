#include "creature.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Bar<float> ManaBar, HpBar;
    PlaccedText HpText, ManaText;
    bool ShiftPressed;

    Player();
    void draw(sf::RenderWindow&);
    void interface(sf::RenderWindow&);
    void move(const vvr&, int&);
    void update(vB&);
    void update(sf::Event&, bool&);
    void ChangeWeapon(int);
    void ChangeWeapon(Weapon*);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Player::Player() : Creature() {
    Health = {0, 10, 5};
    Mana = {0, 75, 75}; ManaRecovery = 10.5;
    Armor = {0, 0, 0};
    Money = 0;
    radius = 60;
    Width = Height = radius * 2;
    PosX = 0; PosY = 0; Velocity = {{-6, -6}, {6, 6}, {0, 0}}, Acceleration = 0.6 * 2;
    ShiftPressed = false;
    LastCheck = sf::Clock().getElapsedTime();
    circle.setRadius(radius);
    // circle.setPointCount(4);
    circle.setFillColor(sf::Color(0, 180, 0));
    circle.setOutlineColor(sf::Color(100, 50, 50));
    circle.setOutlineThickness(Velocity.cur.x);
    SetTexture("sources/Player");

    HpBar.setSize(360, 60);
    HpBar.setPosition(scw - HpBar.getSize().x - 10, 20);
    HpBar.scale = &Health;
    HpBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(192, 0, 0, 160), sf::Color(32, 32, 32, 160));
    
    ManaBar.setSize(240, 50);
    ManaBar.setPosition(scw - ManaBar.getSize().x - 10, HpBar.getPosition().y + HpBar.getSize().y - 5);
    ManaBar.scale = &Mana;
    ManaBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(0, 0, 192, 160), sf::Color(32, 32, 32, 160));
}

void Player::interface(sf::RenderWindow& window) {
    HpText.setText(std::to_string((int)Health.cur));
    HpText.setPosition(HpBar.getPosition().x + HpBar.getSize().x / 2 - HpText.Width  / 2,
                       HpBar.getPosition().y + HpBar.getSize().y / 2 - HpText.Height / 2);
    ManaText.setText(std::to_string((int)Mana.cur));
    ManaText.setPosition(ManaBar.getPosition().x + ManaBar.getSize().x / 2 - ManaText.Width  / 2,
                       ManaBar.getPosition().y + ManaBar.getSize().y / 2 - ManaText.Height / 2);

    HpBar.draw(window);
    ManaBar.draw(window);

    HpText.draw(window);
    ManaText.draw(window);
    CurWeapon->interface(window);
}

void Player::draw(sf::RenderWindow& window) {
    if (rect.getGlobalBounds().width == 0) {
        circle.setPosition(PosX - Camera->x, PosY - Camera->y);
        window.draw(circle);
    } else {
        rect.setPosition(PosX - Camera->x, PosY - Camera->y);
        window.draw(rect);
    }
}

void Player::move(const vvr& walls, int& size) {
    int N = walls.size(), M = walls[0].size(), y = (int(PosY) / size) * 2, x = (int(PosX) / size) * 2;

    if (!(0 <= y && y < N && 0 <= x && x < M)) return;

    std::map<char, bool> PressedKeys = {
        {'w', sf::Keyboard::isKeyPressed(sf::Keyboard::W)},
        {'a', sf::Keyboard::isKeyPressed(sf::Keyboard::A)},
        {'s', sf::Keyboard::isKeyPressed(sf::Keyboard::S)},
        {'d', sf::Keyboard::isKeyPressed(sf::Keyboard::D)}
    };

    if (PressedKeys['w'] || PressedKeys['s']) {
        Velocity.cur.y += Acceleration * ((PressedKeys['s']) ? 1 : -1);
        if (Velocity.fromTop().y < 0 || Velocity.toBottom().y < 0)
            Velocity.cur.y -= Acceleration * ((PressedKeys['s']) ? 1 : -1);
        if      (Velocity.fromTop().y  < 0) Velocity.cur.y -= Acceleration;
        else if (Velocity.toBottom().y < 0) Velocity.cur.y += Acceleration;
    } else {
        if (std::abs(Velocity.cur.y) <= Acceleration)
            Velocity.cur.y = 0;
        else if (Velocity.cur.y != 0)
            Velocity.cur.y -= Acceleration * ((Velocity.cur.y > 0) ? 1 : -1);
    }
    
    if (PressedKeys['a'] || PressedKeys['d']) {
        Velocity.cur.x += Acceleration * ((PressedKeys['d']) ? 1 : -1);
        if (Velocity.fromTop().x < 0 || Velocity.toBottom().x < 0)
            Velocity.cur.x -= Acceleration * ((PressedKeys['d']) ? 1 : -1);
        if      (Velocity.fromTop().x  < 0) Velocity.cur.x -= Acceleration;
        else if (Velocity.toBottom().x < 0) Velocity.cur.x += Acceleration;
    } else {
        if (std::abs(Velocity.cur.x) <= Acceleration)
            Velocity.cur.x = 0;
        else if (Velocity.cur.x != 0)
            Velocity.cur.x -= Acceleration * ((Velocity.cur.x > 0) ? 1 : -1);
    }

    sf::Vector2i tempv = WillCollisionWithWalls(walls, size, PosX, PosY, Width, Height, Velocity.cur.x, Velocity.cur.y);

    if (tempv.x == 1) PosX += Velocity.cur.x;
    else Velocity.cur.x = 0;
    if (tempv.y == 1) PosY += Velocity.cur.y;
    else Velocity.cur.y = 0;
}

void Player::update(vB& Bullets) {
    Mana += ManaRecovery * (Clock->getElapsedTime() - LastCheck).asSeconds();
    if (CurWeapon != nullptr) {
        CurWeapon->Update(Bullets, *this, Clock, Camera);
    }
    LastCheck = Clock->getElapsedTime();
}

void Player::update(sf::Event& event, bool& MiniMapActivated) {
    if (CurWeapon != nullptr) {
        if (!MiniMapActivated)
            CurWeapon->Update(event);
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R)
            CurWeapon->Reload(Mana);
        else if (event.key.code == sf::Keyboard::LShift && !ShiftPressed) {
            Velocity.top    *= 2.f;
            Velocity.bottom *= 2.f;
            ShiftPressed = true;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::LShift) {
            Velocity.top    /= 2.f;
            Velocity.bottom /= 2.f;
            ShiftPressed = false;
        }
    }
}

void Player::ChangeWeapon(int to) {
    CurWeapon = ((to == sf::Keyboard::Num1) ? FirstWeapon : SecondWeapon);
}

void Player::ChangeWeapon(Weapon* to) {
    CurWeapon = to;
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) { return packet << a.PosX << a.PosY; }
sf::Packet& operator>>(sf::Packet& packet, Player& a) { return packet >> a.PosX >> a.PosY; }
