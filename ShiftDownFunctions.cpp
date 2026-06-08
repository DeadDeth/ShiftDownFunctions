#include "ShiftDownFunctions.hpp"
#include "ShiftDownObjects/2D/Object/Object.hpp"
#include "ShiftDownText/TextBox/TextBox.hpp"
#include "ShiftDownDisplay/Colors/Colors.hpp"

#include <immintrin.h>
#include <cmath>

//dodac funckje dla haminga i inne bajery z tym zwiazane


using namespace ShiftDownFunctions;

Function::Function(float Tc, float fs, float f, float PHI, float A, FormulaPtr formula) : Tc(Tc), fs(fs), f(f), PHI(PHI), A(A), Ts(1.f/fs), N(static_cast<uint64_t>(Tc * fs)) {
    function_formula = formula;
    t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));
    f_t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));

    for (uint64_t n = 0; n < N; n++) t[n] = static_cast<float>(n) * Ts;
    for (uint64_t n = 0; n < N; n++) f_t[n] = formula(*this, n, t[n]);
}
Function::~Function() {
    _mm_free(t);
    _mm_free(f_t);
}

SD_DFT::SD_DFT(const Function* function) {
  K = function->N-1;

  Re = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));
  Im = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));

  mod_z = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));
  fk = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));

  //liczenie dft
  for (uint64_t k = 0; k < K; k++) {
    Re[k] = 0;
    Im[k] = 0;
    for (uint64_t n = 0; n < K; n++) {
      Re[k] += function->f_t[n] * cosf((-2.f * M_PIf * static_cast<float>(n) * static_cast<float>(k)) / static_cast<float>(K));
      Im[k] += function->f_t[n] * sinf((-2.f * M_PIf * static_cast<float>(n) * static_cast<float>(k)) / static_cast<float>(K));
    }
  }

  //liczenie |z|
  for (uint64_t k = 0; k < K; k++) {
    mod_z[k] = sqrtf(powf(Re[k], 2) + powf(Im[k], 2));
    fk[k] = static_cast<float>(k) * function->fs / static_cast<float>(K);
  }

  //normalizacja
  mod_z[0] /= static_cast<float>(function->N);
  for (uint64_t k = 1; k < K; k++) {
    mod_z[k] /= static_cast<float>(K) / 2.f;
  }
}
SD_DFT::~SD_DFT() {
  _mm_free(Im);
  _mm_free(Re);

  _mm_free(mod_z);
  _mm_free(fk);
}


//sinusowe
Function* ShiftDownFunctions::modulate_AM_sin(const Function* ftm, float A, float fs, float PHI) {

  auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);

  AM->A = A;
  AM->fs = fs;
  AM->PHI = PHI;

  for (uint64_t t = 0; t < AM->N; t++) {
    AM->f_t[t] = (A + ftm->f_t[t]) * sinf(2.f * M_PIf * fs * AM->t[t] + PHI);
  }

  return AM;
}
Function* ShiftDownFunctions::modulate_PM_sin(const Function* ftm, float A, float fs, float PHI) {

  auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  PM->A = A;
  PM->fs = fs;
  PM->PHI = PHI;

  for (uint64_t t = 0; t < PM->N; t++) {
    PM->f_t[t] = A * sinf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI);
  }

  return PM;
}
Function* ShiftDownFunctions::modulate_FM_sin(const Function* ftm, float A, float fs, float PHI) {
  auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  FM->A = A;
  FM->fs = fs;
  FM->PHI = PHI;

  float integral = 0.f;
  for (uint64_t t = 0; t < FM->N; t++) {
    integral += ftm->f_t[t] * ftm->Ts;
    FM->f_t[t] = A * sinf(2.f * M_PIf * fs * FM->t[t] + 2.f * M_PIf * integral + PHI);
  }
  return FM;
}

//piłkokształtne
Function* ShiftDownFunctions::modulate_AM_saw(const Function* ftm, float A, float fs, float PHI) {
  auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  AM->A = A; AM->fs = fs; AM->PHI = PHI;
  for (uint64_t t = 0; t < AM->N; t++) {
    float angle = fmodf(2.f * M_PIf * fs * AM->t[t] + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle / M_PIf) - 1.f;
    AM->f_t[t] = (A + ftm->f_t[t]) * carrier;
  }
  return AM;
}
Function* ShiftDownFunctions::modulate_PM_saw(const Function* ftm, float A, float fs, float PHI) {
  auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  PM->A = A; PM->fs = fs; PM->PHI = PHI;
  for (uint64_t t = 0; t < PM->N; t++) {
    float angle = fmodf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle / M_PIf) - 1.f;
    PM->f_t[t] = A * carrier;
  }
  return PM;
}
Function* ShiftDownFunctions::modulate_FM_saw(const Function* ftm, float A, float fs, float PHI) {
  auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  FM->A = A; FM->fs = fs; FM->PHI = PHI;
  float integral = 0.f;
  for (uint64_t t = 0; t < FM->N; t++) {
    integral += ftm->f_t[t] * ftm->Ts;
    float angle = fmodf(2.f * M_PIf * fs * FM->t[t] + 2.f * M_PIf * integral + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle / M_PIf) - 1.f;
    FM->f_t[t] = A * carrier;
  }
  return FM;
}

//kwadratowe
Function* ShiftDownFunctions::modulate_AM_rec(const Function* ftm, float A, float fs, float PHI) {
  auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  AM->A = A; AM->fs = fs; AM->PHI = PHI;
  for (uint64_t t = 0; t < AM->N; t++) {
    float angle = fmodf(2.f * M_PIf * fs * AM->t[t] + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle < M_PIf) ? 1.f : -1.f;
    AM->f_t[t] = (A + ftm->f_t[t]) * carrier;
  }
  return AM;
}
Function* ShiftDownFunctions::modulate_PM_rec(const Function* ftm, float A, float fs, float PHI) {
  auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  PM->A = A; PM->fs = fs; PM->PHI = PHI;
  for (uint64_t t = 0; t < PM->N; t++) {
    float angle = fmodf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle < M_PIf) ? 1.f : -1.f;
    PM->f_t[t] = A * carrier;
  }
  return PM;
}
Function* ShiftDownFunctions::modulate_FM_rec(const Function* ftm, float A, float fs, float PHI) {
  auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  FM->A = A; FM->fs = fs; FM->PHI = PHI;
  float integral = 0.f;
  for (uint64_t t = 0; t < FM->N; t++) {
    integral += ftm->f_t[t] * ftm->Ts;
    float angle = fmodf(2.f * M_PIf * fs * FM->t[t] + 2.f * M_PIf * integral + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle < M_PIf) ? 1.f : -1.f;
    FM->f_t[t] = A * carrier;
  }
  return FM;
}

//trojkatne
Function* ShiftDownFunctions::modulate_AM_tri(const Function* ftm, float A, float fs, float PHI) {
  auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  AM->A = A; AM->fs = fs; AM->PHI = PHI;
  for (uint64_t t = 0; t < AM->N; t++) {
    float angle = fmodf(2.f * M_PIf * fs * AM->t[t] + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle < M_PIf) ? (-1.f + 2.f * angle / M_PIf) : (3.f - 2.f * angle / M_PIf);
    AM->f_t[t] = (A + ftm->f_t[t]) * carrier;
  }
  return AM;
}
Function* ShiftDownFunctions::modulate_PM_tri(const Function* ftm, float A, float fs, float PHI) {
  auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  PM->A = A; PM->fs = fs; PM->PHI = PHI;
  for (uint64_t t = 0; t < PM->N; t++) {
    float angle = fmodf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle < M_PIf) ? (-1.f + 2.f * angle / M_PIf) : (3.f - 2.f * angle / M_PIf);
    PM->f_t[t] = A * carrier;
  }
  return PM;
}
Function* ShiftDownFunctions::modulate_FM_tri(const Function* ftm, float A, float fs, float PHI) {
  auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  FM->A = A; FM->fs = fs; FM->PHI = PHI;
  float integral = 0.f;
  for (uint64_t t = 0; t < FM->N; t++) {
    integral += ftm->f_t[t] * ftm->Ts;
    float angle = fmodf(2.f * M_PIf * fs * FM->t[t] + 2.f * M_PIf * integral + PHI, 2.f * M_PIf);
    if (angle < 0.f) angle += 2.f * M_PIf;
    float carrier = (angle < M_PIf) ? (-1.f + 2.f * angle / M_PIf) : (3.f - 2.f * angle / M_PIf);
    FM->f_t[t] = A * carrier;
  }
  return FM;
}


void draw_line(uint32_t* texture, uint32_t texture_width, uint32_t texture_height, uint32_t Ax, uint32_t Ay, uint32_t Bx, uint32_t By, uint32_t color, uint32_t thickness) {

  int x0 = static_cast<int>(Ax);
  int y0 = static_cast<int>(Ay);
  int x1 = static_cast<int>(Bx);
  int y1 = static_cast<int>(By);

  int width = static_cast<int>(texture_width);
  int height = static_cast<int>(texture_height);

  int dx = std::abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -std::abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  int e2;

  int half_thick = static_cast<int>(thickness) / 2;

  while (true) {
    for(int ty = -half_thick; ty <= half_thick; ty++) {
      for(int tx = -half_thick; tx <= half_thick; tx++) {
        int draw_x = x0 + tx;
        int draw_y = y0 + ty;
        if(draw_x >= 0 && draw_x < width && draw_y >= 0 && draw_y < height) {
          texture[draw_y * width + draw_x] = color;
        }
      }
    }
    if (x0 == x1 && y0 == y1) break;

    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}

Graph::Graph(const Function* ftr, char* name_label, char* x_label, char* y_label, uint32_t background_color, uint32_t line_color, uint32_t axis_color, uint32_t grid_color,uint32_t font_color, const char* file_path) : bg(background_color), lc(line_color), ac(axis_color), gc(grid_color), fc(font_color) {
  uint32_t picture_width = 7680;
  uint32_t picture_height = 4320;
  uint64_t picture_size = picture_width * picture_height;

  texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size,32));
  for (uint64_t i = 0; i < picture_size; i++) {
    texture[i] = background_color;
  }

  float min_y = std::numeric_limits<float>::max();
  float max_y = std::numeric_limits<float>::lowest();

  for (uint32_t i = 0; i < ftr->N; i++) {
    min_y = ftr->f_t[i] < min_y ? ftr->f_t[i] : min_y;
    max_y = ftr->f_t[i] > max_y ? ftr->f_t[i] : max_y;
  }

  uint32_t padding_left_x = 736;
  uint32_t padding_right_x = 384;

  uint32_t padding_top_y = 256;
  uint32_t padding_bot_y = 512;

  uint32_t graph_width = picture_width - padding_left_x - padding_right_x; // 7168px
  uint32_t graph_height = picture_height - padding_top_y - padding_bot_y;  // 3552px

  float scale_x = static_cast<float>(graph_width) / (ftr->t[ftr->N - 1] - ftr->t[0]);
  float scale_y = 0;
  if (max_y == min_y) {
    scale_y = static_cast<float>(graph_height) / 1.f;
  }
  else {
    scale_y = static_cast<float>(graph_height) / (max_y - min_y);
  }

  int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int)*ftr->N, 32));
  int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int)*ftr->N, 32));

  for (uint32_t i = 0; i < ftr->N; i++) {
    scaled_x[i] = static_cast<int>(roundf(ftr->t[i] * scale_x));
    scaled_y[i] = static_cast<int>(roundf(ftr->f_t[i] * scale_y));
  }

  int int_min_y = std::numeric_limits<int>::max();
  int int_max_y = std::numeric_limits<int>::lowest();

  for (uint32_t i = 0; i < ftr->N; i++) {
    int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
    int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
  }

  int offset_x = scaled_x[0] * -1;
  int offset_y = int_min_y * -1;

  auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t)*ftr->N, 32));
  auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t)*ftr->N, 32));

  for (uint32_t i = 0; i < ftr->N; i++) {
    scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
    scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
  }


  //name label
  uint32_t end = 0;
  while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
    end++;
  }
  ShiftDownText::TextBox name(0,0,0,0, end, 1, background_color, 0);
  name.add_text(name_label, font_color);

  uint32_t skala_textu_name = 16;
  uint32_t center = name.return_texture()->width * skala_textu_name / 2;

  uint32_t pozycja_y = 0;
  for (uint32_t y = 0; y < name.return_texture()->height; y++) {
    uint32_t pozycja_x = 0;
    for (uint32_t x = 0; x < name.return_texture()->width; x++) {
      for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
          texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y) * picture_width)) - center] = name.return_texture()->colors[x + (y * name.return_texture()->width)];
        }
      }
      pozycja_x += skala_textu_name;
    }
    pozycja_y += skala_textu_name;
  }

  // podpis osi y
  end = 0;
  while ((y_label[end] != '\0') && (y_label[end] != '\n')) {
    end++;
  }
  ShiftDownText::TextBox os_y(0,0,0,0, end, 1, background_color, 0);
  os_y.add_text(y_label, font_color);

  uint32_t* rotated_texture = (uint32_t*)_mm_malloc(sizeof(uint32_t) * os_y.return_texture()->width * os_y.return_texture()->height, 32);
  uint32_t rotated_width = os_y.return_texture()->height;
  uint32_t rotated_height = os_y.return_texture()->width;

  for (uint32_t y = 0; y <  rotated_height; y++) {
    for (uint32_t x = 0; x < rotated_width; x++) {
      rotated_texture[x + (y * rotated_width)] = os_y.return_texture()->colors[(os_y.return_texture()->width - 1 - y) + (x * os_y.return_texture()->width)];
    }
  }

  uint32_t skala_textu_os_y = 16;
  pozycja_y = 0;
  center = rotated_height * skala_textu_os_y / 2;

  for (uint32_t y = 0; y < rotated_height; y++) {
    uint32_t pozycja_x = 0;
    for (uint32_t x = 0; x < rotated_width; x++) {
      for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
          texture[sx + pozycja_x + ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width)] = rotated_texture[x + y * rotated_width];
        }
      }
      pozycja_x += skala_textu_os_y;
    }
    pozycja_y += skala_textu_os_y;
  }
  _mm_free(rotated_texture);


  // podpis osi x
  end = 0;
  while ((x_label[end] != '\0') && (x_label[end] != '\n')) {
    end++;
  }
  ShiftDownText::TextBox os_x(0,0,0,0, end, 1, background_color, 0);
  os_x.add_text(x_label, font_color);

  uint32_t skala_textu_os_x = 16;
  center = os_x.return_texture()->width * skala_textu_os_x / 2;

  pozycja_y = 0;
  for (uint32_t y = 0; y < os_x.return_texture()->height; y++) {
    uint32_t pozycja_x = 0;
    for (uint32_t x = 0; x < os_x.return_texture()->width; x++) {
      for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
        for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
          texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y + picture_height - os_x.return_texture()->height * skala_textu_os_x) * picture_width)) - center] = os_x.return_texture()->colors[x + (y * os_x.return_texture()->width)];
        }
      }
      pozycja_x += skala_textu_os_x;
    }
    pozycja_y += skala_textu_os_x;
  }

  //values on x
  uint32_t segmnets_count = 8;

  uint32_t step_x = ((ftr->N) / segmnets_count);

  char value[8];
  uint32_t skala_textu_value_x = 8;
  step_x = ((ftr->N) / segmnets_count);
  for (uint32_t i = 1; i < segmnets_count; i++) {
    ShiftDownText::TextBox value_x(0,0,0,0, 7, 1, background_color, 0);
    value_x.add_text(ShiftDownParsers::float_to_char(ftr->t[step_x*i], value, 3), font_color);
    uint32_t lenght_value = 0;
    while (value[lenght_value] != '\n') {
      if (value[lenght_value] == '.' || value[lenght_value] == '-' || value[lenght_value] == '0' || value[lenght_value] == '1' || value[lenght_value] == '2' || value[lenght_value] == '3'
        || value[lenght_value] == '4' || value[lenght_value] == '5' || value[lenght_value] == '6' || value[lenght_value] == '7' || value[lenght_value] == '8' || value[lenght_value] == '9') {
        lenght_value++;
      }
      else {
        break;
      }
    }
    center = (lenght_value * 8 * skala_textu_value_x) / 2;

    pozycja_y = 0;
    for (uint32_t y = 0; y < value_x.return_texture()->height; y++) {
      uint32_t pozycja_x = 0;
      for (uint32_t x = 0; x < value_x.return_texture()->width; x++) {
        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
          for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
            texture[(sx + pozycja_x + scaled_uint_x[step_x*i] + ((sy + pozycja_y + padding_top_y + graph_height + 64) * picture_width)) - center] = value_x.return_texture()->colors[x + (y * value_x.return_texture()->width)];
          }
        }
        pozycja_x += skala_textu_value_x;
      }
      pozycja_y += skala_textu_value_x;
    }
  }
  ShiftDownText::TextBox value_x(0,0,0,0, 7, 1, background_color, 0);
  value_x.add_text(ShiftDownParsers::float_to_char(ftr->t[step_x*segmnets_count - 1], value, 3), font_color);
  uint32_t lenght_value = 0;
  while (value[lenght_value] != '\n') {
    if (value[lenght_value] == '.' || value[lenght_value] == '-' || value[lenght_value] == '0' || value[lenght_value] == '1' || value[lenght_value] == '2' || value[lenght_value] == '3'
      || value[lenght_value] == '4' || value[lenght_value] == '5' || value[lenght_value] == '6' || value[lenght_value] == '7' || value[lenght_value] == '8' || value[lenght_value] == '9') {
      lenght_value++;
      }
    else {
      break;
    }
  }
  center = (lenght_value * 8 * skala_textu_value_x) / 2;
  pozycja_y = 0;
  for (uint32_t y = 0; y < value_x.return_texture()->height; y++) {
    uint32_t pozycja_x = 0;
    for (uint32_t x = 0; x < value_x.return_texture()->width; x++) {
      for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
        for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
          texture[(sx + pozycja_x + scaled_uint_x[step_x*segmnets_count - 1] + ((sy + pozycja_y + padding_top_y + graph_height + 64) * picture_width)) - center] = value_x.return_texture()->colors[x + (y * value_x.return_texture()->width)];
        }
      }
      pozycja_x += skala_textu_value_x;
    }
    pozycja_y += skala_textu_value_x;
  }

  // values y
  uint32_t kurwa_zmienna = graph_height / segmnets_count;
  uint32_t skala_textu_value_y = 8;
  float step_y = (max_y - min_y) / (float)segmnets_count;

  for (uint32_t i = 0; i < segmnets_count; i++) {
    ShiftDownText::TextBox value_y(0,0,0,0, 7, 1, background_color, 0);
    value_y.add_text(ShiftDownParsers::float_to_char(max_y - step_y * (float)i, value, 3), font_color);
    uint32_t lenght_value = 0;
    while (value[lenght_value] != '\n') {
      if (value[lenght_value] == '.' || value[lenght_value] == '-' || value[lenght_value] == '0' || value[lenght_value] == '1' || value[lenght_value] == '2' || value[lenght_value] == '3'
        || value[lenght_value] == '4' || value[lenght_value] == '5' || value[lenght_value] == '6' || value[lenght_value] == '7' || value[lenght_value] == '8' || value[lenght_value] == '9') {
        lenght_value++;
        }
      else {
        break;
      }
    }
    center = (lenght_value * 8 * skala_textu_value_y);

    pozycja_y = 0;
    for (uint32_t y = 0; y < value_y.return_texture()->height; y++) {
      uint32_t pozycja_x = 0;
      for (uint32_t x = 0; x < value_y.return_texture()->width; x++) {
        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
          for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
            texture[(padding_left_x + sx + pozycja_x - center - 64) + ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) - ((value_y.return_texture()->height * skala_textu_value_y) / 3)) * picture_width)] = value_y.return_texture()->colors[x + (y * value_y.return_texture()->width)];
          }
        }
        pozycja_x += skala_textu_value_y;
      }
      pozycja_y += skala_textu_value_y;
    }
  }
  ShiftDownText::TextBox value_y(0,0,0,0, 7, 1, background_color, 0);
  value_y.add_text(ShiftDownParsers::float_to_char(max_y - step_y * (float)segmnets_count, value, 3), font_color);
  lenght_value = 0;
  while (value[lenght_value] != '\n') {
    if (value[lenght_value] == '.' || value[lenght_value] == '-' || value[lenght_value] == '0' || value[lenght_value] == '1' || value[lenght_value] == '2' || value[lenght_value] == '3'
      || value[lenght_value] == '4' || value[lenght_value] == '5' || value[lenght_value] == '6' || value[lenght_value] == '7' || value[lenght_value] == '8' || value[lenght_value] == '9') {
      lenght_value++;
      }
    else {
      break;
    }
  }
  center = (lenght_value * 8 * skala_textu_value_y);

  pozycja_y = 0;
  for (uint32_t y = 0; y < value_y.return_texture()->height; y++) {
    uint32_t pozycja_x = 0;
    for (uint32_t x = 0; x < value_y.return_texture()->width; x++) {
      for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
        for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
          texture[(padding_left_x + sx + pozycja_x - center - 64) + ((padding_top_y + sy + pozycja_y + graph_height - ((value_y.return_texture()->height * skala_textu_value_y) / 3)) * picture_width)] = value_y.return_texture()->colors[x + (y * value_y.return_texture()->width)];
        }
      }
      pozycja_x += skala_textu_value_y;
    }
    pozycja_y += skala_textu_value_y;
  }

  //grid on x
  for (uint32_t i = 0; i < segmnets_count; i++) {
    draw_line(texture, picture_width, picture_height, scaled_uint_x[step_x*i], picture_height - padding_bot_y, scaled_uint_x[step_x*i], 0 + padding_top_y, grid_color, 16);
  }
  draw_line(texture, picture_width, picture_height, scaled_uint_x[step_x*segmnets_count - 1], picture_height - padding_bot_y, scaled_uint_x[step_x*segmnets_count - 1], 0 + padding_top_y, grid_color, 16);

  //grid on y
  for (uint32_t i = 0; i < segmnets_count; i++) {
    draw_line(texture, picture_width, picture_height, padding_left_x, padding_top_y + (kurwa_zmienna * i), picture_width - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, 16);
  }
  draw_line(texture, picture_width, picture_height, padding_left_x, padding_top_y + (graph_height), picture_width - padding_right_x, padding_top_y + (graph_height), grid_color, 16);


  //os x
  if (int_min_y <= 0) {
    uint32_t counter = 0;
    for (uint32_t i = 0; i < ftr->N; i++) {
      if (scaled_y[i] == 0) {
        break;
      }
      counter++;
    }
    draw_line(texture, picture_width, picture_height, padding_left_x, scaled_uint_y[counter], picture_width - padding_right_x, scaled_uint_y[counter], axis_color, 16);
  }
  //os y
  if (scaled_x[0] <= 0) {
    draw_line(texture, picture_width, picture_height, 0 + offset_x + padding_left_x, picture_height - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color, 16);
  }
  //wykres
  for (uint32_t i = 0; i < ftr->N - 1; i++) {
    draw_line(texture, picture_width, picture_height, scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i + 1], scaled_uint_y[i + 1], line_color, 8);
  }


  // bmp save
  char filepath[256] = {0};

  if (file_path != nullptr) {
      uint32_t i = 0;
      while (file_path[i] != '\0' && i < 255) {
          filepath[i] = file_path[i];
          i++;
      }
      filepath[i] = '\0';
  } else if (name_label != nullptr) {
      uint32_t i = 0;
      while (name_label[i] != '\0' && name_label[i] != '\n' && i < 240) {
          filepath[i] = name_label[i];
          i++;
      }
      filepath[i] = '.'; filepath[i+1] = 'b'; filepath[i+2] = 'm'; filepath[i+3] = 'p'; filepath[i+4] = '\0';
  } else {
      filepath[0] = 'w'; filepath[1] = 'y'; filepath[2] = 'k'; filepath[3] = 'r';
      filepath[4] = 'e'; filepath[5] = 's'; filepath[6] = '.'; filepath[7] = 'b';
      filepath[8] = 'm'; filepath[9] = 'p'; filepath[10] = '\0';
  }

  uint32_t image_size = picture_width * picture_height * 4;
  uint32_t file_size = 54 + image_size;

  uint8_t header[54] = {0};
  header[0] = 'B'; header[1] = 'M';
  header[2] = static_cast<uint8_t>(file_size);
  header[3] = static_cast<uint8_t>(file_size >> 8);
  header[4] = static_cast<uint8_t>(file_size >> 16);
  header[5] = static_cast<uint8_t>(file_size >> 24);
  header[10] = 54;
  header[14] = 40;

  header[18] = static_cast<uint8_t>(picture_width);
  header[19] = static_cast<uint8_t>(picture_width >> 8);
  header[20] = static_cast<uint8_t>(picture_width >> 16);
  header[21] = static_cast<uint8_t>(picture_width >> 24);

  int32_t neg_height = -static_cast<int32_t>(picture_height);
  header[22] = static_cast<uint8_t>(neg_height);
  header[23] = static_cast<uint8_t>(neg_height >> 8);
  header[24] = static_cast<uint8_t>(neg_height >> 16);
  header[25] = static_cast<uint8_t>(neg_height >> 24);

  header[26] = 1;
  header[28] = 32;
  header[34] = static_cast<uint8_t>(image_size);
  header[35] = static_cast<uint8_t>(image_size >> 8);
  header[36] = static_cast<uint8_t>(image_size >> 16);
  header[37] = static_cast<uint8_t>(image_size >> 24);

  FILE* f = fopen(filepath, "wb");
  if (f) {
      fwrite(header, 1, 54, f);
      fwrite(texture, 1, image_size, f);
      fclose(f);
#ifdef __linux__
      chmod(filepath, 0666);
#endif
  }

  _mm_free(scaled_y);
  _mm_free(scaled_x);
  _mm_free(scaled_uint_x);
  _mm_free(scaled_uint_y);
}
Graph::Graph(const SD_DFT* ftr, char* name_label, char* x_label, char* y_label, uint32_t background_color, uint32_t line_color, uint32_t axis_color, uint32_t grid_color, uint32_t font_color) : bg(background_color), lc(line_color), ac(axis_color), gc(grid_color), fc(font_color) {

  uint32_t picture_width = 7680;
  uint32_t picture_height = 4320;
  uint32_t picture_size = sizeof(uint32_t) * picture_width * picture_height;

  auto* temp_texture = static_cast<uint32_t*>(_mm_malloc(picture_size,32));
  for (uint32_t i = 0; i < picture_size; i++) {
    temp_texture[i] = background_color;
  }
  _mm_free(temp_texture);

  float min_y = std::numeric_limits<float>::max();
  float max_y = std::numeric_limits<float>::lowest();

  for (uint32_t i = 0; i < ftr->K; i++) {
    min_y = ftr->mod_z[i] < min_y ? ftr->mod_z[i] : min_y;
    max_y = ftr->mod_z[i] > max_y ? ftr->mod_z[i] : max_y;
  }

}
Graph::~Graph() {
  _mm_free(texture);
};
















