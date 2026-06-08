#pragma once
#include <cstdint>
#include <cstdio>
#include <immintrin.h>
#include <cmath>
#include <fstream>
#ifdef __linux__
#include <sys/stat.h>
#endif


namespace ShiftDownParsers{

inline uint64_t pars_uint64_hex(const char* number) {
  uint64_t parsed_number{0};
  uint64_t miejsce{0};

  uint64_t i{0};
  do{i++;}while (number[i] != '\0' && number[i] != '\n');
  i--;

  do {
    uint64_t temp_number{0};
    switch (number[i]) {
    case '0':
      temp_number = 0b00000000;

      temp_number = temp_number << (4 * miejsce);

      parsed_number |= temp_number;
      break;
    case '1':
      temp_number = 0b00000001;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '2':
      temp_number = 0b00000010;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '3':
      temp_number = 0b00000011;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '4':
      temp_number = 0b00000100;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '5':
      temp_number = 0b00000101;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '6':
      temp_number = 0b00000110;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '7':
      temp_number = 0b00000111;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '8':
      temp_number = 0b00001000;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case '9':
      temp_number = 0b00001001;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'A':
      temp_number = 0b00001010;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'B':
      temp_number = 0b00001011;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'C':
      temp_number = 0b00001100;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'D':
      temp_number = 0b00001101;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'E':
      temp_number = 0b00001110;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'F':
      temp_number = 0b00001111;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'a':
      temp_number = 0b00001010;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'b':
      temp_number = 0b00001011;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'c':
      temp_number = 0b00001100;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'd':
      temp_number = 0b00001101;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'e':
      temp_number = 0b00001110;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    case 'f':
      temp_number = 0b00001111;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    default:
      temp_number = temp_number & number[i] & 0b00001111;
      temp_number = temp_number << (4 * miejsce);
      parsed_number |= temp_number;
      break;
    }

    miejsce++;
    i--;
  } while (i+1 != 0);


  return parsed_number;
}

inline uint64_t pars_uint64(const char* number) {
  uint64_t miejsce{0};
  uint64_t parsed_number{0};
  uint64_t i{0};

  do{i++;}while (number[i] != '\0' && number[i] != '\n');
  i--;

  do {
    uint64_t temp_number{0};

    temp_number = number[i] & 0b00001111;
    for (uint64_t k = 0; k != miejsce; k++) {
      temp_number = temp_number * 10;
    }
    parsed_number += temp_number;
    miejsce++;

    i--;
  } while (i+1 > 0);

  return parsed_number;
}

inline char* pars_char(uint64_t number, char* buffer) {

  buffer[20] = '\0';

  if (number == 0) {
    buffer[19] = '0';
    return &buffer[19];
  }

  uint64_t i = 19;
  while (number != 0) {
    uint64_t rest = number % 10;
    number /= 10;
    buffer[i] = 0b00110000 | rest;
    i--;
  }
  return &buffer[i + 1];
}




inline char* float_to_char(float number, char* buffer, uint32_t precision = 3) {

  for (int k = 0; k < 32; k++) buffer[k] = '\0';

  if (number == 0.0f) {
    buffer[0] = '0';
    if (precision > 0) {
      buffer[1] = '.';
      for (uint32_t p = 0; p < precision; p++) buffer[2 + p] = '0';
    }
    return buffer;
  }

  uint32_t float_bits = *reinterpret_cast<uint32_t*>(&number);
  bool is_negative = (float_bits >> 31) != 0;
  if (is_negative) number = -number;

  uint64_t multiplier = 1;
  for (uint32_t p = 0; p < precision; p++) multiplier *= 10;

  uint64_t total_val = static_cast<uint64_t>(number * static_cast<float>(multiplier) + 0.5f);

  uint64_t int_part = total_val / multiplier;
  uint64_t frac_int = total_val % multiplier;

  uint32_t idx = 0;
  if (is_negative) {
    buffer[idx] = '-';
    idx++;
  }

  uint64_t temp_int = int_part;
  uint32_t int_len = 0;
  if (temp_int == 0) int_len = 1;
  else while (temp_int != 0) { int_len++; temp_int /= 10; }

  uint32_t int_end_idx = idx + int_len - 1;
  temp_int = int_part;
  if (temp_int == 0) buffer[int_end_idx] = '0';
  else {
    for (uint32_t j = 0; j < int_len; j++) {
      buffer[int_end_idx - j] = 0b00110000 | (temp_int % 10);
      temp_int /= 10;
    }
  }
  idx += int_len;

  if (precision > 0) {
    buffer[idx] = '.';
    idx++;
    uint32_t frac_end_idx = idx + precision - 1;
    for (uint32_t j = 0; j < precision; j++) {
      buffer[frac_end_idx - j] = 0b00110000 | (frac_int % 10);
      frac_int /= 10;
    }
    idx += precision;
  }


  buffer[idx] = '\0';
  return buffer;
}


}
namespace ShiftDownText {
    class TextBox;
}
namespace ShiftDownRenderer {
    class Renderer;
}

namespace ShiftDownObjects {

    struct alignas(64) ObjectHitbox {

        enum class Type {BOX, CIRCLE };
        Type type{Type::BOX};

        float offset_x{0.f};
        float offset_y{0.f};

        float width{0.f};
        float height{0.f};
        float radius{0.f};

        ObjectHitbox(float width, float height, float offset_x = 0.f, float offset_y = 0.f);
        ObjectHitbox(float radius, float offset_x = 0.f, float offset_y = 0.f);
        ~ObjectHitbox() = default;
    };

    struct alignas(64) ObjectTexture {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t* colors = nullptr;

        ObjectTexture(uint32_t width, uint32_t height, uint32_t *pointer);
        ~ObjectTexture();
    };

    class Object {

        friend ShiftDownText::TextBox;
        friend ShiftDownRenderer::Renderer;

        const float* get_model() const;
        uint32_t get_priority() const;
        void update_model();

        struct Position {
            float x{0};
            float y{0};
        };

        alignas(64) const float* parent_model = nullptr;
        alignas(64) float local_model[9]{0};
        alignas(64) float world_model[9]{0};

        Position position{};
        float rotation{0.f};
        uint32_t priority{0}; // z axis for render queue

        ObjectTexture* texture = nullptr;
        ObjectHitbox* hitbox = nullptr;

    public:
        Object(float position_x = 0, float position_y = 0, float rotation = 0, uint32_t priority = 0);
        ~Object() = default;

        void shift_position(float value_x, float value_y);
        void rotate_object(float value);

        const Position* get_position() const;

        void set_hitbox(ObjectHitbox* new_hitbox);
        void set_texture(ObjectTexture* new_texture);
        void set_parent_object(const Object* parent_object);

    };


}
//WORLD MATRIX AS PARENTS
alignas(64) static constexpr float IdentityMatrix[9] = {
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f
};
using namespace ShiftDownObjects;
inline ObjectHitbox::ObjectHitbox(float w, float h, float ox, float oy) : type(Type::BOX), offset_x(ox), offset_y(oy), width(w), height(h), radius(0.f) {}
inline ObjectHitbox::ObjectHitbox(float r, float ox, float oy) : type(Type::CIRCLE), offset_x(ox), offset_y(oy), width(0.f), height(0.f), radius(r) {}
inline ObjectTexture::ObjectTexture(uint32_t w, uint32_t h, uint32_t *p) : width(w), height(h) {
  colors = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t)* width * height, 32));
  for (uint32_t i = 0; i < w*h; i++) {
    colors[i] = p[i];
  }
}
inline ObjectTexture::~ObjectTexture() {
  _mm_free(colors);
}
inline void Object::update_model() {

  float cx = cosf(rotation * 0.017453293f);
  float sx = sinf(rotation * 0.017453293f);

  local_model[0] = cx;
  local_model[1] = -sx;
  local_model[2] = position.x;
  local_model[3] = sx;
  local_model[4] = cx;
  local_model[5] = position.y;
  local_model[6] = 0.0f;
  local_model[7] = 0.0f;
  local_model[8] = 1.0f;

  world_model[0] = parent_model[0] * local_model[0] + parent_model[1] * local_model[3];
  world_model[1] = parent_model[0] * local_model[1] + parent_model[1] * local_model[4];
  world_model[2] = parent_model[0] * local_model[2] + parent_model[1] * local_model[5] + parent_model[2];

  world_model[3] = parent_model[3] * local_model[0] + parent_model[4] * local_model[3];
  world_model[4] = parent_model[3] * local_model[1] + parent_model[4] * local_model[4];
  world_model[5] = parent_model[3] * local_model[2] + parent_model[4] * local_model[5] + parent_model[5];

  world_model[6] = 0.0f;
  world_model[7] = 0.0f;
  world_model[8] = 1.0f;
}
inline Object::Object(float x, float y, float r, uint32_t prio){
  position.x = x;
  position.y = y;
  rotation = r;
  priority = prio;
  parent_model = IdentityMatrix;
  update_model();
}
inline void Object::shift_position(float value_x, float value_y) {
  position.x += value_x;
  position.y += value_y;
  update_model();
}
inline void Object::rotate_object(float value) {
  rotation += value;
  update_model();
}
inline void Object::set_hitbox(ObjectHitbox* new_hitbox) {
  hitbox = new_hitbox;
}
inline void Object::set_texture(ObjectTexture* new_texture) {
  texture = new_texture;
}
inline void Object::set_parent_object(const Object* parent_object) {
  parent_model = parent_object->world_model;
}
inline const float* Object::get_model() const{
  return world_model;
}
inline uint32_t Object::get_priority() const{
  return priority;
}
inline const Object::Position* Object::get_position() const {
  return &position;
}



namespace ShiftDownText {

    constexpr uint32_t font_width = 8;
    constexpr uint32_t font_height = 16;

    class TextBox {
        ShiftDownObjects::Object* text_box = nullptr;
        ShiftDownObjects::ObjectTexture* text_box_texture = nullptr;
        ShiftDownObjects::ObjectHitbox* shape_ptr = nullptr;

        uint32_t current_line{0};
        uint32_t current_letter{0};

    public:
        TextBox(float position_x, float position_y, float phys_width, float phys_height, uint32_t max_chars_per_line, uint32_t max_lines, uint32_t bg_color = 0x00000000, float rotation = 0.f, uint32_t priority = 0);
        ~TextBox();

        void add_text(const char *text, uint32_t font_color);
        ShiftDownObjects::Object* return_object_data(){ return text_box; }
        ShiftDownObjects::ObjectTexture* return_texture(){ return text_box_texture; }
    };

}
// --- CYFRY (0-9) ---
constexpr uint64_t font_0[4] = {0x0000000038004400, 0x4400440044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_1[4] = {0x0000000010003000, 0x1000100010001000, 0x1000380000000000, 0x0000000000000000};
constexpr uint64_t font_2[4] = {0x0000000038004400, 0x0400080010002000, 0x40007C0000000000, 0x0000000000000000};
constexpr uint64_t font_3[4] = {0x0000000038004400, 0x0400180004000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_4[4] = {0x0000000008001800, 0x280048007C000800, 0x0800080000000000, 0x0000000000000000};
constexpr uint64_t font_5[4] = {0x000000007C004000, 0x4000780004000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_6[4] = {0x0000000018002000, 0x4000780044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_7[4] = {0x000000007C000400, 0x0800100020002000, 0x2000200000000000, 0x0000000000000000};
constexpr uint64_t font_8[4] = {0x0000000038004400, 0x4400380044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_9[4] = {0x0000000038004400, 0x440044003C000400, 0x0800300000000000, 0x0000000000000000};

// --- ZNAKI SPECJALNE I MATEMATYKA ---
constexpr uint64_t font_Space[4]   = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Exclam[4]  = {0x0000100010001000, 0x1000100000001000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_At[4]      = {0x0000000038004400, 0x540054005C004000, 0x3800000000000000, 0x0000000000000000};
constexpr uint64_t font_Hash[4]    = {0x0000000024002400, 0x7E0024007E002400, 0x2400000000000000, 0x0000000000000000};
constexpr uint64_t font_Dollar[4]  = {0x0000100038005000, 0x3800140038001000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Percent[4] = {0x0000000062006400, 0x0800100026004600, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Caret[4]   = {0x0000100028004400, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Ampers[4]  = {0x0000000030004800, 0x3000640048003400, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Star[4]    = {0x0000000010005400, 0x3800540010000000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_ParenL[4]  = {0x0000100020004000, 0x4000400040004000, 0x4000200010000000, 0x0000000000000000};
constexpr uint64_t font_ParenR[4]  = {0x0000080004000200, 0x0200020002000200, 0x0200040008000000, 0x0000000000000000};
constexpr uint64_t font_Minus[4]   = {0x0000000000000000, 0x0000380000000000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Under[4]   = {0x0000000000000000, 0x0000000000000000, 0x000000007E000000, 0x0000000000000000};
constexpr uint64_t font_Plus[4]    = {0x0000000000000000, 0x080008003E000800, 0x0800000000000000, 0x0000000000000000};
constexpr uint64_t font_Equals[4]  = {0x0000000000000000, 0x00003E0000003E00, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_BracketL[4]= {0x0000300020002000, 0x2000200020002000, 0x2000300000000000, 0x0000000000000000};
constexpr uint64_t font_BracketR[4]= {0x00000C0004000400, 0x0400040004000400, 0x04000C0000000000, 0x0000000000000000};
constexpr uint64_t font_BraceL[4]  = {0x0000180010001000, 0x2000100010001800, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_BraceR[4]  = {0x0000180008000800, 0x0400080008001800, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Pipe[4]    = {0x0000100010001000, 0x1000100010001000, 0x1000100000000000, 0x0000000000000000};
constexpr uint64_t font_Backsl[4]  = {0x0000400040002000, 0x2000100010000800, 0x0800000000000000, 0x0000000000000000};
constexpr uint64_t font_Colon[4]   = {0x0000000000000000, 0x1800000000001800, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Semic[4]   = {0x0000000000000000, 0x1800000000001800, 0x1000000000000000, 0x0000000000000000};
constexpr uint64_t font_Quote[4]   = {0x0000240024000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Apostr[4]  = {0x0000180018000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Less[4]    = {0x0000000000000800, 0x1000200010000800, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Greater[4] = {0x0000000000002000, 0x1000080010002000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Comma[4]   = {0x0000000000000000, 0x0000000000000000, 0x0000180010000000, 0x0000000000000000};
constexpr uint64_t font_Dot[4]     = {0x0000000000000000, 0x0000000000000000, 0x0000180000000000, 0x0000000000000000};
constexpr uint64_t font_Slash[4]   = {0x0000020004000400, 0x0800080010001000, 0x2000200040000000, 0x0000000000000000};
constexpr uint64_t font_Question[4]= {0x0000380044000400, 0x0800100000001000, 0x0000000000000000, 0x0000000000000000};
constexpr uint64_t font_Tilde[4]   = {0x0000000034005800, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};

// --- DUŻE LITERY (A-Z) ---
constexpr uint64_t font_A[4] = {0x0000000010002800, 0x440044007C004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_B[4] = {0x0000000078002400, 0x2400380024002400, 0x2400780000000000, 0x0000000000000000};
constexpr uint64_t font_C[4] = {0x0000000038004400, 0x4000400040004000, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_D[4] = {0x0000000078002400, 0x2200220022002200, 0x2400780000000000, 0x0000000000000000};
constexpr uint64_t font_E[4] = {0x000000007C004000, 0x4000780040004000, 0x40007C0000000000, 0x0000000000000000};
constexpr uint64_t font_F[4] = {0x000000007C004000, 0x4000780040004000, 0x4000400000000000, 0x0000000000000000};
constexpr uint64_t font_G[4] = {0x0000000038004400, 0x400040004C004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_H[4] = {0x0000000044004400, 0x44007C0044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_I[4] = {0x0000000038001000, 0x1000100010001000, 0x1000380000000000, 0x0000000000000000};
constexpr uint64_t font_J[4] = {0x000000000C000400, 0x0400040004000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_K[4] = {0x0000000044004800, 0x5000600050004800, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_L[4] = {0x0000000040004000, 0x4000400040004000, 0x40007C0000000000, 0x0000000000000000};
constexpr uint64_t font_M[4] = {0x0000000044006C00, 0x5400540044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_N[4] = {0x0000000044006400, 0x54004C0044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_O[4] = {0x0000000038004400, 0x4400440044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_P[4] = {0x0000000078004400, 0x4400440078004000, 0x4000400000000000, 0x0000000000000000};
constexpr uint64_t font_Q[4] = {0x0000000038004400, 0x4400440044005400, 0x4800340000000000, 0x0000000000000000};
constexpr uint64_t font_R[4] = {0x0000000078004400, 0x4400440078005000, 0x4800440000000000, 0x0000000000000000};
constexpr uint64_t font_S[4] = {0x0000000038004400, 0x4000380004000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_T[4] = {0x000000007C001000, 0x1000100010001000, 0x1000100000000000, 0x0000000000000000};
constexpr uint64_t font_U[4] = {0x0000000044004400, 0x4400440044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_V[4] = {0x0000000044004400, 0x4400440028002800, 0x1000100000000000, 0x0000000000000000};
constexpr uint64_t font_W[4] = {0x0000000044004400, 0x4400540054005400, 0x2800280000000000, 0x0000000000000000};
constexpr uint64_t font_X[4] = {0x0000000044004400, 0x2800100010002800, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_Y[4] = {0x0000000044004400, 0x4400280010001000, 0x1000100000000000, 0x0000000000000000};
constexpr uint64_t font_Z[4] = {0x000000007C000400, 0x0800100020004000, 0x40007C0000000000, 0x0000000000000000};

// --- MAŁE LITERY (a-z) ---
constexpr uint64_t font_a[4] = {0x0000000000000000, 0x380004003C004400, 0x44003C0000000000, 0x0000000000000000};
constexpr uint64_t font_b[4] = {0x0000000040004000, 0x5800640044004400, 0x6400580000000000, 0x0000000000000000};
constexpr uint64_t font_c[4] = {0x0000000000000000, 0x3800440040004000, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_d[4] = {0x0000000004000400, 0x34004C0044004400, 0x4C00340000000000, 0x0000000000000000};
constexpr uint64_t font_e[4] = {0x0000000000000000, 0x380044007C004000, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_f[4] = {0x000000001C002000, 0x7800200020002000, 0x2000200000000000, 0x0000000000000000};
constexpr uint64_t font_g[4] = {0x0000000000000000, 0x34004C0044004400, 0x4C00340004003800, 0x0000000000000000};
constexpr uint64_t font_h[4] = {0x0000000040004000, 0x5800640044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_i[4] = {0x0000000010000000, 0x3000100010001000, 0x1000380000000000, 0x0000000000000000};
constexpr uint64_t font_j[4] = {0x0000080000001800, 0x0800080008000800, 0x0800080008003000, 0x0000000000000000};
constexpr uint64_t font_k[4] = {0x0000000040004000, 0x4400480070004800, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_l[4] = {0x0000000030001000, 0x1000100010001000, 0x10000C0000000000, 0x0000000000000000};
constexpr uint64_t font_m[4] = {0x0000000000000000, 0x6800540054005400, 0x5400540000000000, 0x0000000000000000};
constexpr uint64_t font_n[4] = {0x0000000000000000, 0x5800640044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_o[4] = {0x0000000000000000, 0x3800440044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_p[4] = {0x0000000000000000, 0x5800640044004400, 0x6400580040004000, 0x0000000000000000};
constexpr uint64_t font_q[4] = {0x0000000000000000, 0x34004C0044004400, 0x4C00340004000400, 0x0000000000000000};
constexpr uint64_t font_r[4] = {0x0000000000000000, 0x5C00600040004000, 0x4000400000000000, 0x0000000000000000};
constexpr uint64_t font_s[4] = {0x0000000000000000, 0x3C00400038000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_t[4] = {0x0000000020002000, 0x7800200020002000, 0x2400180000000000, 0x0000000000000000};
constexpr uint64_t font_u[4] = {0x0000000000000000, 0x4400440044004400, 0x4C00340000000000, 0x0000000000000000};
constexpr uint64_t font_v[4] = {0x0000000000000000, 0x4400440044002800, 0x2800100000000000, 0x0000000000000000};
constexpr uint64_t font_w[4] = {0x0000000000000000, 0x4400440054005400, 0x5400280000000000, 0x0000000000000000};
constexpr uint64_t font_x[4] = {0x0000000000000000, 0x4400280010001000, 0x2800440000000000, 0x0000000000000000};
constexpr uint64_t font_y[4] = {0x0000000000000000, 0x4400440044004C00, 0x3400040004003800, 0x0000000000000000};
constexpr uint64_t font_z[4] = {0x0000000000000000, 0x7C00080010002000, 0x40007C0000000000, 0x0000000000000000};

// --- POLSKIE ZNAKI (Duże) ---
constexpr uint64_t font_A_pl[4] = {0x0000000010002800, 0x440044007C004400, 0x4400440008001000, 0x0000000000000000};
constexpr uint64_t font_C_pl[4] = {0x0800100038004400, 0x4000400040004000, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_E_pl[4] = {0x000000007C004000, 0x4000780040004000, 0x40007C0008001000, 0x0000000000000000};
constexpr uint64_t font_L_pl[4] = {0x0000000040004000, 0x4000400070004000, 0x40007C0000000000, 0x0000000000000000};
constexpr uint64_t font_N_pl[4] = {0x0800100044006400, 0x54004C0044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_O_pl[4] = {0x0800100038004400, 0x4400440044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_S_pl[4] = {0x0800100038004400, 0x4000380004000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_Z_kres[4]={0x080010007C000400, 0x0800100020004000, 0x40007C0000000000, 0x0000000000000000};
constexpr uint64_t font_Z_krop[4]={0x100000007C000400, 0x0800100020004000, 0x40007C0000000000, 0x0000000000000000};

// --- POLSKIE ZNAKI (Małe) ---
constexpr uint64_t font_a_pl[4] = {0x0000000000000000, 0x380004003C004400, 0x44003C0008001000, 0x0000000000000000};
constexpr uint64_t font_c_pl[4] = {0x0800100000000000, 0x3800440040004000, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_e_pl[4] = {0x0000000000000000, 0x380044007C004000, 0x4400380008001000, 0x0000000000000000};
constexpr uint64_t font_l_pl[4] = {0x0000000030001000, 0x1000100070001000, 0x10000C0000000000, 0x0000000000000000};
constexpr uint64_t font_n_pl[4] = {0x0800100000000000, 0x5800640044004400, 0x4400440000000000, 0x0000000000000000};
constexpr uint64_t font_o_pl[4] = {0x0800100000000000, 0x3800440044004400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_s_pl[4] = {0x0800100000000000, 0x3C00400038000400, 0x4400380000000000, 0x0000000000000000};
constexpr uint64_t font_z_kres[4]={0x0800100000000000, 0x7C00080010002000, 0x40007C0000000000, 0x0000000000000000};
constexpr uint64_t font_z_krop[4]={0x1000000000000000, 0x7C00080010002000, 0x40007C0000000000, 0x0000000000000000};
using namespace ShiftDownText;
inline TextBox::TextBox(float position_x, float position_y, float phys_width, float phys_height, uint32_t max_chars_per_line, uint32_t max_lines, uint32_t bg_color, float rotation, uint32_t priority) {

  uint32_t tex_w = max_chars_per_line * font_width;
  uint32_t tex_h = max_lines * font_height;

  text_box = new ShiftDownObjects::Object(position_x, position_y, rotation, priority);

  shape_ptr = new ShiftDownObjects::ObjectHitbox(phys_width, phys_height, 0.f, 0.f);
  text_box->set_hitbox(shape_ptr);

  uint32_t* temp_background = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * tex_w * tex_h, 32));
  for (uint64_t i = 0; i < tex_w * tex_h; i++) {
    temp_background[i] = bg_color;
  }

  text_box_texture = new ShiftDownObjects::ObjectTexture(tex_w, tex_h, temp_background);
  text_box->set_texture(text_box_texture);

  _mm_free(temp_background);
}
inline TextBox::~TextBox() {
  delete text_box;
  delete text_box_texture;
  delete shape_ptr;
}
inline void TextBox::add_text(const char *text, uint32_t font_color) {
  uint64_t i = 0;
  while (text[i] != '\0') {

    const uint64_t* current_font_data = nullptr;
    auto letter = static_cast<unsigned char>(text[i]);

    if (letter < 0x80) {
      switch (letter) {
        case 'A': current_font_data = font_A; break;
        case 'B': current_font_data = font_B; break;
        case 'C': current_font_data = font_C; break;
        case 'D': current_font_data = font_D; break;
        case 'E': current_font_data = font_E; break;
        case 'F': current_font_data = font_F; break;
        case 'G': current_font_data = font_G; break;
        case 'H': current_font_data = font_H; break;
        case 'I': current_font_data = font_I; break;
        case 'J': current_font_data = font_J; break;
        case 'K': current_font_data = font_K; break;
        case 'L': current_font_data = font_L; break;
        case 'M': current_font_data = font_M; break;
        case 'N': current_font_data = font_N; break;
        case 'O': current_font_data = font_O; break;
        case 'P': current_font_data = font_P; break;
        case 'Q': current_font_data = font_Q; break;
        case 'R': current_font_data = font_R; break;
        case 'S': current_font_data = font_S; break;
        case 'T': current_font_data = font_T; break;
        case 'U': current_font_data = font_U; break;
        case 'V': current_font_data = font_V; break;
        case 'W': current_font_data = font_W; break;
        case 'X': current_font_data = font_X; break;
        case 'Y': current_font_data = font_Y; break;
        case 'Z': current_font_data = font_Z; break;
        case 'a': current_font_data = font_a; break;
        case 'b': current_font_data = font_b; break;
        case 'c': current_font_data = font_c; break;
        case 'd': current_font_data = font_d; break;
        case 'e': current_font_data = font_e; break;
        case 'f': current_font_data = font_f; break;
        case 'g': current_font_data = font_g; break;
        case 'h': current_font_data = font_h; break;
        case 'i': current_font_data = font_i; break;
        case 'j': current_font_data = font_j; break;
        case 'k': current_font_data = font_k; break;
        case 'l': current_font_data = font_l; break;
        case 'm': current_font_data = font_m; break;
        case 'n': current_font_data = font_n; break;
        case 'o': current_font_data = font_o; break;
        case 'p': current_font_data = font_p; break;
        case 'q': current_font_data = font_q; break;
        case 'r': current_font_data = font_r; break;
        case 's': current_font_data = font_s; break;
        case 't': current_font_data = font_t; break;
        case 'u': current_font_data = font_u; break;
        case 'v': current_font_data = font_v; break;
        case 'w': current_font_data = font_w; break;
        case 'x': current_font_data = font_x; break;
        case 'y': current_font_data = font_y; break;
        case 'z': current_font_data = font_z; break;
        case '0': current_font_data = font_0; break;
        case '1': current_font_data = font_1; break;
        case '2': current_font_data = font_2; break;
        case '3': current_font_data = font_3; break;
        case '4': current_font_data = font_4; break;
        case '5': current_font_data = font_5; break;
        case '6': current_font_data = font_6; break;
        case '7': current_font_data = font_7; break;
        case '8': current_font_data = font_8; break;
        case '9': current_font_data = font_9; break;
        case ' ': current_font_data = font_Space; break;
        case '!': current_font_data = font_Exclam; break;
        case '@': current_font_data = font_At; break;
        case '#': current_font_data = font_Hash; break;
        case '$': current_font_data = font_Dollar; break;
        case '%': current_font_data = font_Percent; break;
        case '^': current_font_data = font_Caret; break;
        case '&': current_font_data = font_Ampers; break;
        case '*': current_font_data = font_Star; break;
        case '(': current_font_data = font_ParenL; break;
        case ')': current_font_data = font_ParenR; break;
        case '-': current_font_data = font_Minus; break;
        case '_': current_font_data = font_Under; break;
        case '+': current_font_data = font_Plus; break;
        case '=': current_font_data = font_Equals; break;
        case '[': current_font_data = font_BracketL; break;
        case ']': current_font_data = font_BracketR; break;
        case '{': current_font_data = font_BraceL; break;
        case '}': current_font_data = font_BraceR; break;
        case '|': current_font_data = font_Pipe; break;
        case '\\': current_font_data = font_Backsl; break;
        case ':': current_font_data = font_Colon; break;
        case ';': current_font_data = font_Semic; break;
        case '"': current_font_data = font_Quote; break;
        case '\'': current_font_data = font_Apostr; break;
        case '<': current_font_data = font_Less; break;
        case '>': current_font_data = font_Greater; break;
        case ',': current_font_data = font_Comma; break;
        case '.': current_font_data = font_Dot; break;
        case '/': current_font_data = font_Slash; break;
        case '?': current_font_data = font_Question; break;
        case '~': current_font_data = font_Tilde; break;
        default:  current_font_data = font_Question; break;
      }
      i++;
    }
    else if ((letter & 0xE0) == 0xC0) {
      auto letter_second_part = static_cast<unsigned char>(text[i+1]);
      if (letter_second_part == '\0') break;

      switch ((letter << 8) | letter_second_part) {
      case 0xC484: current_font_data = font_A_pl; break;
      case 0xC486: current_font_data = font_C_pl; break;
      case 0xC498: current_font_data = font_E_pl; break;
      case 0xC581: current_font_data = font_L_pl; break;
      case 0xC583: current_font_data = font_N_pl; break;
      case 0xC393: current_font_data = font_O_pl; break;
      case 0xC59A: current_font_data = font_S_pl; break;
      case 0xC5B9: current_font_data = font_Z_kres; break;
      case 0xC5BB: current_font_data = font_Z_krop; break;
      case 0xC485: current_font_data = font_a_pl; break;
      case 0xC487: current_font_data = font_c_pl; break;
      case 0xC499: current_font_data = font_e_pl; break;
      case 0xC582: current_font_data = font_l_pl; break;
      case 0xC584: current_font_data = font_n_pl; break;
      case 0xC3B3: current_font_data = font_o_pl; break;
      case 0xC59B: current_font_data = font_s_pl; break;
      case 0xC5BA: current_font_data = font_z_kres; break;
      case 0xC5BC: current_font_data = font_z_krop; break;
      default:     current_font_data = font_Question; break;
      }
      i += 2;
    }
    else {
      current_font_data = font_Question;
      i++;
    }

    if (current_font_data != nullptr) {
      uint64_t temp_y = 0;
      uint64_t temp_x = 0;

      for (uint64_t n = 0; n < 4; n++) {
        uint64_t temp_buffer = current_font_data[n];

        for (uint64_t y = 0; y < 64; y++) {
          switch ((temp_buffer >> (63 - y)) & 0x01) {
          case 1:
            text_box_texture->colors[(current_line + temp_y) * text_box_texture->width + (current_letter + temp_x)] = font_color;
            break;
          default:
            break;
          }
          temp_x++;
          if (temp_x == 16) {
            temp_y++;
            temp_x = 0;
          }
        }
      }
      current_letter += 8;
      if (current_letter >= text_box_texture->width) {
        current_letter = 0;
        current_line += 16;
        if (current_line >= text_box_texture->height) return;
      }
    }
    else {
      current_letter += 8;
      if (current_letter >= text_box_texture->width) {
        current_letter = 0;
        current_line += 16;
        if (current_line >= text_box_texture->height) return;
      }
    }
  }
}


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
//dodac funckje dla haminga i inne bajery z tym zwiazane
using namespace ShiftDownFunctions;
inline Function::Function(float Tc, float fs, float f, float PHI, float A, FormulaPtr formula) : Tc(Tc), fs(fs), f(f), PHI(PHI), A(A), Ts(1.f/fs), N(static_cast<uint64_t>(Tc * fs)) {
    function_formula = formula;
    t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));
    f_t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));

    for (uint64_t n = 0; n < N; n++) t[n] = static_cast<float>(n) * Ts;
    for (uint64_t n = 0; n < N; n++) f_t[n] = formula(*this, n, t[n]);
}
inline Function::~Function() {
    _mm_free(t);
    _mm_free(f_t);
}
inline SD_DFT::SD_DFT(const Function* function) {
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
inline SD_DFT::~SD_DFT() {
  _mm_free(Im);
  _mm_free(Re);

  _mm_free(mod_z);
  _mm_free(fk);
}
//sinusowe
inline Function* ShiftDownFunctions::modulate_AM_sin(const Function* ftm, float A, float fs, float PHI) {

  auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);

  AM->A = A;
  AM->fs = fs;
  AM->PHI = PHI;

  for (uint64_t t = 0; t < AM->N; t++) {
    AM->f_t[t] = (A + ftm->f_t[t]) * sinf(2.f * M_PIf * fs * AM->t[t] + PHI);
  }

  return AM;
}
inline Function* ShiftDownFunctions::modulate_PM_sin(const Function* ftm, float A, float fs, float PHI) {

  auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
  PM->A = A;
  PM->fs = fs;
  PM->PHI = PHI;

  for (uint64_t t = 0; t < PM->N; t++) {
    PM->f_t[t] = A * sinf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI);
  }

  return PM;
}
inline Function* ShiftDownFunctions::modulate_FM_sin(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_AM_saw(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_PM_saw(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_FM_saw(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_AM_rec(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_PM_rec(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_FM_rec(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_AM_tri(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_PM_tri(const Function* ftm, float A, float fs, float PHI) {
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
inline Function* ShiftDownFunctions::modulate_FM_tri(const Function* ftm, float A, float fs, float PHI) {
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
inline void draw_line(uint32_t* texture, uint32_t texture_width, uint32_t texture_height, uint32_t Ax, uint32_t Ay, uint32_t Bx, uint32_t By, uint32_t color, uint32_t thickness) {

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
inline Graph::Graph(const Function* ftr, char* name_label, char* x_label, char* y_label, uint32_t background_color, uint32_t line_color, uint32_t axis_color, uint32_t grid_color,uint32_t font_color, const char* file_path) : bg(background_color), lc(line_color), ac(axis_color), gc(grid_color), fc(font_color) {
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
inline Graph::Graph(const SD_DFT* ftr, char* name_label, char* x_label, char* y_label, uint32_t background_color, uint32_t line_color, uint32_t axis_color, uint32_t grid_color, uint32_t font_color) : bg(background_color), lc(line_color), ac(axis_color), gc(grid_color), fc(font_color) {

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
inline Graph::~Graph() {
  _mm_free(texture);
};
