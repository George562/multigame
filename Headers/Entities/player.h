#pragma once
#include "creature.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Player();
    void move(Location*) override;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

void to_json(json& j, const Player& p) {
	j["Name"] = p.Name.getString();
	j["Health"] = p.Health;
	j["HealthRecovery"] = p.HealthRecovery;
	j["Mana"] = p.Mana;
	j["ManaRecovery"] = p.ManaRecovery;
	j["Inventory"] = json({});
	j["Money"] = p.inventory.money;

	for (Item* item : p.inventory.items) {
		j["Inventory"][itemName[item->id]] = item->amount;
	}
}

void from_json(const json& j, Player& p) {
	p.Name.setString(j["Name"].template get<std::string>());
	p.Health = j["Health"].template get<Scale<float>>();
	p.HealthRecovery = j["HealthRecovery"].template get<float>();
	p.Mana = j["Mana"].template get<Scale<float>>();
	p.ManaRecovery = j["ManaRecovery"].template get<float>();
	p.inventory.money = j["Money"].template get<int>();
	for (int id = 0; id < ItemID::ItemCount; id++) {
		if (j["Inventory"].contains(itemName[id])) {
			Item* newItem = new Item(id, j["Inventory"][itemName[id]].template get<int>());
			p.inventory.addItem(newItem);
		}
	}
}

Player::Player() : Creature("Player", faction::Player) {
	Health = { {0.f, 20.f, 20.f} }; HealthRecovery = Health.top / 5;
	Mana = { {0.f, 10.f, 10.f} }; ManaRecovery = Mana.top / 5;
    Armor = { {0.f, 0.f, 0.f} };
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
