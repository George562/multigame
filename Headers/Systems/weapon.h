#pragma once
#include "../Entities/bullet.h"
#include "../Abstracts/upgradable.h"
#include "../Abstracts/scale.h"
#define M_PI_RAD M_PI / 180.f

std::map<std::string, std::string> weaponDescString {
    { "Pistol", "A basic device, that fires a single accurate bullet per shot."
        "\nBetter used as a backup to save mana on other weapons." },
    { "Shotgun", "Powerful device. Sends out multiple bullets in a fan shape per single shot.\n"
                 "Deals high damage if all bullets hit, but requires a lot of mana." },
    { "Rifle", "This device rapidly shoots bullets in exchange for low accuracy.\n"
        "Good attacking device, but takes a while to holster or deploy." }
};

////////////////////////////////////////////////////////////
// Weapon
#pragma pack(push, 1)
class Weapon {
public:
	std::string Name;

	Scale<float> ManaStorage;
	Upgradable<float> MaxManaStorage;      // Is used so that we don't have an entire vector of scales, and just change the max for the storage
	Upgradable<float> ReloadSpeed;
	sf::Clock* ReloadTimer = nullptr;

	Upgradable<sf::Time> TimeToHolster;
	Upgradable<sf::Time> TimeToDispatch;
	sf::Clock* HolsterTimer = nullptr;     // Putting the weapon in the holster to reload takes time
	sf::Clock* DispatchTimer = nullptr;    // Same thing with getting it out
	bool holstered;                        // All weapons are active by default. A holstered state for them is when they are being reloaded.

	Upgradable<float> ManaCostOfBullet;    // Is equal to the damage of bullet
	Upgradable<int> Multishot;             // How many projectiles to fire after 1 shot
	Upgradable<sf::Time> FireRate;

	Upgradable<float> BulletVelocity;
	Upgradable<float> Scatter;             // at degre

	sf::Clock* TimeFromLastShot = nullptr;
	bool lock;                             // Bullets are like a stream and "lock" is blocking the stream

	sf::Sound ShootSound;

	Weapon() {
		ReloadTimer = new sf::Clock();
		HolsterTimer = new sf::Clock();
		TimeFromLastShot = new sf::Clock();
		DispatchTimer = new sf::Clock();
		holstered = false;
		lock = true;
	}
	Weapon(std::string name) : Weapon() {
		std::ifstream defaultWeapon("sources/JSON/default" + name + ".json");
		json j = json::parse(defaultWeapon);
		this->Name = j["Name"].template get<std::string>();
		this->ManaStorage = j["ManaStorage"].template get<Scale<float>>();
		this->MaxManaStorage = j["MaxManaStorage"].template get<Upgradable<float>>();
		this->ReloadSpeed = j["ReloadSpeed"].template get<Upgradable<float>>();
		this->TimeToHolster = j["TimeToHolster"].template get<Upgradable<float>>();
		this->TimeToDispatch = j["TimeToDispatch"].template get<Upgradable<float>>();
		this->holstered = j["holstered"].template get<bool>();
		this->ManaCostOfBullet = j["ManaCostOfBullet"].template get<Upgradable<float>>();
		this->Multishot = j["Multishot"].template get<Upgradable<int>>();
		this->FireRate = j["FireRate"].template get<Upgradable<float>>();
		this->BulletVelocity = j["BulletVelocity"].template get<Upgradable<float>>();
		this->Scatter = j["Scatter"].template get<Upgradable<float>>();

		this->lock = true;
		defaultWeapon.close();
		ShootSound.setVolume(50);
		ShootSound.setAttenuation(1.f / size);
		ShootSound.setMinDistance(size / 4.f);
	}
	virtual ~Weapon() {
		if (TimeFromLastShot) { delete TimeFromLastShot; }
		if (ReloadTimer) { delete ReloadTimer; }
		if (HolsterTimer) { delete HolsterTimer; }
		if (DispatchTimer) { delete DispatchTimer; }
	}

	virtual void Update(sf::Event& event) {
		ManaStorage.top = MaxManaStorage;
		if (mouseButtonPressed(event, sf::Mouse::Left))
			lock = false;
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			lock = true;
	}

	virtual bool CanShoot() {
		if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return false; }
		if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return false;
		if (holstered || DispatchTimer->getElapsedTime() <= TimeToDispatch) return false;
		return true;
	}

	virtual void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
		if (!CanShoot()) return;

		sf::Vector2f d = direction - shooter.getCenter();
		float len = hypotf(d.x, d.y);
		if (len == 0) return;
		d = RotateOn(M_PI_RAD * (rand() % (int)(Scatter)-Scatter / 2.f), d) * BulletVelocity / len;
		sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
		Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet));
		ManaStorage -= ManaCostOfBullet;
		TimeFromLastShot->restart();
		ShootSound.setBuffer(SoundBuffers::Shoot2);
		ShootSound.setPosition(shooter.getCenter().x, shooter.getCenter().y, 0.f);
		ShootSound.play();
	}

	virtual void Reload(Scale<float>& Mana) { // Reloads ReloadSpeed/sec
		if (ManaStorage.fromTop() == 0) return;
		if (holstered && HolsterTimer->getElapsedTime() > TimeToHolster) {
			float x = std::min(std::min(std::min(oneOverSixty, ReloadTimer->restart().asSeconds()) * ReloadSpeed,
									    ManaStorage.fromTop()),
							   Mana.toBottom());
			Mana -= x;
			ManaStorage += x;
			if (ManaStorage.fromTop() == 0) HolsterAction();
			return;
		}
	}

	virtual void HolsterAction() {            // Moves weapon to holster or takes it out of it
		if (holstered && HolsterTimer->getElapsedTime() > TimeToHolster) {
			holstered = false;
			DispatchTimer->restart();
		} else if (!holstered && DispatchTimer->getElapsedTime() >= TimeToDispatch) {
			if (ManaStorage.fromTop() == 0) return;
			holstered = true;
			HolsterTimer->restart();
		}
	}
};
#pragma pack(pop)
////////////////////////////////////////////////////////////

void to_json(json& j, const Weapon& w) {
	j["Name"] = w.Name;
	j["ManaStorage"] = w.ManaStorage;
	j["MaxManaStorage"] = w.MaxManaStorage;
	j["ReloadSpeed"] = w.ReloadSpeed;
	j["TimeToHolster"] = w.TimeToHolster;
	j["TimeToDispatch"] = w.TimeToDispatch;
	j["holstered"] = w.holstered;
	j["ManaCostOfBullet"] = w.ManaCostOfBullet;
	j["Multishot"] = w.Multishot;
	j["FireRate"] = w.FireRate;
	j["BulletVelocity"] = w.BulletVelocity;
	j["Scatter"] = w.Scatter;
}

void from_json(const json& j, Weapon& w) {
	w.Name = j["Name"].template get<std::string>();
	w.ManaStorage = j["ManaStorage"].template get<Scale<float>>();
	w.MaxManaStorage = j["MaxManaStorage"].template get<Upgradable<float>>();
	w.ReloadSpeed = j["ReloadSpeed"].template get<Upgradable<float>>();
	w.TimeToHolster = j["TimeToHolster"].template get<Upgradable<float>>();
	w.TimeToDispatch = j["TimeToDispatch"].template get<Upgradable<float>>();
	w.holstered = j["holstered"].template get<bool>();
	w.ManaCostOfBullet = j["ManaCostOfBullet"].template get<Upgradable<float>>();
	w.Multishot = j["Multishot"].template get<Upgradable<int>>();
	w.FireRate = j["FireRate"].template get<Upgradable<float>>();
	w.BulletVelocity = j["BulletVelocity"].template get<Upgradable<float>>();
	w.Scatter = j["Scatter"].template get<Upgradable<float>>();

	w.lock = true;
}


std::ostream& operator<<(std::ostream& stream, Weapon& weapon) {
    stream << weapon.Name << ' ';
    stream << weapon.ManaStorage << ' ';
    stream << weapon.MaxManaStorage << ' ';
    stream << weapon.ReloadSpeed << ' ';

    stream << weapon.TimeToHolster << ' ';
    stream << weapon.TimeToDispatch << ' ';
    stream << weapon.holstered << ' ';

    stream << weapon.ManaCostOfBullet << ' ';
    stream << weapon.Multishot << ' ';
    stream << weapon.FireRate << ' ';
    
    stream << weapon.BulletVelocity << ' ';
    stream << weapon.Scatter << ' ';
    return stream;
}

std::istream& operator>>(std::istream& stream, Weapon& weapon) {
    stream >> weapon.Name;
    stream >> weapon.ManaStorage;
    stream >> weapon.MaxManaStorage;
    stream >> weapon.ReloadSpeed;

    stream >> weapon.TimeToHolster;
    stream >> weapon.TimeToDispatch;
    stream >> weapon.holstered;

    stream >> weapon.ManaCostOfBullet;
    stream >> weapon.Multishot;
    stream >> weapon.FireRate;
    
    stream >> weapon.BulletVelocity;
    stream >> weapon.Scatter;

    weapon.ReloadTimer = new sf::Clock();
    weapon.HolsterTimer = new sf::Clock();
    weapon.TimeFromLastShot = new sf::Clock();
    weapon.DispatchTimer = new sf::Clock();
    weapon.holstered = false;
	weapon.ManaStorage = { {0, weapon.MaxManaStorage, weapon.MaxManaStorage} };
    weapon.lock = true;
    return stream;
}

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon("Pistol") {}
};

// Revolver
// class Revolver : public Weapon {
// public:
//     Revolver() : Weapon("Revolver", 6, 2, 0, 5, 0.5, 0.5) {
//         BulletVelocity = Upgradable(std::vector<int>(5, 960));
//         Scatter = Upgradable(std::vector<int>(5, 10));
//     }
//     void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
//         Weapon::Shoot(shooter, direction, f);
//         lock = true;
//     }
// };

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon("Shotgun") {}
    
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (!CanShoot()) return;

        sf::Vector2f d = direction - shooter.getCenter();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (Scatter / 2.f), d) * BulletVelocity / len;
        for (int i = 0; i < Multishot; i++, d = RotateOn(M_PI_RAD * Scatter / (Multishot - 1), d)) {
            sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
            Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet));
        }
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
        lock = true;
		ShootSound.setBuffer(SoundBuffers::Shoot);
		ShootSound.setPosition(shooter.getCenter().x, shooter.getCenter().y, 0.f);
		ShootSound.play();
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon("Rifle") {}
};

// Armageddon
// class Armageddon : public Weapon {
// public:
//     Armageddon() : Weapon("Armageddon", 300, 0.1, 1.f / 16, 3, 5, 5) {
//         BulletVelocity = Upgradable(std::vector<int>(5, 180));
//     }
//     void Update(sf::Event& event) {
//         if (mouseButtonPressed(event, sf::Mouse::Left)) {
//             Multishot = 0;
//             lock = false;
//         } if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
//             lock = true;
//     }
//     void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
//         if (!CanShoot()) return;

//         sf::Vector2f d{0, BulletVelocity};
//         d = RotateOn(float(-M_PI * Multishot) / 12, d);
//         sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
//         Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet));
//         ManaStorage -= ManaCostOfBullet;
//         Multishot.stats[Multishot.curLevel]++;
//         TimeFromLastShot->restart();
//     }
// };

// Chaotic
// class Chaotic : public Weapon {
// public:
//     Chaotic() : Weapon("Chaotic", 300, 0.1, 1.f / 16, 3, 5, 5) {
//         BulletVelocity = Upgradable(std::vector<int>(5, 180));
//     }
//     void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
//         if (!CanShoot()) return;

//         sf::Vector2f d{0, BulletVelocity};
//         d = RotateOn(float(rand()), d);
//         sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
//         Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet));
//         ManaStorage -= ManaCostOfBullet;
//         TimeFromLastShot->restart();
//     }
// };