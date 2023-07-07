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
    int n, m;
    std::vector<std::vector<TileID>> data; // location itself

    Location() {}
    Location(int w, int h) { SetSize(w, h); }
    void SetSize(int w, int h);
    std::vector<TileID>& operator[](int index) { return data[index]; }
    int BuildWayFrom(int x, int y);
    void WallGenerator(float probability);
    bool LoadLocationFromFile(str FileName);
    bool WriteLocationToFile(str FileName);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

void Location::SetSize(int NewN, int NewM) {
    n = NewN;
    m = NewM;
    data.assign(n * 2 + 1, std::vector<TileID>(0));
    for (int i = 0; i < data.size(); i++) data[i].assign(m + (i % 2), Tiles::nothing);
}

int Location::BuildWayFrom(int x, int y) {
    vvb used(n, vb(m, false));
    std::queue<Point> q; q.push(Point{x, y});
    int res = 1;
    Point cur, check;
    Rect UsedAreaRect{0, 0, float(m - 1), float(n - 1)};
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
        for (int j = 0; j < data[i].size(); j++)
            data[i][j] = (float(rand() % 100) / 100 < probability) ? Tiles::wall : Tiles::nothing;
        if (i % 2 == 1) {
            data[i][0] = Tiles::wall;
            data[i][m] = Tiles::wall;
        }
    }

    data[data.size() - 1].assign(m, Tiles::wall);
}

bool Location::LoadLocationFromFile(str FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) return false;
    int n, m; file >> n >> m;
    SetSize(n, m);
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