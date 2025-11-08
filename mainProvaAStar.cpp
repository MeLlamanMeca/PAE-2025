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

    std::pair<int,int> start = {0,0};
    std::pair<int,int> goal = {4,4};

    auto path = aStar(mapa, start, goal);

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
