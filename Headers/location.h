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

    Location() {}
    Location(int w, int h) { SetSize(w, h); }
    void SetSize(int w, int h);
    std::vector<TileID>& operator[](int index) { return walls[index]; }
    int BuildWayFrom(sf::Vector2f);
    void WallGenerator(float probability);
    bool LoadFromFile(str FileName);
    bool WriteToFile(str FileName);
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

int Location::BuildWayFrom(sf::Vector2f p) {
    vvb used(n, vb(m, false));
    std::queue<Point> q; q.push(Point{int(p.x), int(p.y)});
    int res = 1;
    Point cur, check;
    Rect UsedAreaRect{0, 0, float(m - 1), float(n - 1)};
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y][cur.x]) continue;
        used[cur.y][cur.x] = true;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && walls[check.y * 2 + 1][check.x] == Tiles::nothing) {
            q.push(check); res++;
        }
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && walls[check.y * 2 + 1][check.x + 1] == Tiles::nothing) {
            q.push(check); res++;
        }
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && walls[check.y * 2][check.x] == Tiles::nothing) {
            q.push(check); res++;
        }
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && walls[check.y * 2 + 2][check.x] == Tiles::nothing) {
            q.push(check); res++;
        }
    }
    bool todel;
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++) {
            todel = true;
            check = Point{j, i / 2};
            if (UsedAreaRect.contains(check.x, check.y) && used[check.y][check.x])
                todel = false;
            check = (i % 2 == 0) ? Point{j, i / 2 - 1} : Point{j - 1, i / 2};
            if (UsedAreaRect.contains(check.x, check.y) && used[check.y][check.x])
                todel = false;
            if (todel) walls[i][j] = Tiles::nothing;
        }
    return res;
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
    file.close();
    return true;
}

void CreateMapRectByLocation(Location& arr, vvr& wallsRect, std::vector<sf::Sprite>& Sprites) {
    Sprites.clear();
    wallsRect.assign(arr.walls.size(), vr(0));
    for (int i = 0; i < arr.walls.size(); i++)
        for (int j = 0; j < arr[i].size(); j++)
            if (arr[i][j] == Tiles::wall) {
                if (i % 2 == 1) // |
                    wallsRect[i].push_back(Rect{size * j - WallMinSize / 2, float(size * i / 2) - WallMaxSize / 2, WallMinSize, WallMaxSize});
                else // -
                    wallsRect[i].push_back(Rect{float(size * j), size * i / 2 - WallMinSize / 2, WallMaxSize, WallMinSize});
            } else wallsRect[i].push_back(Rect{0, 0, 0, 0});
    // wallsRect[i][j].setRect(-1, -1, -1, -1);
    // if (arr[i][j] == Tiles::box) {
    //     sf::Texture* tempTexture = new sf::Texture;
    //     tempTexture->loadFromFile("sources/Box.png");
    //     sf::Sprite* tempSprite = new sf::Sprite;
    //     tempSprite->setTexture(*tempTexture);
    //     tempSprite->setScale(250 / tempSprite->getGlobalBounds().width, 250 / tempSprite->getGlobalBounds().height);
    //     tempSprite->setPosition(size * j / 2, size * i / 2);
    //     Sprites.push_back(*tempSprite);
    // }
}