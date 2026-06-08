#pragma once
#include <cstdint>
#include <cstdio>
#ifdef __linux__
#include <sys/stat.h>
#endif

namespace ShiftDownFunctions {

struct Function {
  float Tc{0.f}; //czas_całkowity
  float fs{0.f}; //czestotliwosc_probkowania
  float f{0.f}; //czestotliwosc
  float PHI{0.f}; //faza_poczatkowa
  float A{0.f}; //amplituda
  float Ts{0.f}; //krok_czasu
  uint64_t N{0}; // ilosc probek


  float *t = nullptr; //wartosci czasowe konkretne
  float *f_t = nullptr; //wartości funkcji

  typedef float (*FormulaPtr)(const Function&, uint64_t, float);
  FormulaPtr function_formula = nullptr;
  Function(float Tc, float fs, float f, float PHI, float A, FormulaPtr formula);
  ~Function();
};

struct SD_DFT {
  uint32_t K{0};

  float* Re = nullptr;
  float* Im = nullptr;

  float* fk = nullptr; // spectrum x
  float* mod_z = nullptr; // spectrum y

  SD_DFT(const Function* function);
  ~SD_DFT();
};

// --- MODULACJA SINUSOIDALNA (SIN)---
Function* modulate_AM_sin(const Function* ftm, float A, float fs, float PHI);
Function* modulate_PM_sin(const Function* ftm, float A, float fs, float PHI);
Function* modulate_FM_sin(const Function* ftm, float A, float fs, float PHI);

// --- MODULACJA PIŁOKSZTAŁTNA (SAW) ---
Function* modulate_AM_saw(const Function* ftm, float A, float fs, float PHI);
Function* modulate_PM_saw(const Function* ftm, float A, float fs, float PHI);
Function* modulate_FM_saw(const Function* ftm, float A, float fs, float PHI);

// --- MODULACJA PROSTOKĄTNA (REC) ---
Function* modulate_AM_rec(const Function* ftm, float A, float fs, float PHI);
Function* modulate_PM_rec(const Function* ftm, float A, float fs, float PHI);
Function* modulate_FM_rec(const Function* ftm, float A, float fs, float PHI);

// --- MODULACJA TRÓJKĄTNA (TRI) ---
Function* modulate_AM_tri(const Function* ftm, float A, float fs, float PHI);
Function* modulate_PM_tri(const Function* ftm, float A, float fs, float PHI);
Function* modulate_FM_tri(const Function* ftm, float A, float fs, float PHI);

class Graph {
private:
  //colors
  uint32_t bg; // background
  uint32_t lc; // line
  uint32_t ac; // axis
  uint32_t gc; //grid
  uint32_t fc; // font

  uint32_t* texture = nullptr;
public:
  Graph(const Function* function_to_render, char* name_label = nullptr, char* x_label = nullptr, char* y_label = nullptr, uint32_t background_color = 0xFF000000, uint32_t line_color = 0xFF00FF00, uint32_t axis_color = 0xFFFFFFFF, uint32_t grid_color = 0xFF7D7D7D, uint32_t font_color = 0xFFFFFFFF, const char* file_path = nullptr);
  Graph(const SD_DFT* dft_to_render, char* name_label = nullptr, char* x_label = nullptr, char* y_label = nullptr, uint32_t background_color = 0xFF000000, uint32_t line_color = 0xFF00FF00, uint32_t axis_color = 0xFFFFFFFF, uint32_t grid_color = 0xFF7D7D7D, uint32_t font_color = 0xFFFFFFFF);
  ~Graph();
  uint32_t* get_texture() {
    return texture;
  }
};

}