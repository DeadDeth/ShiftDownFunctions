#include "ShiftDownFunctions.hpp"
#include <cmath>

float my_sinus_formula(const ShiftDownFunctions::Function& func, uint64_t n, float current_t) {
    return func.A * sinf(2.f * (float)M_PI * func.f * current_t + func.PHI);
}

int main() {
    ShiftDownFunctions::Function funkcja(4, 8000, 1, 0, 10, my_sinus_formula);
    ShiftDownFunctions::Graph graf_funkcji(&funkcja, "wykres x(t)", "Czas [t]", "Amplituda [A]");

    return 0;
}