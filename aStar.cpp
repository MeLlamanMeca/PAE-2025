#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include "aStar.h"
#include "point.hpp"

using namespace std;

struct Node {
    Point pos;
    double g;      // cost acumulat real (amb diagonals sqrt2)
    double h;      // heurística (calculada en la clase point)
    Node* parent;

    Node(Point p, double g_, double h_, Node* parent_ = nullptr)
        : pos(p), g(g_), h(h_), parent(parent_) {}
    // g -> cost real acumulat en cada camí analitzat
    // h -> cost estimat romanent fins al destí

     double f() const { return g + h; }
};

struct CompararNodos {
    bool operator()(const Node* a, const Node* b) const {
        return a->f() > b->f();
    }
};

/* ES CALCULA EN LA CLASSE POINT

// Estimacio del cost per arribar de la posicio actual a la final
// Pre: (x1, y1) i (x2, y2) son coordenades dins de la matriu
// Post: retorna la distancia euclidiana entre els dos punts

double heuristica(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); // Distancia euclidiana
}
*/

/*
* Ajustar el valor de zona lenta segons les necessitats del magatzem
* Pre: valor es un enter que representa el tipus de cel·la
* Post: retorna el cost associat a la cel·la
*/
double costeCelda(int valor) {
    if (valor == 252) return 2.0; // zona lenta
    else if (valor == 254) return 1.0; // zona normal
    else if (valor >= 0 && valor <= 250) return 1.0; // punt d’interès
    else return numeric_limits<double>::infinity(); // no transitable
}

bool esTransitable(int valor) {
    return (valor != 255 && valor != 253 && valor != 251);
}

/*
* Comprova si una cel·la està ocupada per un robot en un temps donat
* Pre: x, y son coordenades dins de la matriu, t es el temps actual, routes es el vector de rutes de robots, robots es el vector de posicions actuals de robots
* Post: retorna true si la cel·la està ocupada, false en cas contrari
*/
bool cellOccupied(
    int x, int y, double t, //t es el temps (cost g acumulat) en el que el robot A* arribaria a (x,y)
    const vector<vector<Point>>& routes, //rutes dels altres robots
    const vector<Point>& robots, //posicions actuals dels altres robots
    const vector<vector<int>>& map
) {
    // retorna true si la cel·la (x,y) conincideix amb el vector de posicions de robots (està ocupada per un robot)
    // check istantani: cost 0
    for (const Point& p : robots) {
        if (p.x == x && p.y == y)
            return true;
    }

    /* 
    *  Bucle que itera les rutes (sequencies de posicions)
    *  que els demes robots seguiran
    */
    for (const auto& route : routes) {
        double timeAccum = 0; // timeAccum -> temps d'arribada del robot a cada cel·la de la seva ruta 

        /*
        * Bucle que itera totes les posicions de cada ruta
        */
        for (size_t i = 0; i < route.size(); i++) {
            int rx = route[i].x;
            int ry = route[i].y;

            // Robot reaches this cell exactly at timeAccum
            if (rx == x && ry == y && fabs(timeAccum - t) < 0.0001)
                return true;

            // Si no es l'ultima posicio, acumulem el cost fins a la seguent posicio
            if (i + 1 < route.size()) {
                int nextX = route[i+1].x;
                int nextY = route[i+1].y;

                // El cost depen de la casella del mapa on el robon entra.
                int cellCode = map[nextX][nextY];

                double cost;
                if (cellCode == 252) cost = 2.0;     // zona lenta
                else cost = 1.0;                     // zona normal o punt d'interès
                timeAccum += cost;
            }
        }
    }

    return false;
}


/*
* Pre: matrix es una matriu 2D d'enters que representa el mapa del magatzem,
*      inici y fi son parelles de enters que representen las coordenadas de inici y fi
* Post: retorna un vector de parelles d'enteros que representen el camí trobat
*       o un vector buit si no s'ha trobat ningún camí
*/
pair<vector<Point>, int> aStar(
    const vector<vector<int>>& map,
    const vector<vector<Point>>& arrayRutes,
    const vector<Point>& arrayPosicionsRobots,
    Point posIni,
    Point posFi
){
    int files = map.size();
    int columnes = map[0].size();

    vector<vector<double>> gScore(files, vector<double>(columnes, numeric_limits<double>::infinity()));
    vector<vector<bool>> visitat(files, vector<bool>(columnes, false));

    priority_queue<Node*, vector<Node*>, CompararNodos> oberts;

    Node* nodeInici = new Node(
        posIni,
        0.0,
        posIni.distanceTo(posFi)
    );

    oberts.push(nodeInici);
    gScore[posIni.x][posIni.y] = 0.0;

    vector<Point> direccions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},   // direccions (veerticals i horitzontals)
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // diagonals
    };

    while (!oberts.empty()) {
        Node* actual = oberts.top();
        oberts.pop();

        int ax = actual->pos.x;
        int ay = actual->pos.y;

        if (visitat[ax][ay]) continue;
        visitat[ax][ay] = true;

        if (actual->pos == posFi) {
            // Reconstrucció del camí
            vector<Point> path;
            Node* n = actual;
            int costTotal = 0;

            while (n) {
                path.push_back(n->pos);
                int val = map[n->pos.x][n->pos.y];
                if (val == 252) costTotal += 2;
                else if (val == 254 || (val >= 0 && val <= 250)) costTotal += 1;
                n = n->parent;
            }
            
            //invertim el cami perque està de fi a inici
            reverse(path.begin(), path.end());
            return {path, costTotal};
        }

        for (const Point& d : direccions) {
            Point next = actual->pos + d;
            int nx = next.x;
            int ny = next.y;

            if (nx < 0 || ny < 0 || nx >= files || ny >= columnes) continue;
            if (!esTransitable(map[nx][ny])) continue;

            //avoid collisions with robots and robot routes
            if (cellOccupied(nx, ny, actual->g, arrayRutes, arrayPosicionsRobots, map))
                continue; //La clausula continue fa que es salti a la següent iteració del bucle [es l'equivalent de skip this neighbor and check the next one;]

            //Els moviments diagonals tenen un cost addicional de sqrt(2)
            bool diagonal = (d.x != 0 && d.y != 0);
            double factor = diagonal ? sqrt(2.0) : 1.0;

            double newG = actual->g + costeCelda(map[nx][ny]) * factor;

            
            if (newG < gScore[nx][ny]) {
                gScore[nx][ny] = newG;
                double h = next.distanceTo(posFi);
                oberts.push(new Node(next, newG, h, actual));
            }
        }
    }

    return {{}, -1}; // no path found
}