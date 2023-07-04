#include "init.h"

using TileID = sf::Uint16;

namespace Tiles {
    TileID nothing = 0;
    TileID wall = 1;
    TileID box = 2;
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Location {
public:
    size_t n, m;
    std::vector<std::vector<TileID>> data; // location itself

    Location() {}
    Location(size_t w, size_t h) { SetSize(w, h); }
    void SetSize(size_t w, size_t h);
    std::vector<TileID>& operator[](size_t index) { return data[index]; }
    size_t BuildWayFrom(int x, int y);
    void WallGenerator(float probability);
    bool LoadLocationFromFile(str FileName);
    bool WriteLocationToFile(str FileName);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

void Location::SetSize(size_t w, size_t h) {
    m = w;
    n = h;
    data.assign(n * 2 + 1, std::vector<TileID>(m, Tiles::nothing));
    for (size_t i = 1; i < data.size(); i += 2) data[i].push_back(Tiles::nothing);
}

size_t Location::BuildWayFrom(int x, int y) {
    vvb used(n, vb(m, false));
    std::queue<Point> q; q.push(Point{x, y});
    size_t res = 1;
    Point cur, check;
    Rect UsedAreaRect{0, 0, float(used.size()) - 1, float(used[0].size()) - 1};
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y][cur.x]) continue;
        used[cur.y][cur.x] = true;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && data[check.y * 2 + 1][check.x] == Tiles::nothing) {
            q.push(check); res++;
        }
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && data[check.y * 2 + 1][check.x + 1] == Tiles::nothing) {
            q.push(check); res++;
        }
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && data[check.y * 2][check.x] == Tiles::nothing) {
            q.push(check); res++;
        }
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check.x, check.y) && !used[check.y][check.x] && data[check.y * 2 + 2][check.x] == Tiles::nothing) {
            q.push(check); res++;
        }
    }
    bool todel;
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[i].size(); j++) {
            todel = true;
            check = Point{j, i / 2};
            if (UsedAreaRect.contains(check.x, check.y) && used[check.y][check.x])
                todel = false;
            check = (i % 2 == 0) ? Point{j, i / 2 - 1} : Point{j - 1, i / 2};
            if (UsedAreaRect.contains(check.x, check.y) && used[check.y][check.x])
                todel = false;
            if (todel) data[i][j] = Tiles::nothing;
        }
    return res;
}

void Location::WallGenerator(float probability) {
    data[0].assign(m, Tiles::wall);

    for (int i = 1; i < data.size() - 1; i++) {
        if (i % 2 == 1) data[i][0] = Tiles::wall;
        for (int j = i % 2; j < data[i].size() - 1; j++)
            data[i][j] = (float(rand() % 100) / 100 < probability) ? Tiles::wall : Tiles::nothing;
        if (i % 2 == 1) data[i][data[i].size() - 1] = Tiles::wall;
    }

    data[data.size() - 1].assign(m, Tiles::wall);
}

bool Location::LoadLocationFromFile(str FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) return false;
    int n, m; file >> n >> m;
    SetSize(m, n);
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[i].size(); j++)
            file >> data[i][j];
    file.close();
    return true;
}

bool Location::WriteLocationToFile(str FileName) {
    std::ofstream file(FileName);
    file << n << ' ' << m << '\n';
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++)
            file << data[i][j] << ' ';
        file << '\n';
    }
    file.close();
    return true;
}