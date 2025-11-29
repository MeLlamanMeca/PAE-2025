#pragma once
#include "../common/Point.h"


class Task {
    private:
        Point ini;
        Point fin;
        int taskID;

    public:
        inline Task(const Point& ini, const Point& fin, int taskID) : ini(ini), fin(fin), taskID(taskID) {}
        inline const Point& getIni() const { return ini; }
        inline const Point& getFin() const { return fin; }
        inline int getID() const { return taskID; }
};

    inline void to_json(json& j, const Task& p) {
        j = json{
            {"ini", p.getIni()},
            {"fin", p.getFin()},
            {"ID", p.getID()}
        };
    };