#pragma once
#include "../Entities/bullet.h"
#include "../Abstracts/upgradable.h"
#include "../Abstracts/scale.h"
#include "../Abstracts/serializable.h"

#define M_PI_RAD M_PI / 180.f

namespace WeaponStats {
    namespace Pistol {
        std::vector<float>    MaxManaStorage   {10, 12, 15, 20, 25};
        std::vector<float>    ReloadSpeed      {2, 3, 5, 8};
        std::vector<sf::Time> TimeToHolster    {sf::seconds(0.5), sf::seconds(0.35), sf::seconds(0.2)};
        std::vector<sf::Time> TimeToDispatch   {sf::seconds(1), sf::seconds(0.8), sf::seconds(0.5)};
        std::vector<sf::Time> FireRate         {sf::seconds(0.35), sf::seconds(0.3), sf::seconds(0.225), sf::seconds(0.15)};
        std::vector<float>    ManaCostOfBullet {2, 3, 4, 5};
        std::vector<int>      Multishot        {1}; 
        std::vector<float>    BulletVelocity   {800, 1100, 1500};
        std::vector<float>    scatter          {20, 15, 8, 1};
    };
    namespace Shotgun {
        std::vector<float>    MaxManaStorage   {15, 25, 40, 60};
        std::vector<float>    ReloadSpeed      {2.5, 5};
        std::vector<sf::Time> TimeToHolster    {sf::seconds(1.5), sf::seconds(1.2), sf::seconds(0.9)};
        std::vector<sf::Time> TimeToDispatch   {sf::seconds(2), sf::seconds(1.5), sf::seconds(1)};
        std::vector<sf::Time> FireRate         {sf::seconds(1), sf::seconds(0.75), sf::seconds(0.25)};
        std::vector<float>    ManaCostOfBullet {5, 10};
        std::vector<int>      Multishot        {10, 12, 15, 20};
        std::vector<float>    BulletVelocity   {600, 900, 1300};
        std::vector<float>    scatter          {50, 40};
    };
    namespace Rifle {
        std::vector<float>    MaxManaStorage   {30, 50, 80, 120};
        std::vector<float>    ReloadSpeed      {1, 2, 4};
        std::vector<sf::Time> TimeToHolster    {sf::seconds(2), sf::seconds(1.5), sf::seconds(1)};
        std::vector<sf::Time> TimeToDispatch   {sf::seconds(1.5), sf::seconds(1)};
        std::vector<sf::Time> FireRate         {sf::seconds(0.25), sf::seconds(0.2), sf::seconds(0.1)};
        std::vector<float>    ManaCostOfBullet {1, 2, 3};
        std::vector<int>      Multishot        {1};
        std::vector<float>    BulletVelocity   {700, 900, 1200};
        std::vector<float>    scatter          {20, 10, 5};
    };
};

////////////////////////////////////////////////////////////
// Weapon
#pragma pack(push, 1)
class Weapon : public Serializable {
public:
    std::string Name;

    Scale<float> ManaStorage;
    Upgradable<float> MaxManaStorage;           // Is used so that we don't have an entire vector of scales, and just change the max for the storage
    Upgradable<float> ReloadSpeed;
    sf::Clock* ReloadTimer = nullptr;

    Upgradable<sf::Time> TimeToHolster;
    Upgradable<sf::Time> TimeToDispatch;
    sf::Clock* HolsterTimer = nullptr;          // Putting the weapon in the holster to reload takes time
    sf::Clock* DispatchTimer = nullptr;         // Same thing with getting it out
    bool holstered;                             // All weapons are active by default. A holstered state for them is when they are being reloaded.

    Upgradable<float> ManaCostOfBullet;         // Is equal to the damage of bullet
    Upgradable<int> Multishot;                  // How many projectiles to fire after 1 shot
    Upgradable<sf::Time> FireRate;
    
    Upgradable<float> BulletVelocity;
    Upgradable<float> Scatter; // at degree

    sf::Clock* TimeFromLastShot = nullptr;
    bool lock;                                  // Bullets are like a stream and "lock" is blocking the stream

    Weapon() {}
    Weapon(std::string name,
           std::vector<float> MaxManaStorage, std::vector<float> ReloadSpeed,
           std::vector<sf::Time> TimeToHolster, std::vector<sf::Time> TimeToDispatch,
           std::vector<sf::Time> FireRate, std::vector<float> ManaCostOfBullet, std::vector<int> Multishot,
           std::vector<float> BulletVelocity, std::vector<float> Scatter) {
        Name = name;

        this->MaxManaStorage   .setStats(MaxManaStorage);
        this->ReloadSpeed      .setStats(ReloadSpeed);

        this->ManaCostOfBullet .setStats(ManaCostOfBullet);
        this->Multishot        .setStats(Multishot);
        this->FireRate         .setStats(FireRate);

        this->TimeToHolster    .setStats(TimeToHolster);
        this->TimeToDispatch   .setStats(TimeToDispatch);

        this->BulletVelocity   .setStats(BulletVelocity);
        this->Scatter          .setStats(Scatter);

        ReloadTimer = new sf::Clock();
        HolsterTimer = new sf::Clock();
        TimeFromLastShot = new sf::Clock();
        DispatchTimer = new sf::Clock();
        holstered = false;
        ManaStorage.setScale(0, MaxManaStorage[0], MaxManaStorage[0]);
        lock = true;
    }
    virtual ~Weapon() {
        if (TimeFromLastShot) {
            delete TimeFromLastShot;
        }
        if (ReloadTimer) {
            delete ReloadTimer;
        }
        if (HolsterTimer) {
            delete HolsterTimer;
        }
        if (DispatchTimer) {
            delete DispatchTimer;
        }
    }

    virtual void Update(sf::Event& event) {
        ManaStorage.top = MaxManaStorage;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
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
        // d = RotateOn(-M_PI_RAD * (rand() % (int)(Scatter - Scatter / 2.f)), d) * BulletVelocity / len;
        // sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        // Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet));
        for (int i = 0; i < Multishot; i++, d = RotateOn(M_PI_RAD * Scatter / (Multishot - 1), d)) {
            sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
            Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet));
        }
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
    }

    virtual void Reload(Scale<float>& Mana) { // Reloads ReloadSpeed/sec
        if (ManaStorage.fromTop() == 0) return;
        if (holstered && HolsterTimer->getElapsedTime() > TimeToHolster) {
            float x = std::min(std::min(std::min(oneOverSixty, ReloadTimer->restart().asSeconds()) * ReloadSpeed,
                                        ManaStorage.fromTop()), Mana.toBottom());
            Mana -= x;
            ManaStorage += x;
            return;
        }
    }

    virtual void HolsterAction() {            // Moves weapon to holster or takes it out of it
        if (holstered && HolsterTimer->getElapsedTime() > TimeToHolster) {
            holstered = false;
            DispatchTimer->restart();
            return;
        }
        if (!holstered && DispatchTimer->getElapsedTime() >= TimeToDispatch) {
            if (ManaStorage.fromTop() == 0) return;
            holstered = true;
            HolsterTimer->restart();
            return;
        }
    }

    json writeJSON() {
        json j;
        j["Name"]               = this->Name;
        j["ManaStorage"]        = this->ManaStorage.writeJSON();
        j["MaxManaStorage"]     = this->MaxManaStorage.writeJSON();
        j["ReloadSpeed"]        = this->ReloadSpeed.writeJSON();
        j["TimeToHolster"]      = this->TimeToHolster.writeJSON();
        j["TimeToDispatch"]     = this->TimeToDispatch.writeJSON();
        j["holstered"]          = this->holstered;
        j["ManaCostOfBullet"]   = this->ManaCostOfBullet.writeJSON();
        j["Multishot"]          = this->Multishot.writeJSON();
        j["FireRate"]           = this->FireRate.writeJSON();
        j["BulletVelocity"]     = this->BulletVelocity.writeJSON();
        j["Scatter"]            = this->Scatter.writeJSON();

        return j;
    }

    void readJSON(json& j) {
        this->Name = j["Name"];
        this->MaxManaStorage   .readJSON(j["MaxManaStorage"]);
        this->ReloadSpeed      .readJSON(j["ReloadSpeed"]);

        this->ManaCostOfBullet .readJSON(j["ManaCostOfBullet"]);
        this->Multishot        .readJSON(j["Multishot"]);
        this->FireRate         .readJSON(j["FireRate"]);

        this->TimeToHolster    .readJSON(j["TimeToHolster"]);
        this->TimeToDispatch   .readJSON(j["TimeToDispatch"]);

        this->BulletVelocity   .readJSON(j["BulletVelocity"]);
        this->Scatter          .readJSON(j["Scatter"]);

        this->ReloadTimer = new sf::Clock();
        this->HolsterTimer = new sf::Clock();
        this->TimeFromLastShot = new sf::Clock();
        this->DispatchTimer = new sf::Clock();
        this->holstered = false;
        this->ManaStorage = {0, this->MaxManaStorage, this->MaxManaStorage};
        this->lock = true;
    }
};
#pragma pack(pop)
////////////////////////////////////////////////////////////

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
    weapon.ManaStorage = {0, weapon.MaxManaStorage, weapon.MaxManaStorage};
    weapon.lock = true;
    return stream;
}

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon("Pistol",
                      WeaponStats::Pistol::MaxManaStorage  ,
                      WeaponStats::Pistol::ReloadSpeed     ,

                      WeaponStats::Pistol::TimeToHolster   ,
                      WeaponStats::Pistol::TimeToDispatch  ,

                      WeaponStats::Pistol::FireRate        ,
                      WeaponStats::Pistol::ManaCostOfBullet,
                      WeaponStats::Pistol::Multishot       ,

                      WeaponStats::Pistol::BulletVelocity  ,
                      WeaponStats::Pistol::scatter          ) {}
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
    Shotgun() : Weapon("Shotgun",
                       WeaponStats::Shotgun::MaxManaStorage  ,
                       WeaponStats::Shotgun::ReloadSpeed     ,

                       WeaponStats::Shotgun::TimeToHolster   ,
                       WeaponStats::Shotgun::TimeToDispatch  ,

                       WeaponStats::Shotgun::FireRate        ,
                       WeaponStats::Shotgun::ManaCostOfBullet,
                       WeaponStats::Shotgun::Multishot       ,

                       WeaponStats::Shotgun::BulletVelocity  ,
                       WeaponStats::Shotgun::scatter          ) {}
    
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
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon("Rifle",
                     WeaponStats::Rifle::MaxManaStorage  ,
                     WeaponStats::Rifle::ReloadSpeed     ,

                     WeaponStats::Rifle::TimeToHolster   ,
                     WeaponStats::Rifle::TimeToDispatch  ,

                     WeaponStats::Rifle::FireRate        ,
                     WeaponStats::Rifle::ManaCostOfBullet,
                     WeaponStats::Rifle::Multishot       ,

                     WeaponStats::Rifle::BulletVelocity  ,
                     WeaponStats::Rifle::scatter          ) {}
};

// Bubblegun
// class Bubblegun : public Weapon {
// public:
//     Bubblegun() : Weapon("Bubblegun", 30, 3, 0.03, 4, 3, 1) {
//         BulletVelocity = Upgradable(std::vector<int>(5, 540));
//         Multishot = Upgradable(std::vector<int>(5, 10));
//         Scatter = Upgradable(std::vector<int>(5, 40));
//     }
//     void Update(sf::Event& event) {
//         if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
//             lock = false;
//     }
//     void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
//         if (!CanShoot()) return;

//         sf::Vector2f d = direction - shooter.getCenter();
//         float len = hypotf(d.x, d.y);
//         if (len == 0) return;
//         d = RotateOn(-M_PI_RAD * (rand() % (int)(Scatter - Scatter / 2.f)), d) * BulletVelocity / len;
//         sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
//         Bullets.push_back(new Bullet(f, SpawnPoint, d, ManaCostOfBullet, COMMON_BULLET_PENETRATION, Bullet::Bubble, sf::seconds(1)));
//         ManaStorage -= ManaCostOfBullet;
//         TimeFromLastShot->restart();
//         if (--(Multishot.stats[Multishot.curLevel]) == 0) {
//             Multishot = 10;
//             lock = true;
//         }
//     }
// };

// Armageddon
// class Armageddon : public Weapon {
// public:
//     Armageddon() : Weapon("Armageddon", 300, 0.1, 1.f / 16, 3, 5, 5) {
//         BulletVelocity = Upgradable(std::vector<int>(5, 180));
//     }
//     void Update(sf::Event& event) {
//         if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
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