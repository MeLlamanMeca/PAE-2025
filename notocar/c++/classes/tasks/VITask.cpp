#pragma once
#include "Task.cpp"
#include <iostream>


class VITask : public Task {
private:

public:
    VITask() noexcept = default;

    VITask(const Point& inicio, const Point& fin) noexcept
        : Task(inicio, fin) {}

    bool isVIT() override {
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const VITask& t) {
        os << "VITask{" << t.posIni << " -> " << t.posFin << "}";
        return os;
    }
};

