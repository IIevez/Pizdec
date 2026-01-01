#include <vector>
#include <cstdlib>
#include<iostream>
#include <random>
#include <ctime>
// для начала так сделал, потом отключу
using namespace std;
struct Board {
     //
    class Ship {
    public:
        vector<pair<int,int>> cells;        // Вектор клеток корабля (каждая — координата)
        vector<bool> hits;                  // Те же размеры, но true/false — попадание

        // Конструктор. Принимает вектор клеток.
        Ship(const vector<pair<int,int>>& cells): cells(cells), hits(cells.size(), false) {}

        // Проверяет, потоплен ли корабль (все клетки подбиты)
        bool isSunk() const {
            for (bool h : hits)             // Проходим по каждому элементу hits
                if (!h) return false;       // Если есть хоть одно неповреждённое — корабль жив
            return true;                    // Все подбиты — корабль утонул
        }

        // Отмечает попадание, если координата попала по кораблю
        bool registerHit(int x, int y) {
            for (int i = 0; i < cells.size(); i++) {   // Перебираем клетки корабля
                if (cells[i].first == x && cells[i].second == y) {
                    hits[i] = true;        // Отмечаем попадание
                    return true;
                }
            }
            return false;                  // Нет попадания
        }
    };
    //
    // главное отличие структуры от класса - в классе все эелементы базово приватные, в структуре же - публичные.
    class DESK {
    private:
        // первая строка и столбец - числа(номера полей, чтоб их наглядно видел пользователь)
        const int size = 13;
        vector <vector<char>> desk;
        vector <Board::Ship> ships;
    public:
        // можно использовать самоссылку, написав Board::desk():..., но внутри классов структуры и ее методов это использовать излишне
        //конструктор 
        DESK() : desk(size, vector<char>(size, '.')) {}
        //интересный способ высунуть что-то из приватного типа)
        int getsize() const { return size; }
        const vector<vector<char>>& getDesk() const { return desk; }
        vector<Board::Ship>& getShips() { return ships; }
        //Добавить проверку, что x1y1 лежит рядом с xy
        // xy - начальная позиция, x1y1 - направление корабля, len - его длинна

        bool CanPlaceShip(int x, int y, int x1, int y1, int len) {
            if (desk[x][y] == '.') {
                int delta_x = x1 - x;
                int delta_y = y1 - y;
                len--;
                if (x + len > size || y + len > size) {
                    return false;
                }
                while (len != 0) {
                    if (desk[x + delta_x + len][y + delta_y + len] != '.') {}
                }
            }
            else {
                return false;
            }
        }

    };
};
int main() {
    cout << "jopa";
    return 0;
}