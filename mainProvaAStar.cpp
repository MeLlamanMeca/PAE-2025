#include <iostream>
#include "aStar.h"
#include "./assignTasksToRobots/point/point.hpp"

using namespace std;

int main() {
    // definim un mapa de prova
    vector<vector<int>> mapa = {
        {254,254,254,254,254,254},
        {254,253,253,253,254,254},
        {254,252,252,254,254,254},
        {254,254,254,254,255,254},
        {254,254,254,254,254,254}
    };

    //rutes dels robots
    vector<vector<Point>> arrayRutes = {
        { {0,5}, {1,5}, {2,5}, {3,5} },   // robot 1 route
        { {4,0}, {4,1}, {4,2} }           // robot 2 route
    };

    //posicions inicials dels robots
    vector<Point> arrayPosRobots = {
        {2,5},   // robot on route
        {4,1}    // robot blocking this cell
    };

    // punt de partida i posicio final
    Point ini(0,0);
    Point fi(4,5);

    auto result = aStar(mapa, arrayRutes, arrayPosRobots, ini, fi);

    auto path = result.first;
    int cost = result.second;

    //imprimir per pantalla el cami i el cost
    if (path.empty()) {
        cout << "No path found.\n";
        return 0;
    }

    cout << "Path found:\n";
    for (auto& p : path)
        cout << "(" << p.x << "," << p.y << ") ";
    cout << "\n";

    cout << "Total path cost = " << cost << "\n";

    return 0;
}
