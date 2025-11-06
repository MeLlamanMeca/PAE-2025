#pragma once
#include "../utils/Point.cpp"
#include <iostream>


class Task {
protected:
    Point posIni;
    Point posFin;

public:
    Task() noexcept = default;
    Task(const Point& inicio, const Point& fin) noexcept
        : posIni(inicio), posFin(fin) {}

    virtual ~Task() = default; // destructor virtual obligatorio en clases abstractas

    // Getters y setters
    [[nodiscard]] constexpr const Point& getPosIni() const noexcept { return posIni; }
    [[nodiscard]] constexpr const Point& getPosFin() const noexcept { return posFin; }

    constexpr void setPosIni(const Point& p) noexcept { posIni = p; }
    constexpr void setPosFin(const Point& p) noexcept { posFin = p; }

    // Método abstracto puro → hace la clase abstracta
    virtual bool isVIT() { return false; }
    virtual bool isPriority() { return false; }
    virtual bool isCommon() { return false; }

    // (Opcional) Representación textual
    friend std::ostream& operator<<(std::ostream& os, const Task& t) {
        os << "Tarea desde " << t.posIni << " hasta " << t.posFin;
        return os;
    }
};
