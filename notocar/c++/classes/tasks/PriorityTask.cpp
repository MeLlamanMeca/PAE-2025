#pragma once
#include "Task.cpp"
#include <iostream>


class PriorityTask : public Task {
private:

public:
    PriorityTask() noexcept = default;

    PriorityTask(const Point& inicio, const Point& fin) noexcept
        : Task(inicio, fin) {}

    bool isPriority() override {
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const PriorityTask& t) {
        os << "CommonTask{" << t.posIni << " -> " << t.posFin << "}";
        return os;
    }
};

