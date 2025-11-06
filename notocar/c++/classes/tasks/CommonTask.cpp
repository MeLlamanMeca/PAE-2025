#pragma once
#include "Task.cpp"
#include <iostream>


class CommonTask : public Task {
private:
    int priority{0};

public:
    CommonTask() noexcept = default;

    CommonTask(const Point& inicio, const Point& fin, int priority_) noexcept
        : Task(inicio, fin), priority(priority_) {}

    [[nodiscard]] constexpr int getPriority() const noexcept { return priority; }
    constexpr void setPriority(int p) noexcept { priority = p; }

    bool isCommon() override {
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const CommonTask& t) {
        os << "CommonTask{" << t.posIni << " -> " << t.posFin
           << ", priority=" << t.priority << "}";
        return os;
    }
};
