#include <iostream>
#include "astar.h"

int main() {
    std::vector<std::vector<int>> mapa = {
        {254, 254, 254, 254, 254},
        {254, 253, 253, 253, 254},
        {254, 254, 252, 254, 254},
        {254, 255, 255, 254, 254},
        {254, 254, 254, 254, 254}
    };

    Point inici{0,0};
    Point fi{4,4};

    auto result  = aStar(mapa, inici, fi);
    vector<Point> path = result.first;

    if (!path.empty()) {
        std::cout << "Ruta trobada:\n";
        for (auto [x,y] : path)
            std::cout << "(" << x << "," << y << ") ";
        std::cout << std::endl;
    } else {
        std::cout << "No s'ha trobat cap ruta.\n";
    }

    return 0;
}
