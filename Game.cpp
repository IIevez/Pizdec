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
            bool checker = true;
            int startlen = len;
            if (desk[x][y] == '.' && ((x == x1) || (y == y1) == 1) && x + len <= size && y + len <= size) {
                // delta_smth - для вычисления приращения
                int sum = 1;
                int delta_x = x1 - x;
                int delta_y = y1 - y;
                len--;
                if (abs(delta_x) == 1) {
                    while (len != 0) {
                        sum += (desk[x + len*delta_x][y] == '.');
                        len--;
                    }
                }
                if (abs(delta_y) == 1) {
                    while (len != 0) {
                        sum += (desk[x][y + len * delta_y] == '.');
                        len--;
                    }
                }
                if (sum != startlen) {
                    checker = false;
                }
            }
            else {
                checker = false;
            }
            return checker;
        }


    };
};
int main() {
    cout << "jopa";
    return 0;
}