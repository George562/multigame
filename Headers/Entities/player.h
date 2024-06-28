#pragma once
#include "creature.h"
#include "../Abstracts/serializable.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature, public Serializable {
public:
    Player();
    void move(Location*) override;

    json writeJSON() {
        json j;
        j["Name"]           = this->Name.getString();
        j["Health"]         = this->Health.writeJSON();
        j["HealthRecovery"] = this->HealthRecovery;
        j["Mana"]           = this->Mana.writeJSON();
        j["ManaRecovery"]   = this->ManaRecovery;
        j["Inventory"]      = json({});
        j["Money"]          = this->inventory.money;

        for (Item*& item : this->inventory.items) {
            if (item->id == ItemID::coin)
                continue;
            j["Inventory"][itemName[item->id]] =  item->amount;
        }
        
        return j;
    }

    void readJSON(json& j) {
        this->Name.setString((std::string)j["Name"]);
        this->Health.readJSON(j["Health"]);
        this->HealthRecovery = j["HealthRecovery"];
        this->Mana.readJSON(j["Mana"]);
        this->ManaRecovery = j["ManaRecovery"];
        this->inventory.money = j["Money"];
        for (int id = 0; id < ItemID::ItemCount; id++) {
            if (id == ItemID::coin)
                continue;
            if (j["Inventory"].contains(itemName[id])) {
                Item* newItem = new Item(id, j["Inventory"][itemName[id]]);
                this->inventory.addItem(newItem);
            }
        }
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Player::Player() : Creature("Player", faction::Player) {
    Health = {0.f, 2000.f, 20.f}; HealthRecovery = Health.top / 5;
    Mana = {0.f, 10.f, 10.f}; ManaRecovery = Mana.top / 5;
    Armor = {0.f, 0.f, 0.f};
    hitbox.setRadius(60.f);
    Velocity = {0.f, 0.f}; MaxVelocity = 900.f;
    Acceleration = 10000.f;
    Name.setOutlineColor(sf::Color::Green);
    Name.setFillColor(sf::Color::Black);
}

void Player::move(Location* location) {
    std::vector<float> PressedKeys = {
        sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1.f : 0.f
    };

    VelocityBuff = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        VelocityBuff *= 2;

    setTarget(hitbox.getCenter() + sf::Vector2f(PressedKeys[3] - PressedKeys[1], PressedKeys[2] - PressedKeys[0]) * MaxVelocity * VelocityBuff);
    Creature::move(location);
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) {
    return packet << a.hitbox.getCenter().x << a.hitbox.getCenter().y;
}
sf::Packet& operator>>(sf::Packet& packet, Player& a) {
    sf::Vector2f v;
    packet >> v.x >> v.y;
    a.hitbox.setCenter(v);
    return packet;
}
