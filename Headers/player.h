#include "creature.h"

#define MAX_MANA 75
#define MAX_HEALTH 10
#define MAX_VELOCITY 6

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Bar<float> ManaBar, HpBar;
    bool ShiftPressed;

    Player();
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    void interface(sf::RenderTarget&);
    void move(vvr&);
    void update(vB&);
    void update(sf::Event&, bool&);
    void ChangeWeapon(int);
    void ChangeWeapon(Weapon*);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Player::Player() : Creature() {
    Health = {0, MAX_HEALTH, MAX_HEALTH};
    Mana = {0, MAX_MANA, MAX_MANA}; ManaRecovery = 10.5;
    Armor = {0, 0, 0};
    Money = 0;
    radius = 60;
    Width = Height = radius * 2;
    PosX = 0; PosY = 0; Velocity = {{-MAX_VELOCITY, -MAX_VELOCITY}, {MAX_VELOCITY, MAX_VELOCITY}, {0, 0}}, Acceleration = 0.6 * 2;
    ShiftPressed = false;
    LastCheck = sf::seconds(0);

    SetTexture("sources/textures/Player");

    HpBar.setWidth(360);
    HpBar.setPosition(scw - HpBar.getSize().x - 10, 20);
    HpBar.value = &Health;
    HpBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(192, 0, 0, 160), sf::Color(32, 32, 32, 160));
    
    ManaBar.setWidth(240);
    ManaBar.setPosition(scw - ManaBar.getSize().x - 10, HpBar.getPosition().y + HpBar.getSize().y);
    ManaBar.value = &Mana;
    ManaBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(0, 0, 192, 160), sf::Color(32, 32, 32, 160));
}

void Player::interface(sf::RenderTarget& target) {
    target.draw(HpBar);
    target.draw(ManaBar);
    if (CurWeapon != nullptr)
        CurWeapon->interface(target);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rect);
}

void Player::move(vvr& walls) {
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

    sf::Vector2i tempv = WillCollisionWithWalls(walls, PosX, PosY, Width, Height, Velocity.cur.x, Velocity.cur.y);

    if (tempv.x == 1) PosX += Velocity.cur.x;
    else Velocity.cur.x = 0;
    if (tempv.y == 1) PosY += Velocity.cur.y;
    else Velocity.cur.y = 0;

    rect.setPosition(PosX, PosY);
}

void Player::update(vB& Bullets) {
    Mana += ManaRecovery * (GlobalClock.getElapsedTime() - LastCheck).asSeconds();
    if (CurWeapon != nullptr) {
        CurWeapon->Update(Bullets, *this);
    }
    LastCheck = GlobalClock.getElapsedTime();
    ManaBar.Update();
    HpBar.Update();
}

void Player::update(sf::Event& event, bool& MiniMapActivated) {
    if (CurWeapon != nullptr) {
        if (!MiniMapActivated)
            CurWeapon->Update(event);
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R) {
            if (CurWeapon != nullptr)
                CurWeapon->Reload(Mana);
        } else if (event.key.code == sf::Keyboard::LShift && !ShiftPressed) {
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
    ManaBar.Update();
    HpBar.Update();
}

void Player::ChangeWeapon(int to) {
    CurWeapon = ((to == sf::Keyboard::Num1) ? FirstWeapon : SecondWeapon);
}

void Player::ChangeWeapon(Weapon* to) {
    CurWeapon = to;
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) { return packet << a.PosX << a.PosY; }
sf::Packet& operator>>(sf::Packet& packet, Player& a) { return packet >> a.PosX >> a.PosY; }
