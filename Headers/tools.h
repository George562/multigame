#pragma once
#include "init.h"

// check existing element in vector
template <typename T>
bool in(const std::vector<T>& arr, const T& x) {
    for (const T& y: arr) if (y == x) return true;
    return false;
}

bool in(const std::vector<Rect>& arr, const float x, const float y, const float w = 0, const float h = 0) {
    for (const Rect& t: arr)
        if (t.PosX <= x + w && x <= t.PosX + t.Width && t.PosY <= y + h && y <= t.PosY + t.Height)
            return true;
    return false;
}
// Rect in Rect{x, y, w, h}
bool in(const Rect& t, const float& x, const float& y, const float& w = 0, const float& h = 0) {
    return t.PosX <= x + w && x <= t.PosX + t.Width && t.PosY <= y + h && y <= t.PosY + t.Height;
}
// Rect in Rect
bool in(const Rect& a, const Rect& b) {
    return a.PosX <= b.PosX + b.Width && b.PosX <= a.PosX + a.Width && a.PosY <= b.PosY + b.Height && b.PosY <= a.PosY + a.Height;
}
// Point in Rect
bool in(const sf::Vector2f& a, const float& x, const float& y, const float& w = 0, const float& h = 0) {
    return a.x <= x + w && x <= a.x && a.y <= y + h && y <= a.y;
}
bool in(const float ax, const float ay, const float& x, const float& y, const float& w = 0, const float& h = 0) {
    return ax <= x + w && x <= ax && ay <= y + h && y <= ay;
}

// {x = 1, y = -1} => collision at the y, up or down doesn't matter, because u know "dy" already
sf::Vector2i WillCollisionWithWalls(vvr Walls, float& PosX, float& PosY, float& Width, float& Height, float& dx, float& dy) {
    sf::Vector2i res = {-1, -1};
    int y = (int(PosY) / size) * 2, x = (int(PosX) / size) * 2;
    int WallHight = Walls.size(), WallLength = Walls[0].size();
    if (0 > y || y > WallHight || 0 > x || x > WallLength) return res;

    float nextX = PosX + dx, nextY = PosY + dy;

    if (dy < 0) {
        if ((y <= 0 || !in(Walls[y - 1][x],              PosX, nextY, Width, Height)) &&
            (!in(Walls[y][x + 1],                        PosX, nextY, Width, Height)) &&
            (y <= 0 || !in(Walls[y - 1][x + 2],          PosX, nextY, Width, Height)) &&
            (x + 3 >= WallLength || !in(Walls[y][x + 3], PosX, nextY, Width, Height)))
            res.y = 1;
    }
    if (dx < 0) {
        if ((x <= 0 || !in(Walls[y][x - 1],             nextX, PosY, Width, Height)) &&
            (!in(Walls[y + 1][x],                       nextX, PosY, Width, Height)) &&
            (x <= 0 || !in(Walls[y + 2][x - 1],         nextX, PosY, Width, Height)) &&
            (y + 3 >= WallHight || !in(Walls[y + 3][x], nextX, PosY, Width, Height)))
            res.x = 1;
    }
    if (dy > 0) {
        if ((y + 3 >= WallHight || !in(Walls[y + 3][x],      PosX, nextY, Width, Height)) &&
            (!in(Walls[y + 2][x + 1],                        PosX, nextY, Width, Height)) &&
            (y + 3 >= WallHight || !in(Walls[y + 3][x + 2],  PosX, nextY, Width, Height)) &&
            (x + 3 >= WallLength || !in(Walls[y + 2][x + 3], PosX, nextY, Width, Height)))
            res.y = 1;
    }
    if (dx > 0) {
        if ((x + 3 >= WallLength || !in(Walls[y][x + 3],     nextX, PosY, Width, Height)) &&
            (!in(Walls[y + 1][x + 2],                        nextX, PosY, Width, Height)) &&
            (x + 3 >= WallLength || !in(Walls[y + 2][x + 3], nextX, PosY, Width, Height)) &&
            (y + 3 >= WallHight || !in(Walls[y + 3][x + 2],  nextX, PosY, Width, Height)))
            res.x = 1;
    }
    return res;  // if value of vector == -1 => there was collision
}

void find_ways(vp& arr, location& walls, const int x, const int y, const int n, const int m) {
    arr.clear();
    vvb used(n, vb(m, false));
    std::queue<Point> q; q.push(Point{x, y});
    arr.push_back(Point{x, y});
    int N = n * 2, M = m * 2;
    Point cur, check, next;
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y / 2][cur.x / 2]) continue;
        used[cur.y / 2][cur.x / 2] = true;
        for (Point dir: dirs) {
            check = cur + dir;
            next = check + dir;
            if (M > next.x && next.x > 0 && 0 < next.y && next.y < N && !used[next.y / 2][next.x / 2] && !walls[check.y][check.x]) {
                q.push(next);
                arr.push_back(next);
            }
        }
    }
    bool todel;
    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= M; j++) {
            todel = true;
            for (Point& dir: dirs) {
                check = Point{j, i} + dir;
                if (check.y % 2 == 0 || check.x % 2 == 0) continue;
                if (M > check.x && check.x > 0 && 0 < check.y && check.y < N && used[check.y / 2][check.x / 2]) {
                    todel = false;
                    break;
                }
            }
            if (todel) walls[i][j] = false;
        }
}

void generation(location& arr, int& n, int& m, float probability) {
    int N = n * 2, M = m * 2;
    arr.assign(N + 1, vu(M + 1, 0));

    for (int j = 1; j < M; j += 2) arr[0][j] = 1;

    for (int i = 1; i < N; i++) {
        if (i % 2 == 1) arr[i][0] = 1;
        for (int j = 1 + i % 2; j < M; j += 2)
            if (float(rand() % 100) / 100 < probability)
                arr[i][j] = 1;
        if (i % 2 == 1) arr[i][M] = 1;
    }

    for (int j = 1; j < M; j += 2) arr[N][j] = 1;
}

void RotateOn(float phi, float& x, float& y) {
    float oldX = x, OldY = y;
    x =   oldX * cos(phi) + OldY * sin(phi);
    y = - oldX * sin(phi) + OldY * cos(phi);
}
sf::Vector2f RotateOn(float phi, sf::Vector2f& a) {
    sf::Vector2f newA;
    newA.x =   a.x * cos(phi) + a.y * sin(phi);
    newA.y = - a.x * sin(phi) + a.y * cos(phi);
    return newA;
}

void RotateAround(float phi, float& x, float& y, float& X, float& Y) {
    float oldX = x, OldY = y;
    x =   (oldX - X) * cos(phi) + (OldY - Y) * sin(phi) + X;
    y = - (oldX - X) * sin(phi) + (OldY - Y) * cos(phi) + Y;
}
sf::Vector2f RotateAround(float phi, sf::Vector2f& a, float& X, float& Y) {
    sf::Vector2f newA;
    newA.x =   (a.x - X) * cos(phi) + (a.y - Y) * sin(phi) + X;
    newA.y = - (a.x - X) * sin(phi) + (a.y - Y) * cos(phi) + Y;
    return newA;
}

bool LoadLocationFromFile(location& arr, str FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) return false;
    int n, m; file >> n >> m;
    arr.assign(n, vu(m, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            file >> arr[i][j];
    file.close();
    return true;
}

void CreateWallRectByLocation(location& arr, vvr& wallsRect, std::vector<sf::Sprite>& Sprites) {
    int n = arr.size(), m = arr[0].size();
    Sprites.clear();
    wallsRect.assign(n, vr(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (arr[i][j] == LocationIndex::wall) {
                if (i % 2 == 1) // |
                    wallsRect[i][j].setRect((size * j - WallMinSize) / 2, size * (i - 1) / 2.f, WallMinSize, float(size));
                else // -
                    wallsRect[i][j].setRect(size * (j - 1) / 2.f, (size * i - WallMinSize) / 2, float(size), WallMinSize);
            } else  {
                wallsRect[i][j].setRect(-1, -1, -1, -1);
                if (arr[i][j] == LocationIndex::box) {
                    sf::Texture* tempTexture = new sf::Texture;
                    tempTexture->loadFromFile("sources/Box.png");
                    sf::Sprite* tempSprite = new sf::Sprite;
                    tempSprite->setTexture(*tempTexture);
                    tempSprite->setScale(250 / tempSprite->getGlobalBounds().width, 250 / tempSprite->getGlobalBounds().height);
                    tempSprite->setPosition(size * j / 2, size * i / 2);
                    Sprites.push_back(*tempSprite);
                }
            }
        }
}

Rect& operator+=(Rect& a, Point& b) { a.PosX += b.x; a.PosY += b.y; return a; } // summ position

sf::Packet& operator<<(sf::Packet& packet, Point& a) {
    return packet << a.x << a.y;
}
sf::Packet& operator>>(sf::Packet& packet, Point& a) {
    return packet >> a.x >> a.y;
}

sf::Packet& operator<<(sf::Packet& packet, Rect& a) {
    return packet << a.PosX << a.PosY << a.Width << a.Height;
}
sf::Packet& operator>>(sf::Packet& packet, Rect& a) {
    return packet >> a.PosX >> a.PosY >> a.Width >> a.Height;
}

sf::Packet& operator<<(sf::Packet& packet, sf::Color& a) {
    return packet << a.r << a.g << a.b << a.a;
}
sf::Packet& operator>>(sf::Packet& packet, sf::Color& a) {
    return packet >> a.r >> a.g >> a.b >> a.a;
}
