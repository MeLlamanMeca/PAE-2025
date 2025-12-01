#pragma once
#include "../common/Point.h"

class Task {
    private:
        Point ini;
        Point fin;
        int weight;
        int taskID;

    public:
        inline Task(const Point& ini, const Point& fin, int weight, int taskID) : ini(ini), fin(fin), weight(weight), taskID(taskID) {}
        inline const Point& getIni() const { return ini; }
        inline const Point& getFin() const { return fin; }
        inline int getWeight() const { return weight; }
        inline int getID() const { return taskID; }
};