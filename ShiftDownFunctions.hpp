/*
    ShiftDownFunctions - C with classes, simple function renderer
    Author: DeadDeth
    Repository: https://github.com/DeadDeth/ShiftDownFunctions
    License: MIT

    This I hope will simplify the process of rendering graphs for someone on bare cpp,
    No extra libraries needed, all is in one file, easy to get, easy to use <3
*/


/*
    Jak dodać do swojego projektu / How to add to your own project

    A. Można pobrać ShiftDownFunctions.hpp i dodać do projektu / Just download the ShiftDownFunctions.hpp file and add to project

    B. Dodać to do swojego CMakeLists.txt / Add this to yours CMakeLists.txt
       Potrzebuje zainstalowanego gita do działania / Needs git to be installed in order to work
       Należy dodać to pod "add_executable(...)" / Place it under the "add_executable(...)"

    include(FetchContent)
    FetchContent_Declare(ShiftDownFunctions GIT_REPOSITORY https://github.com/DeadDeth/ShiftDownFunctions.git GIT_TAG main)
    FetchContent_MakeAvailable(ShiftDownFunctions)
    target_link_libraries([program name from add_executable()] PRIVATE ShiftDownFunctions)

    To powinno, przynajmniej dla Clion, pobrać wszystko automatycznie z GitHub. / This should automatically download the file, at least for Clion, from GitHub.
*/


#pragma once
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <thread>

#ifdef __linux__
#include <sys/stat.h>
#endif

#ifndef M_PIf
#define M_PIf 3.14159265358979323846f
#endif

// predefiniowane kolory jako ARGB
namespace Colors {
//  ARGB -> ALPHA, RED, GREEN, BLUE -> 0xAARRGGBB (rozkład w hexa-decymalnym)
union ARGB {
  uint32_t color{0};
  struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
  } argb;
  constexpr operator uint32_t() const{return color;};
  constexpr ARGB(uint32_t argb) : color(argb) {}
  constexpr ARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : argb{b,g,r,a} {}
};

// --- PODSTAWOWE ---
constexpr ARGB RED     = {255, 0, 0};         // #FF0000
constexpr ARGB GREEN   = {0, 255, 0};         // #00FF00
constexpr ARGB BLUE    = {0, 0, 255};         // #0000FF
constexpr ARGB WHITE   = {255, 255, 255};     // #FFFFFF
constexpr ARGB BLACK   = {0, 0, 0};           // #000000
constexpr ARGB GRAY    = {128, 128, 128};     // #808080
constexpr ARGB YELLOW  = {255, 255, 0};       // #FFFF00
constexpr ARGB CYAN    = {0, 255, 255};       // #00FFFF
constexpr ARGB MAGENTA = {255, 0, 255};       // #FF00FF

// --- PRZYDATNE DO WYKRESÓW (Jaskrawe / Kontrastowe) ---
constexpr ARGB ORANGE       = {255, 165, 0};  // #FFA500
constexpr ARGB PINK         = {255, 192, 203};// #FFC0CB
constexpr ARGB HOT_PINK     = {255, 105, 180};// #FF69B4
constexpr ARGB LIME         = {50, 205, 50};  // #32CD32
constexpr ARGB TEAL         = {0, 128, 128};  // #008080
constexpr ARGB PURPLE       = {128, 0, 128};  // #800080
constexpr ARGB VIOLET       = {238, 130, 238};// #EE82EE
constexpr ARGB GOLD         = {255, 215, 0};  // #FFD700
constexpr ARGB CORAL        = {255, 127, 80}; // #FF7F50
constexpr ARGB CRIMSON      = {220, 20, 60};  // #DC143C
constexpr ARGB AQUA         = {0, 255, 255};  // #00FFFF
constexpr ARGB AMBER        = {255, 191, 0};  // #FFBF00

// --- TŁA / UI / TRYB CIEMNY ---
constexpr ARGB DARK_GRAY    = {64, 64, 64};   // #404040
constexpr ARGB LIGHT_GRAY   = {192, 192, 192};// #C0C0C0
constexpr ARGB SLATE_GRAY   = {112, 128, 144};// #708090
constexpr ARGB NAVY         = {0, 0, 128};    // #000080
constexpr ARGB MIDNIGHT_BLUE= {25, 25, 112};  // #191970
constexpr ARGB CHARCOAL     = {54, 69, 79};   // #36454F
constexpr ARGB MATTE_BLACK  = {28, 28, 28};   // #1C1C1C
constexpr ARGB DARK_PURPLE  = {42, 0, 77};    // #2A004D
constexpr ARGB BLOOD_RED    = {138, 3, 3};    // #8A0303
constexpr ARGB FOREST_GREEN = {34, 139, 34};  // #228B22
constexpr ARGB OLIVE        = {128, 128, 0};  // #808000
constexpr ARGB CHOCOLATE    = {210, 105, 30}; // #D2691E

// --- PASTELOWE (Do wypełnień pod wykresem) ---
constexpr ARGB PASTEL_RED   = {255, 105, 97}; // #FF6961
constexpr ARGB PASTEL_GREEN = {119, 221, 119};// #77DD77
constexpr ARGB PASTEL_BLUE  = {174, 198, 207};// #AEC6CF
constexpr ARGB PASTEL_YELLOW= {253, 253, 150};// #FDFD96
constexpr ARGB PASTEL_PURPLE= {179, 158, 181};// #B39EB5

// --- PRZEZROCZYSTE (Z obniżonym kanałem Alpha) ---
constexpr ARGB TRANSPARENT  = {0, 0, 0, 0};         // #00000000
constexpr ARGB SEMI_BLACK   = {0, 0, 0, 128};       // #00000080
constexpr ARGB SEMI_WHITE   = {255, 255, 255, 128}; // #FFFFFF80
constexpr ARGB GLASS_RED    = {255, 0, 0, 128};     // #FF000080
constexpr ARGB GLASS_BLUE   = {0, 0, 255, 128};     // #0000FF80
constexpr ARGB GLASS_GREEN  = {0, 255, 0, 128};     // #00FF0080
}
// predefiniowana czcionka
namespace ShiftDownFonts {
#pragma region Sekcja czcionki
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
    constexpr uint64_t font_Space[4] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Exclam[4] = {0x0000100010001000, 0x1000100000001000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_At[4] = {0x0000000038004400, 0x540054005C004000, 0x3800000000000000, 0x0000000000000000};
    constexpr uint64_t font_Hash[4] = {0x0000000024002400, 0x7E0024007E002400, 0x2400000000000000, 0x0000000000000000};
    constexpr uint64_t font_Dollar[4] = {0x0000100038005000, 0x3800140038001000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Percent[4] = {0x0000000062006400, 0x0800100026004600, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Caret[4] = {0x0000100028004400, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Ampers[4] = {0x0000000030004800, 0x3000640048003400, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Star[4] = {0x0000000010005400, 0x3800540010000000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_ParenL[4] = {0x0000100020004000, 0x4000400040004000, 0x4000200010000000, 0x0000000000000000};
    constexpr uint64_t font_ParenR[4] = {0x0000080004000200, 0x0200020002000200, 0x0200040008000000, 0x0000000000000000};
    constexpr uint64_t font_Minus[4] = {0x0000000000000000, 0x0000380000000000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Under[4] = {0x0000000000000000, 0x0000000000000000, 0x000000007E000000, 0x0000000000000000};
    constexpr uint64_t font_Plus[4] = {0x0000000000000000, 0x080008003E000800, 0x0800000000000000, 0x0000000000000000};
    constexpr uint64_t font_Equals[4] = {0x0000000000000000, 0x00003E0000003E00, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_BracketL[4] = {0x0000300020002000, 0x2000200020002000, 0x2000300000000000, 0x0000000000000000};
    constexpr uint64_t font_BracketR[4] = {0x00000C0004000400, 0x0400040004000400, 0x04000C0000000000, 0x0000000000000000};
    constexpr uint64_t font_BraceL[4] = {0x0000180010001000, 0x2000100010001800, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_BraceR[4] = {0x0000180008000800, 0x0400080008001800, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Pipe[4] = {0x0000100010001000, 0x1000100010001000, 0x1000100000000000, 0x0000000000000000};
    constexpr uint64_t font_Backsl[4] = {0x0000400040002000, 0x2000100010000800, 0x0800000000000000, 0x0000000000000000};
    constexpr uint64_t font_Colon[4] = {0x0000000000000000, 0x1800000000001800, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Semic[4] = {0x0000000000000000, 0x1800000000001800, 0x1000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Quote[4] = {0x0000240024000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Apostr[4] = {0x0000180018000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Less[4] = {0x0000000000000800, 0x1000200010000800, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Greater[4] = {0x0000000000002000, 0x1000080010002000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Comma[4] = {0x0000000000000000, 0x0000000000000000, 0x0000180010000000, 0x0000000000000000};
    constexpr uint64_t font_Dot[4] = {0x0000000000000000, 0x0000000000000000, 0x0000180000000000, 0x0000000000000000};
    constexpr uint64_t font_Slash[4] = {0x0000020004000400, 0x0800080010001000, 0x2000200040000000, 0x0000000000000000};
    constexpr uint64_t font_Question[4] = {0x0000380044000400, 0x0800100000001000, 0x0000000000000000, 0x0000000000000000};
    constexpr uint64_t font_Tilde[4] = {0x0000000034005800, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};

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
    constexpr uint64_t font_Z_kres[4] = {0x080010007C000400, 0x0800100020004000, 0x40007C0000000000, 0x0000000000000000};
    constexpr uint64_t font_Z_krop[4] = {0x100000007C000400, 0x0800100020004000, 0x40007C0000000000, 0x0000000000000000};

    // --- POLSKIE ZNAKI (Małe) ---
    constexpr uint64_t font_a_pl[4] = {0x0000000000000000, 0x380004003C004400, 0x44003C0008001000, 0x0000000000000000};
    constexpr uint64_t font_c_pl[4] = {0x0800100000000000, 0x3800440040004000, 0x4400380000000000, 0x0000000000000000};
    constexpr uint64_t font_e_pl[4] = {0x0000000000000000, 0x380044007C004000, 0x4400380008001000, 0x0000000000000000};
    constexpr uint64_t font_l_pl[4] = {0x0000000030001000, 0x1000100070001000, 0x10000C0000000000, 0x0000000000000000};
    constexpr uint64_t font_n_pl[4] = {0x0800100000000000, 0x5800640044004400, 0x4400440000000000, 0x0000000000000000};
    constexpr uint64_t font_o_pl[4] = {0x0800100000000000, 0x3800440044004400, 0x4400380000000000, 0x0000000000000000};
    constexpr uint64_t font_s_pl[4] = {0x0800100000000000, 0x3C00400038000400, 0x4400380000000000, 0x0000000000000000};
    constexpr uint64_t font_z_kres[4] = {0x0800100000000000, 0x7C00080010002000, 0x40007C0000000000, 0x0000000000000000};
    constexpr uint64_t font_z_krop[4] = {0x1000000000000000, 0x7C00080010002000, 0x40007C0000000000, 0x0000000000000000};
#pragma endregion
}
// właściwa biblioteka
namespace ShiftDownFunctions {

namespace ShiftDownFunctionsColorThemes
{
    // ==========================================================
    // ZMIENNE GLOBALNE (Od razu z domyślnym motywem "Original")
    // ==========================================================
    inline uint32_t global_theme_BackGround_color  = 0xFF06030B;
    inline uint32_t global_theme_Line_color        = 0xFF0DE6F6;
    inline uint32_t global_theme_Axis_color        = 0xFF354259;
    inline uint32_t global_theme_Grid_color        = 0xFF171D2B;
    inline uint32_t global_theme_SubSegments_color = 0xFF090A14;
    inline uint32_t global_theme_Font_color        = 0xFFB4C5D6;

    // ==========================================================
    // 0. ORIGINAL
    // ==========================================================

    inline void ShiftDownOriginal_IPS() {
        // Zoptymalizowane pod matryce IPS / OLED
        global_theme_BackGround_color  = 0xFF06030B;
        global_theme_Line_color        = 0xFF0DE6F6;
        global_theme_Axis_color        = 0xFF354259;
        global_theme_Grid_color        = 0xFF171D2B;
        global_theme_SubSegments_color = 0xFF090A14;
        global_theme_Font_color        = 0xFFB4C5D6;
    }
    inline void ShiftDownOriginal_VA() {
        // --- ORIGINAL ---
        // Zoptymalizowane pod matryce VA
        global_theme_BackGround_color  = 0xFF06030B;
        global_theme_Line_color        = 0xFF0DE6F6;
        global_theme_Axis_color        = 0xFF354259;
        global_theme_Grid_color        = 0xFF222B3D;
        global_theme_SubSegments_color = 0xFF0F121A;
        global_theme_Font_color        = 0xFFB4C5D6;
    }

    inline void ShiftDownOriginalGreen_IPS() {
        // --- ORIGINAL GREEN ---
        // Zoptymalizowane pod matryce IPS / OLED
        global_theme_BackGround_color  = 0xFF06030B;
        global_theme_Line_color        = 0xFF39FF14;
        global_theme_Axis_color        = 0xFF405246;
        global_theme_Grid_color        = 0xFF1D2620;
        global_theme_SubSegments_color = 0xFF080A0C;
        global_theme_Font_color        = 0xFFB4CDB9;
    }
    inline void ShiftDownOriginalGreen_VA() {
        // --- ORIGINAL GREEN ---
        // Zoptymalizowane pod matryce VA
        global_theme_BackGround_color  = 0xFF06030B;
        global_theme_Line_color        = 0xFF39FF14;
        global_theme_Axis_color        = 0xFF405246;
        global_theme_Grid_color        = 0xFF28362D;
        global_theme_SubSegments_color = 0xFF0E1411;
        global_theme_Font_color        = 0xFFB4CDB9;
    }

    // ==========================================================
    // 1. KLASYKI I UŻYTECZNE
    // ==========================================================

    inline void ShiftDownLight() {
        // "Soft Pastel / Textbook"
        global_theme_BackGround_color  = 0xFFF0EBE1;
        global_theme_Line_color        = 0xFF5A7684;
        global_theme_Axis_color        = 0xFF8A857F;
        global_theme_Grid_color        = 0xFFC9C3BB;
        global_theme_SubSegments_color = 0xFFDDD7CF;
        global_theme_Font_color        = 0xFF4A4A4A;
    }

    inline void ShiftDownDark() {
        // "One Dark Pro"
        global_theme_BackGround_color  = 0xFF282C34;
        global_theme_Line_color        = 0xFFC678DD;
        global_theme_Axis_color        = 0xFFABB2BF;
        global_theme_Grid_color        = 0xFF3E4451;
        global_theme_SubSegments_color = 0xFF2D313A;
        global_theme_Font_color        = 0xFFE5C07B;
    }

    // ==========================================================
    // 2. DATA SCIENCE & DESIGN
    // ==========================================================

    inline void ShiftDownMinimal() {
        // "Soft E-Ink"
        global_theme_BackGround_color  = 0xFFD2D4D6;
        global_theme_Line_color        = 0xFF606468;
        global_theme_Axis_color        = 0xFF888D92;
        global_theme_Grid_color        = 0xFFB8BCC0;
        global_theme_SubSegments_color = 0xFFC5C8CB;
        global_theme_Font_color        = 0xFF404448;
    }

    inline void ShiftDownSolarizedDark() {
        // "Solarized"
        global_theme_BackGround_color  = 0xFF002B36;
        global_theme_Line_color        = 0xFF2AA198;
        global_theme_Axis_color        = 0xFF839496;
        global_theme_Grid_color        = 0xFF073642;
        global_theme_SubSegments_color = 0xFF03303B;
        global_theme_Font_color        = 0xFFB58900;
    }

    // ==========================================================
    // 3. ESTETYCZNE / KLIMATYCZNE
    // ==========================================================

    inline void ShiftDownGentleman() {
        // Klimat premium
        global_theme_BackGround_color  = 0xFF2A2825;
        global_theme_Line_color        = 0xFFD4AF37;
        global_theme_Axis_color        = 0xFF8C7B65;
        global_theme_Grid_color        = 0xFF3E3A35;
        global_theme_SubSegments_color = 0xFF302E2B;
        global_theme_Font_color        = 0xFFF5E6CC;
    }

    inline void ShiftDownBrutalist() {
        // maksymalny techniczny kontrast
        global_theme_BackGround_color  = 0xFF111111;
        global_theme_Line_color        = 0xFFFF3300;
        global_theme_Axis_color        = 0xFFFFFFFF;
        global_theme_Grid_color        = 0xFF333333;
        global_theme_SubSegments_color = 0xFF191919;
        global_theme_Font_color        = 0xFFFFFF00;
    }

    inline void ShiftDownCyberpunk() {
        // Night City vibes
        global_theme_BackGround_color  = 0xFF0D0221;
        global_theme_Line_color        = 0xFFFCE205;
        global_theme_Axis_color        = 0xFFFF003C;
        global_theme_Grid_color        = 0xFF261447;
        global_theme_SubSegments_color = 0xFF150A2A;
        global_theme_Font_color        = 0xFF00FFFF;
    }

    inline void ShiftDownBareMetal() {
        // Kolory płytki PCB
        global_theme_BackGround_color  = 0xFF004225;
        global_theme_Line_color        = 0xFFFFD700;
        global_theme_Axis_color        = 0xFFC0C0C0;
        global_theme_Grid_color        = 0xFF002B18;
        global_theme_SubSegments_color = 0xFF003A20;
        global_theme_Font_color        = 0xFFFFFFFF;
    }

    // ==========================================================
    // 4. POJEBANE / DO TESTÓW
    // ==========================================================

    inline void ShiftDownHelpMe() {
        // "Missing Source Texture"
        global_theme_BackGround_color  = 0xFFFF00FF;
        global_theme_Line_color        = 0xFF00FF00;
        global_theme_Axis_color        = 0xFF00FFFF;
        global_theme_Grid_color        = 0xFFFFFF00;
        global_theme_SubSegments_color = 0xFFFF1AFF;
        global_theme_Font_color        = 0xFFFF0000;
    }

    // ==========================================================
    // 5. TRYB RNG ( LOSOWE KOLORY )
    // ==========================================================
    inline uint32_t generate_random_argb() {
        return 0xFF000000 | ((std::rand() % 256) << 16) | ((std::rand() % 256) << 8) | (std::rand() % 256);
    }

    inline void ShiftDownRNG() {
        global_theme_BackGround_color  = generate_random_argb();
        global_theme_Line_color        = generate_random_argb();
        global_theme_Axis_color        = generate_random_argb();
        global_theme_Grid_color        = generate_random_argb();
        global_theme_SubSegments_color = generate_random_argb();
        global_theme_Font_color        = generate_random_argb();
    }
}

    // stuff I wanted to hide from the UI, not needed by the user to enjoy the library, only used by the engine itself
    namespace z_detail_no_need_to_think_about_it
    {
        // saving texture / picture / image, .png or .bmp, png is made via os commands, and is much slower, read instructions
        static void save_texture_to_file(const uint32_t* texture, uint32_t width, uint32_t height, const char* file_path) {
        if (file_path == nullptr || file_path[0] == '\0') {
            file_path = "wykres.bmp";
        }

        std::filesystem::path fs_path(file_path);

        if (fs_path.has_parent_path()) {
            std::error_code ec;
            std::filesystem::create_directories(fs_path.parent_path(), ec);
        }

        bool wants_png = (fs_path.extension() == ".png");

        std::string bmp_path = fs_path.string();
        std::string png_path = fs_path.string();

        if (wants_png) {
            bmp_path = fs_path.replace_extension(".bmp").string();
        }

        uint32_t image_size = width * height * 4;
        uint32_t file_size = 54 + image_size;

        uint8_t header[54] = {0};
        header[0] = 'B'; header[1] = 'M';
        header[2] = static_cast<uint8_t>(file_size); header[3] = static_cast<uint8_t>(file_size >> 8);
        header[4] = static_cast<uint8_t>(file_size >> 16); header[5] = static_cast<uint8_t>(file_size >> 24);
        header[10] = 54; header[14] = 40;
        header[18] = static_cast<uint8_t>(width); header[19] = static_cast<uint8_t>(width >> 8);
        header[20] = static_cast<uint8_t>(width >> 16); header[21] = static_cast<uint8_t>(width >> 24);

        int32_t neg_height = -static_cast<int32_t>(height);
        header[22] = static_cast<uint8_t>(neg_height); header[23] = static_cast<uint8_t>(neg_height >> 8);
        header[24] = static_cast<uint8_t>(neg_height >> 16); header[25] = static_cast<uint8_t>(neg_height >> 24);

        header[26] = 1; header[28] = 32;
        header[34] = static_cast<uint8_t>(image_size); header[35] = static_cast<uint8_t>(image_size >> 8);
        header[36] = static_cast<uint8_t>(image_size >> 16); header[37] = static_cast<uint8_t>(image_size >> 24);

        if (FILE* f = fopen(bmp_path.c_str(), "wb")) {
            fwrite(header, 1, 54, f);
            fwrite(texture, 1, image_size, f);
            fclose(f);

#ifdef __linux__
            chmod(bmp_path.c_str(), 0666);
#endif

            if (wants_png) {
#ifdef __linux__
                std::string linux_cmd =
                    "if command -v magick >/dev/null 2>&1; then magick \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "elif command -v convert >/dev/null 2>&1; then convert \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "elif command -v ffmpeg >/dev/null 2>&1; then ffmpeg -y -i \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; fi";
                int unused_linux = std::system(linux_cmd.c_str());
#elif defined(_WIN32) || defined(__NT__)
                std::string win_cmd =
                    "powershell -Command \"Add-Type -AssemblyName System.Drawing; if (Test-Path '" + bmp_path + "') { $img = [System.Drawing.Image]::FromFile('" + bmp_path + "'); $img.Save('" + png_path + "', [System.Drawing.Imaging.ImageFormat]::Png); $img.Dispose(); Remove-Item '" + bmp_path + "'; }\" >nul 2>&1";
                int unused_win = std::system(win_cmd.c_str());
#endif
            }
        }
    }

        // slightly modified Bresenham's algorithm to draw on 2D textures, quiet handy
        static void draw_line(uint32_t* texture, uint32_t texture_width, uint32_t texture_height, uint32_t Ax, uint32_t Ay, uint32_t Bx, uint32_t By, uint32_t color, uint32_t thickness) {

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

                for (int ty = -half_thick; ty <= half_thick; ty++) {
                    for (int tx = -half_thick; tx <= half_thick; tx++) {
                        int draw_x = x0 + tx;
                        int draw_y = y0 + ty;
                        if (draw_x >= 0 && draw_x < width && draw_y >= 0 && draw_y < height) {
                            texture[draw_y * width + draw_x] = color;
                        }
                    }
                }
                if (x0 == x1 && y0 == y1)
                    break;
                e2 = 2 * err;

                if (e2 >= dy) {
                    err += dy;
                    x0 += sx;
                }
                if (e2 <= dx) {
                    err += dx;
                    y0 += sy;
                }
            }
        }

        // float to char, returns char* with set precision, also works with negative numbers,
        static char* float_to_char(float number, char* buffer, uint32_t precision = 3) {

        for (int k = 0; k < 32; k++)
            buffer[k] = '\0';

        if (number == 0.0f) {
            buffer[0] = '0';
            buffer[1] = '.';
            for (uint32_t p = 0; p < precision; p++)
                buffer[2 + p] = '0';

            return buffer;
        }

        uint32_t float_bits = *reinterpret_cast<uint32_t*>(&number);
        bool is_negative = (float_bits >> 31) != 0;
        if (is_negative)
            number = -number;

        uint64_t multiplier = 1;
        for (uint32_t p = 0; p < precision; p++)
            multiplier *= 10;

        auto total_val = static_cast<uint64_t>(number * static_cast<float>(multiplier) + 0.5f);
        if (total_val == 0) is_negative = false;

        uint64_t int_part = total_val / multiplier;
        uint64_t frac_int = total_val % multiplier;

        uint32_t idx = 0;
        if (is_negative) {
            buffer[idx] = '-';
            idx++;
        }

        uint64_t temp_int = int_part;
        uint32_t int_len = 0;
        if (temp_int == 0)
            int_len = 1;
        else
            while (temp_int != 0) {
                int_len++;
                temp_int /= 10;
            }

        uint32_t int_end_idx = idx + int_len - 1;
        temp_int = int_part;
        if (temp_int == 0)
            buffer[int_end_idx] = '0';
        else {
            for (uint32_t j = 0; j < int_len; j++) {
                buffer[int_end_idx - j] = 0b00110000 | (temp_int % 10);
                temp_int /= 10;
            }
        }
        idx += int_len;

            buffer[idx] = '.';
            idx++;
            uint32_t frac_end_idx = idx + precision - 1;
            for (uint32_t j = 0; j < precision; j++) {
                buffer[frac_end_idx - j] = 0b00110000 | (frac_int % 10);
                frac_int /= 10;
            }
            idx += precision;



        buffer[idx] = '\0';
        return buffer;
    }

        //converts number into a something more human friendly
        static void float_to_engineering_7chars(float value, char* buffer) {
                if (value == 0.0f || (value > -1e-10f && value < 1e-10f)) {
                    snprintf(buffer, 8, " 0.000 ");
                    return;
                }

                char sign = value < 0 ? '-' : ' ';
                value = fabsf(value);

                char prefix = ' ';
                if (value >= 1e12f)      { value /= 1e12f; prefix = 'T'; }
                else if (value >= 1e9f)  { value /= 1e9f;  prefix = 'G'; }
                else if (value >= 1e6f)  { value /= 1e6f;  prefix = 'M'; }
                else if (value >= 1e3f)  { value /= 1e3f;  prefix = 'k'; }
                else if (value >= 1.0f)  { prefix = ' '; }
                else if (value >= 1e-3f) { value *= 1e3f;  prefix = 'm'; }
                else if (value >= 1e-6f) { value *= 1e6f;  prefix = 'u'; }
                else if (value >= 1e-9f) { value *= 1e9f;  prefix = 'n'; }

                // Goły, czysty float wjeżdża prosto do bufora - snprintf sam go wygładzi!
                if (value >= 100.0f) {
                    snprintf(buffer, 8, "%c%5.1f%c", sign, value, prefix);
                } else if (value >= 10.0f) {
                    snprintf(buffer, 8, "%c%5.2f%c", sign, value, prefix);
                } else {
                    snprintf(buffer, 8, "%c%5.3f%c", sign, value, prefix);
                }
            }

        // a very, very slimmed down version of the TextBox class form my graphic engine, made thin enough to work with the library, converts given text to a texture with it, using my own font
        struct TextBox {
        uint32_t font_width = 8;
        uint32_t font_height = 16;

        uint32_t current_line{0};
        uint32_t current_letter{0};

        uint32_t texture_width = 0;
        uint32_t texture_height = 0;
        uint32_t* texture = nullptr;

    TextBox(uint32_t max_chars_per_line = 1, uint32_t max_lines = 1, uint32_t bg_color = 0x00000000) {

        texture_width = max_chars_per_line * font_width;
        texture_height = max_lines * font_height;

        texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * texture_width * texture_height, 32));
        for (uint64_t i = 0; i < texture_width * texture_height; i++) {
            texture[i] = bg_color;
        }
    }
    ~TextBox() { _mm_free(texture); }
    void add_text(const char* text, uint32_t font_color) {
        uint64_t i = 0;
        while (text[i] != '\0') {
            const uint64_t* current_font_data = nullptr;
            auto letter = static_cast<unsigned char>(text[i]);

            if (letter < 0x80) {
                switch (letter) {
                case 'A':
                    current_font_data = ShiftDownFonts::font_A;
                    break;
                case 'B':
                    current_font_data = ShiftDownFonts::font_B;
                    break;
                case 'C':
                    current_font_data = ShiftDownFonts::font_C;
                    break;
                case 'D':
                    current_font_data = ShiftDownFonts::font_D;
                    break;
                case 'E':
                    current_font_data = ShiftDownFonts::font_E;
                    break;
                case 'F':
                    current_font_data = ShiftDownFonts::font_F;
                    break;
                case 'G':
                    current_font_data = ShiftDownFonts::font_G;
                    break;
                case 'H':
                    current_font_data = ShiftDownFonts::font_H;
                    break;
                case 'I':
                    current_font_data = ShiftDownFonts::font_I;
                    break;
                case 'J':
                    current_font_data = ShiftDownFonts::font_J;
                    break;
                case 'K':
                    current_font_data = ShiftDownFonts::font_K;
                    break;
                case 'L':
                    current_font_data = ShiftDownFonts::font_L;
                    break;
                case 'M':
                    current_font_data = ShiftDownFonts::font_M;
                    break;
                case 'N':
                    current_font_data = ShiftDownFonts::font_N;
                    break;
                case 'O':
                    current_font_data = ShiftDownFonts::font_O;
                    break;
                case 'P':
                    current_font_data = ShiftDownFonts::font_P;
                    break;
                case 'Q':
                    current_font_data = ShiftDownFonts::font_Q;
                    break;
                case 'R':
                    current_font_data = ShiftDownFonts::font_R;
                    break;
                case 'S':
                    current_font_data = ShiftDownFonts::font_S;
                    break;
                case 'T':
                    current_font_data = ShiftDownFonts::font_T;
                    break;
                case 'U':
                    current_font_data = ShiftDownFonts::font_U;
                    break;
                case 'V':
                    current_font_data = ShiftDownFonts::font_V;
                    break;
                case 'W':
                    current_font_data = ShiftDownFonts::font_W;
                    break;
                case 'X':
                    current_font_data = ShiftDownFonts::font_X;
                    break;
                case 'Y':
                    current_font_data = ShiftDownFonts::font_Y;
                    break;
                case 'Z':
                    current_font_data = ShiftDownFonts::font_Z;
                    break;
                case 'a':
                    current_font_data = ShiftDownFonts::font_a;
                    break;
                case 'b':
                    current_font_data = ShiftDownFonts::font_b;
                    break;
                case 'c':
                    current_font_data = ShiftDownFonts::font_c;
                    break;
                case 'd':
                    current_font_data = ShiftDownFonts::font_d;
                    break;
                case 'e':
                    current_font_data = ShiftDownFonts::font_e;
                    break;
                case 'f':
                    current_font_data = ShiftDownFonts::font_f;
                    break;
                case 'g':
                    current_font_data = ShiftDownFonts::font_g;
                    break;
                case 'h':
                    current_font_data = ShiftDownFonts::font_h;
                    break;
                case 'i':
                    current_font_data = ShiftDownFonts::font_i;
                    break;
                case 'j':
                    current_font_data = ShiftDownFonts::font_j;
                    break;
                case 'k':
                    current_font_data = ShiftDownFonts::font_k;
                    break;
                case 'l':
                    current_font_data = ShiftDownFonts::font_l;
                    break;
                case 'm':
                    current_font_data = ShiftDownFonts::font_m;
                    break;
                case 'n':
                    current_font_data = ShiftDownFonts::font_n;
                    break;
                case 'o':
                    current_font_data = ShiftDownFonts::font_o;
                    break;
                case 'p':
                    current_font_data = ShiftDownFonts::font_p;
                    break;
                case 'q':
                    current_font_data = ShiftDownFonts::font_q;
                    break;
                case 'r':
                    current_font_data = ShiftDownFonts::font_r;
                    break;
                case 's':
                    current_font_data = ShiftDownFonts::font_s;
                    break;
                case 't':
                    current_font_data = ShiftDownFonts::font_t;
                    break;
                case 'u':
                    current_font_data = ShiftDownFonts::font_u;
                    break;
                case 'v':
                    current_font_data = ShiftDownFonts::font_v;
                    break;
                case 'w':
                    current_font_data = ShiftDownFonts::font_w;
                    break;
                case 'x':
                    current_font_data = ShiftDownFonts::font_x;
                    break;
                case 'y':
                    current_font_data = ShiftDownFonts::font_y;
                    break;
                case 'z':
                    current_font_data = ShiftDownFonts::font_z;
                    break;
                case '0':
                    current_font_data = ShiftDownFonts::font_0;
                    break;
                case '1':
                    current_font_data = ShiftDownFonts::font_1;
                    break;
                case '2':
                    current_font_data = ShiftDownFonts::font_2;
                    break;
                case '3':
                    current_font_data = ShiftDownFonts::font_3;
                    break;
                case '4':
                    current_font_data = ShiftDownFonts::font_4;
                    break;
                case '5':
                    current_font_data = ShiftDownFonts::font_5;
                    break;
                case '6':
                    current_font_data = ShiftDownFonts::font_6;
                    break;
                case '7':
                    current_font_data = ShiftDownFonts::font_7;
                    break;
                case '8':
                    current_font_data = ShiftDownFonts::font_8;
                    break;
                case '9':
                    current_font_data = ShiftDownFonts::font_9;
                    break;
                case ' ':
                    current_font_data = ShiftDownFonts::font_Space;
                    break;
                case '!':
                    current_font_data = ShiftDownFonts::font_Exclam;
                    break;
                case '@':
                    current_font_data = ShiftDownFonts::font_At;
                    break;
                case '#':
                    current_font_data = ShiftDownFonts::font_Hash;
                    break;
                case '$':
                    current_font_data = ShiftDownFonts::font_Dollar;
                    break;
                case '%':
                    current_font_data = ShiftDownFonts::font_Percent;
                    break;
                case '^':
                    current_font_data = ShiftDownFonts::font_Caret;
                    break;
                case '&':
                    current_font_data = ShiftDownFonts::font_Ampers;
                    break;
                case '*':
                    current_font_data = ShiftDownFonts::font_Star;
                    break;
                case '(':
                    current_font_data = ShiftDownFonts::font_ParenL;
                    break;
                case ')':
                    current_font_data = ShiftDownFonts::font_ParenR;
                    break;
                case '-':
                    current_font_data = ShiftDownFonts::font_Minus;
                    break;
                case '_':
                    current_font_data = ShiftDownFonts::font_Under;
                    break;
                case '+':
                    current_font_data = ShiftDownFonts::font_Plus;
                    break;
                case '=':
                    current_font_data = ShiftDownFonts::font_Equals;
                    break;
                case '[':
                    current_font_data = ShiftDownFonts::font_BracketL;
                    break;
                case ']':
                    current_font_data = ShiftDownFonts::font_BracketR;
                    break;
                case '{':
                    current_font_data = ShiftDownFonts::font_BraceL;
                    break;
                case '}':
                    current_font_data = ShiftDownFonts::font_BraceR;
                    break;
                case '|':
                    current_font_data = ShiftDownFonts::font_Pipe;
                    break;
                case '\\':
                    current_font_data = ShiftDownFonts::font_Backsl;
                    break;
                case ':':
                    current_font_data = ShiftDownFonts::font_Colon;
                    break;
                case ';':
                    current_font_data = ShiftDownFonts::font_Semic;
                    break;
                case '"':
                    current_font_data = ShiftDownFonts::font_Quote;
                    break;
                case '\'':
                    current_font_data = ShiftDownFonts::font_Apostr;
                    break;
                case '<':
                    current_font_data = ShiftDownFonts::font_Less;
                    break;
                case '>':
                    current_font_data = ShiftDownFonts::font_Greater;
                    break;
                case ',':
                    current_font_data = ShiftDownFonts::font_Comma;
                    break;
                case '.':
                    current_font_data = ShiftDownFonts::font_Dot;
                    break;
                case '/':
                    current_font_data = ShiftDownFonts::font_Slash;
                    break;
                case '?':
                    current_font_data = ShiftDownFonts::font_Question;
                    break;
                case '~':
                    current_font_data = ShiftDownFonts::font_Tilde;
                    break;
                default:
                    current_font_data = ShiftDownFonts::font_Question;
                    break;
                }
                i++;
            }
            else if ((letter & 0xE0) == 0xC0) {
                auto letter_second_part = static_cast<unsigned char>(text[i + 1]);
                if (letter_second_part == '\0')
                    break;

                switch ((letter << 8) | letter_second_part) {
                case 0xC484:
                    current_font_data = ShiftDownFonts::font_A_pl;
                    break;
                case 0xC486:
                    current_font_data = ShiftDownFonts::font_C_pl;
                    break;
                case 0xC498:
                    current_font_data = ShiftDownFonts::font_E_pl;
                    break;
                case 0xC581:
                    current_font_data = ShiftDownFonts::font_L_pl;
                    break;
                case 0xC583:
                    current_font_data = ShiftDownFonts::font_N_pl;
                    break;
                case 0xC393:
                    current_font_data = ShiftDownFonts::font_O_pl;
                    break;
                case 0xC59A:
                    current_font_data = ShiftDownFonts::font_S_pl;
                    break;
                case 0xC5B9:
                    current_font_data = ShiftDownFonts::font_Z_kres;
                    break;
                case 0xC5BB:
                    current_font_data = ShiftDownFonts::font_Z_krop;
                    break;
                case 0xC485:
                    current_font_data = ShiftDownFonts::font_a_pl;
                    break;
                case 0xC487:
                    current_font_data = ShiftDownFonts::font_c_pl;
                    break;
                case 0xC499:
                    current_font_data = ShiftDownFonts::font_e_pl;
                    break;
                case 0xC582:
                    current_font_data = ShiftDownFonts::font_l_pl;
                    break;
                case 0xC584:
                    current_font_data = ShiftDownFonts::font_n_pl;
                    break;
                case 0xC3B3:
                    current_font_data = ShiftDownFonts::font_o_pl;
                    break;
                case 0xC59B:
                    current_font_data = ShiftDownFonts::font_s_pl;
                    break;
                case 0xC5BA:
                    current_font_data = ShiftDownFonts::font_z_kres;
                    break;
                case 0xC5BC:
                    current_font_data = ShiftDownFonts::font_z_krop;
                    break;
                default:
                    current_font_data = ShiftDownFonts::font_Question;
                    break;
                }
                i += 2;
            }
            else {
                current_font_data = ShiftDownFonts::font_Question;
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
                            texture[(current_line + temp_y) * texture_width + (current_letter + temp_x)] = font_color;
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
                if (current_letter >= texture_width) {
                    current_letter = 0;
                    current_line += 16;
                    if (current_line >= texture_height)
                        return;
                }
            }
            else {
                current_letter += 8;
                if (current_letter >= texture_width) {
                    current_letter = 0;
                    current_line += 16;
                    if (current_line >= texture_height)
                        return;
                }
            }
        }
    };
    [[nodiscard]] const uint32_t* return_texture() const { return texture; }
};
    }

#pragma region Instrukcja obsługi
/*
------------------------------------------------------ KROK 1. Tworzymy funkcje cpp dla wzoru matematycznego naszej funkcji --------------------------------------------------------------------------------------------------------

        float nazwa_funkcji_wzoru ([stała referencja obiektu naszej funkcji], [aktualna iteracja 'n' jako uint64_t]], [wartość naszego t dla f(t) albo x dla f(x) jako float]) {
            ---ciało funkcji---
            zwracamy po prostu nasz wzór matematyczny, wykorzystując wskaźnik na obiekt funkcji:
            return [tutaj podajemy wzór]
        }
                                                                                ---- PRZYKŁAD ----

        float sinus_formula (const Function& function_object, uint64_t n, float t) {
            // n jest zazwyczaj niepotrzebne dla prostych funkcji, przyda się w tych bardziej złożonych lub gdy potrzebny jest index naszego t zamiast wartości
            return function_object.A * sinf(2.f * M_PIf * function_object.f * t + function_object.PHI); //M_PIf to jest wartość liczby PI jako float
        }

------------------------------------------------------ KROK 2. Tworzymy obiekt Function w main albo gdzie tam się chce --------------------------------------------------------------------------------------------------------

        Function nazwa_naszej_funkcji([wartość Tc], [wartość fs], [wartość f], [wartość PHI], [wartość A], [nazwa naszej funkcji wzoru]);

                                                                                ---- PRZYKŁAD ----
                                                        ---- różne obiekty mogą dostawać ten sam wzór, nie jest to problemem ----

        Function funkcja_sinus(2.f, 8000.f, 4.f, 0.f, 1.f, sinus_formula);
        Function funkcja_sinus_saw(2.f, 8000.f, 4.f, 0.f, 1.f, sinus_formula);
        Function funkcja_sinus_rec(2.f, 8000.f, 4.f, 0.f, 1.f, sinus_formula);
        Function funkcja_sinus_tri(2.f, 8000.f, 4.f, 0.f, 1.f, sinus_formula);

------------------------------------------------------ KROK 3. Modulujemy, liczmy DFT albo Rysujemy przygotowane funkcje --------------------------------------------------------------------------------------------------------
                                                                               ---- Modulacja ----
                                                      --- Modulowanie funkcji tworzy nowy obiekt i zwraca wskaźnik na niego ---
                                         --- dzieki temu nie trzeba się bać że modulacja zmieni wartości oryginalnej funkcji przed rysowaniem ---

        Function* [nazwa_funkcji_zmodulowanej] = [nazwa wybranej funkcji modulującej] ([adres pamieci obiektu funkcji do modulacji], ["A" funkcji nośnej], ["fs" funkcji nośnej], ["PHI" funkcji nośnej]);

        // do wyboru mamy 4 kategorie modulacji:
            -sinusowa
            -piłokształtna
            -prostokątna
            -trójkątna

        // oraz 3 rodzaje każdej z nich:
            AM - po amplitudzie
            FM - po częstotliwości
            PM - po PHI

                                                                                ---- PRZYKŁAD ----

        Function* sin_modulacja_AM_sin = modulate_AM_sin(&funkcja_sinus, 5.0f, 120.0f, 0.f);
        Function* sin_modulacja_PM_saw = modulate_PM_saw(&funkcja_sinus, 5.0f, 120.0f, 0.f);
        Function* sin_modulacja_FM_rec = modulate_FM_rec(&funkcja_sinus, 5.0f, 120.0f, 0.f);
        Function* sin_modulacja_FM_tri = modulate_FM_tri(&funkcja_sinus, 5.0f, 120.0f, 0.f);

                                                                                   ---- DFT ----
                                                                            --- Tworzymy obiekt DFT ---
        DFT [nazwa_obiektu_dft]([obiekt funkcji do narysowania]);

                                                                                ---- PRZYKŁAD ----

        DFT widmo_sinusa(&funkcja_sinus);

                                                                                ---- Rysowanie ----
                                                                            --- Tworzymy obiekt Graph ---

        Graph [nazwa_obiektu_grafu]([obiekt funkcji do narysowania], [tekst nazwy grafu], [tekst osi X], [tekst osi Y]);

                                                                                ---- PRZYKŁAD ----

        Graph graf_sinus(&funkcja_sinus, "funkcja sinus", "Czas [s]", "Amplituda [A]");
        Graph graf_sin_modulacja_AM_sin(sin_modulacja_AM_sin, "Modulacja sin, AM", "Czas [s]", "Amplituda [A]");
        Graph graf_sin_modulacja_PM_saw(sin_modulacja_PM_saw, "Modulacja saw, PM", "Czas [s]", "Amplituda [A]");
        Graph graf_sin_modulacja_FM_rec(sin_modulacja_FM_rec, "Modulacja rec, FM", "Czas [s]", "Amplituda [A]");
        Graph graf_sin_modulacja_FM_tri(sin_modulacja_FM_tri, "Modulacja tri, FM", "Czas [s]", "Amplituda [A]");

                                                                            --- Zasada ta sama dla DFT ---

        Graph graf_widma_sinusa(&widmo_sinusa, "Widmo funkcji_sinus", "Częstotliwość [Hz]", "Amplituda [A]");

---------------------------------------------------------------------------------- KROK 4. DELETE --------------------------------------------------------------------------------------------------------

                                                    !!! Pamiętamy o delete dla każdego obiektu stworzonego przez funkcje modulujące !!!

                                                                            delete sin_modulacja_FM_tri;
                                                                            delete sin_modulacja_FM_rec;
                                                                            delete sin_modulacja_PM_saw;
                                                                            delete sin_modulacja_AM_sin;

------------------------------------------------------------------------------ PRZYKŁADOWY KOD TESTOWY --------------------------------------------------------------------------------------------------------

#include "ShiftDownFunctions.hpp"

    // albo używamy:
    // using namespace ShiftDownFunctions;
    // albo trzeba pamiętać o pisaniu ShiftDownFunctions::, przed wszystkim co pochodzi z tego kontenera


//KROK 1 - tworzenie wzoru funkcji
float sinus_formula (const ShiftDownFunctions::Function& function_object, uint64_t n, float t) {
    return function_object.A * sinf(2.f * M_PIf * function_object.f * t + function_object.PHI); //M_PIf to jest wartość liczby PI jako float
}

int main() {

    //KROK 2 - tworzenie obiektów funkcji
    ShiftDownFunctions::Function funkcja_sinus(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);
    ShiftDownFunctions::Function funkcja_sinus_saw(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);
    ShiftDownFunctions::Function funkcja_sinus_rec(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);
    ShiftDownFunctions::Function funkcja_sinus_tri(1.0f, 44100.f, 2.0f, 0.f, 1.0f, sinus_formula);

    //KROK 3 - modulacja funkcji
    ShiftDownFunctions::Function* sin_modulacja_AM_sin = modulate_AM_sin(&funkcja_sinus, 2.0f, 40.0f, 0.f);
    ShiftDownFunctions::Function* sin_modulacja_PM_saw = modulate_PM_saw(&funkcja_sinus, 1.0f, 30.0f, 0.f);
    ShiftDownFunctions::Function* sin_modulacja_FM_rec = modulate_FM_rec(&funkcja_sinus, 1.0f, 50.0f, 0.f);
    ShiftDownFunctions::Function* sin_modulacja_FM_tri = modulate_FM_tri(&funkcja_sinus, 1.0f, 50.0f, 0.f);

    //KROK 3 - tworzenie dft

    DFT widmo_sinusa(&funkcja_sinus);

    //KROK 3 - rysowanie funkcji
    ShiftDownFunctions::Graph graf_sinus(&funkcja_sinus, "funkcja sinus", "Czas [s]", "Amplituda [A]");
    ShiftDownFunctions::Graph graf_widma_sinusa(&widmo_sinusa, "Widmo funkcji_sinus", "Częstotliwość [Hz]", "Amplituda [A]"); // WIDMO
    ShiftDownFunctions::Graph graf_sin_modulacja_AM_sin(sin_modulacja_AM_sin, "Modulacja sin, AM", "Czas [s]", "Amplituda [A]");
    ShiftDownFunctions::Graph graf_sin_modulacja_PM_saw(sin_modulacja_PM_saw, "Modulacja saw, PM", "Czas [s]", "Amplituda [A]");
    ShiftDownFunctions::Graph graf_sin_modulacja_FM_rec(sin_modulacja_FM_rec, "Modulacja rec, FM", "Czas [s]", "Amplituda [A]");
    ShiftDownFunctions::Graph graf_sin_modulacja_FM_tri(sin_modulacja_FM_tri, "Modulacja tri, FM", "Czas [s]", "Amplituda [A]");


    //rysowanie w innych kolorach tej samej funkcji
    ShiftDownFunctions::Graph graf_sinus_red_theme(&funkcja_sinus, "funkcja sinus RED", "Czas [s]", "Amplituda [A]", Colors::BLOOD_RED, Colors::RED, Colors::PASTEL_RED, Colors::GLASS_RED);
    ShiftDownFunctions::Graph graf_sinus_blue_theme(&funkcja_sinus, "funkcja sinus BLUE", "Czas [s]", "Amplituda [A]", Colors::MIDNIGHT_BLUE, Colors::BLUE, Colors::PASTEL_BLUE, Colors::GLASS_BLUE);
    ShiftDownFunctions::Graph graf_sinus_purple_theme(&funkcja_sinus, "funkcja sinus PURPLE", "Czas [s]", "Amplituda [A]", Colors::DARK_PURPLE, Colors::PINK, Colors::PASTEL_PURPLE, Colors::PURPLE);

    //KROK 4 - usuwanie funkcji
    delete sin_modulacja_FM_tri;
    delete sin_modulacja_FM_rec;
    delete sin_modulacja_PM_saw;
    delete sin_modulacja_AM_sin;



    return 0;
}


------------------------------------------------------------------------------------ !!! UWAGA !!! --------------------------------------------------------------------------------------------------------

        Program automatycznie dokona próby zapisu do PNG i na windows i na linux, jeśli natomiast się nie uda to zachowa zdjęcia we wskazanym folderze jako bmp,
        jeśli lokalizacja folderu nie została wskazana ręcznie, program zapisze to w miejscu gdzie zapisana jest jego binarka. Mogą występować różne fikuśne błędy i bugi, nie wiem nie testowałem
        każdego scenariusza w razie co można starać się naprawić błąd samemu, sekcji Font lepiej nie ruszac tam znajduje się czcionka do którą wkleja się w graf, ma wymiary fizyczne 16 x 16 jakby
        ktoś chciał podmienic natomiast faktyczne litery są 16 x 8, gdzie litera znajduje się z lewej strony tekstury, i taki format musi być zachowany,

        W grafach można modyfikować kolory dla osi, czcionki, tła, wykresu, można podać wartości ARGB jako uint32_t, polecam jako hex w formacie 0xAARRGGBB, albo gotowa sekcja kolorów Colors::[nazwa gotowego koloru jaki tam jest]
        A-alpha, od 0 do 255 czyli dla hex 0 to 0x00 a 255 to 0xFF
        R-czerwony, od 0 do 255 czyli dla hex 0 to 0x00 a 255 to 0xFF
        G-zielony, od 0 do 255 czyli dla hex 0 to 0x00 a 255 to 0xFF
        B-niebieski od 0 do 255 czyli dla hex 0 to 0x00 a 255 to 0xFF
        Przykład: 0xFF7D7D7D lub Colors::GRAY -> daje kolor szary

        Wartości na wykresach, czasem Amplituda jest w zakresie od np 1 do -0.984 albo coś około tego, wynika to niestety z akumulacji błędu zmiennoprzecinkowego (IEEE 754, Metody Numeryczne) pewnie da się to ręcznie zabezpieczyć
        ale nie miałem na to ochoty, więc jeśli komuś przeszkadza można w funkcji rysującej Graph, sekcja values on x i values on y, dodać bramke przed wysłaniem wartości do funkcji z_detail_no_need_to_think_about_it::float_to_char, która wyrówna wartości
        do tego co powinno być, problem nie pojawia się zawsze ale czasem, i jest to specyfika działania tego tworu, ja mówie że to funkcjonalność, może kiedyś sam poprawie w wolnym czasie.

        Windows, a Linux. Domyślnie wszystko jest napisane i przetestowane na Linux Fedora 43, działa bez problemu, Na windows też powinno ale nic nie mogę obiecać, niech się cieszą że w ogole mi się chciało o nich pamiętać
        Zdjęcia są w 8k robione, skalowanie z poziomu systemu nie programu ze względu na dosyć leniwe podejście do statycznie wyliczanych odległości i zależności między elementami wykresu w celu osiągnięcia najbardziej
        satysfakcjonujących rezultatów, gotowe zdjęcie można już skalować do woli. Pliki .png nie są tragiczne w rozmiarach, wahają się w zależności od wyglądu wykresu, .bmp natomiast to sztywne około 130MB, i uwaga
        tutaj mówię o miejscu na dysku, w ramie i procesie tworzenia, przekraczamy te 130MB na funkcje, prosty wzór na obliczenia ile to zajmie (mniej wiecej), sama funkcja w sobie to Tc * fs * rozmiar_float (32bity) * 2,
        bo mamy oś x i y, potem modulacje, dft i inne jako że to osobne obiekty zasada ta sama, renderowanie grafu to zawsze 8K, szerokość * wysokość * rozmiar uint32_t (32bity) co daje około 130MB
        wiec dla funkcji o czasie Tc = 2 sekundy i fs = 32k mamy 2 * 32k * 2 czyli 128k * 32 co daje 4 096 000 bitów czyli około 0.5MB, dla czasu 4s to już 1MB dla, 10s mamy 5MB i tak dalej, wiec przy renderowaniu kilku
        funkcji i zdjęć łatwo pójść w gigabajty pamięci RAM, tylko ostrzegam, i przypominam o magicznych delete albo robieniu każdego wykresu w osobnych funkcjach by obiekty ginęły automatycznie,
        wraz z jej końcem (za wyjątkiem modulacji tam zawsze delete), wycieków pamięci nie widziałem ale gwarancji też nie dam więc warto mieć to gdzieś z tyłu głowy.

        Dla poprawnego działania formatu .png na linux potrzeba zainstalować ImageMagick albo FFmpeg na windows jakimś cudem działa powershell script,

        Komendy dla Linuxa dla działania formatowania do .png:

              --- Fedora 43 i podobne ---
        sudo dnf install ImageMagick ffmpeg

            --- Ubuntu / Debian / Pop!_OS ---
        sudo apt update
        sudo apt install imagemagick ffmpeg

            --- Arch Linux / Manjaro ---
        sudo pacman -S imagemagick ffmpeg


        Całość jest na licencji MIT.
*/
#pragma endregion

#pragma region Sekcja Funkcyjna

    // this is main structure, use it to create function objects to use later on in the engine, almost everything uses it as default input object
    struct Function {
        float Tc{0.f};  // total time / czas całkowity [s]
        float fs{0.f};  // sampling rate / czestotliwosc probkowania [Hz]
        float f{0.f};   // frequency / czestotliwosc [Hz]
        float PHI{0.f}; // initial phase / faza_poczatkowa [rad]
        float A{0.f};   // amplitude / amplituda [A], [V], [dB]
        float Ts{0.f};  // sampling interval(step on x) / krok_czasu [s]
        uint64_t N{0};  // samples count / ilosc probek

        // those void* are for extra data if needed, instead of merging 2 functions by counting the other one again, just put it in as a void pointer and then cast to what you need, you can insert extra
        // variables also just cast them to what type is needed, like float arrays, other functions or just whatever you may need
        void* evj = nullptr;
        void* evd = nullptr;
        void* evt = nullptr;
        void* evc = nullptr;

        float* t = nullptr; // time value of given sample number, (values on x axis) / wartosci t dla próbki n
        float* f_t = nullptr; // function value for given t of the same n index (values on y axis) / wartosci funkcji dla t tej samej próbki n

        typedef float (*FormulaPtr)(const Function&, uint64_t, float, void*, void*, void*, void*);
        FormulaPtr function_formula = nullptr; // formula pointer, for the object to use its own variables in calculations

        Function(float Tc, float fs, float f, float PHI, float A, FormulaPtr formula, void* extra_variable_one = nullptr, void* extra_variable_two = nullptr, void* extra_variable_three = nullptr, void* extra_variable_four = nullptr) : Tc(Tc), fs(fs), f(f), PHI(PHI), A(A), Ts(1.f / fs), N(static_cast<uint64_t>(Tc * fs)), evj(extra_variable_one), evd(extra_variable_two), evt(extra_variable_three), evc(extra_variable_four)  {

            if (N == 0) return; // well no point in counting anything for N == 0

            function_formula = formula;
            t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));
            f_t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));

            for (uint64_t n = 0; n < N; n++)
                t[n] = static_cast<float>(n) * Ts;
            for (uint64_t n = 0; n < N; n++)
                f_t[n] = formula(*this, n, t[n], evj, evd, evt, evc);
        }
        ~Function() {
            _mm_free(t);
            _mm_free(f_t);
        }
    };
    // DFT, n^2, very slow for high count of points, included just for flex, or research purposes to compare it with FFT, kinda funny
    // also normalization already included
    struct DFT {
        uint64_t K{0};

        float* Re = nullptr;
        float* Im = nullptr; // część zmyślona :)

        float* fk = nullptr;
        float* mod_z = nullptr;

        DFT(const Function* function) {
            if (function->N == 1) return;

            K = function->N;

            Re = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));
            Im = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));

            mod_z = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));
            fk = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));

            for (uint64_t k = 0; k < K; k++) {
                Re[k] = 0;
                Im[k] = 0;
                for (uint64_t n = 0; n < K; n++) {
                    Re[k] += function->f_t[n] * cosf((-2.f * M_PIf * static_cast<float>(n) * static_cast<float>(k)) / static_cast<float>(K));
                    Im[k] += function->f_t[n] * sinf((-2.f * M_PIf * static_cast<float>(n) * static_cast<float>(k)) / static_cast<float>(K));
                }
            }

            for (uint64_t k = 0; k < K; k++) {
                mod_z[k] = sqrtf((Re[k] * Re[k]) + (Im[k] * Im[k]));
                fk[k] = static_cast<float>(k) * function->fs / static_cast<float>(K);
            }
            mod_z[0] /= static_cast<float>(function->N);
            for (uint64_t k = 1; k < K; k++) {
                mod_z[k] /= static_cast<float>(K) / 2.f;
            }
        }
        ~DFT() {
            _mm_free(Im);
            _mm_free(Re);

            _mm_free(mod_z);
            _mm_free(fk);
        }
    };

    // Faster DFT, n log n instead of n^2, much faster, for 100 samples instead of n^2 = 10 000 operations, we have n log n = about 664 operations, and this scales with n like crazy for DFT
    struct FFT {

        uint64_t K{0};

        float* Re = nullptr;
        float* Im = nullptr;

        float* fk = nullptr;
        float* mod_z = nullptr;

        FFT(const Function* function) {

            if (function->N == 1) return;
            K = function->N;

            uint32_t old_k = K;
            uint32_t bits_shift = 0;
            while (K != 1) {
                K = K >> 1;
                bits_shift++;
            }
            K = K << bits_shift;
            K = K << (old_k > K);

            Re = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));
            Im = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));

            mod_z = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));
            fk = static_cast<float*>(_mm_malloc(sizeof(float) * K, 32));

            uint64_t i = 0;
            for (i = 0; i < old_k; i++) {
                Re[i] = function->f_t[i];
                Im[i] = 0;
            }
            for (; i < K; i++) {
                Re[i] = 0;
                Im[i] = 0;
            }

            i = 0;
            uint64_t j = 0;
            for (;i<K - 1;i++) {
                if (i < j) {
                    float temp = Re[i];
                    Re[i] = Re[j];
                    Re[j] = temp;
                }
                uint64_t m = K >> 1;
                while (m >= 1 && j >=m) {
                    j -= m;
                    m = m >> 1;
                }
                j += m;
            }

            float angle_def = -2.0f * M_PIf;
            for (uint64_t size = 2; size <= K; size = size << 1) {

                float angle = angle_def / static_cast<float>(size);
                float w_step_re = cosf(angle);
                float w_step_im = sinf(angle);

                for (uint64_t b = 0; b < K; b+= size) {
                    float w_re = 1.0f;
                    float w_im = 0.0f;

                    for (uint64_t k = b; k < b + (size >> 1); k++) {
                        uint64_t pair_idx = k + (size >> 1);

                        float t_re = Re[pair_idx] * w_re - Im[pair_idx] * w_im;
                        float t_im = Re[pair_idx] * w_im + Im[pair_idx] * w_re;

                        Re[pair_idx] = Re[k] - t_re;
                        Im[pair_idx] = Im[k] - t_im;

                        Re[k] += t_re;
                        Im[k] += t_im;

                        float next_w_re = w_re * w_step_re - w_im * w_step_im;
                        w_im = w_re * w_step_im + w_im * w_step_re;
                        w_re = next_w_re;
                    }
                }
            }
            for (uint64_t k = 0; k <= K / 2; k++) {
                mod_z[k] = sqrtf((Re[k] * Re[k]) + (Im[k] * Im[k]));
                fk[k] = static_cast<float>(k) * function->fs / static_cast<float>(K);
            }
            mod_z[0] /= static_cast<float>(function->N);
            for (uint64_t k = 1; k <= K / 2; k++) {
                mod_z[k] /= static_cast<float>(K) / 2.f;
            }
        }
        ~FFT() {
            _mm_free(Im);
            _mm_free(Re);

            _mm_free(mod_z);
            _mm_free(fk);
        }


    };

    // this is main class for graph rendering with dft, fft or function objects, read instructions on how to use (it's pretty simple)
    class Graph {
        // index:
        // 0 = 8k default, created and tested with it in mind
        // 1 = 4k
        uint32_t picture_size_index = 0;
        uint32_t picture_width[2]{7680, 7680 / 2};
        uint32_t picture_height[2]{4320, 4320 / 2};

        //default values, chosen by me for 8k resolution, works well with 4k also
        // in pixels
        uint32_t line_thickness = 4;
        uint32_t grid_thickness = 4;
        uint32_t axis_thickness = 4;
        uint32_t sub_segments_thickness = 2;
        // text scale
        uint32_t values_text_scale = 6;
        uint32_t labels_text_scale = 10;

        //paddings for manual tweaking
        uint32_t padding_left_x = 556;
        uint32_t padding_right_x = 244;
        uint32_t padding_top_y = 260;
        uint32_t padding_bot_y = 380;

        // multithreading included in constructor, keep in mind, in order to fully exploit it, capp the max graph rendered in one function to the amount of threads in your cpu
        // in other case it may slow down the code instead, because of the constant context switching by the os, any way it should work one way or another
        std::thread graph_thread;

        uint32_t* texture = nullptr;

    public:
        Graph(const Function* function_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", const char* file_path = nullptr, uint32_t background_color = ShiftDownFunctionsColorThemes::global_theme_BackGround_color, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color, uint32_t axis_color = ShiftDownFunctionsColorThemes::global_theme_Axis_color, uint32_t grid_color = ShiftDownFunctionsColorThemes::global_theme_Grid_color, uint32_t sub_segments_color = ShiftDownFunctionsColorThemes::global_theme_SubSegments_color, uint32_t font_color = ShiftDownFunctionsColorThemes::global_theme_Font_color) {

            graph_thread = std::thread([=, this]() {

                //math section
                uint64_t picture_size = picture_width[picture_size_index] * picture_height[picture_size_index];
                uint32_t divider = picture_size_index * 2;
                if (divider == 0) divider = 1;

                texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size, 32));
                for (uint64_t i = 0; i < picture_size; i++) {
                    texture[i] = background_color;
                }

                float min_y = std::numeric_limits<float>::max();
                float max_y = std::numeric_limits<float>::lowest();

                for (uint32_t i = 0; i < function_to_render->N; i++) {
                    min_y = function_to_render->f_t[i] < min_y ? function_to_render->f_t[i] : min_y;
                    max_y = function_to_render->f_t[i] > max_y ? function_to_render->f_t[i] : max_y;
                }

                padding_left_x /= divider;
                padding_right_x /= divider;

                padding_top_y /= divider;
                padding_bot_y /= divider;

                uint32_t graph_width = picture_width[picture_size_index] - padding_left_x - padding_right_x; // 7168px
                uint32_t graph_height = picture_height[picture_size_index] - padding_top_y - padding_bot_y; // 3552px

                float scale_x = static_cast<float>(graph_width) / (function_to_render->t[function_to_render->N - 1] - function_to_render->t[0]);
                float scale_y = 0;
                if (max_y == min_y) {
                    scale_y = static_cast<float>(graph_height) / 1.f;
                }
                else {
                    scale_y = static_cast<float>(graph_height) / (max_y - min_y);
                }

                int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * function_to_render->N, 32));
                int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * function_to_render->N, 32));

                for (uint32_t i = 0; i < function_to_render->N; i++) {
                    scaled_x[i] = static_cast<int>(roundf(function_to_render->t[i] * scale_x));
                    scaled_y[i] = -(static_cast<int>(roundf(function_to_render->f_t[i] * scale_y)));
                }

                int int_min_y = std::numeric_limits<int>::max();
                int int_max_y = std::numeric_limits<int>::lowest();

                for (uint32_t i = 0; i < function_to_render->N; i++) {
                    int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
                    int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
                }

                int offset_x = scaled_x[0] * -1;
                int offset_y = int_min_y * -1;

                auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * function_to_render->N, 32));
                auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * function_to_render->N, 32));

                for (uint32_t i = 0; i < function_to_render->N; i++) {
                    scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                    scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
                }


                // name label
                uint32_t end = 0;
                while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
                    end++;
                }
                z_detail_no_need_to_think_about_it::TextBox name(end, 1, background_color);
                name.add_text(name_label, font_color);

                uint32_t skala_textu_name = labels_text_scale / divider;
                uint32_t center = name.texture_width * skala_textu_name / 2;

                uint32_t pozycja_y = 0;
                for (uint32_t y = 0; y < name.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < name.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                                texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y + (48 / divider)) * picture_width[picture_size_index])) - center] = name.texture[x + (y * name.texture_width)];
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
                z_detail_no_need_to_think_about_it::TextBox os_y(end, 1, background_color);
                os_y.add_text(y_label, font_color);

                auto* rotated_texture = static_cast<uint32_t*>(_mm_malloc(
                    sizeof(uint32_t) * os_y.texture_width * os_y.texture_height, 32));
                uint32_t rotated_width = os_y.texture_height;
                uint32_t rotated_height = os_y.texture_width;

                for (uint32_t y = 0; y < rotated_height; y++) {
                    for (uint32_t x = 0; x < rotated_width; x++) {
                        rotated_texture[x + (y * rotated_width)] =
                            os_y.texture[(os_y.texture_width - 1 - y) + (x * os_y.texture_width)];
                    }
                }

                uint32_t skala_textu_os_y = labels_text_scale / divider;
                pozycja_y = 0;
                center = rotated_height * skala_textu_os_y / 2;

                for (uint32_t y = 0; y < rotated_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < rotated_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                                texture[sx + pozycja_x + ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width[picture_size_index])] = rotated_texture[x + y * rotated_width];
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
                z_detail_no_need_to_think_about_it::TextBox os_x(end, 1, background_color);
                os_x.add_text(x_label, font_color);

                uint32_t skala_textu_os_x = labels_text_scale / divider;
                center = os_x.texture_width * skala_textu_os_x / 2;

                pozycja_y = 0;
                for (uint32_t y = 0; y < os_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < os_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                                texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y - (16 / divider) + picture_height[picture_size_index] - os_x.texture_height * skala_textu_os_x) * picture_width[picture_size_index])) - center] = os_x.texture[x + (y * os_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_os_x;
                    }
                    pozycja_y += skala_textu_os_x;
                }

                // values on x
                uint32_t segments_count = 8;
                uint32_t steps_x = function_to_render->N / segments_count;
                char value[32];
                uint32_t skala_textu_value_x = values_text_scale / divider;
                for (uint32_t i = 1; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                    z_detail_no_need_to_think_about_it::float_to_engineering_7chars((function_to_render->Tc * (float)i) / (float)segments_count, value);
                    value_x.add_text(value, font_color);

                    center = value[0] == '-' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) - (skala_textu_value_x << 1));
                    center = value[6] != ' ' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) + (skala_textu_value_x << 1));

                    pozycja_y = 0;
                    for (uint32_t y = 0; y < value_x.texture_height; y++) {
                        uint32_t pozycja_x = 0;
                        for (uint32_t x = 0; x < value_x.texture_width; x++) {
                            for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                                for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                    texture[(sx + pozycja_x + scaled_uint_x[(steps_x * i) - ((steps_x * i) > 0)] +
                                             ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                            center] = value_x.texture[x + (y * value_x.texture_width)];
                                }
                            }
                            pozycja_x += skala_textu_value_x;
                        }
                        pozycja_y += skala_textu_value_x;
                    }
                }
                z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                z_detail_no_need_to_think_about_it::float_to_engineering_7chars(function_to_render->Tc, value);
                value_x.add_text(value, font_color);

                center = value[0] == '-' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) - (skala_textu_value_x << 1));
                center = value[6] != ' ' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) + (skala_textu_value_x << 1));
                pozycja_y = 0;
                for (uint32_t y = 0; y < value_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                texture[(sx + pozycja_x + scaled_uint_x[function_to_render->N-1] + ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) - center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }

                // values y
                uint32_t kurwa_zmienna = graph_height / segments_count;
                uint32_t skala_textu_value_y = values_text_scale / divider;
                float step_y = (max_y - min_y) / static_cast<float>(segments_count);

                for (uint32_t i = 0; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                    z_detail_no_need_to_think_about_it::float_to_engineering_7chars(max_y - step_y * static_cast<float>(i), value);
                    value_y.add_text(value, font_color);

                    center = (7 * 8 * skala_textu_value_y);

                    pozycja_y = 0;
                    for (uint32_t y = 0; y < value_y.texture_height; y++) {
                        uint32_t pozycja_x = 0;
                        for (uint32_t x = 0; x < value_y.texture_width; x++) {
                            for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                                for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                    texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                            ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                              ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                             picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                                }
                            }
                            pozycja_x += skala_textu_value_y;
                        }
                        pozycja_y += skala_textu_value_y;
                    }
                }
                z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                z_detail_no_need_to_think_about_it::float_to_engineering_7chars(max_y - step_y * static_cast<float>(segments_count), value);
                value_y.add_text(value, font_color);

                center = (7 * 8 * skala_textu_value_y);

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_y.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_y.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) + ((padding_top_y + sy + pozycja_y + graph_height - ((value_y.texture_height * skala_textu_value_y) / 3)) * picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }



                //sub segments on x
                uint32_t sub_segments_count = 20;
                uint32_t sub_segments_count_x = sub_segments_count * segments_count;

                uint32_t sub_steps_x = graph_width / sub_segments_count_x;
                for (uint32_t i = 0; i < sub_segments_count_x; i++) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], (sub_steps_x * i) + padding_left_x, picture_height[picture_size_index] - padding_bot_y, (sub_steps_x * i) + padding_left_x, 0 + padding_top_y, sub_segments_color, sub_segments_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[function_to_render->N - 1],
                          picture_height[picture_size_index] - padding_bot_y, graph_width + padding_left_x, 0 + padding_top_y,
                          sub_segments_color, sub_segments_thickness / divider);

                // grid on y
                uint32_t kurwa_zmienna_2 = graph_height / (sub_segments_count * segments_count);
                for (uint32_t i = 0; i < segments_count; i++) {
                    for (uint32_t j = 0; j < sub_segments_count; j++) {
                        z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i) + (kurwa_zmienna_2 * j),
                                  picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i) + (kurwa_zmienna_2 * j) , sub_segments_color, sub_segments_thickness / divider);
                    }
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i),
                              picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, grid_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (graph_height),
                          picture_width[picture_size_index] - padding_right_x, padding_top_y + (graph_height), grid_color, grid_thickness / divider);

                // grid on x
                steps_x = graph_width / segments_count;
                for (uint32_t i = 0; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], (steps_x * i) + padding_left_x, picture_height[picture_size_index] - padding_bot_y, (steps_x * i) + padding_left_x, 0 + padding_top_y, grid_color, grid_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[function_to_render->N - 1],
                          picture_height[picture_size_index] - padding_bot_y, graph_width + padding_left_x, 0 + padding_top_y,
                          grid_color, grid_thickness / divider);


                // os x
                if (min_y <= 0.0f && max_y >= 0.0f) {
                    uint32_t zero_y_pixel = offset_y + padding_top_y;
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, zero_y_pixel, picture_width[picture_size_index] - padding_right_x, zero_y_pixel, axis_color, axis_thickness / divider);
                }
                // os y
                if (scaled_x[0] <= 0) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], 0 + offset_x + padding_left_x,picture_height[picture_size_index] - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color, axis_thickness / divider);
                }
                // wykres
                // stara oryginalna funkcja, problem dla f > bardzo dużo, oraz Bresenham dostawał zawału i czasy leciały w bardzo dużo
                // for (uint32_t i = 0; i < function_to_render->N - 1; i++) {
                //     z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i],scaled_uint_x[i + 1], scaled_uint_y[i + 1], line_color, line_thickness / divider);
                // }
                // zoptymalizowane rysowanie, dla f > bardzo dużo, czasy mniejsze względem oryginału
                uint32_t local_max_y = scaled_uint_y[0];
                uint32_t local_min_y = scaled_uint_y[0];
                for (uint32_t i = 0; i < function_to_render->N - 1; i++) {
                    local_max_y = scaled_uint_y[i] < local_max_y ? scaled_uint_y[i] : local_max_y;
                    local_min_y = scaled_uint_y[i] >= local_min_y ? scaled_uint_y[i] : local_min_y;
                    i = (scaled_uint_x[i] != scaled_uint_x[i + 1]) ?
                    (z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index],
                        picture_height[picture_size_index], scaled_uint_x[i], local_max_y,scaled_uint_x[i],
                        local_min_y, line_color, line_thickness / divider),
                        local_max_y = scaled_uint_y[i + 1],
                        local_min_y = scaled_uint_y[i + 1],
                        z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index],
                            scaled_uint_x[i], scaled_uint_y[i],scaled_uint_x[i + 1], scaled_uint_y[i + 1],
                            line_color, line_thickness / divider),
                        i) : i;
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index],
                        picture_height[picture_size_index], scaled_uint_x[function_to_render->N - 1], local_max_y,scaled_uint_x[function_to_render->N - 1],
                        local_min_y, line_color, line_thickness / divider);


                //zapis na dysk
                z_detail_no_need_to_think_about_it::save_texture_to_file(texture, picture_width[picture_size_index], picture_height[picture_size_index], file_path);

                _mm_free(scaled_y);
                _mm_free(scaled_x);
                _mm_free(scaled_uint_x);
                _mm_free(scaled_uint_y);
            });
        }
        Graph(const DFT* dft_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", const char* file_path = nullptr, uint32_t background_color = ShiftDownFunctionsColorThemes::global_theme_BackGround_color, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color, uint32_t axis_color = ShiftDownFunctionsColorThemes::global_theme_Axis_color, uint32_t grid_color = ShiftDownFunctionsColorThemes::global_theme_Grid_color, uint32_t sub_segments_color = ShiftDownFunctionsColorThemes::global_theme_SubSegments_color, uint32_t font_color = ShiftDownFunctionsColorThemes::global_theme_Font_color) {

            graph_thread = std::thread([=, this]() {

                //math section
                uint64_t picture_size = picture_width[picture_size_index] * picture_height[picture_size_index];
                uint32_t divider = picture_size_index * 2;
                if (divider == 0) divider = 1;
                uint32_t K_render = (dft_to_render->K / 2) + 1;


                texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size, 32));
                for (uint64_t i = 0; i < picture_size; i++) {
                    texture[i] = background_color;
                }

                float min_y = std::numeric_limits<float>::max();
                float max_y = std::numeric_limits<float>::lowest();

                for (uint32_t i = 0; i < K_render; i++) {
                    max_y = dft_to_render->mod_z[i] > max_y ? dft_to_render->mod_z[i] : max_y;
                }
                min_y = 0.0f;
                padding_left_x /= divider;
                padding_right_x /= divider;

                padding_top_y /= divider;
                padding_bot_y /= divider;

                uint32_t graph_width = picture_width[picture_size_index] - padding_left_x - padding_right_x; // 7168px
                uint32_t graph_height = picture_height[picture_size_index] - padding_top_y - padding_bot_y; // 3552px



                float scale_x = static_cast<float>(graph_width) / (dft_to_render->fk[K_render - 1] - dft_to_render->fk[0]);
                float scale_y = 0;
                if (max_y == min_y) {
                    scale_y = static_cast<float>(graph_height) / 1.f;
                }
                else {
                    scale_y = static_cast<float>(graph_height) / (max_y - min_y);
                }

                int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));
                int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));

                for (uint32_t i = 0; i < K_render; i++) {
                    scaled_x[i] = static_cast<int>(roundf(dft_to_render->fk[i] * scale_x));
                    scaled_y[i] = -(static_cast<int>(roundf(dft_to_render->mod_z[i] * scale_y)));
                }

                int int_min_y = std::numeric_limits<int>::max();
                int int_max_y = std::numeric_limits<int>::lowest();

                for (uint32_t i = 0; i < K_render; i++) {
                    int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
                    int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
                }

                int offset_x = scaled_x[0] * -1;
                int offset_y = int_min_y * -1;

                auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));
                auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));

                for (uint32_t i = 0; i < K_render; i++) {
                    scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                    scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
                }


                // drawing section

                // name label
                uint32_t end = 0;
                uint32_t pozycja_y = 0;
                uint32_t center = 0;

                while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
                    end++;
                }
                z_detail_no_need_to_think_about_it::TextBox name(end, 1, background_color);
                name.add_text(name_label, font_color);

                uint32_t skala_textu_name = labels_text_scale / divider;
                center = name.texture_width * skala_textu_name / 2;

                for (uint32_t y = 0; y < name.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < name.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                                texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                         ((sy + pozycja_y + (48 / divider)) * picture_width[picture_size_index])) -
                                        center] = name.texture[x + (y * name.texture_width)];
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
                z_detail_no_need_to_think_about_it::TextBox os_y(end, 1, background_color);
                os_y.add_text(y_label, font_color);

                auto* rotated_texture = static_cast<uint32_t*>(_mm_malloc(
                    sizeof(uint32_t) * os_y.texture_width * os_y.texture_height, 32));
                uint32_t rotated_width = os_y.texture_height;
                uint32_t rotated_height = os_y.texture_width;

                for (uint32_t y = 0; y < rotated_height; y++) {
                    for (uint32_t x = 0; x < rotated_width; x++) {
                        rotated_texture[x + (y * rotated_width)] =
                            os_y.texture[(os_y.texture_width - 1 - y) + (x * os_y.texture_width)];
                    }
                }

                uint32_t skala_textu_os_y = labels_text_scale / divider;
                pozycja_y = 0;
                center = rotated_height * skala_textu_os_y / 2;

                for (uint32_t y = 0; y < rotated_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < rotated_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_os_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_os_y; sx++) {
                                texture[sx + pozycja_x +
                                        ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width[picture_size_index])] =
                                    rotated_texture[x + y * rotated_width];
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
                z_detail_no_need_to_think_about_it::TextBox os_x(end, 1, background_color);
                os_x.add_text(x_label, font_color);

                uint32_t skala_textu_os_x = labels_text_scale / divider;
                center = os_x.texture_width * skala_textu_os_x / 2;

                pozycja_y = 0;
                for (uint32_t y = 0; y < os_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < os_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                                texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y - (16 / divider) + picture_height[picture_size_index] - os_x.texture_height * skala_textu_os_x) * picture_width[picture_size_index])) - center] = os_x.texture[x + (y * os_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_os_x;
                    }
                    pozycja_y += skala_textu_os_x;
                }

                // values on x
                uint32_t segments_count = 8;

                char value[32];
                uint32_t skala_textu_value_x = values_text_scale / divider;
                for (uint32_t i = 1; i < segments_count; i++) {

                    z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                    z_detail_no_need_to_think_about_it::float_to_engineering_7chars(dft_to_render->fk[((K_render - 1) * i) / segments_count], value);
                    value_x.add_text(value, font_color);

                    center = value[0] == '-' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) - (skala_textu_value_x << 1));
                    center = value[6] != ' ' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) + (skala_textu_value_x << 1));

                    pozycja_y = 0;
                    for (uint32_t y = 0; y < value_x.texture_height; y++) {
                        uint32_t pozycja_x = 0;
                        for (uint32_t x = 0; x < value_x.texture_width; x++) {
                            for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                                for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                    texture[(sx + pozycja_x + scaled_uint_x[((K_render - 1) * i) / segments_count] +
                                             ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                            center] = value_x.texture[x + (y * value_x.texture_width)];
                                }
                            }
                            pozycja_x += skala_textu_value_x;
                        }
                        pozycja_y += skala_textu_value_x;
                    }
                }
                z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                z_detail_no_need_to_think_about_it::float_to_engineering_7chars(dft_to_render->fk[(K_render - 1)], value);
                value_x.add_text(value, font_color);

                center = value[0] == '-' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) - (skala_textu_value_x << 1));
                center = value[6] != ' ' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) + (skala_textu_value_x << 1));
                pozycja_y = 0;
                for (uint32_t y = 0; y < value_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                texture[(sx + pozycja_x + scaled_uint_x[(K_render - 1)] +
                                         ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                        center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }

                // values y
                uint32_t kurwa_zmienna = graph_height / segments_count;

                uint32_t skala_textu_value_y = values_text_scale / divider;
                float step_y = (max_y - min_y) / static_cast<float>(segments_count);

                for (uint32_t i = 0; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                    z_detail_no_need_to_think_about_it::float_to_engineering_7chars(max_y - step_y * static_cast<float>(i), value);
                    value_y.add_text(value, font_color);

                    center = (7 * 8 * skala_textu_value_y);

                    pozycja_y = 0;
                    for (uint32_t y = 0; y < value_y.texture_height; y++) {
                        uint32_t pozycja_x = 0;
                        for (uint32_t x = 0; x < value_y.texture_width; x++) {
                            for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                                for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                    texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                            ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                              ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                             picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                                }
                            }
                            pozycja_x += skala_textu_value_y;
                        }
                        pozycja_y += skala_textu_value_y;
                    }
                }
                z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                z_detail_no_need_to_think_about_it::float_to_engineering_7chars(max_y - step_y * static_cast<float>(segments_count), value);
                value_y.add_text(value, font_color);

                center = (7 * 8 * skala_textu_value_y);

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_y.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_y.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                        ((padding_top_y + sy + pozycja_y + graph_height -
                                          ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                         picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }


                //sub segments on x
                uint32_t sub_segments_count = 20;
                uint32_t sub_segments_count_x = sub_segments_count * segments_count;
                uint32_t sub_steps_x = graph_width / sub_segments_count_x;

                for (uint32_t i = 0; i <= sub_segments_count_x; i++) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], sub_steps_x * i + padding_left_x, picture_height[picture_size_index] - padding_bot_y, sub_steps_x * i + padding_left_x, 0 + padding_top_y, sub_segments_color, sub_segments_thickness / divider);
                }


                // grid on y
                uint32_t kurwa_zmienna_2 = graph_height / (sub_segments_count * segments_count);
                for (uint32_t i = 0; i < segments_count; i++) {
                    for (uint32_t j = 0; j < sub_segments_count; j++) {
                        z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i) + (kurwa_zmienna_2 * j),
                                  picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i) + (kurwa_zmienna_2 * j) , sub_segments_color, sub_segments_thickness / divider);
                    }
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i),
                              picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, grid_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (graph_height),
                          picture_width[picture_size_index] - padding_right_x, padding_top_y + (graph_height), grid_color, grid_thickness / divider);



                // grid on x
                for (uint32_t i = 0; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[((K_render - 1) * i) / segments_count],
                              picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[((K_render - 1) * i) / segments_count], 0 + padding_top_y, grid_color, grid_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[(K_render - 1)],
                          picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[(K_render - 1)], 0 + padding_top_y,
                          grid_color, grid_thickness / divider);



                // os x
                if (min_y <= 0.0f && max_y >= 0.0f) {
                    uint32_t zero_y_pixel = offset_y + padding_top_y;
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, zero_y_pixel, picture_width[picture_size_index] - padding_right_x, zero_y_pixel, axis_color, axis_thickness / divider);
                }
                // os y
                if (scaled_x[0] <= 0) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], 0 + offset_x + padding_left_x,
                    picture_height[picture_size_index] - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                              axis_thickness / divider);
                }


                // wykres
                //stary kod do rysowania
                // for (uint32_t i = 0; i < K_render; i++) {
                //     uint32_t zero_y_pixel = offset_y + padding_top_y;
                //     z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, line_thickness / divider);
                // }

                uint32_t zero_y_pixel = offset_y + padding_top_y;
                uint32_t local_peak_y = zero_y_pixel;
                for (uint32_t i = 0; i < K_render; i++) {
                    local_peak_y = local_peak_y < scaled_uint_y[i] ? local_peak_y : scaled_uint_y[i];
                    local_peak_y = (scaled_uint_x[i] != scaled_uint_x[i + 1]) ?
                    (z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index],
                        scaled_uint_x[i], local_peak_y, scaled_uint_x[i], zero_y_pixel,
                        line_color, line_thickness / divider),
                        local_peak_y = zero_y_pixel) : local_peak_y;
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index],
                        scaled_uint_x[K_render - 1], scaled_uint_y[K_render - 1], scaled_uint_x[K_render - 1], zero_y_pixel,
                        line_color, line_thickness / divider);

                // save to file
                z_detail_no_need_to_think_about_it::save_texture_to_file(texture, picture_width[picture_size_index], picture_height[picture_size_index], file_path);

                _mm_free(scaled_y);
                _mm_free(scaled_x);
                _mm_free(scaled_uint_x);
                _mm_free(scaled_uint_y);
            });
        }
        Graph(const FFT* fft_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", const char* file_path = nullptr, uint32_t background_color = ShiftDownFunctionsColorThemes::global_theme_BackGround_color, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color, uint32_t axis_color = ShiftDownFunctionsColorThemes::global_theme_Axis_color, uint32_t grid_color = ShiftDownFunctionsColorThemes::global_theme_Grid_color, uint32_t sub_segments_color = ShiftDownFunctionsColorThemes::global_theme_SubSegments_color, uint32_t font_color = ShiftDownFunctionsColorThemes::global_theme_Font_color) {

            graph_thread = std::thread([=, this]() {

                //math section
                uint64_t picture_size = picture_width[picture_size_index] * picture_height[picture_size_index];
                uint32_t divider = picture_size_index * 2;
                if (divider == 0) divider = 1;
                uint32_t K_render = (fft_to_render->K / 2) + 1;

                texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size, 32));
                for (uint64_t i = 0; i < picture_size; i++) {
                    texture[i] = background_color;
                }



                float min_y = std::numeric_limits<float>::max();
                float max_y = std::numeric_limits<float>::lowest();

                for (uint32_t i = 0; i < K_render; i++) {
                    max_y = fft_to_render->mod_z[i] > max_y ? fft_to_render->mod_z[i] : max_y;
                }
                min_y = 0.0f;

                padding_left_x /= divider;
                padding_right_x /= divider;

                padding_top_y /= divider;
                padding_bot_y /= divider;

                uint32_t graph_width = picture_width[picture_size_index] - padding_left_x - padding_right_x;
                uint32_t graph_height = picture_height[picture_size_index] - padding_top_y - padding_bot_y;



                float scale_x = static_cast<float>(graph_width) / (fft_to_render->fk[K_render - 1] - fft_to_render->fk[0]);
                float scale_y = 0;
                if (max_y == min_y) {
                    scale_y = static_cast<float>(graph_height) / 1.f;
                }
                else {
                    scale_y = static_cast<float>(graph_height) / (max_y - min_y);
                }

                int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));
                int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));

                for (uint32_t i = 0; i < K_render; i++) {
                    scaled_x[i] = static_cast<int>(roundf(fft_to_render->fk[i] * scale_x));
                    scaled_y[i] = -(static_cast<int>(roundf(fft_to_render->mod_z[i] * scale_y)));
                }

                int int_min_y = std::numeric_limits<int>::max();
                int int_max_y = std::numeric_limits<int>::lowest();

                for (uint32_t i = 0; i < K_render; i++) {
                    int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
                    int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
                }

                int offset_x = scaled_x[0] * -1;
                int offset_y = int_min_y * -1;

                auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));
                auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));

                for (uint32_t i = 0; i < K_render; i++) {
                    scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                    scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
                }


                // drawing section

                // name label
                uint32_t end = 0;
                uint32_t pozycja_y = 0;
                uint32_t center = 0;

                while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
                    end++;
                }
                z_detail_no_need_to_think_about_it::TextBox name(end, 1, background_color);
                name.add_text(name_label, font_color);

                uint32_t skala_textu_name = labels_text_scale / divider;
                center = name.texture_width * skala_textu_name / 2;

                for (uint32_t y = 0; y < name.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < name.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                                texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y + (48 / divider)) * picture_width[picture_size_index])) - center] = name.texture[x + (y * name.texture_width)];
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
                z_detail_no_need_to_think_about_it::TextBox os_y(end, 1, background_color);
                os_y.add_text(y_label, font_color);

                auto* rotated_texture = static_cast<uint32_t*>(_mm_malloc(
                    sizeof(uint32_t) * os_y.texture_width * os_y.texture_height, 32));
                uint32_t rotated_width = os_y.texture_height;
                uint32_t rotated_height = os_y.texture_width;

                for (uint32_t y = 0; y < rotated_height; y++) {
                    for (uint32_t x = 0; x < rotated_width; x++) {
                        rotated_texture[x + (y * rotated_width)] =
                            os_y.texture[(os_y.texture_width - 1 - y) + (x * os_y.texture_width)];
                    }
                }

                uint32_t skala_textu_os_y = labels_text_scale / divider;
                pozycja_y = 0;
                center = rotated_height * skala_textu_os_y / 2;

                for (uint32_t y = 0; y < rotated_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < rotated_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_os_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_os_y; sx++) {
                                texture[sx + pozycja_x +
                                        ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width[picture_size_index])] =
                                    rotated_texture[x + y * rotated_width];
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
                z_detail_no_need_to_think_about_it::TextBox os_x(end, 1, background_color);
                os_x.add_text(x_label, font_color);

                uint32_t skala_textu_os_x = labels_text_scale / divider;
                center = os_x.texture_width * skala_textu_os_x / 2;

                pozycja_y = 0;
                for (uint32_t y = 0; y < os_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < os_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                                texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y - (16 / divider) + picture_height[picture_size_index] - os_x.texture_height * skala_textu_os_x) * picture_width[picture_size_index])) - center] = os_x.texture[x + (y * os_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_os_x;
                    }
                    pozycja_y += skala_textu_os_x;
                }

                // values on x
                uint32_t segments_count = 8;
                char value[32];
                uint32_t skala_textu_value_x = values_text_scale / divider;
                uint32_t step_x = ((K_render - 1) / segments_count);
                for (uint32_t i = 1; i < segments_count; i++) {

                    z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                    z_detail_no_need_to_think_about_it::float_to_engineering_7chars(fft_to_render->fk[step_x * i], value);
                    value_x.add_text(value, font_color);

                    center = value[0] == '-' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) - (skala_textu_value_x << 1));
                    center = value[6] != ' ' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) + (skala_textu_value_x << 1));

                    pozycja_y = 0;
                    for (uint32_t y = 0; y < value_x.texture_height; y++) {
                        uint32_t pozycja_x = 0;
                        for (uint32_t x = 0; x < value_x.texture_width; x++) {
                            for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                                for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                    texture[(sx + pozycja_x + scaled_uint_x[step_x * i] + ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) - center] = value_x.texture[x + (y * value_x.texture_width)];
                                }
                            }
                            pozycja_x += skala_textu_value_x;
                        }
                        pozycja_y += skala_textu_value_x;
                    }
                }
                z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                z_detail_no_need_to_think_about_it::float_to_engineering_7chars(fft_to_render->fk[step_x * segments_count], value);
                value_x.add_text(value, font_color);
                center = value[0] == '-' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) - (skala_textu_value_x << 1));
                center = value[6] != ' ' ? ((56 * skala_textu_value_x) >> 1) : (((56 * skala_textu_value_x) >> 1) + (skala_textu_value_x << 1));

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                texture[(sx + pozycja_x + scaled_uint_x[step_x * segments_count] +
                                         ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) - center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }

                // values y
                uint32_t kurwa_zmienna = graph_height / segments_count;
                uint32_t skala_textu_value_y = values_text_scale / divider;
                float step_y = (max_y - min_y) / static_cast<float>(segments_count);

                for (uint32_t i = 0; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                    z_detail_no_need_to_think_about_it::float_to_engineering_7chars(max_y - step_y * static_cast<float>(i), value);
                    value_y.add_text(value, font_color);

                    center = (7 * 8 * skala_textu_value_y);

                    pozycja_y = 0;
                    for (uint32_t y = 0; y < value_y.texture_height; y++) {
                        uint32_t pozycja_x = 0;
                        for (uint32_t x = 0; x < value_y.texture_width; x++) {
                            for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                                for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                    texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) + ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) - ((value_y.texture_height * skala_textu_value_y) / 3)) * picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                                }
                            }
                            pozycja_x += skala_textu_value_y;
                        }
                        pozycja_y += skala_textu_value_y;
                    }
                }
                z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                z_detail_no_need_to_think_about_it::float_to_engineering_7chars(max_y - step_y * static_cast<float>(segments_count), value);
                value_y.add_text(value, font_color);

                center = (7 * 8 * skala_textu_value_y);

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_y.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_y.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) + ((padding_top_y + sy + pozycja_y + graph_height - ((value_y.texture_height * skala_textu_value_y) / 3)) * picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }

                //sub segments on x
                uint32_t sub_segments_count = 20;
                uint32_t sub_segments_count_x = sub_segments_count * segments_count;
                uint32_t sub_steps_x = graph_width / sub_segments_count_x;

                for (uint32_t i = 0; i <= sub_segments_count_x; i++) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], sub_steps_x * i + padding_left_x, picture_height[picture_size_index] - padding_bot_y, sub_steps_x * i + padding_left_x, 0 + padding_top_y, sub_segments_color, sub_segments_thickness / divider);
                }


                // grid on y
                uint32_t kurwa_zmienna_2 = graph_height / (sub_segments_count * segments_count);
                for (uint32_t i = 0; i < segments_count; i++) {
                    for (uint32_t j = 0; j < sub_segments_count; j++) {
                        z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i) + (kurwa_zmienna_2 * j),
                                  picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i) + (kurwa_zmienna_2 * j) , sub_segments_color, sub_segments_thickness / divider);
                    }
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i),
                              picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, grid_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (graph_height),
                          picture_width[picture_size_index] - padding_right_x, padding_top_y + (graph_height), grid_color, grid_thickness / divider);



                // grid on x
                for (uint32_t i = 0; i < segments_count; i++) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[step_x * i],
                              picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[step_x * i], 0 + padding_top_y, grid_color, grid_thickness / divider);
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[step_x * segments_count],
                          picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[step_x * segments_count], 0 + padding_top_y,
                          grid_color, grid_thickness / divider);

                // os x
                if (min_y <= 0.0f && max_y >= 0.0f) {
                    uint32_t zero_y_pixel = offset_y + padding_top_y;
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, zero_y_pixel, picture_width[picture_size_index] - padding_right_x, zero_y_pixel, axis_color, axis_thickness / divider);
                }
                // os y
                if (scaled_x[0] <= 0) {
                    z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], 0 + offset_x + padding_left_x,
                    picture_height[picture_size_index] - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                              axis_thickness / divider);
                }


                // wykres
                //stary kod do rysowania
                // for (uint32_t i = 0; i < K_render; i++) {
                //     uint32_t zero_y_pixel = offset_y + padding_top_y;
                //     z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, line_thickness / divider);
                // }


                // zoptymalizowany kod
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                uint32_t local_peak_y = zero_y_pixel;
                for (uint32_t i = 0; i < K_render; i++) {
                    local_peak_y = local_peak_y < scaled_uint_y[i] ? local_peak_y : scaled_uint_y[i];
                    local_peak_y = (scaled_uint_x[i] != scaled_uint_x[i + 1]) ?
                    (z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index],
                        scaled_uint_x[i], local_peak_y, scaled_uint_x[i], zero_y_pixel,
                        line_color, line_thickness / divider),
                        local_peak_y = zero_y_pixel) : local_peak_y;
                }
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index],
                        scaled_uint_x[K_render - 1], scaled_uint_y[K_render - 1], scaled_uint_x[K_render - 1], zero_y_pixel,
                        line_color, line_thickness / divider);


                // save to file
                z_detail_no_need_to_think_about_it::save_texture_to_file(texture, picture_width[picture_size_index], picture_height[picture_size_index], file_path);

                _mm_free(scaled_y);
                _mm_free(scaled_x);
                _mm_free(scaled_uint_x);
                _mm_free(scaled_uint_y);
            });
        }

        ~Graph() {
            if (graph_thread.joinable()) {
                graph_thread.join();
            }
            _mm_free(texture);
        };
    };
    // to samo co Graph ale pozwala nakładać na siebie wykresy. Skalowanie odbywa się względem pierwszego dodanego wykresu oddanie A, B nie wygeneruje tego samego co B, A
    class GraphMulti {
        // index:
        // 0 = 8k
        // 1 = 4k
        // 2 = full hd
        uint32_t picture_size_index = 0;
        uint32_t picture_width[3]{7680, 7680 / 2, 7680 / 4};
        uint32_t picture_height[3]{4320, 4320 / 2, 4320 / 4};

        const char* file_path = nullptr;
        const char* name_label = nullptr;

        float scale_x = 0;
        float scale_y = 0;

        uint32_t offset_x = 0;
        uint32_t offset_y = 0;
        uint32_t padding_left_x = 0;
        uint32_t padding_right_x = 0;
        uint32_t padding_top_y = 0;
        uint32_t padding_bot_y = 0;
        uint32_t divider = 0;

        std::thread multigraph_thread;

        uint32_t* texture = nullptr;

    public:

        GraphMulti(const Function* function_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", const char* file_path = nullptr, uint32_t background_color = ShiftDownFunctionsColorThemes::global_theme_BackGround_color, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color, uint32_t axis_color = ShiftDownFunctionsColorThemes::global_theme_Axis_color, uint32_t grid_color = ShiftDownFunctionsColorThemes::global_theme_Grid_color, uint32_t font_color = ShiftDownFunctionsColorThemes::global_theme_Font_color) {
            this->file_path = file_path;
            this->name_label = name_label;
            //math section
            uint64_t picture_size = picture_width[picture_size_index] * picture_height[picture_size_index];
            divider = picture_size_index * 2;
            divider += (divider == 0);

            texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size, 32));
            for (uint64_t i = 0; i < picture_size; i++) {
                texture[i] = background_color;
            }

            float min_y = std::numeric_limits<float>::max();
            float max_y = std::numeric_limits<float>::lowest();

            for (uint32_t i = 0; i < function_to_render->N; i++) {
                min_y = function_to_render->f_t[i] < min_y ? function_to_render->f_t[i] : min_y;
                max_y = function_to_render->f_t[i] > max_y ? function_to_render->f_t[i] : max_y;
            }

            padding_left_x = 736 / divider;
            padding_right_x = 384 / divider;

            padding_top_y = 256 / divider;
            padding_bot_y = 512 / divider;

            uint32_t graph_width = picture_width[picture_size_index] - padding_left_x - padding_right_x; // 7168px
            uint32_t graph_height = picture_height[picture_size_index] - padding_top_y - padding_bot_y; // 3552px

            scale_x = static_cast<float>(graph_width) / (function_to_render->t[function_to_render->N - 1] - function_to_render->t[0]);
            scale_y = 0;
            if (max_y == min_y) {
                scale_y = static_cast<float>(graph_height) / 1.f;
            }
            else {
                scale_y = static_cast<float>(graph_height) / (max_y - min_y);
            }

            int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * function_to_render->N, 32));
            int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * function_to_render->N, 32));

            for (uint32_t i = 0; i < function_to_render->N; i++) {
                scaled_x[i] = static_cast<int>(roundf(function_to_render->t[i] * scale_x));
                scaled_y[i] = -(static_cast<int>(roundf(function_to_render->f_t[i] * scale_y)));
            }

            int int_min_y = std::numeric_limits<int>::max();
            int int_max_y = std::numeric_limits<int>::lowest();

            for (uint32_t i = 0; i < function_to_render->N; i++) {
                int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
                int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
            }

            offset_x = scaled_x[0] * -1;
            offset_y = int_min_y * -1;

            auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * function_to_render->N, 32));
            auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * function_to_render->N, 32));

            for (uint32_t i = 0; i < function_to_render->N; i++) {
                scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
            }


            // name label
            uint32_t end = 0;
            while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
                end++;
            }
            z_detail_no_need_to_think_about_it::TextBox name(end, 1, background_color);
            name.add_text(name_label, font_color);

            uint32_t skala_textu_name = 16 / divider;
            uint32_t center = name.texture_width * skala_textu_name / 2;

            uint32_t pozycja_y = 0;
            for (uint32_t y = 0; y < name.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < name.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) + ((sy + pozycja_y) * picture_width[picture_size_index])) - center] = name.texture[x + (y * name.texture_width)];
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
            z_detail_no_need_to_think_about_it::TextBox os_y(end, 1, background_color);
            os_y.add_text(y_label, font_color);

            auto* rotated_texture = static_cast<uint32_t*>(_mm_malloc(
                sizeof(uint32_t) * os_y.texture_width * os_y.texture_height, 32));
            uint32_t rotated_width = os_y.texture_height;
            uint32_t rotated_height = os_y.texture_width;

            for (uint32_t y = 0; y < rotated_height; y++) {
                for (uint32_t x = 0; x < rotated_width; x++) {
                    rotated_texture[x + (y * rotated_width)] =
                        os_y.texture[(os_y.texture_width - 1 - y) + (x * os_y.texture_width)];
                }
            }

            uint32_t skala_textu_os_y = 16 / divider;
            pozycja_y = 0;
            center = rotated_height * skala_textu_os_y / 2;

            for (uint32_t y = 0; y < rotated_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < rotated_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[sx + pozycja_x +
                                    ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width[picture_size_index])] =
                                rotated_texture[x + y * rotated_width];
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
            z_detail_no_need_to_think_about_it::TextBox os_x(end, 1, background_color);
            os_x.add_text(x_label, font_color);

            uint32_t skala_textu_os_x = 16 / divider;
            center = os_x.texture_width * skala_textu_os_x / 2;

            pozycja_y = 0;
            for (uint32_t y = 0; y < os_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < os_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y + picture_height[picture_size_index] -
                                       os_x.texture_height * skala_textu_os_x) *
                                      picture_width[picture_size_index])) -
                                    center] = os_x.texture[x + (y * os_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_os_x;
                }
                pozycja_y += skala_textu_os_x;
            }

            // values on x
            uint32_t segments_count = 8;
            uint32_t steps_x = function_to_render->N / (float)segments_count;
            char value[32];
            uint32_t skala_textu_value_x = 8 / divider;
            for (uint32_t i = 1; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                value_x.add_text(z_detail_no_need_to_think_about_it::float_to_char(function_to_render->Tc * (float)i, value, 3), font_color);
                uint32_t length_value = 0;
                while (value[length_value] != '\n') {
                    if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                        value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                        value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                        value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                        length_value++;
                    }
                    else {
                        break;
                    }
                }
                center = (length_value * 8 * skala_textu_value_x) / 2;

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                texture[(sx + pozycja_x + scaled_uint_x[(steps_x * i) - ((steps_x * i) > 0)] +
                                         ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                        center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }
            }
            z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
            value_x.add_text(z_detail_no_need_to_think_about_it::float_to_char(function_to_render->Tc, value, 3),
                             font_color);
            uint32_t length_value = 0;
            while (value[length_value] != '\n') {
                if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                    value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                    value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                    value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                    length_value++;
                }
                else {
                    break;
                }
            }
            center = (length_value * 8 * skala_textu_value_x) / 2;
            pozycja_y = 0;
            for (uint32_t y = 0; y < value_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < value_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                            texture[(sx + pozycja_x + scaled_uint_x[function_to_render->N-1] + ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) - center] = value_x.texture[x + (y * value_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_x;
                }
                pozycja_y += skala_textu_value_x;
            }

            // values y
            uint32_t kurwa_zmienna = graph_height / segments_count;
            uint32_t skala_textu_value_y = 8 / divider;
            float step_y = (max_y - min_y) / static_cast<float>(segments_count);

            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                value_y.add_text(z_detail_no_need_to_think_about_it::float_to_char(max_y - step_y * static_cast<float>(i), value, 3), font_color);
                length_value = 0;
                while (value[length_value] != '\n') {
                    if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                        value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                        value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                        value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                        length_value++;
                    }
                    else {
                        break;
                    }
                }
                center = (length_value * 8 * skala_textu_value_y);

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_y.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_y.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                        ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                          ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                         picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }
            }
            z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
            value_y.add_text(z_detail_no_need_to_think_about_it::float_to_char(max_y - step_y * static_cast<float>(segments_count), value, 3),
                             font_color);
            length_value = 0;
            while (value[length_value] != '\n') {
                if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                    value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                    value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                    value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                    length_value++;
                }
                else {
                    break;
                }
            }
            center = (length_value * 8 * skala_textu_value_y);

            pozycja_y = 0;
            for (uint32_t y = 0; y < value_y.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < value_y.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                            texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) + ((padding_top_y + sy + pozycja_y + graph_height - ((value_y.texture_height * skala_textu_value_y) / 3)) * picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_y;
                }
                pozycja_y += skala_textu_value_y;
            }

            // grid on x
            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[(steps_x * i) - ((steps_x * i) > 0)], picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[(steps_x * i) - ((steps_x * i) > 0)], 0 + padding_top_y, grid_color, 16 / divider);
            }
            z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[function_to_render->N - 1],
                      picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[function_to_render->N - 1], 0 + padding_top_y,
                      grid_color, 16 / divider);

            // grid on y
            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i),
                          picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, 16 / divider);
            }
            z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (graph_height),
                      picture_width[picture_size_index] - padding_right_x, padding_top_y + (graph_height), grid_color, 16 / divider);


            // os x
            if (min_y <= 0.0f && max_y >= 0.0f) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;

                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, zero_y_pixel, picture_width[picture_size_index] - padding_right_x, zero_y_pixel, axis_color, 16 / divider);
            }
            // os y
            if (scaled_x[0] <= 0) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], 0 + offset_x + padding_left_x,
                          picture_height[picture_size_index] - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                          16 / divider);
            }
            // wykres
            for (uint32_t i = 0; i < function_to_render->N - 1; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i],
                          scaled_uint_x[i + 1], scaled_uint_y[i + 1], line_color, 4 / divider);
            }

            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }
        // a ten dla funkcji po DFT,
        GraphMulti(const DFT* dft_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", const char* file_path = nullptr, uint32_t background_color = ShiftDownFunctionsColorThemes::global_theme_BackGround_color, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color, uint32_t axis_color = ShiftDownFunctionsColorThemes::global_theme_Axis_color, uint32_t grid_color = ShiftDownFunctionsColorThemes::global_theme_Grid_color, uint32_t font_color = ShiftDownFunctionsColorThemes::global_theme_Font_color) {
            this->file_path = file_path;
            this->name_label = name_label;
            //math section
            uint64_t picture_size = picture_width[picture_size_index] * picture_height[picture_size_index];
            divider = picture_size_index * 2;
            divider += (divider == 0);
            uint32_t K_render = (dft_to_render->K / 2) + 1;


            texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size, 32));
            for (uint64_t i = 0; i < picture_size; i++) {
                texture[i] = background_color;
            }

            float min_y = std::numeric_limits<float>::max();
            float max_y = std::numeric_limits<float>::lowest();

            for (uint32_t i = 0; i < K_render; i++) {
                min_y = dft_to_render->mod_z[i] < min_y ? dft_to_render->mod_z[i] : min_y;
                max_y = dft_to_render->mod_z[i] > max_y ? dft_to_render->mod_z[i] : max_y;
            }

            padding_left_x = 736 / divider;
            padding_right_x = 384 / divider;

            padding_top_y = 256 / divider;
            padding_bot_y = 512 / divider;

            uint32_t graph_width = picture_width[picture_size_index] - padding_left_x - padding_right_x; // 7168px
            uint32_t graph_height = picture_height[picture_size_index] - padding_top_y - padding_bot_y; // 3552px



            scale_x = static_cast<float>(graph_width) / (dft_to_render->fk[K_render - 1] - dft_to_render->fk[0]);
            scale_y = 0;
            if (max_y == min_y) {
                scale_y = static_cast<float>(graph_height) / 1.f;
            }
            else {
                scale_y = static_cast<float>(graph_height) / (max_y - min_y);
            }

            int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));
            int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_x[i] = static_cast<int>(roundf(dft_to_render->fk[i] * scale_x));
                scaled_y[i] = -(static_cast<int>(roundf(dft_to_render->mod_z[i] * scale_y)));
            }

            int int_min_y = std::numeric_limits<int>::max();
            int int_max_y = std::numeric_limits<int>::lowest();

            for (uint32_t i = 0; i < K_render; i++) {
                int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
                int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
            }

            offset_x = scaled_x[0] * -1;
            offset_y = int_min_y * -1;

            auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));
            auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
            }


            // drawing section

            // name label
            uint32_t end = 0;
            uint32_t pozycja_y = 0;
            uint32_t center = 0;

            while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
                end++;
            }
            z_detail_no_need_to_think_about_it::TextBox name(end, 1, background_color);
            name.add_text(name_label, font_color);

            uint32_t skala_textu_name = 16 / divider;
            center = name.texture_width * skala_textu_name / 2;

            for (uint32_t y = 0; y < name.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < name.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y) * picture_width[picture_size_index])) -
                                    center] = name.texture[x + (y * name.texture_width)];
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
            z_detail_no_need_to_think_about_it::TextBox os_y(end, 1, background_color);
            os_y.add_text(y_label, font_color);

            auto* rotated_texture = static_cast<uint32_t*>(_mm_malloc(
                sizeof(uint32_t) * os_y.texture_width * os_y.texture_height, 32));
            uint32_t rotated_width = os_y.texture_height;
            uint32_t rotated_height = os_y.texture_width;

            for (uint32_t y = 0; y < rotated_height; y++) {
                for (uint32_t x = 0; x < rotated_width; x++) {
                    rotated_texture[x + (y * rotated_width)] =
                        os_y.texture[(os_y.texture_width - 1 - y) + (x * os_y.texture_width)];
                }
            }

            uint32_t skala_textu_os_y = 16 / divider;
            pozycja_y = 0;
            center = rotated_height * skala_textu_os_y / 2;

            for (uint32_t y = 0; y < rotated_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < rotated_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_y; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_y; sx++) {
                            texture[sx + pozycja_x +
                                    ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width[picture_size_index])] =
                                rotated_texture[x + y * rotated_width];
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
            z_detail_no_need_to_think_about_it::TextBox os_x(end, 1, background_color);
            os_x.add_text(x_label, font_color);

            uint32_t skala_textu_os_x = 16 / divider;
            center = os_x.texture_width * skala_textu_os_x / 2;

            pozycja_y = 0;
            for (uint32_t y = 0; y < os_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < os_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y + picture_height[picture_size_index] -
                                       os_x.texture_height * skala_textu_os_x) *
                                      picture_width[picture_size_index])) -
                                    center] = os_x.texture[x + (y * os_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_os_x;
                }
                pozycja_y += skala_textu_os_x;
            }

            // values on x
            uint32_t segments_count = 8;

            char value[32];
            uint32_t skala_textu_value_x = 8 / divider;
            uint32_t step_x = (K_render - 1) / segments_count;
            for (uint32_t i = 1; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                value_x.add_text(z_detail_no_need_to_think_about_it::float_to_char(dft_to_render->fk[step_x * i], value, 3), font_color);
                uint32_t length_value = 0;
                while (value[length_value] != '\n') {
                    if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                        value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                        value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                        value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                        length_value++;
                    }
                    else {
                        break;
                    }
                }
                center = (length_value * 8 * skala_textu_value_x) / 2;

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                texture[(sx + pozycja_x + scaled_uint_x[step_x * i] +
                                         ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                        center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }
            }
            z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
            value_x.add_text(z_detail_no_need_to_think_about_it::float_to_char(dft_to_render->fk[step_x * segments_count], value, 3),
                             font_color);
            uint32_t length_value = 0;
            while (value[length_value] != '\n') {
                if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                    value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                    value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                    value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                    length_value++;
                }
                else {
                    break;
                }
            }
            center = (length_value * 8 * skala_textu_value_x) / 2;
            pozycja_y = 0;
            for (uint32_t y = 0; y < value_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < value_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                            texture[(sx + pozycja_x + scaled_uint_x[step_x * segments_count] +
                                     ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                    center] = value_x.texture[x + (y * value_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_x;
                }
                pozycja_y += skala_textu_value_x;
            }

            // values y
            uint32_t kurwa_zmienna = graph_height / segments_count;

            uint32_t skala_textu_value_y = 8 / divider;
            float step_y = (max_y - min_y) / static_cast<float>(segments_count);

            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                value_y.add_text(z_detail_no_need_to_think_about_it::float_to_char(max_y - step_y * static_cast<float>(i), value, 3), font_color);
                length_value = 0;
                while (value[length_value] != '\n') {
                    if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                        value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                        value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                        value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                        length_value++;
                    }
                    else {
                        break;
                    }
                }
                center = (length_value * 8 * skala_textu_value_y);

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_y.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_y.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                        ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                          ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                         picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }
            }
            z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
            value_y.add_text(z_detail_no_need_to_think_about_it::float_to_char(max_y - step_y * static_cast<float>(segments_count), value, 3),
                             font_color);
            length_value = 0;
            while (value[length_value] != '\n') {
                if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                    value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                    value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                    value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                    length_value++;
                }
                else {
                    break;
                }
            }
            center = (length_value * 8 * skala_textu_value_y);

            pozycja_y = 0;
            for (uint32_t y = 0; y < value_y.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < value_y.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                            texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                    ((padding_top_y + sy + pozycja_y + graph_height -
                                      ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                     picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_y;
                }
                pozycja_y += skala_textu_value_y;
            }

            // grid on x
            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[step_x * i],
                          picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[step_x * i], 0 + padding_top_y, grid_color, 16 / divider);
            }
            z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[step_x * segments_count],
                      picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[step_x * segments_count], 0 + padding_top_y,
                      grid_color, 16 / divider);

            // grid on y
            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i),
                          picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, 16 / divider);
            }
            z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (graph_height),
                      picture_width[picture_size_index] - padding_right_x, padding_top_y + (graph_height), grid_color, 16 / divider);

            // os x
            if (min_y <= 0.0f && max_y >= 0.0f) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, zero_y_pixel, picture_width[picture_size_index] - padding_right_x, zero_y_pixel, axis_color, 16 / divider);
            }
            // os y
            if (scaled_x[0] <= 0) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], 0 + offset_x + padding_left_x,
                picture_height[picture_size_index] - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                          16 / divider);
            }
            // wykres
            for (uint32_t i = 0; i < K_render; i++) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, 4 / divider);
            }

            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }
        //  ten dla fft
        GraphMulti(const FFT* fft_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", const char* file_path = nullptr, uint32_t background_color = ShiftDownFunctionsColorThemes::global_theme_BackGround_color, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color, uint32_t axis_color = ShiftDownFunctionsColorThemes::global_theme_Axis_color, uint32_t grid_color = ShiftDownFunctionsColorThemes::global_theme_Grid_color, uint32_t font_color = ShiftDownFunctionsColorThemes::global_theme_Font_color) {
            this->file_path = file_path;
            this->name_label = name_label;
            //math section
            uint64_t picture_size = picture_width[picture_size_index] * picture_height[picture_size_index];
            divider = picture_size_index * 2;
            divider += (divider == 0);
            uint32_t K_render = (fft_to_render->K / 2) + 1;


            texture = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * picture_size, 32));
            for (uint64_t i = 0; i < picture_size; i++) {
                texture[i] = background_color;
            }

            float min_y = std::numeric_limits<float>::max();
            float max_y = std::numeric_limits<float>::lowest();

            for (uint32_t i = 0; i < K_render; i++) {
                min_y = fft_to_render->mod_z[i] < min_y ? fft_to_render->mod_z[i] : min_y;
                max_y = fft_to_render->mod_z[i] > max_y ? fft_to_render->mod_z[i] : max_y;
            }

            padding_left_x = 736 / divider; //736
            padding_right_x = 384 / divider; //384

            padding_top_y = 256 / divider; //256
            padding_bot_y = 512 / divider; //512

            uint32_t graph_width = picture_width[picture_size_index] - padding_left_x - padding_right_x; // 7168px
            uint32_t graph_height = picture_height[picture_size_index] - padding_top_y - padding_bot_y; // 3552px



            scale_x = static_cast<float>(graph_width) / (fft_to_render->fk[K_render - 1] - fft_to_render->fk[0]);
            scale_y = 0;
            if (max_y == min_y) {
                scale_y = static_cast<float>(graph_height) / 1.f;
            }
            else {
                scale_y = static_cast<float>(graph_height) / (max_y - min_y);
            }

            int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));
            int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_x[i] = static_cast<int>(roundf(fft_to_render->fk[i] * scale_x));
                scaled_y[i] = -(static_cast<int>(roundf(fft_to_render->mod_z[i] * scale_y)));
            }

            int int_min_y = std::numeric_limits<int>::max();
            int int_max_y = std::numeric_limits<int>::lowest();

            for (uint32_t i = 0; i < K_render; i++) {
                int_min_y = scaled_y[i] < int_min_y ? scaled_y[i] : int_min_y;
                int_max_y = scaled_y[i] > int_max_y ? scaled_y[i] : int_max_y;
            }

            offset_x = scaled_x[0] * -1;
            offset_y = int_min_y * -1;

            auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));
            auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
            }


            // drawing section

            // name label
            uint32_t end = 0;
            uint32_t pozycja_y = 0;
            uint32_t center = 0;

            while ((name_label[end] != '\0') && (name_label[end] != '\n')) {
                end++;
            }
            z_detail_no_need_to_think_about_it::TextBox name(end, 1, background_color);
            name.add_text(name_label, font_color);

            uint32_t skala_textu_name = 16 / divider;
            center = name.texture_width * skala_textu_name / 2;

            for (uint32_t y = 0; y < name.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < name.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y) * picture_width[picture_size_index])) -
                                    center] = name.texture[x + (y * name.texture_width)];
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
            z_detail_no_need_to_think_about_it::TextBox os_y(end, 1, background_color);
            os_y.add_text(y_label, font_color);

            auto* rotated_texture = static_cast<uint32_t*>(_mm_malloc(
                sizeof(uint32_t) * os_y.texture_width * os_y.texture_height, 32));
            uint32_t rotated_width = os_y.texture_height;
            uint32_t rotated_height = os_y.texture_width;

            for (uint32_t y = 0; y < rotated_height; y++) {
                for (uint32_t x = 0; x < rotated_width; x++) {
                    rotated_texture[x + (y * rotated_width)] =
                        os_y.texture[(os_y.texture_width - 1 - y) + (x * os_y.texture_width)];
                }
            }

            uint32_t skala_textu_os_y = 16 / divider;
            pozycja_y = 0;
            center = rotated_height * skala_textu_os_y / 2;

            for (uint32_t y = 0; y < rotated_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < rotated_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_y; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_y; sx++) {
                            texture[sx + pozycja_x +
                                    ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width[picture_size_index])] =
                                rotated_texture[x + y * rotated_width];
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
            z_detail_no_need_to_think_about_it::TextBox os_x(end, 1, background_color);
            os_x.add_text(x_label, font_color);

            uint32_t skala_textu_os_x = 16 / divider;
            center = os_x.texture_width * skala_textu_os_x / 2;

            pozycja_y = 0;
            for (uint32_t y = 0; y < os_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < os_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y + picture_height[picture_size_index] -
                                       os_x.texture_height * skala_textu_os_x) *
                                      picture_width[picture_size_index])) -
                                    center] = os_x.texture[x + (y * os_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_os_x;
                }
                pozycja_y += skala_textu_os_x;
            }

            // values on x
            uint32_t segments_count = 8;

            char value[32];
            uint32_t skala_textu_value_x = 8 / divider;
            uint32_t step_x = ((K_render - 1) / segments_count);
            for (uint32_t i = 1; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
                value_x.add_text(z_detail_no_need_to_think_about_it::float_to_char(fft_to_render->fk[step_x * i], value, 3), font_color);
                uint32_t length_value = 0;
                while (value[length_value] != '\n') {
                    if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                        value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                        value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                        value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                        length_value++;
                    }
                    else {
                        break;
                    }
                }
                center = (length_value * 8 * skala_textu_value_x) / 2;

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_x.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_x.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                                texture[(sx + pozycja_x + scaled_uint_x[step_x * i] + ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) - center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }
            }
            z_detail_no_need_to_think_about_it::TextBox value_x(7, 1, background_color);
            value_x.add_text(z_detail_no_need_to_think_about_it::float_to_char(fft_to_render->fk[step_x * segments_count], value, 3),
                             font_color);
            uint32_t length_value = 0;
            while (value[length_value] != '\n') {
                if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                    value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                    value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                    value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                    length_value++;
                }
                else {
                    break;
                }
            }
            center = (length_value * 8 * skala_textu_value_x) / 2;
            pozycja_y = 0;
            for (uint32_t y = 0; y < value_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < value_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_value_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_value_x; sx++) {
                            texture[(sx + pozycja_x + scaled_uint_x[step_x * segments_count] +
                                     ((sy + pozycja_y + padding_top_y + graph_height + (64 / divider)) * picture_width[picture_size_index])) -
                                    center] = value_x.texture[x + (y * value_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_x;
                }
                pozycja_y += skala_textu_value_x;
            }

            // values y
            uint32_t kurwa_zmienna = graph_height / segments_count;

            uint32_t skala_textu_value_y = 8 / divider;
            float step_y = (max_y - min_y) / static_cast<float>(segments_count);

            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
                value_y.add_text(z_detail_no_need_to_think_about_it::float_to_char(max_y - step_y * static_cast<float>(i), value, 3), font_color);
                length_value = 0;
                while (value[length_value] != '\n') {
                    if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                        value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                        value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                        value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                        length_value++;
                    }
                    else {
                        break;
                    }
                }
                center = (length_value * 8 * skala_textu_value_y);

                pozycja_y = 0;
                for (uint32_t y = 0; y < value_y.texture_height; y++) {
                    uint32_t pozycja_x = 0;
                    for (uint32_t x = 0; x < value_y.texture_width; x++) {
                        for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                            for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                                texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) +
                                        ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                          ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                         picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }
            }
            z_detail_no_need_to_think_about_it::TextBox value_y(7, 1, background_color);
            value_y.add_text(z_detail_no_need_to_think_about_it::float_to_char(max_y - step_y * static_cast<float>(segments_count), value, 3),
                             font_color);
            length_value = 0;
            while (value[length_value] != '\n') {
                if (value[length_value] == '.' || value[length_value] == '-' || value[length_value] == '0' ||
                    value[length_value] == '1' || value[length_value] == '2' || value[length_value] == '3' ||
                    value[length_value] == '4' || value[length_value] == '5' || value[length_value] == '6' ||
                    value[length_value] == '7' || value[length_value] == '8' || value[length_value] == '9') {
                    length_value++;
                }
                else {
                    break;
                }
            }
            center = (length_value * 8 * skala_textu_value_y);

            pozycja_y = 0;
            for (uint32_t y = 0; y < value_y.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < value_y.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_value_y; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_value_y; sx++) {
                            texture[(padding_left_x + sx + pozycja_x - center - (64 / divider)) + ((padding_top_y + sy + pozycja_y + graph_height - ((value_y.texture_height * skala_textu_value_y) / 3)) * picture_width[picture_size_index])] = value_y.texture[x + (y * value_y.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_y;
                }
                pozycja_y += skala_textu_value_y;
            }

            // grid on x
            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[step_x * i],
                          picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[step_x * i], 0 + padding_top_y, grid_color, 16 / divider);
            }
            z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[step_x * segments_count],
                      picture_height[picture_size_index] - padding_bot_y, scaled_uint_x[step_x * segments_count], 0 + padding_top_y,
                      grid_color, 16 / divider);

            // grid on y
            for (uint32_t i = 0; i < segments_count; i++) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (kurwa_zmienna * i),
                          picture_width[picture_size_index] - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, 16 / divider);
            }
            z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, padding_top_y + (graph_height),
                      picture_width[picture_size_index] - padding_right_x, padding_top_y + (graph_height), grid_color, 16 / divider);

            // os x
            if (min_y <= 0.0f && max_y >= 0.0f) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], padding_left_x, zero_y_pixel, picture_width[picture_size_index] - padding_right_x, zero_y_pixel, axis_color, 16 / divider);
            }
            // os y
            if (scaled_x[0] <= 0) {
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], 0 + offset_x + padding_left_x,
                picture_height[picture_size_index] - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                          16 / divider);
            }
            // wykres
            for (uint32_t i = 0; i < K_render; i++) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, 4 / divider);
            }

            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }

        ~GraphMulti() {
            if (multigraph_thread.joinable()) {
                multigraph_thread.join();
            }
            _mm_free(texture);
        };
        // tego używamy, aby dodać funkcję do wykresu
        void add(const Function* function_to_render, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color) {
            // wykres
            int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * function_to_render->N, 32));
            int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * function_to_render->N, 32));

            for (uint32_t i = 0; i < function_to_render->N; i++) {
                scaled_x[i] = static_cast<int>(roundf(function_to_render->t[i] * scale_x));
                scaled_y[i] = -(static_cast<int>(roundf(function_to_render->f_t[i] * scale_y)));
            }

            auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * function_to_render->N, 32));
            auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * function_to_render->N, 32));

            for (uint32_t i = 0; i < function_to_render->N; i++) {
                scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
            }

            for (uint32_t i = 0; i < function_to_render->N - 1; i++) {
                if (scaled_uint_x[i] > picture_width[picture_size_index] - padding_right_x || scaled_uint_x[i] < padding_left_x) continue;
                if (scaled_uint_y[i] > picture_height[picture_size_index] - padding_bot_y || scaled_uint_y[i] < padding_top_y) continue;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i + 1], scaled_uint_y[i + 1], line_color, 4 / divider);
            }
            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }
        void add(const DFT* dft_to_render, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color) {
            uint32_t K_render = (dft_to_render->K / 2) + 1;
            // wykres
            int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));
            int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_x[i] = static_cast<int>(roundf(dft_to_render->fk[i] * scale_x));
                scaled_y[i] = -(static_cast<int>(roundf(dft_to_render->mod_z[i] * scale_y)));
            }

            auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));
            auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
            }

            for (uint32_t i = 0; i < K_render; i++) {
                if (scaled_uint_x[i] > picture_width[picture_size_index] - padding_right_x || scaled_uint_x[i] < padding_left_x) continue;
                if (scaled_uint_y[i] > picture_height[picture_size_index] - padding_bot_y || scaled_uint_y[i] < padding_top_y) continue;
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, 4 / divider);
            }
            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }
        void add(const FFT* fft_to_render, uint32_t line_color = ShiftDownFunctionsColorThemes::global_theme_Line_color) {
            uint32_t K_render = (fft_to_render->K / 2) + 1;
            // wykres
            int* scaled_x = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));
            int* scaled_y = static_cast<int*>(_mm_malloc(sizeof(int) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_x[i] = static_cast<int>(roundf(fft_to_render->fk[i] * scale_x));
                scaled_y[i] = -(static_cast<int>(roundf(fft_to_render->mod_z[i] * scale_y)));
            }

            auto* scaled_uint_x = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));
            auto* scaled_uint_y = static_cast<uint32_t*>(_mm_malloc(sizeof(uint32_t) * K_render, 32));

            for (uint32_t i = 0; i < K_render; i++) {
                scaled_uint_x[i] = scaled_x[i] + offset_x + padding_left_x;
                scaled_uint_y[i] = scaled_y[i] + offset_y + padding_top_y;
            }

            for (uint32_t i = 0; i < K_render; i++) {
                if (scaled_uint_x[i] > picture_width[picture_size_index] - padding_right_x || scaled_uint_x[i] < padding_left_x) continue;
                if (scaled_uint_y[i] > picture_height[picture_size_index] - padding_bot_y || scaled_uint_y[i] < padding_top_y) continue;
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                z_detail_no_need_to_think_about_it::draw_line(texture, picture_width[picture_size_index], picture_height[picture_size_index], scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, 4 / divider);
            }
            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }

        // to zapisuje przygotowany multi grapg na dysk jako zdjęcie
        void GenerateGraphMulti() {
            multigraph_thread = std::thread([this]() {
                z_detail_no_need_to_think_about_it::save_texture_to_file(texture, picture_width[picture_size_index], picture_height[picture_size_index], file_path);
            });
        }
    };
#pragma endregion

#pragma region Funkcje modulujące

    /* Krótkie wprowadzenie co to i po co to.

     Modulacja polega na wsadzeniu sygnału o niskiej częstotliwości do innego sygnału o bardzo
     wysokiej częstotliwości (tak zwanej nośnej), by móc wysłać go na większe odległości,

     Modulować funkcje modulujemy po 3 wartościach:
        - Amplitudzie [A] -> AM, podatna na szumy i burze.
        - Częstotliwości [f] -> FM, odporna na burze,
        - Fazy [PHI] -> PM, pozwala upchać najwiecej danych w jednym cyklu

    Modulacje zaimplementowane przeze mnie to:
        - sinusoidalna (SIN) -> fala jest gładka, nie generuje niepotrzebnych zakłócających harmonicznych.
        - prostokątna (REC) -> głównie dla sygnałów cyfrowych, zamiast fali pracuje bardziej na stanach  jak 0, 1
        - trójkątna (TRI) i piłokształtna (SAW) -> działa na zasadzie gdzie częstotliwość wchodzi powoli do góry, a potem natychmiast spada do wartości początkowej, stosowane w CRT, radarach FMCW

    W skrócie, mała funkcja pakowana w pociąg, wysyłana w świat, odbierana dekodowana i mamy informacje w niej zawarte setki kilometrów dalej.
     */

    // --- MODULACJA SINUSOIDALNA (SIN) ---
    inline Function* modulate_AM_sin(const Function* ftm, float A, float fs, float PHI) {

        auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);

        AM->A = A;
        AM->fs = fs;
        AM->PHI = PHI;

        for (uint64_t t = 0; t < AM->N; t++) {
            // dodajemy wartości naszej funkcji do amplitudy [A] sygnału
            AM->f_t[t] = (A + ftm->f_t[t]) * sinf(2.f * M_PIf * fs * AM->t[t] + PHI);
        }

        return AM;
    }
    inline Function* modulate_PM_sin(const Function* ftm, float A, float fs, float PHI) {

        auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        PM->A = A;
        PM->fs = fs;
        PM->PHI = PHI;


        for (uint64_t t = 0; t < PM->N; t++) {
            // dodajemy wartości naszej funkcji do fazy [PHI] sygnału
            PM->f_t[t] = A * sinf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI);
        }

        return PM;
    }
    inline Function* modulate_FM_sin(const Function* ftm, float A, float fs, float PHI) {
        auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        FM->A = A;
        FM->fs = fs;
        FM->PHI = PHI;

        float integral = 0.f;
        for (uint64_t t = 0; t < FM->N; t++) {
            // wyliczamy całkę (integral) z naszej funkcji, a potem ładujemy do częstotliwości
            integral += ftm->f_t[t] * ftm->Ts;
            FM->f_t[t] = A * sinf(2.f * M_PIf * fs * FM->t[t] + 2.f * M_PIf * integral + PHI);
        }
        return FM;
    }

    // --- MODULACJA PIŁOKSZTAŁTNA (SAW) ---
    inline Function* modulate_AM_saw(const Function* ftm, float A, float fs, float PHI) {
        auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        AM->A = A;
        AM->fs = fs;
        AM->PHI = PHI;
        for (uint64_t t = 0; t < AM->N; t++) {
            // angle to faza, rośnie w nieskończoność, fmodf to modulo dla liczb zmiennoprzecinkowych, kiedy faza dobije 2PI, to resetuje się do 0 i tak w kółko
            // stąd pojawiają się te kształty, funkcja rośnie, potem obraca się z powrotem na 0
            float angle = fmodf(2.f * M_PIf * fs * AM->t[t] + PHI, 2.f * M_PIf);
            if (angle < 0.f) angle += 2.f * M_PIf;
            float carrier = (angle / M_PIf) - 1.f;
            AM->f_t[t] = (A + ftm->f_t[t]) * carrier;
        }
        return AM;
    }
    inline Function* modulate_PM_saw(const Function* ftm, float A, float fs, float PHI) {
        auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        PM->A = A;
        PM->fs = fs;
        PM->PHI = PHI;
        for (uint64_t t = 0; t < PM->N; t++) {
            float angle = fmodf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI, 2.f * M_PIf);
            if (angle < 0.f) angle += 2.f * M_PIf;
            float carrier = (angle / M_PIf) - 1.f;
            PM->f_t[t] = A * carrier;
        }
        return PM;
    }
    inline Function* modulate_FM_saw(const Function* ftm, float A, float fs, float PHI) {
        auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        FM->A = A;
        FM->fs = fs;
        FM->PHI = PHI;
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

    // --- MODULACJA PROSTOKĄTNA (REC) ---
    inline Function* modulate_AM_rec(const Function* ftm, float A, float fs, float PHI) {
        auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        AM->A = A;
        AM->fs = fs;
        AM->PHI = PHI;
        for (uint64_t t = 0; t < AM->N; t++) {
            float angle = fmodf(2.f * M_PIf * fs * AM->t[t] + PHI, 2.f * M_PIf);
            if (angle < 0.f) angle += 2.f * M_PIf;
            float carrier = (angle < M_PIf) ? 1.f : -1.f;
            AM->f_t[t] = (A + ftm->f_t[t]) * carrier;
        }
        return AM;
    }
    inline Function* modulate_PM_rec(const Function* ftm, float A, float fs, float PHI) {
        auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        PM->A = A;
        PM->fs = fs;
        PM->PHI = PHI;
        for (uint64_t t = 0; t < PM->N; t++) {
            float angle = fmodf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI, 2.f * M_PIf);
            if (angle < 0.f) angle += 2.f * M_PIf;
            float carrier = (angle < M_PIf) ? 1.f : -1.f;
            PM->f_t[t] = A * carrier;
        }
        return PM;
    }
    inline Function* modulate_FM_rec(const Function* ftm, float A, float fs, float PHI) {
        auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        FM->A = A;
        FM->fs = fs;
        FM->PHI = PHI;
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

    // --- MODULACJA TRÓJKĄTNA (TRI) ---
    inline Function* modulate_AM_tri(const Function* ftm, float A, float fs, float PHI) {
        auto* AM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        AM->A = A;
        AM->fs = fs;
        AM->PHI = PHI;
        for (uint64_t t = 0; t < AM->N; t++) {
            float angle = fmodf(2.f * M_PIf * fs * AM->t[t] + PHI, 2.f * M_PIf);
            if (angle < 0.f) angle += 2.f * M_PIf;
            float carrier = (angle < M_PIf) ? (-1.f + 2.f * angle / M_PIf) : (3.f - 2.f * angle / M_PIf);
            AM->f_t[t] = (A + ftm->f_t[t]) * carrier;
        }
        return AM;
    }
    inline Function* modulate_PM_tri(const Function* ftm, float A, float fs, float PHI) {
        auto* PM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        PM->A = A;
        PM->fs = fs;
        PM->PHI = PHI;
        for (uint64_t t = 0; t < PM->N; t++) {
            float angle = fmodf(2.f * M_PIf * fs * PM->t[t] + ftm->f_t[t] + PHI, 2.f * M_PIf);
            if (angle < 0.f) angle += 2.f * M_PIf;
            float carrier = (angle < M_PIf) ? (-1.f + 2.f * angle / M_PIf) : (3.f - 2.f * angle / M_PIf);
            PM->f_t[t] = A * carrier;
        }
        return PM;
    }
    inline Function* modulate_FM_tri(const Function* ftm, float A, float fs, float PHI) {
        auto* FM = new Function(ftm->Tc, ftm->fs, ftm->f, ftm->PHI, ftm->A, ftm->function_formula);
        FM->A = A;
        FM->fs = fs;
        FM->PHI = PHI;
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



    // --- MODULACJA/DEMODULACJA CYFROWA ---
    inline Function* modulate_ASK(const uint8_t* bits, float bitrate, float A_HIGH, float A_LOW, float f, float fs, float PHI = 0.f) {
        uint64_t bit_count = 0;
        while (bits[bit_count] != 0xD) {
            bit_count++;
        }
        float Tc = static_cast<float>(bit_count) / bitrate;
        auto* ASK = new Function(Tc, fs, f, PHI, A_HIGH, [](const Function&, uint64_t, float, void*, void*, void*, void*) -> float {
            return 0.f;
        });

        for (uint64_t n = 0; n < ASK->N; n++) {
            uint64_t current_bit = static_cast<uint64_t>(ASK->t[n] * bitrate);
            if (current_bit >= bit_count) current_bit = bit_count - 1;
            float bit_val = (bits[current_bit] > 0) ? A_HIGH : A_LOW;
            ASK->f_t[n] = bit_val * sinf(2.f * M_PIf * f * ASK->t[n] + PHI);
        }
        return ASK;
    }
    inline Function* modulate_PSK(const uint8_t* bits, float bitrate, float A, float f, float fs, float PHI_HIGH, float PHI_LOW, float PHI = 0.f) {
        uint64_t bit_count = 0;
        while (bits[bit_count] != 0xD) {
            bit_count++;
        }
        float Tc = static_cast<float>(bit_count) / bitrate;
        auto* PSK = new Function(Tc, fs, f, PHI, A, [](const Function&, uint64_t, float, void*, void*, void*, void*) -> float {
            return 0.f;
        });

        for (uint64_t n = 0; n < PSK->N; n++) {
            uint64_t current_bit = static_cast<uint64_t>(PSK->t[n] * bitrate);
            if (current_bit >= bit_count) current_bit = bit_count - 1;
            float phi_current = (bits[current_bit] > 0) ? PHI_HIGH : PHI_LOW;
            PSK->f_t[n] = A * sinf(2.f * M_PIf * f * PSK->t[n] + phi_current);
        }
        return PSK;
    }
    inline Function* modulate_FSK(const uint8_t* bits, float bitrate, float A, float f_HIGH, float f_LOW, float fs, float PHI = 0.f) {
        uint64_t bit_count = 0;
        while (bits[bit_count] != 0xD) {
            bit_count++;
        }
        float Tc = static_cast<float>(bit_count) / bitrate;
        auto* FSK = new Function(Tc, fs, f_HIGH, PHI, A, [](const Function&, uint64_t, float, void*, void*, void*, void*) -> float {
            return 0.f;
        });

        for (uint64_t n = 0; n < FSK->N; n++) {
            uint64_t current_bit = static_cast<uint64_t>(FSK->t[n] * bitrate);
            if (current_bit >= bit_count) current_bit = bit_count - 1;
            float f_current = (bits[current_bit] > 0) ? f_HIGH : f_LOW;
            FSK->f_t[n] = A * sinf(2.f * M_PIf * f_current * FSK->t[n] + PHI);
        }
        return FSK;
    }


    inline uint8_t* demodulate_ASK(const Function* sig, float bitrate, float f_carrier, float A_HIGH, float A_LOW) {
    uint64_t bit_count = static_cast<uint64_t>(sig->Tc * bitrate + 0.1f);
    uint64_t samples_per_bit = static_cast<uint64_t>(sig->fs / bitrate);

    uint8_t* demod_bits = (uint8_t*)_mm_malloc(bit_count + 1, 64);

    float threshold = 0.25f * (A_HIGH + A_LOW) * static_cast<float>(samples_per_bit);

    for (uint64_t b = 0; b < bit_count; b++) {
        float sum = 0.f;
        for (uint64_t s = 0; s < samples_per_bit; s++) {
            uint64_t n = b * samples_per_bit + s;
            if (n >= sig->N) break;
            sum += sig->f_t[n] * sinf(2.f * M_PIf * f_carrier * sig->t[n]);
        }
        demod_bits[b] = (sum > threshold) ? 1 : 0;
    }
    demod_bits[bit_count] = 0xD;
    return demod_bits;
}
    inline uint8_t* demodulate_PSK(const Function* sig, float bitrate, float f_carrier, float PHI_HIGH, float PHI_LOW) {
    uint64_t bit_count = static_cast<uint64_t>(sig->Tc * bitrate + 0.1f);
    uint64_t samples_per_bit = static_cast<uint64_t>(sig->fs / bitrate);

    uint8_t* demod_bits = (uint8_t*)_mm_malloc(bit_count + 1, 64);

    for (uint64_t b = 0; b < bit_count; b++) {
        float sum_high = 0.f;
        float sum_low = 0.f;
        for (uint64_t s = 0; s < samples_per_bit; s++) {
            uint64_t n = b * samples_per_bit + s;
            if (n >= sig->N) break;
            sum_high += sig->f_t[n] * sinf(2.f * M_PIf * f_carrier * sig->t[n] + PHI_HIGH);
            sum_low  += sig->f_t[n] * sinf(2.f * M_PIf * f_carrier * sig->t[n] + PHI_LOW);
        }
        demod_bits[b] = (sum_high > sum_low) ? 1 : 0;
    }
    demod_bits[bit_count] = 0xD;
    return demod_bits;
}
    inline uint8_t* demodulate_FSK(const Function* sig, float bitrate, float f_HIGH, float f_LOW, float PHI = 0.f) {
    uint64_t bit_count = static_cast<uint64_t>(sig->Tc * bitrate + 0.1f);
    uint64_t samples_per_bit = static_cast<uint64_t>(sig->fs / bitrate);

    uint8_t* demod_bits = (uint8_t*)_mm_malloc(bit_count + 1, 64);

    for (uint64_t b = 0; b < bit_count; b++) {
        float sum_high = 0.f;
        float sum_low = 0.f;
        for (uint64_t s = 0; s < samples_per_bit; s++) {
            uint64_t n = b * samples_per_bit + s;
            if (n >= sig->N) break;
            sum_high += sig->f_t[n] * sinf(2.f * M_PIf * f_HIGH * sig->t[n] + PHI);
            sum_low  += sig->f_t[n] * sinf(2.f * M_PIf * f_LOW * sig->t[n] + PHI);
        }
        demod_bits[b] = (sum_high > sum_low) ? 1 : 0;
    }
    demod_bits[bit_count] = 0xD;
    return demod_bits;
}


#pragma endregion

#pragma region Funkcje kodujące

    struct alignas(64) HammingCoder {

        uint64_t n = 0;
        uint8_t* coded_bits = nullptr; // ostatni index tablicy to 0xD i oznacza on koniec bufora, iteracja do coded_bits[i] == 0xD;

        HammingCoder(const void* Word_to_code, uint64_t m = 4) {

            char* word_to_code = (char*)Word_to_code;

            uint64_t size = 0;
            while (word_to_code[size] != '\0') {
                size++;
            }
            size = size * 8;
            uint8_t* bits_to_code = (uint8_t*)_mm_malloc((sizeof(uint8_t) * size) + 1, 64);
            bits_to_code[size] = 0xD;
            for (uint64_t i = 0; word_to_code[i] != '\0'; i++) {
                uint64_t offset = i * 8;
                for (uint8_t j = 0; j < 8; j++) {
                    bits_to_code[7 + offset - j] = (word_to_code[i] & (1<<j)) > 0 ;
                }
            }

            size = 0;
        {
            uint64_t temp = m;
            while (temp != 0) {
                temp >>= 1;
                n++;
            }
            // 2^n >= n + m + 1 // warunek
            // n = n + m // rachunek
            n = (n + ((1<<n) < (n + m + 1) ? 1 : 0)) + m;

            uint64_t frame_size = 0;

            for (uint64_t i = 0; bits_to_code[i] != 0xD; i++) {
                size++;
                frame_size += (i % m == 0);
            }
            frame_size = (frame_size + (frame_size * m < size != 0)) * n;

            // + 2 bo index 0 jest pusty oraz 0xD jako znak konca bufora
            coded_bits = (uint8_t*)_mm_malloc(sizeof(uint8_t) * frame_size + 2, 64);
            coded_bits[frame_size + 1] = 0xD;
            for (uint64_t i = 1; coded_bits[i] != 0xD; i++) coded_bits[i] = 0;
            coded_bits[0] = n - m;
        }

            // kodowanie po przez xorowanie
            uint64_t frame_count = (((size % m) == 0) ? (size / m) : ((size / m) + 1));
            uint64_t bits_to_code_index = 0;

            for (uint64_t j = 0; j < frame_count; j++) {
                uint64_t offset = n * j;
                uint64_t parity_bits_count = 0;
                for (uint64_t i = 1; i <= n; i++) {
                    coded_bits[i + offset] = (i & (1 << (parity_bits_count))) == 0 ? (bits_to_code_index++, bits_to_code[bits_to_code_index - 1]) : (parity_bits_count++,0);
                }

                for (uint64_t k = 0; k < coded_bits[0]; k++) {
                    for (uint64_t d = 1; d <= n; d++) {
                        coded_bits[offset + (1 << k)] = ((d & (1<<k)) != 0) ? (coded_bits[offset + (1 << k)] ^ coded_bits[offset + d]) : coded_bits[offset + (1 << k)];
                    }
                }
            }
        }
        ~HammingCoder() {
            _mm_free(coded_bits);
        }
    };

    struct alignas(64) HammingDecoder{

        uint8_t* decoded_bits = nullptr;

        HammingDecoder(uint8_t* bits_to_decode, uint64_t m = 4) {
            // zakladamy że bits_to_decode zaczynaja sie od idx[1] i koncza formulka 0xD
            uint64_t n = 0;

            // zanjdoywanie dlugosci ramki i tak dalej
            uint64_t temp = m;
            while (temp != 0) {
                temp >>= 1;
                n++;
            }
            n = (n + ((1<<n) < (n + m + 1) ? 1 : 0)) + m;

            //alokacja rozmiaru dekodowanych bitow
            uint64_t size = 0;
            for (uint64_t i = 1; bits_to_decode[i] != 0xD; i++) {
                size++;
            }
            // alokuje pamiec dla ilosci tego ile mam bajtow, do tego dodaje jeszcze telemetrie, 8 bajtow, i do tego dodaje jeszcze 1 bajt znaku konca
            uint64_t frame_count = (size + n - 1) / n;
            uint64_t malloc_size = ((frame_count * m) / 8) + 6;
            decoded_bits = (uint8_t*)_mm_malloc(sizeof(uint8_t) * malloc_size, 64);
            for (uint64_t i = 0; i < malloc_size; i++) {
                decoded_bits[i] = 0;
            }

            // ----

            // dekodowanie

            uint64_t decoded_bits_index_global = 4;
            uint64_t decoded_bits_index_local = 0;
            float errors = 0;

            for (uint64_t j = 0; j < frame_count; j++) {
                uint64_t offset = n * j;
                uint64_t Z = 0;
                uint64_t parity_bits_count = 0;
                for (uint64_t i = 1; i <= n; i++) {
                    Z = bits_to_decode[offset + i] > 0 ? Z^i : Z;
                    decoded_bits[decoded_bits_index_global] = (i & (1 << parity_bits_count)) != 0 ? (parity_bits_count++, decoded_bits[decoded_bits_index_global]) : (decoded_bits_index_local++, decoded_bits[decoded_bits_index_global] | (bits_to_decode[offset + i] << (8 - decoded_bits_index_local)));
                    decoded_bits_index_local = decoded_bits_index_local == 8 ? (decoded_bits_index_global++, 0) : decoded_bits_index_local;
                }
                //naprawa zepsutego bitu
                uint64_t err_bit = m * j + Z - 2 - (63 - __builtin_clzll(Z));
                errors = ((Z & (Z - 1)) == 0) ? errors : (decoded_bits[4 + (err_bit >> 3)] ^= 1 << (7 - (err_bit & 7)), errors + 1);
            }
            errors = errors / (float)size;
            *(reinterpret_cast<float*>(decoded_bits)) = errors;
            decoded_bits[malloc_size - 1] = '\0';
        }
        ~HammingDecoder() {
            _mm_free(decoded_bits);
        }
    };
#pragma endregion

} // namespace ShiftDownFunctions
