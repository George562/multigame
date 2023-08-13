#pragma once
#include "init.h"

using TileID = sf::Uint16;

namespace Tiles {
    TileID nothing = 0;
    TileID wall = 1;
    TileID box = 2;
    TileID portal = 3;
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Location {
public:
    struct LocationObject { TileID id; sf::Vector2f pos; };

    int n, m;
    std::vector<std::vector<TileID>> walls;
    std::vector<LocationObject> objects;
    vvb EnableTiles;
    size_t AmountOfEnableTiles;

    vvr wallsRect;
    vvb SeenWalls;

    Location() { AmountOfEnableTiles = 0; }
    Location(int w, int h) { SetSize(w, h); AmountOfEnableTiles = 0; }
    void GenerateLocation(int n, int m, sf::Vector2f RootPoint);
    void SetSize(int w, int h);
    std::vector<TileID>& operator[](int index) { return walls[index]; }
    void BuildWayFrom(sf::Vector2f);
    void WallGenerator(float probability);
    bool LoadFromFile(str FileName);
    bool WriteToFile(str FileName);
    void FindEnableTilesFrom(sf::Vector2f);
    void FillWallsRect();
    void AddObject(LocationObject obj) { objects.push_back(obj); }
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
    walls.assign(n * 2 + 1, std::vector<TileID>(0));
    for (int i = 0; i < walls.size(); i++) walls[i].assign(m + (i % 2), Tiles::nothing);
}

void Location::BuildWayFrom(sf::Vector2f p) {
    FindEnableTilesFrom(p);
    AmountOfEnableTiles = 1;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) AmountOfEnableTiles += EnableTiles[i][j] ? 1 : 0;
    Rect UsedAreaRect{0, 0, float(m - 1), float(n - 1)};
    Point check;
    bool todel;
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++) {
            if (walls[i][j] == Tiles::nothing) continue;
            
            todel = true;
            check = Point{j, i / 2};
            if (UsedAreaRect.contains(check.x, check.y) && EnableTiles[check.y][check.x])
                todel = false;
            check = (i % 2 == 0) ? Point{j, i / 2 - 1} : Point{j - 1, i / 2};
            if (UsedAreaRect.contains(check.x, check.y) && EnableTiles[check.y][check.x])
                todel = false;
            if (todel) walls[i][j] = Tiles::nothing;
        }
}

void Location::WallGenerator(float probability) {
    walls[0].assign(m, Tiles::wall);

    for (int i = 1; i < walls.size() - 1; i++) {
        for (int j = 0; j < walls[i].size(); j++)
            walls[i][j] = (float(rand() % 100) / 100 < probability) ? Tiles::wall : Tiles::nothing;
        if (i % 2 == 1) {
            walls[i][0] = Tiles::wall;
            walls[i][m] = Tiles::wall;
        }
    }

    walls[walls.size() - 1].assign(m, Tiles::wall);
    ClearSeenWalls();
    FillWallsRect();
}

bool Location::LoadFromFile(str FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) return false;
    file >> n >> m;
    SetSize(n, m);
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++)
            file >> walls[i][j];
    
    if (!file.eof()) {
        int k; file >> k;
        objects.resize(k);
        for (LocationObject& obj: objects) file >> obj.id >> obj.pos.x >> obj.pos.y;
    }

    file.close();
    FillWallsRect();
    ClearSeenWalls();
    return true;
}

bool Location::WriteToFile(str FileName) {
    std::ofstream file(FileName);
    file << n << ' ' << m << '\n';
    for (int i = 0; i < walls.size(); i++) {
        for (int j = 0; j < walls[i].size(); j++)
            file << walls[i][j] << ' ';
        file << '\n';
    }

    file << objects.size() << '\n';
    for (LocationObject& obj: objects) file << obj.id << ' ' << obj.pos.x << ' ' << obj.pos.y << '\n';

    file.close();
    return true;
}

void Location::FindEnableTilesFrom(sf::Vector2f p) {
    EnableTiles.assign(n, vb(m, false));
    std::queue<Point> q; q.push(Point{int(p.x), int(p.y)});
    Point cur, check;
    Rect UsedAreaRect{0, 0, float(m - 1), float(n - 1)};
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (EnableTiles[cur.y][cur.x]) continue;
        EnableTiles[cur.y][cur.x] = true;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !EnableTiles[check.y][check.x] && walls[check.y * 2 + 1][check.x] == Tiles::nothing)
            q.push(check);
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !EnableTiles[check.y][check.x] && walls[check.y * 2 + 1][check.x + 1] == Tiles::nothing)
            q.push(check);
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check.x, check.y) && !EnableTiles[check.y][check.x] && walls[check.y * 2][check.x] == Tiles::nothing)
            q.push(check);
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check.x, check.y) && !EnableTiles[check.y][check.x] && walls[check.y * 2 + 2][check.x] == Tiles::nothing)
            q.push(check);
    }
}

void Location::GenerateLocation(int n, int m, sf::Vector2f RootPoint) {
    SetSize(n, m);
    objects.clear();
    int CounterOfGenerations = 0;
    do {
        WallGenerator(0.48);
        BuildWayFrom(RootPoint / (float)size);
        CounterOfGenerations++;
    } while (AmountOfEnableTiles < float(n * m) / 3 || AmountOfEnableTiles > float(n * m) / 1.5);
    std::cout << "total count of generations = " << CounterOfGenerations
              << " Count Of Enable Tiles = " << AmountOfEnableTiles << '\n';
}

void Location::FillWallsRect() {
    wallsRect.assign(walls.size(), vr(0));
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++)
            if (walls[i][j] == Tiles::wall) {
                if (i % 2 == 1) // |
                    wallsRect[i].push_back(Rect{size * j - WallMinSize / 2, float(size * i / 2) - WallMaxSize / 2, WallMinSize, WallMaxSize});
                else // -
                    wallsRect[i].push_back(Rect{float(size * j), size * i / 2 - WallMinSize / 2, WallMaxSize, WallMinSize});
            } else wallsRect[i].push_back(Rect{0, 0, 0, 0});
}

sf::Packet& operator>>(sf::Packet& packet, Location& loc) {
    packet >> loc.n >> loc.m;
    std::cout << "n = " << loc.n << " m = " << loc.m << '\n';
    loc.SetSize(loc.n, loc.m);
    for (std::vector<TileID>& line: loc.walls)
        for (TileID& tile: line)
            packet >> tile;

    sf::Uint32 objSize; packet >> objSize;
    loc.objects.resize(objSize);
    std::cout << "count of objects = " << loc.objects.size() << '\n';
    for (int i = 0; i < loc.objects.size(); i++) {
        packet >> loc.objects[i].id >> loc.objects[i].pos.x >> loc.objects[i].pos.y;
        std::cout << "ID: " << loc.objects[i].id << "\tx: " << loc.objects[i].pos.x << "\ty: " << loc.objects[i].pos.y << '\n';
    }
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, Location& loc) {
    packet << sf::Int32(pacetStates::Labyrinth) << loc.n << loc.m;
    for (std::vector<TileID>& line: loc.walls)
        for (TileID& tile: line)
            packet << tile;
    packet << sf::Uint32(loc.objects.size());
    for (int i = 0; i < loc.objects.size(); i++)
        packet << loc.objects[i].id << loc.objects[i].pos.x << loc.objects[i].pos.y;
    return packet;
}

void FindTheWay(Location& where, sf::Vector2f from, sf::Vector2f to, std::vector<sf::Vector2f>& theWay) {
    std::vector<std::vector<sf::Vector2i>> place(where.n, std::vector<sf::Vector2i>(where.m, {-1, -1}));
    std::queue<Point> q; q.push(Point{int(from.x / size), int(from.y / size)});
    Point cur, check;
    Rect UsedAreaRect{0, 0, float(where.m - 1), float(where.n - 1)};
    vvb used(where.n, vb(where.m, false));
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y][cur.x]) continue;
        used[cur.y][cur.x] = true;
        if (sf::Vector2i(to / (float)size) == cur) break;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && where.walls[check.y * 2 + 1][check.x] == Tiles::nothing) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && where.walls[check.y * 2 + 1][check.x + 1] == Tiles::nothing) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && where.walls[check.y * 2][check.x] == Tiles::nothing) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && where.walls[check.y * 2 + 2][check.x] == Tiles::nothing) {
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
    for (; place[cur.y][cur.x] != sf::Vector2i{-1, -1}; cur = place[cur.y][cur.x])
        theWay.push_back(((sf::Vector2f)place[cur.y][cur.x] + sf::Vector2f{0.5f, 0.5f}) * (float)size);
    if (theWay.size() > 1) theWay.pop_back();
}