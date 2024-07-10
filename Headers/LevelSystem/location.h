#pragma once
#include "../init.h"
#include <queue>
#include <iostream>
#include <fstream>

using vb = std::vector<bool>;
using vvb = std::vector<std::vector<bool>>;

sf::Vector2i dirs[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

// namespace Side { может понадобиться, если нужно будет опрделять положение чего-то
//     using Type = sf::Uint8;
//     enum : Type {
//         Left,
//         Top,
//         Right,
//         Bottom
//     };
// }

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Location {
public:
    int n, m;
    vvb walls;
    vvb EnableTiles;
    size_t AmountOfEnableTiles;

    vvr wallsRect;
    vvb SeenWalls;

    sf::Vector2i room;
    sf::Vector2i doorPos;

    Location() { AmountOfEnableTiles = 0; room = sf::Vector2i(-1, -1); }
    Location(int w, int h) { SetSize(w, h); room = sf::Vector2i(-1, -1); }
    void SetSize(int w, int h);
    void GenerateLocation(int n, int m, sf::Vector2f RootPoint);
    void BuildWayFrom(sf::Vector2f);
    void WallGenerator(float probability);
    void RoomGenerator();
    std::vector<sf::Vector2i> getPassages(int _n, int _m);
    void FindEnableTilesFrom(sf::Vector2f&);
    void FillWallsRect();
    bool LoadFromFile(std::string FileName);
    bool WriteToFile(std::string FileName);
    void ClearSeenWalls() {
        SeenWalls.assign(walls.size(), vb(0));
        for (int i = 0; i < walls.size(); i++)
            SeenWalls[i].assign(walls[i].size(), false);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

void Location::SetSize(int NewN, int NewM) {
    n = NewN;
    m = NewM;
    walls.assign(n * 2 + 1, vb(0));
    for (int i = 0; i < walls.size(); i++) walls[i].assign(m + (i % 2), false);
    EnableTiles.assign(n, vb(m, false));
    AmountOfEnableTiles = 0;
    ClearSeenWalls();
}

void Location::GenerateLocation(int n, int m, sf::Vector2f RootPoint) {
    SetSize(n, m);
    int CounterOfGenerations = 0;
    sf::Clock timer;
    do {
        WallGenerator(0.48);
        BuildWayFrom(RootPoint);
        CounterOfGenerations++;
    } while (AmountOfEnableTiles < float(n * m) * 0.3f || AmountOfEnableTiles > float(n * m) * 0.7f);
    RoomGenerator();
    FillWallsRect();
    std::cout << "Location was generated in " << timer.getElapsedTime().asSeconds() << " seconds with total number of generations = "
              << CounterOfGenerations << "; Count Of Enable Tiles = " << AmountOfEnableTiles << '\n';
}

void Location::BuildWayFrom(sf::Vector2f p) {
    FindEnableTilesFrom(p);
    sf::IntRect UsedAreaRect(0, 0, m, n);
    sf::Vector2i check;
    bool todel;
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++) {
            if (!walls[i][j]) continue;

            todel = true;
            check = sf::Vector2i(j, i / 2);
            if (UsedAreaRect.contains(check) && EnableTiles[check.y][check.x])
                todel = false;
            check = (i % 2 == 0) ? sf::Vector2i(j, i / 2 - 1) : sf::Vector2i(j - 1, i / 2);
            if (UsedAreaRect.contains(check) && EnableTiles[check.y][check.x])
                todel = false;
            if (todel) walls[i][j] = false;
        }
}

void Location::WallGenerator(float probability) {
    walls[0].assign(m, true);

    for (int i = 1; i < walls.size() - 1; i++) {
        for (int j = 0; j < walls[i].size(); j++)
            walls[i][j] = (float(rand() % 100) / 100 < probability);
        if (i % 2 == 1) {
            walls[i][0] = true;
            walls[i][m] = true;
        }
    }

    walls[walls.size() - 1].assign(m, true);
    ClearSeenWalls();
}

void Location::RoomGenerator() {
    std::vector<sf::Vector2i> probabblePositions;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (EnableTiles[i][j] && getPassages(i, j).size() == 1)
                probabblePositions.emplace_back(j, i);
    if (probabblePositions.size() != 0) {
        room = probabblePositions[rand() % probabblePositions.size()];
        doorPos = getPassages(room.y, room.x)[0];
        walls[doorPos.y][doorPos.x] = true;
    } else {
        std::cout << "Room hasn't been created\n";
        room = sf::Vector2i(-1, -1);
    }
}

std::vector<sf::Vector2i> Location::getPassages(int _n, int _m) {
    std::vector<sf::Vector2i> passages;
    if (!walls[_n * 2 + 1][_m    ]) passages.push_back(sf::Vector2i(_m    , _n * 2 + 1)); // стена слева
    if (!walls[_n * 2 + 1][_m + 1]) passages.push_back(sf::Vector2i(_m + 1, _n * 2 + 1)); // стена справа
    if (!walls[_n * 2    ][_m    ]) passages.push_back(sf::Vector2i(_m    , _n * 2    )); // стена сверху
    if (!walls[_n * 2 + 2][_m    ]) passages.push_back(sf::Vector2i(_m    , _n * 2 + 2)); // стена снизу
    return passages;
}

void Location::FindEnableTilesFrom(sf::Vector2f& p) {
    EnableTiles.assign(n, vb(m, false));
    AmountOfEnableTiles = 0;
    std::queue<sf::Vector2i> q; q.push(sf::Vector2i(p));
    sf::Vector2i cur, check;
    sf::IntRect UsedAreaRect(0, 0, m, n);
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (EnableTiles[cur.y][cur.x]) continue;
        EnableTiles[cur.y][cur.x] = true;
        AmountOfEnableTiles++;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2 + 1][check.x])
            q.push(check);
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2 + 1][check.x + 1])
            q.push(check);
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2][check.x])
            q.push(check);
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2 + 2][check.x])
            q.push(check);
    }
}

void Location::FillWallsRect() {
    wallsRect.assign(walls.size(), vr(0));

    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++)
            if (walls[i][j]) {
                if (i % 2 == 1) // |
                    wallsRect[i].push_back(CollisionRect(size * j - WallMinSize / 2, float(size * i / 2) - WallMaxSize / 2, WallMinSize, WallMaxSize));
                else // -
                    wallsRect[i].push_back(CollisionRect(float(size * j), size * i / 2 - WallMinSize / 2, WallMaxSize, WallMinSize));
            } else wallsRect[i].push_back(CollisionRect(0, 0, 0, 0));

    if (room.x != -1 && room.y != -1) {
        if (doorPos.y % 2 == 1)
            wallsRect[doorPos.y][doorPos.x].setSize(WallMinSize / 2, WallMaxSize / 2);
        else
            wallsRect[doorPos.y][doorPos.x].setSize(WallMaxSize / 2, WallMinSize / 2);
    }
}

bool Location::LoadFromFile(std::string FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) return false;
    file >> n >> m;
    SetSize(n, m);
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0, t; j < walls[i].size(); j++) {
            file >> t;
            walls[i][j] = t;
        }

    file.close();
    FillWallsRect();
    ClearSeenWalls();
    return true;
}

bool Location::WriteToFile(std::string FileName) {
    std::ofstream file(FileName);
    file << n << ' ' << m << '\n';
    for (int i = 0; i < walls.size(); i++) {
        for (int j = 0; j < walls[i].size(); j++)
            file << walls[i][j] << ' ';
        file << '\n';
    }
    file.close();
    return true;
}

////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////

sf::Packet& operator>>(sf::Packet& packet, Location& loc) {
    packet >> loc.n >> loc.m;
    std::cout << "n = " << loc.n << " m = " << loc.m << '\n';
    loc.SetSize(loc.n, loc.m);
    for (int i = 0; i < loc.walls.size(); i++)
        for (int j = 0, t; j < loc.walls[i].size(); j++) {
            packet >> t;
            loc.walls[i][j] = t;
        }
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, Location& loc) {
    packet << sf::Int32(pacetStates::Labyrinth) << loc.n << loc.m;
    for (int i = 0; i < loc.walls.size(); i++)
        for (int j = 0; j < loc.walls[i].size(); j++)
            packet << loc.walls[i][j];
    return packet;
}

void FindTheWay(Location* where, sf::Vector2f from, sf::Vector2f to, std::vector<sf::Vector2f>& theWay) {
    std::vector<std::vector<sf::Vector2i>> place(where->n, std::vector<sf::Vector2i>(where->m, sf::Vector2i(-1, -1)));
    std::queue<sf::Vector2i> q; q.push(sf::Vector2i(from.x / size, from.y / size));
    sf::Vector2i cur, check;
    sf::IntRect UsedAreaRect(0, 0, where->m, where->n);
    vvb used(where->n, vb(where->m, false));
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y][cur.x]) continue;
        used[cur.y][cur.x] = true;
        if (sf::Vector2i(to / (float)size) == cur) break;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2 + 1][check.x]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2 + 1][check.x + 1]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2][check.x]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2 + 2][check.x]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
    }

    theWay.clear();
    theWay.push_back(to);
    if (sf::Vector2i(to / (float)size) != cur) {
        std::cout << "Don't find the way\n";
        cur = {-1, -1};
    }
    for (; place[cur.y][cur.x] != sf::Vector2i(-1, -1); cur = place[cur.y][cur.x])
        theWay.push_back(((sf::Vector2f)place[cur.y][cur.x] + sf::Vector2f(0.5f, 0.5f)) * (float)size);
    if (theWay.size() > 1) theWay.pop_back();
}

void FindAllWaysTo(Location* location, sf::Vector2f to, std::vector<std::vector<sf::Vector2f>>& theWays) {
    if (theWays.size() != location->n || theWays[0].size() != location->m) {
        theWays.assign(location->n, std::vector<sf::Vector2f>(location->m, to));
    }
    std::queue<sf::Vector2i> q; q.push(sf::Vector2i(to.x / size, to.y / size));
    sf::Vector2i cur, check;
    sf::IntRect UsedAreaRect(0, 0, location->m, location->n);
    vvb used(location->n, vb(location->m, false));
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y][cur.x]) continue;
        used[cur.y][cur.x] = true;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !location->walls[check.y * 2 + 1][check.x]) {
            q.push(check);
            theWays[check.y][check.x] = ((sf::Vector2f)cur + sf::Vector2f(0.5f, 0.5f)) * (float)size;
        }
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !location->walls[check.y * 2 + 1][check.x + 1]) {
            q.push(check);
            theWays[check.y][check.x] = ((sf::Vector2f)cur + sf::Vector2f(0.5f, 0.5f)) * (float)size;
        }
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !location->walls[check.y * 2][check.x]) {
            q.push(check);
            theWays[check.y][check.x] = ((sf::Vector2f)cur + sf::Vector2f(0.5f, 0.5f)) * (float)size;
        }
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !location->walls[check.y * 2 + 2][check.x]) {
            q.push(check);
            theWays[check.y][check.x] = ((sf::Vector2f)cur + sf::Vector2f(0.5f, 0.5f)) * (float)size;
        }
    }
    check = (sf::Vector2i)to / size; theWays[check.y][check.x] = to;

    check = (sf::Vector2i)to / size + dirs[0]; // {1, 0}
    if (check.x < location->m && !location->walls[check.y * 2 + 1][check.x])
        theWays[check.y][check.x] = to;
    check = (sf::Vector2i)to / size + dirs[2]; // {-1, 0}
    if (check.x >= 0 && !location->walls[check.y * 2 + 1][check.x + 1])
        theWays[check.y][check.x] = to;
    check = (sf::Vector2i)to / size + dirs[1]; // {0, 1}
    if (check.y  < location->n && !location->walls[check.y * 2][check.x])
        theWays[check.y][check.x] = to;
    check = (sf::Vector2i)to / size + dirs[3]; // {0, -1}
    if (check.y >= 0 && !location->walls[check.y * 2 + 2][check.x])
        theWays[check.y][check.x] = to;
}

// {x = 1, y = -1} => collision at the y, up or down doesn't matter, because u know "dy" already
sf::Vector2i WillCollisionWithWalls(vvr& Walls, CollisionShape& obj, sf::Vector2f Velocity) {
    int y = int(obj.getCenter().y) / size, x = int(obj.getCenter().x) / size;
    sf::Vector2i res(Velocity.x != 0 ? -1 : 1, Velocity.y != 0 ? -1 : 1);

    obj.move(0, Velocity.y);
    if (Velocity.y < 0) {
        if ((y * 2 - 1 < 0 || !Walls[y * 2 - 1][x].intersect(obj)) &&
            (!Walls[y * 2][x].intersect(obj)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x + 1].intersect(obj)) &&
            (x + 1 >= Walls[0].size() || !Walls[y * 2][x + 1].intersect(obj)) &&
            (x - 1 < 0 || !Walls[y * 2][x - 1].intersect(obj)))
            res.y = 1;
    }
    if (Velocity.y > 0) {
        if ((y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x].intersect(obj)) &&
            (!Walls[y * 2 + 2][x].intersect(obj)) &&
            (y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x + 1].intersect(obj)) &&
            (x + 1 >= Walls[0].size() || !Walls[y * 2 + 2][x + 1].intersect(obj)) &&
            (x - 1 < 0 || !Walls[y * 2 + 2][x - 1].intersect(obj)))
            res.y = 1;
    }
    obj.move(0, -Velocity.y);
    obj.move(Velocity.x, 0);
    if (Velocity.x < 0) {
        if ((x - 1 < 0 || !Walls[y * 2][x - 1].intersect(obj)) &&
            (!Walls[y * 2 + 1][x].intersect(obj)) &&
            (x - 1 < 0 || !Walls[y * 2 + 2][x - 1].intersect(obj)) &&
            (y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x].intersect(obj)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x].intersect(obj)))
            res.x = 1;
    }
    if (Velocity.x > 0) {
        if ((x + 1 >= Walls[0].size() || !Walls[y * 2][x + 1].intersect(obj)) &&
            (!Walls[y * 2 + 1][x + 1].intersect(obj)) &&
            (x + 1 >= Walls[0].size() || !Walls[y * 2 + 2][x + 1].intersect(obj)) &&
            (y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x + 1].intersect(obj)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x + 1].intersect(obj)))
            res.x = 1;
    }
    obj.move(-Velocity.x, 0);
    return res;  // if value of vector == -1 => there was collision
}

////////////////////////////////////////////////////////////
// perhaps unusable code, but usefull for future
////////////////////////////////////////////////////////////

// void Location::WallGenerator(float probability) {
    // auto gen = []() {
    //     thread_local std::mt19937 rng{std::random_device()()};
    //     thread_local std::uniform_real_distribution<float> distr(0, 100);
    //     return distr(rng);
    // };
    // if (walls.size() == 0) return;

    // walls[0].assign(m, true);
    // walls[walls.size() - 1].assign(m, true);

    // sf::Thread* thr[7];
    // int from = 1, to = walls.size() / 8;
    // for (int i = 0; i < 7;) {
    //     thr[i] = new sf::Thread([&, to, from] {
    //         for (int i = from; i < to; i++) {
    //             for (int j = 0; j < walls[i].size(); j++)
    //                 walls[i][j] = (gen() / 100 < probability);
    //             if (i % 2 == 1)
    //                 walls[i][0] = walls[i][m] = true;
    //         }
    //     });
    //     thr[i++]->launch();
    //     from = i * walls.size() / 8; to = (i + 1) * walls.size() / 8;
    // }
    // for (int i = from; i < to - 1; i++) {
    //     for (int j = 0; j < walls[i].size(); j++)
    //         walls[i][j] = (gen() / 100 < probability);
    //     if (i % 2 == 1)
    //         walls[i][0] = walls[i][m] = true;
    // }
    // ClearSeenWalls();
    // for (int i = 0; i < 7; i++)
    //     thr[i]->wait();
    // FillWallsRect();
// }