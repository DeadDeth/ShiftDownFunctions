// #include "ShiftDownFunctions.hpp"
// #include <chrono>
// #include <iostream>
// #include <vector>
// #include <string>
//
// using namespace ShiftDownFunctions;
//
// // 1. Zwykły sinus
// float f_sinus(const Function& func, uint64_t n, float t) {
//     return func.A * sinf(2.f * (float)M_PI * func.f * t + func.PHI);
// }
// // 2. Sinus mocno przesunięty (DC Offset + Przesunięcie fazowe)
// float f_sinus_shifted(const Function& func, uint64_t n, float t) {
//     // Dodajemy +4.5 do amplitudy (przesunięcie w górę) i wymuszamy opóźnienie w czasie
//     return func.A * sinf(2.f * (float)M_PI * func.f * (t - 0.25f) + func.PHI) + 4.5f;
// }
//
// // 3. Tłumiony sinus (jak drgająca, uspokajająca się struna)
// float f_damped_sinus(const Function& func, uint64_t n, float t) {
//     // e^(-2t) sprawia, że amplituda maleje z czasem
//     return func.A * expf(-2.0f * t) * sinf(2.f * (float)M_PI * func.f * t + func.PHI);
// }
//
// // 4. Sygnał absolutny (wyprostowany dwupołówkowo)
// float f_abs_sinus(const Function& func, uint64_t n, float t) {
//     return fabsf(func.A * sinf(2.f * (float)M_PI * func.f * t + func.PHI));
// }
//
// int main() {
//     std::cout << "[INFO] Rozpoczynam ekstremalny Stress Test silnika graficznego..." << std::endl;
//     auto start_total = std::chrono::high_resolution_clock::now();
//
//     // =========================================================================
//     // ETAP 1: BAZOWE FUNKCJE O NIESTANDARDOWYch PARAMETRACH
//     // Tc = 1.733s, fs = 48000 Hz -> ponad 83 tysiące próbek na jeden wykres!
//     // =========================================================================
//     std::cout << "\n[1/4] Renderowanie bazowych, wrednych funkcji (fs=48000)..." << std::endl;
//
//     Function base_normal(1.733f, 48000.f, 3.5f, 0.f, 8.f, f_sinus);
//     Function base_shifted(1.733f, 48000.f, 3.5f, (float)M_PI / 4.f, 5.f, f_sinus_shifted);
//     Function base_damped(1.733f, 48000.f, 15.f, 0.f, 10.f, f_damped_sinus);
//     Function base_abs(1.733f, 48000.f, 4.f, 0.f, 6.f, f_abs_sinus);
//
//     Graph g1(&base_normal,  "01_Base_Normal.bmp",  "Czas [s]", "Amplituda [V]");
//     Graph g2(&base_shifted, "02_Base_Shifted.bmp", "Czas [s]", "Amplituda [V]");
//     Graph g3(&base_damped,  "03_Base_Damped.bmp",  "Czas [s]", "Amplituda [V]");
//     Graph g4(&base_abs,     "04_Base_Absolute.bmp","Czas [s]", "Amplituda [V]");
//
//     // =========================================================================
//     // ETAP 2: MASOWY TEST WSZYSTKICH MODULACJI DLA ZWYKŁEGO SYGNAŁU
//     // =========================================================================
//     std::cout << "[2/4] Aplikowanie 12 modulacji na sygnał podstawowy..." << std::endl;
//
//     // Parametry nośnej (carrier) dla modulacji
//     float mod_A = 2.0f;
//     float mod_fs = 45.0f; // Szybka nośna
//     float mod_PHI = 0.0f;
//
//     // AM
//     Function* am_sin = modulate_AM_sin(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* am_saw = modulate_AM_saw(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* am_rec = modulate_AM_rec(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* am_tri = modulate_AM_tri(&base_normal, mod_A, mod_fs, mod_PHI);
//     Graph g5(am_sin, "05_Mod_AM_Sin.bmp", "Czas [s]", "Amplituda");
//     Graph g6(am_saw, "06_Mod_AM_Saw.bmp", "Czas [s]", "Amplituda");
//     Graph g7(am_rec, "07_Mod_AM_Rec.bmp", "Czas [s]", "Amplituda");
//     Graph g8(am_tri, "08_Mod_AM_Tri.bmp", "Czas [s]", "Amplituda");
//     delete am_sin; delete am_saw; delete am_rec; delete am_tri;
//
//     // PM
//     Function* pm_sin = modulate_PM_sin(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* pm_saw = modulate_PM_saw(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* pm_rec = modulate_PM_rec(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* pm_tri = modulate_PM_tri(&base_normal, mod_A, mod_fs, mod_PHI);
//     Graph g9(pm_sin,  "09_Mod_PM_Sin.bmp", "Czas [s]", "Amplituda");
//     Graph g10(pm_saw, "10_Mod_PM_Saw.bmp", "Czas [s]", "Amplituda");
//     Graph g11(pm_rec, "11_Mod_PM_Rec.bmp", "Czas [s]", "Amplituda");
//     Graph g12(pm_tri, "12_Mod_PM_Tri.bmp", "Czas [s]", "Amplituda");
//     delete pm_sin; delete pm_saw; delete pm_rec; delete pm_tri;
//
//     // FM
//     Function* fm_sin = modulate_FM_sin(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* fm_saw = modulate_FM_saw(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* fm_rec = modulate_FM_rec(&base_normal, mod_A, mod_fs, mod_PHI);
//     Function* fm_tri = modulate_FM_tri(&base_normal, mod_A, mod_fs, mod_PHI);
//     Graph g13(fm_sin, "13_Mod_FM_Sin.bmp", "Czas [s]", "Amplituda");
//     Graph g14(fm_saw, "14_Mod_FM_Saw.bmp", "Czas [s]", "Amplituda");
//     Graph g15(fm_rec, "15_Mod_FM_Rec.bmp", "Czas [s]", "Amplituda");
//     Graph g16(fm_tri, "16_Mod_FM_Tri.bmp", "Czas [s]", "Amplituda");
//     delete fm_sin; delete fm_saw; delete fm_rec; delete fm_tri;
//
//     // =========================================================================
//     // ETAP 3: EKSTREMALNY TEST NA ZANIKAJĄCYM SYGNALE (Damped) I OVERKILL Hz
//     // =========================================================================
//     std::cout << "[3/4] Ekstremalne przeciążenie - tłumiony sygnał, fs=96000..." << std::endl;
//     Function extreme_damped(2.111f, 96000.f, 20.f, 0.f, 15.f, f_damped_sinus);
//
//     Function* ext_am = modulate_AM_tri(&extreme_damped, 5.0f, 120.0f, 0.f);
//     Function* ext_fm = modulate_FM_saw(&extreme_damped, 5.0f, 120.0f, 0.f);
//     Function* ext_pm = modulate_PM_sin(&extreme_damped, 5.0f, 120.0f, 0.f);
//
//     Graph g17(&extreme_damped, "17_Extreme_Base.bmp", "Czas [s]", "A");
//     Graph g18(ext_am, "18_Extreme_AM_Tri.bmp", "Czas [s]", "A");
//     Graph g19(ext_fm, "19_Extreme_FM_Saw.bmp", "Czas [s]", "A");
//     Graph g20(ext_pm, "20_Extreme_PM_Sin.bmp", "Czas [s]", "A");
//
//     delete ext_am; delete ext_fm; delete ext_pm;
//
//     // =========================================================================
//     // ETAP 4: WARIACJE KOLORYSTYCZNE
//     // =========================================================================
//     std::cout << "[4/4] Test palety kolorów dla funkcji przesuniętej..." << std::endl;
//     // format to ARGB hex, np. 0xFFRRGGBB
//     Graph g21(&base_shifted, "21_Color_Neon.bmp", "t", "A", 0xFF000000, 0xFFFF00FF, 0xFFFFFFFF, 0xFF333333, 0xFF00FFFF);
//     Graph g22(&base_shifted, "22_Color_Hacker.bmp", "t", "A", 0xFF050505, 0xFF00FF00, 0xFF00AA00, 0xFF003300, 0xFF00FF00);
//     Graph g23(&base_shifted, "23_Color_Blood.bmp", "t", "A", 0xFF110000, 0xFFFF0000, 0xFFFF5555, 0xFF550000, 0xFFFFAAAA);
//     Graph g24(&base_shifted, "24_Color_Gold.bmp", "t", "A", 0xFF1A1A00, 0xFFFFD700, 0xFFFFAA00, 0xFF665500, 0xFFFFFF00);
//
//     auto end_total = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> diff_total = end_total - start_total;
//
//     std::cout << "\n[SUKCES] Wygenerowano 24 wykresy w 8K!" << std::endl;
//     std::cout << "[CZAS] Calkowity czas operacji (obliczenia + I/O dysku + konwersja): " << diff_total.count() << " s\n";
//     std::cout << "Mozesz teraz podziwiac swoje dzielo w folderze. :)" << std::endl;
//
//     return 0;
// }

#include "ShiftDownFunctions.hpp"

    // albo używamy:
    // using namespace ShiftDownFunctions;
    // albo trzeba pamiętać o pisaniu ShiftDownFunctions::, przed wszystkim co pochodzi z tego kontenera


//KROK 1 - tworzenie wzoru funkcji
// float sinus_formula (const ShiftDownFunctions::Function& function_object, uint64_t n, float t) {
//     return function_object.A * sinf(2.f * M_PIf * function_object.f * t + function_object.PHI); //M_PIf to jest wartość liczby PI jako float
// }




float cosinus(const ShiftDownFunctions::Function &funkcja, uint64_t n, float t) {
    return cosf(2*M_PIf * funkcja.f * t + funkcja.PHI) * cosf(2.5f * powf(t, 0.2f) * M_PIf);
}

void second_main() {

    ShiftDownFunctions::Function funkcja_cosinus(2.f, 8000.f, 4.f, 0.f, 0.f, cosinus);
    ShiftDownFunctions::Graph graf_sin(&funkcja_cosinus, "cosinus", "Czas [s]", "Amplituda [A]");

    ShiftDownFunctions::DFT cos_dft(&funkcja_cosinus);
    ShiftDownFunctions::Graph graf_dft(&cos_dft, "Widmo cosinus", "Czestotliwosc [Hz]", "Amplituda [A]");
}


int main() {
    second_main();
    //KROK 2 - tworzenie obiektow funkcji
    //ShiftDownFunctions::Function funkcja_sinus(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);
    // ShiftDownFunctions::Function funkcja_sinus_saw(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);
    // ShiftDownFunctions::Function funkcja_sinus_rec(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);
    // ShiftDownFunctions::Function funkcja_sinus_tri(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);

    //KROK 3 - modulacja funkcji
    // ShiftDownFunctions::Function* sin_modulacja_AM_sin = modulate_AM_sin(&funkcja_sinus, 2.0f, 40.0f, 0.f);
    // ShiftDownFunctions::Function* sin_modulacja_PM_saw = modulate_PM_saw(&funkcja_sinus, 1.0f, 30.0f, 0.f);
    // ShiftDownFunctions::Function* sin_modulacja_FM_rec = modulate_FM_rec(&funkcja_sinus, 1.0f, 50.0f, 0.f);
    // ShiftDownFunctions::Function* sin_modulacja_FM_tri = modulate_FM_tri(&funkcja_sinus, 1.0f, 50.0f, 0.f);

    //KROK 3 - rysowanie funkcji
    //ShiftDownFunctions::Graph graf_sinus(&funkcja_sinus, "funkcja sinus", "Czas [s]", "Amplituda [A]");
    // ShiftDownFunctions::Graph graf_sin_modulacja_AM_sin(sin_modulacja_AM_sin, "Modulacja sin, AM", "Czas [s]", "Amplituda [A]");
    // ShiftDownFunctions::Graph graf_sin_modulacja_PM_saw(sin_modulacja_PM_saw, "Modulacja saw, PM", "Czas [s]", "Amplituda [A]");
    // ShiftDownFunctions::Graph graf_sin_modulacja_FM_rec(sin_modulacja_FM_rec, "Modulacja rec, FM", "Czas [s]", "Amplituda [A]");
    // ShiftDownFunctions::Graph graf_sin_modulacja_FM_tri(sin_modulacja_FM_tri, "Modulacja tri, FM", "Czas [s]", "Amplituda [A]");


    //rysowanie w innych kolorach tej samej funkcji
    // ShiftDownFunctions::Graph graf_sinus_red_theme(&funkcja_sinus, "funkcja sinus RED", "Czas [s]", "Amplituda [A]", Colors::BLOOD_RED, Colors::RED, Colors::PASTEL_RED, Colors::GLASS_RED);
    // ShiftDownFunctions::Graph graf_sinus_blue_theme(&funkcja_sinus, "funkcja sinus BLUE", "Czas [s]", "Amplituda [A]", Colors::MIDNIGHT_BLUE, Colors::BLUE, Colors::PASTEL_BLUE, Colors::GLASS_BLUE);
    // ShiftDownFunctions::Graph graf_sinus_purple_theme(&funkcja_sinus, "funkcja sinus PURPLE", "Czas [s]", "Amplituda [A]", Colors::DARK_PURPLE, Colors::PINK, Colors::PASTEL_PURPLE, Colors::PURPLE);

    //KROK 4 - usuwanie funkcji
    // delete sin_modulacja_FM_tri;
    // delete sin_modulacja_FM_rec;
    // delete sin_modulacja_PM_saw;
    // delete sin_modulacja_AM_sin;



    return 0;
}