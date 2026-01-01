 #include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <Windows.h>

using namespace std;

// ===============================================================
// Player — добавлены деньги
// ===============================================================
class Player {
    int money = 0;

public:
    void addMoney(int value) { money += value; }
    bool canAfford(int price) const { return money >= price; }
    void spendMoney(int price) { money -= price; }
    int getMoney() const { return money; }
};

// ===============================================================
// Ship
// ===============================================================
class Ship {
public:
    vector<pair<int, int>> cells;
    vector<bool> hits;

    Ship(const vector<pair<int, int>>& cells)
        : cells(cells), hits(cells.size(), false) {
    }

    bool isSunk() const {
        for (bool h : hits)
            if (!h) return false;
        return true;
    }

    bool registerHit(int x, int y) {
        for (int i = 0; i < cells.size(); i++) {
            if (cells[i].first == x && cells[i].second == y) {
                hits[i] = true;
                return true;
            }
        }
        return false;
    }
};

// ===============================================================
// Board — привязан к Player (деньги)
// ===============================================================
class Board {
    int size;
    vector<vector<char>> grid;
    vector<Ship> ships;
    mt19937 rng;
    Player& owner;

public:
    Board(int n, Player& p)
        : size(n), grid(n, vector<char>(n, '.')),
        rng(time(nullptr)), owner(p) {
    }

    int getSize() const { return size; }
    const vector<vector<char>>& getGrid() const { return grid; }

    int shipPrice(int len) const {
        switch (len) {
        case 2: return 60;
        case 3: return 90;
        case 4: return 150;
        default: return 0;
        }
    }

    bool canPlaceShip(int x, int y, int len, bool horizontal) {
        int price = shipPrice(len);
        if (!owner.canAfford(price)) return false;

        if (horizontal) {
            if (y + len > size) return false;
            for (int j = y; j < y + len; j++)
                if (grid[x][j] != '.') return false;
        }
        else {
            if (x + len > size) return false;
            for (int i = x; i < x + len; i++)
                if (grid[i][y] != '.') return false;
        }
        return true;
    }

    void placeShip(int x, int y, int len, bool horizontal) {
        vector<pair<int, int>> cells;

        if (horizontal) {
            for (int j = y; j < y + len; j++) {
                grid[x][j] = 'S';
                cells.push_back({ x, j });
            }
        }
        else {
            for (int i = x; i < x + len; i++) {
                grid[i][y] = 'S';
                cells.push_back({ i, y });
            }
        }

        owner.spendMoney(shipPrice(len));
        ships.push_back(Ship(cells));
    }

    void autoPlaceShips(int count) {
        uniform_int_distribution<int> dir(0, 1);
        uniform_int_distribution<int> dist(0, size - 1);

        for (int k = 0; k < count; k++) {
            bool placed = false;
            int len = 2 + (k % 3);

            while (!placed) {
                int x = dist(rng);
                int y = dist(rng);
                bool horizontal = dir(rng);

                if (canPlaceShip(x, y, len, horizontal)) {
                    placeShip(x, y, len, horizontal);
                    placed = true;
                }
                else {
                    break; // денег не хватает
                }
            }
        }
    }

    bool shoot(int x, int y) {
        if (grid[x][y] == 'S') {
            grid[x][y] = 'X';
            for (auto& s : ships)
                s.registerHit(x, y);
            return true;
        }
        if (grid[x][y] == '.')
            grid[x][y] = 'o';
        return false;
    }


    bool allSunk() const {
        for (const auto& s : ships)
            if (!s.isSunk()) return false;
        return true;
    }
};

// ===============================================================
// Player wrapper
// ===============================================================
class GamePlayer {
public:
    Player wallet;
    Board board;

    GamePlayer(int size, int startMoney)
        : wallet(), board(size, wallet)
    {
        wallet.addMoney(startMoney);
    }
};

// ===============================================================
// Game
// ===============================================================
class Game {
    GamePlayer p1;
    GamePlayer p2;

public:
    Game(int size, int ships, int money)
        : p1(size, money), p2(size, money)
    {
        p1.board.autoPlaceShips(ships);
        p2.board.autoPlaceShips(ships);
    }

    void printBoard(const Board& b, bool hideShips) {
        int n = b.getSize();
        cout << "   ";
        for (int j = 0; j < n; j++) cout << j + 1 << " ";
        cout << "\n";

        for (int i = 0; i < n; i++) {
            cout << i + 1 << "  ";
            for (int j = 0; j < n; j++) {
                char c = b.getGrid()[i][j];
                if (hideShips && c == 'S') cout << ". ";
                else cout << c << " ";
            }
            cout << "\n";
        }
    }

    void run() {
        while (true) {

            cout << "\nДеньги игрока 1: " << p1.wallet.getMoney() << "\n";
            //printBoard(p2.board, true);

            cout << "\n=== ВАШЕ ПОЛЕ (Игрок 1) ===\n";
            printBoard(p1.board, false);   // ← ВОТ ЭТА СТРОКА

            cout << "\n=== ПОЛЕ ПРОТИВНИКА ===\n";
            printBoard(p2.board, true);

            cout << "Ход игрока 1 (x y): ";
            int x, y;
            cin >> x >> y;
            p1.board.shoot(x - 1, y - 1);

            if (p2.board.allSunk()) {
                cout << "Игрок 1 победил\n";
                break;
            }

            cout << "\nДеньги игрока 2: " << p2.wallet.getMoney() << "\n";
            printBoard(p1.board, true);

            cout << "\n=== ВАШЕ ПОЛЕ (Игрок 2) ===\n";
            printBoard(p2.board, false);   // игрок 2 видит СВОИ корабли

            cout << "\n=== ПОЛЕ ПРОТИВНИКА ===\n";
            printBoard(p1.board, true);    // поле игрока 1 скрыто
            
            cout << "Ход игрока 2 (x y): ";
            cin >> x >> y;
            p2.board.shoot(x - 1, y - 1);

            if (p1.board.allSunk()) {
                cout << "Игрок 2 победил\n";
                break;
            }
        }
    }
};

// ===============================================================
// MAIN
// ===============================================================
int main() {
    SetConsoleOutputCP(1251);
    int size, ships, money;
    cout << "Размер поля: ";
    cin >> size;
    cout << "Количество кораблей: ";
    cin >> ships;
    cout << "Стартовые деньги: ";
    cin >> money;

    Game g(size, ships, money);
    g.run();
    return 0;
}
