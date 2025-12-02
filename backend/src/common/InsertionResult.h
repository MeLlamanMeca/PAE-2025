#pragma once

struct InsertionResult {
    int index;      // Mejor índice para insertar la tarea
    double cost;    // Coste asociado a esa inserción
    double distancia; // Distancia total tras la inserción
    double desgaste;   // Desgaste total tras la inserción

    InsertionResult(int idx, double c, double dist, double desg)
        : index(idx), cost(c), distancia(dist), desgaste(desg) {}
};