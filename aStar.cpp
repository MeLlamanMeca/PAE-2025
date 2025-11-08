#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include "aStar.h"

using namespace std;

struct Node {
    int x, y;
    double g, h;
    Node* parent;

    Node(int x_, int y_, double g_, double h_, Node* p = nullptr)
        : x(x_), y(y_), g(g_), h(h_), parent(p) {}
    // g -> cost real acumulat en cada camí analitzat
    // h -> cost estimat romanent fins al destí

    double f() const { return g + h; }
};

struct CompararNodos {
    bool operator()(const Node* a, const Node* b) const {
        return a->f() > b->f();
    }
};

/*
* Estimacio del cost per arribar de la posicio actual a la final
* Pre: (x1, y1) i (x2, y2) son coordenades dins de la matriu
* Post: retorna la distancia euclidiana entre els dos punts
*/
double heuristica(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); // Distancia euclidiana
}

/*
* Ajustar el valor de zona lenta segons les necessitats del magatzem
* Pre: valor es un enter que representa el tipus de cel·la
* Post: retorna el cost associat a la cel·la
*/
double costeCelda(int valor) {
    if (valor == 252) return 2.0; // zona lenta
    else if (valor == 254) return 1.0; // normal
    else if (valor >= 0 && valor <= 250) return 1.0; // punt d'interés
    else return numeric_limits<double>::infinity(); // no transitable
}

bool esTransitable(int valor) {
    return (valor != 255 && valor != 253 && valor != 251);
}

/*
* Pre: matrix es una matriu 2D d'enters que representa el mapa del magatzem,
*      inici y fi son parelles de enters que representen las coordenadas de inici y fi
* Post: retorna un vector de parelles d'enteros que representen el camí trobat
*       o un vector buit si no s'ha trobat ningún camí
*/
vector<pair<int, int>> aStar(const vector<vector<int>>& matrix,
                                 pair<int, int> inici,
                                 pair<int, int> fi) {
    int files = matrix.size();
    int columnes = matrix[0].size();

    vector<vector<double>> gScore(files, vector<double>(columnes, numeric_limits<double>::infinity()));
    vector<vector<bool>> visitat(files, vector<bool>(columnes, false));

    priority_queue<Node*, vector<Node*>, CompararNodos> abiertos;

    Node* nodeInici = new Node(inici.first, inici.second, 0.0,
                                heuristica(inici.first, inici.second, fi.first, fi.second));
    abiertos.push(nodeInici);
    gScore[inici.first][inici.second] = 0.0;

    vector<pair<int, int>> direccions = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1},   // verticales y horizontales
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // diagonales
    };

    while (!abiertos.empty()) {
        Node* actual = abiertos.top();
        abiertos.pop();

        if (visitat[actual->x][actual->y]) continue;
        visitat[actual->x][actual->y] = true;

        if (actual->x == fi.first && actual->y == fi.second) {
            // Reconstrucciò del camí
            vector<pair<int, int>> path;
            Node* node = actual;
            while (node) {
                path.push_back({node->x, node->y});
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (auto [dx, dy] : direccions) {
            int nx = actual->x + dx;
            int ny = actual->y + dy;

            if (nx < 0 || ny < 0 || nx >= files || ny >= columnes) continue;
            if (!esTransitable(matrix[nx][ny])) continue;

            //Els moviments diagonals tenen un cost addicional de sqrt(2)
            double factorDistancia = (dx != 0 && dy != 0) ? sqrt(2.0) : 1.0;
            double newG = actual->g + costeCelda(matrix[nx][ny]) * factorDistancia;
            if (newG < gScore[nx][ny]) {
                gScore[nx][ny] = newG;
                double h = heuristica(nx, ny, fi.first, fi.second);
                abiertos.push(new Node(nx, ny, newG, h, actual));
            }
        }
    }

    return {}; // No path found
}

/*
// Funció main per provar l'algorisme A*
// Eliminar aquesta funció main quan s'integri amb altres components
int main() {
    vector<vector<int>> matriz = {
        {254, 254, 254, 254, 254},
        {254, 253, 253, 253, 254},
        {254, 254, 252, 254, 254},
        {254, 255, 255, 254, 254},
        {254, 254, 254, 254, 254}
    };

    pair<int, int> inicio = {0, 0};
    pair<int, int> fin = {4, 4};

    vector<pair<int, int>> camino = aStar(matriz, inicio, fin);


    // IMPRIMIR EL RESULTAT PER PANTALLA PER VERIFICAR EL FUNCIONAMENT
    if (!camino.empty()) {
        cout << "Path found:\n";
        for (auto [x, y] : camino)
            cout << "(" << x << "," << y << ") ";
        cout << endl;
    } else {
        cout << "No path found.\n";
    }
    // ==============================

    return 0;
}

*/