/*
    Jak dodać do swojego projektu / How to add to your own project

    A. Można pobrać ShiftDownFunctions.hpp i dodać do projektu / Just download the ShiftDownFunctions.hpp file and add to project

    B. Dodać to do swojego CMakeLists.txt / Add this to CMakeLists.txt

    include(FetchContent)
    FetchContent_Declare(ShiftDownFunctions GIT_REPOSITORY https://github.com/DeadDeth/ShiftDownFunctions.git GIT_TAG main)
    FetchContent_MakeAvailable(ShiftDownFunctions)
    target_link_libraries({PROJECT_NAME} PRIVATE ShiftDownFunctions)

    To powinno, przynajmniej dla Clion, pobrać wszystko automatycznie z githuba. / This should automatically download the file, at least for Clion, from github.
*/


#pragma once
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <immintrin.h>
#ifdef __linux__
#include <sys/stat.h>
#endif


// predefiniowane kolory jako ARGB
namespace Colors {
//  ARGB -> ALPHA, RED, GREEN, BLUE -> 0xAARRGGBB (rozkład w hexa-decymalnym)
//  Przykład: Colors::RED -> to daje czerwony jako uint32_t, czyli po prostu 0xFFFF0000 w hexa-decymalnym ARGB
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
constexpr ARGB RED     = {255, 0, 0};
constexpr ARGB GREEN   = {0, 255, 0};
constexpr ARGB BLUE    = {0, 0, 255};
constexpr ARGB WHITE   = {255, 255, 255};
constexpr ARGB BLACK   = {0, 0, 0};
constexpr ARGB GRAY    = {128, 128, 128};
constexpr ARGB YELLOW  = {255, 255, 0};
constexpr ARGB CYAN    = {0, 255, 255};
constexpr ARGB MAGENTA = {255, 0, 255};

// --- PRZYDATNE DO WYKRESÓW (Jaskrawe / Kontrastowe) ---
constexpr ARGB ORANGE       = {255, 165, 0};
constexpr ARGB PINK         = {255, 192, 203};
constexpr ARGB HOT_PINK     = {255, 105, 180};
constexpr ARGB LIME         = {50, 205, 50};
constexpr ARGB TEAL         = {0, 128, 128};
constexpr ARGB PURPLE       = {128, 0, 128};
constexpr ARGB VIOLET       = {238, 130, 238};
constexpr ARGB GOLD         = {255, 215, 0};
constexpr ARGB CORAL        = {255, 127, 80};
constexpr ARGB CRIMSON      = {220, 20, 60};
constexpr ARGB AQUA         = {0, 255, 255};
constexpr ARGB AMBER        = {255, 191, 0};

// --- TŁA / UI / TRYB CIEMNY ---
constexpr ARGB DARK_GRAY    = {64, 64, 64};
constexpr ARGB LIGHT_GRAY   = {192, 192, 192};
constexpr ARGB SLATE_GRAY   = {112, 128, 144};
constexpr ARGB NAVY         = {0, 0, 128};
constexpr ARGB MIDNIGHT_BLUE= {25, 25, 112};
constexpr ARGB CHARCOAL     = {54, 69, 79};
constexpr ARGB MATTE_BLACK  = {28, 28, 28};
constexpr ARGB DARK_PURPLE  = {42, 0, 77};
constexpr ARGB BLOOD_RED    = {138, 3, 3};
constexpr ARGB FOREST_GREEN = {34, 139, 34};
constexpr ARGB OLIVE        = {128, 128, 0};
constexpr ARGB CHOCOLATE    = {210, 105, 30};

// --- PASTELOWE (Do wypełnień pod wykresem) ---
constexpr ARGB PASTEL_RED   = {255, 105, 97};
constexpr ARGB PASTEL_GREEN = {119, 221, 119};
constexpr ARGB PASTEL_BLUE  = {174, 198, 207};
constexpr ARGB PASTEL_YELLOW= {253, 253, 150};
constexpr ARGB PASTEL_PURPLE= {179, 158, 181};

// --- PRZEZROCZYSTE (Z obniżonym kanałem Alpha) ---
constexpr ARGB TRANSPARENT  = {0, 0, 0, 0};
constexpr ARGB SEMI_BLACK   = {0, 0, 0, 128};
constexpr ARGB SEMI_WHITE   = {255, 255, 255, 128};
constexpr ARGB GLASS_RED    = {255, 0, 0, 128};
constexpr ARGB GLASS_BLUE   = {0, 0, 255, 128};
constexpr ARGB GLASS_GREEN  = {0, 255, 0, 128};
}

namespace ShiftDownFunctions
{
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
        ale nie miałem na to ochoty, więc jeśli komuś przeszkadza można w funkcji rysującej Graph, sekcja values on x i values on y, dodać bramke przed wysłaniem wartości do funkcji float_to_char, która wyrówna wartości
        do tego co powinno być, problem nie pojawia się zawsze ale czasem, i jest to specyfika działania tego tworu, ja mówie że to funkcjonalność, może kiedyś sam poprawie w wolnym czasie.

        Windows, a Linux. Domyślnie wszystko jest napisane i przetestowane na Linux Fedora 43, działa bez problemu, Na windows też powinno ale nic nie mogę obiecać, niech się cieszą że w ogole mi się chciało o nich pamiętać
        Zdjęcia są w 8k robione, skalowanie z poziomu systemu nie programu ze względu na dosyć leniwe podejście do statycznie wyliczanych odległości i zależności między elementami wykresu w celu osiągnięcia najbardziej
        satysfakcjonujących rezultatów, gotowe zdjęcie można już skalować do woli. Pliki .png nie są tragiczne w rozmiarach, wahają się w zależności od wyglądu wykresu, .bmp natomiast to sztywne około 130MB, i uwaga
        tutaj mówię o miejscu na dysku, w ramie i procesie tworzenia, przekraczamy te 130MB na funkcje, prosty wzór na obliczenia ile to zajmie (mniej wiecej), sama funkcja w sobie to Tc * fs * rozmiar_float (32bity) * 2,
        bo mamy oś x i y, potem modulacje, dft i inne jako że to osobne obiekty zasada ta sama, renderowanie grafu to zawsze 8K, szerokość * wysokość * rozmiar uint32_t (32bity) co daje około 130MB
        wiec dla funkcji o czasie Tc = 2 sekundy i fs = 32k mamy 2 * 32k * 2 czyli 128k * 32 co daje 4 096 000 bitów czyli około 0.5MB, dla czasu 4s to już 1MB dla, 10s mamy 5MB i tak dalej, wiec przy renderowaniu kilku
        funkcji i zdjęć łatwo pójść w gigabajty pamięci RAM, tylko ostrzegam, i przypominam o magicznych delete albo robieniu każdego wykresu w osobnych funkcjach by obiekty ginęły automatycznie,
        wraz z jej końcem (za wyjątkiem modulacji tam zawsze delete), wycieków pamięci nie widziałem ale gwarancji też nie dam więc warto mieć to gdzieś z tyłu głowy.

*/
#pragma endregion

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

    // Struktura tworząca obiekt funkcji
    struct Function {
        float Tc{0.f}; // czas_całkowity
        float fs{0.f}; // czestotliwosc_probkowania
        float f{0.f}; // czestotliwosc
        float PHI{0.f}; // faza_poczatkowa
        float A{0.f}; // amplituda
        float Ts{0.f}; // krok_czasu
        uint64_t N{0}; // ilosc probek


        float* t = nullptr; // wartosci czasowe konkretne
        float* f_t = nullptr; // wartości funkcji

        typedef float (*FormulaPtr)(const Function&, uint64_t, float);
        FormulaPtr function_formula = nullptr; // wskaznik na funkcje wzoru, jak korzystać opisane w instrukcji

        Function(float Tc, float fs, float f, float PHI, float A, FormulaPtr formula) : Tc(Tc), fs(fs), f(f), PHI(PHI), A(A), Ts(1.f / fs), N(static_cast<uint64_t>(Tc * fs)) {
            function_formula = formula;
            t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));
            f_t = static_cast<float*>(_mm_malloc(sizeof(float) * N, 64));

            for (uint64_t n = 0; n < N; n++)
                t[n] = static_cast<float>(n) * Ts;
            for (uint64_t n = 0; n < N; n++)
                f_t[n] = formula(*this, n, t[n]);
        }
        ~Function() {
            _mm_free(t);
            _mm_free(f_t);
        }
    };

    // Struktura tworząca obiekt DFT na podstawie obiektu funkcji
    struct DFT {
        uint32_t K{0}; // liczba elementów, K = N - 1
        // Należy pamiętać, że wykresy DFT tworzymy tylko dla połowy wartości, ponieważ druga połowa wartości zawiera
        // w sobie lustrzane odbicie tej pierwszej, rysowanie jej jest po prostu zbędne i bez sensu, dlatego
        // funkcja rysująca DFT będzie to robić dla (K - 1) / 2, natomiast liczyć wartości normalnie do K

        // potem nasze 'a' i 'b' dla wzoru Pitagorasa na moduł z
        float* Re = nullptr; // część rzeczywista
        float* Im = nullptr; // część zmyślona :)

        // nasze współrzędne x i y, różnią się względem zwykłej funkcji tylko nazwą, zwykłe (x, y)
        float* fk = nullptr; // spectrum x -> częstotliwość w danym punkcie
        float* mod_z = nullptr; // spectrum y -> siła sygnału w danej częstotliwości

        DFT(const Function* function) {
            K = function->N - 1;

            Re = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));
            Im = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));

            mod_z = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));
            fk = static_cast<float*>(_mm_malloc(sizeof(float) * (function->N - 1), 32));

            // liczenie dft
            for (uint64_t k = 0; k < K; k++) {
                Re[k] = 0;
                Im[k] = 0;
                for (uint64_t n = 0; n < K; n++) {

                    //  Liczymy ze wzoru zamieniając liczbe eulera na cos([wyrażenie wykładnika e ze wzoru]) + isin([wyrażenie wykładnika e ze wzoru])
                    //  i mamy: f(x) * cos([wyrażenie wykładnika e ze wzoru]) + i*sin([wyrażenie wykładnika e ze wzoru])
                    //  znak + rozdziela nam liczby Re od Im gdzie nasza liczba zespolona jest po prostu sumą tego, jako że komputer
                    //  średnio kuma liczby zespolone, a przynajmniej liczbe 'i', to przechowujemy to osobno lub jako jeden struct nazwany np.
                    //  struct liczba_zespolona {
                    //      float/double Re, Im;
                    //  } ;
                    //  gdzie:
                    //  Re = f(x) * cos([wyrażenie wykładnika e ze wzoru]), a
                    //  Im = f(x) * sin([wyrażenie wykładnika e ze wzoru]), należy pamiętać, że znak przed 'i' ma znaczenie dla wyników,
                    //  stąd w moim wzorze pojawiło się -2.f, ponieważ 'i' miało znak '-' przed sobą.

                    Re[k] += function->f_t[n] * cosf((-2.f * M_PIf * static_cast<float>(n) * static_cast<float>(k)) / static_cast<float>(K));
                    Im[k] += function->f_t[n] * sinf((-2.f * M_PIf * static_cast<float>(n) * static_cast<float>(k)) / static_cast<float>(K));
                }
            }

            // liczenie |z| - moduł z
            for (uint64_t k = 0; k < K; k++) {
                // moduł z to jest Amplituda naszej funkcji w danej częstotliwości, to ta wartość daję nam wysokość prążka, a liczmy ją Pitagorasem, gdzie Re i Im służą nam za 'a' i 'b'
                // stąd mamy c^2 = a^2 + b^2, przekształcamy i otrzymujemy c = pierwiastek(a^2 + b^2)
                mod_z[k] = sqrtf(powf(Re[k], 2) + powf(Im[k], 2));

                // fk to po prostu miejsce na osi x, dla danego moduł z gdzie oś x to częstotliwość, a nie czas jak w przypadku zwykłej funkcji
                fk[k] = static_cast<float>(k) * function->fs / static_cast<float>(K);
            }

            // normalizacja jest wymagana, aby wykres nie poleciał w kosmos, z powrotem sprowadzamy go do oryginalnej wartości Amplitudy
            mod_z[0] /= static_cast<float>(function->N); // stała składowa dzielona przez N
            for (uint64_t k = 1; k < K; k++) {
                // ja wykonałem normalizacje na zasadzie, podzielenia naszego modułu z przez połowę wszystkich elementów
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

    // Klasa renderująca wykresy na podstawie podanego obiektu funkcji albo dft
    class Graph {

        // colors (kolory)
        uint32_t bg; // background (tło)
        uint32_t lc; // line (kolor linii wykresu)
        uint32_t ac; // axis (osie)
        uint32_t gc; // grid (siatka)
        uint32_t fc; // font (czcionka)

        uint32_t* texture = nullptr;
        uint32_t picture_width = 7680; // lepiej nie zmieniac :)
        uint32_t picture_height = 4320; // lepiej nie zmieniac :)

        // funkcja do rysowania linii w przestrzeni 2D po teksturach
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

        // funkcja zmieniająca liczby typu float w tekst z zadaną precyzją
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

        // Oryginalnie jest to część mojego silnika graficznego więc kiedy zamieniałem pisany kod pod silnikiem na taki samowystarczalny,
        // to zostawiłem to najbliżej oryginałowi, aby nie musieć przepisywać tego od nowa, w skrócie tworzyło to kiedyś obiekt textbox,
        // który był fizycznym obiektem na ekranie, aktualnie wykorzystuje tylko część tworzenia samej tekstury i to też mocno odchudzonej.
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
                    current_font_data = font_A;
                    break;
                case 'B':
                    current_font_data = font_B;
                    break;
                case 'C':
                    current_font_data = font_C;
                    break;
                case 'D':
                    current_font_data = font_D;
                    break;
                case 'E':
                    current_font_data = font_E;
                    break;
                case 'F':
                    current_font_data = font_F;
                    break;
                case 'G':
                    current_font_data = font_G;
                    break;
                case 'H':
                    current_font_data = font_H;
                    break;
                case 'I':
                    current_font_data = font_I;
                    break;
                case 'J':
                    current_font_data = font_J;
                    break;
                case 'K':
                    current_font_data = font_K;
                    break;
                case 'L':
                    current_font_data = font_L;
                    break;
                case 'M':
                    current_font_data = font_M;
                    break;
                case 'N':
                    current_font_data = font_N;
                    break;
                case 'O':
                    current_font_data = font_O;
                    break;
                case 'P':
                    current_font_data = font_P;
                    break;
                case 'Q':
                    current_font_data = font_Q;
                    break;
                case 'R':
                    current_font_data = font_R;
                    break;
                case 'S':
                    current_font_data = font_S;
                    break;
                case 'T':
                    current_font_data = font_T;
                    break;
                case 'U':
                    current_font_data = font_U;
                    break;
                case 'V':
                    current_font_data = font_V;
                    break;
                case 'W':
                    current_font_data = font_W;
                    break;
                case 'X':
                    current_font_data = font_X;
                    break;
                case 'Y':
                    current_font_data = font_Y;
                    break;
                case 'Z':
                    current_font_data = font_Z;
                    break;
                case 'a':
                    current_font_data = font_a;
                    break;
                case 'b':
                    current_font_data = font_b;
                    break;
                case 'c':
                    current_font_data = font_c;
                    break;
                case 'd':
                    current_font_data = font_d;
                    break;
                case 'e':
                    current_font_data = font_e;
                    break;
                case 'f':
                    current_font_data = font_f;
                    break;
                case 'g':
                    current_font_data = font_g;
                    break;
                case 'h':
                    current_font_data = font_h;
                    break;
                case 'i':
                    current_font_data = font_i;
                    break;
                case 'j':
                    current_font_data = font_j;
                    break;
                case 'k':
                    current_font_data = font_k;
                    break;
                case 'l':
                    current_font_data = font_l;
                    break;
                case 'm':
                    current_font_data = font_m;
                    break;
                case 'n':
                    current_font_data = font_n;
                    break;
                case 'o':
                    current_font_data = font_o;
                    break;
                case 'p':
                    current_font_data = font_p;
                    break;
                case 'q':
                    current_font_data = font_q;
                    break;
                case 'r':
                    current_font_data = font_r;
                    break;
                case 's':
                    current_font_data = font_s;
                    break;
                case 't':
                    current_font_data = font_t;
                    break;
                case 'u':
                    current_font_data = font_u;
                    break;
                case 'v':
                    current_font_data = font_v;
                    break;
                case 'w':
                    current_font_data = font_w;
                    break;
                case 'x':
                    current_font_data = font_x;
                    break;
                case 'y':
                    current_font_data = font_y;
                    break;
                case 'z':
                    current_font_data = font_z;
                    break;
                case '0':
                    current_font_data = font_0;
                    break;
                case '1':
                    current_font_data = font_1;
                    break;
                case '2':
                    current_font_data = font_2;
                    break;
                case '3':
                    current_font_data = font_3;
                    break;
                case '4':
                    current_font_data = font_4;
                    break;
                case '5':
                    current_font_data = font_5;
                    break;
                case '6':
                    current_font_data = font_6;
                    break;
                case '7':
                    current_font_data = font_7;
                    break;
                case '8':
                    current_font_data = font_8;
                    break;
                case '9':
                    current_font_data = font_9;
                    break;
                case ' ':
                    current_font_data = font_Space;
                    break;
                case '!':
                    current_font_data = font_Exclam;
                    break;
                case '@':
                    current_font_data = font_At;
                    break;
                case '#':
                    current_font_data = font_Hash;
                    break;
                case '$':
                    current_font_data = font_Dollar;
                    break;
                case '%':
                    current_font_data = font_Percent;
                    break;
                case '^':
                    current_font_data = font_Caret;
                    break;
                case '&':
                    current_font_data = font_Ampers;
                    break;
                case '*':
                    current_font_data = font_Star;
                    break;
                case '(':
                    current_font_data = font_ParenL;
                    break;
                case ')':
                    current_font_data = font_ParenR;
                    break;
                case '-':
                    current_font_data = font_Minus;
                    break;
                case '_':
                    current_font_data = font_Under;
                    break;
                case '+':
                    current_font_data = font_Plus;
                    break;
                case '=':
                    current_font_data = font_Equals;
                    break;
                case '[':
                    current_font_data = font_BracketL;
                    break;
                case ']':
                    current_font_data = font_BracketR;
                    break;
                case '{':
                    current_font_data = font_BraceL;
                    break;
                case '}':
                    current_font_data = font_BraceR;
                    break;
                case '|':
                    current_font_data = font_Pipe;
                    break;
                case '\\':
                    current_font_data = font_Backsl;
                    break;
                case ':':
                    current_font_data = font_Colon;
                    break;
                case ';':
                    current_font_data = font_Semic;
                    break;
                case '"':
                    current_font_data = font_Quote;
                    break;
                case '\'':
                    current_font_data = font_Apostr;
                    break;
                case '<':
                    current_font_data = font_Less;
                    break;
                case '>':
                    current_font_data = font_Greater;
                    break;
                case ',':
                    current_font_data = font_Comma;
                    break;
                case '.':
                    current_font_data = font_Dot;
                    break;
                case '/':
                    current_font_data = font_Slash;
                    break;
                case '?':
                    current_font_data = font_Question;
                    break;
                case '~':
                    current_font_data = font_Tilde;
                    break;
                default:
                    current_font_data = font_Question;
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
                    current_font_data = font_A_pl;
                    break;
                case 0xC486:
                    current_font_data = font_C_pl;
                    break;
                case 0xC498:
                    current_font_data = font_E_pl;
                    break;
                case 0xC581:
                    current_font_data = font_L_pl;
                    break;
                case 0xC583:
                    current_font_data = font_N_pl;
                    break;
                case 0xC393:
                    current_font_data = font_O_pl;
                    break;
                case 0xC59A:
                    current_font_data = font_S_pl;
                    break;
                case 0xC5B9:
                    current_font_data = font_Z_kres;
                    break;
                case 0xC5BB:
                    current_font_data = font_Z_krop;
                    break;
                case 0xC485:
                    current_font_data = font_a_pl;
                    break;
                case 0xC487:
                    current_font_data = font_c_pl;
                    break;
                case 0xC499:
                    current_font_data = font_e_pl;
                    break;
                case 0xC582:
                    current_font_data = font_l_pl;
                    break;
                case 0xC584:
                    current_font_data = font_n_pl;
                    break;
                case 0xC3B3:
                    current_font_data = font_o_pl;
                    break;
                case 0xC59B:
                    current_font_data = font_s_pl;
                    break;
                case 0xC5BA:
                    current_font_data = font_z_kres;
                    break;
                case 0xC5BC:
                    current_font_data = font_z_krop;
                    break;
                default:
                    current_font_data = font_Question;
                    break;
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

    public:
        // ten konstruktor odpowiada za generowanie dla funkcji normalnych
        Graph(const Function* function_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", uint32_t background_color = 0xFF000000, uint32_t line_color = 0xFF00FF00, uint32_t axis_color = 0xFFFFFFFF, uint32_t grid_color = 0xFF7D7D7D, uint32_t font_color = 0xFFFFFFFF, const char* file_path = nullptr) : bg(background_color), lc(line_color), ac(axis_color), gc(grid_color), fc(font_color) {

            uint64_t picture_size = picture_width * picture_height;

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

            uint32_t padding_left_x = 736;
            uint32_t padding_right_x = 384;

            uint32_t padding_top_y = 256;
            uint32_t padding_bot_y = 512;

            uint32_t graph_width = picture_width - padding_left_x - padding_right_x; // 7168px
            uint32_t graph_height = picture_height - padding_top_y - padding_bot_y; // 3552px

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
            TextBox name(end, 1, background_color);
            name.add_text(name_label, font_color);

            uint32_t skala_textu_name = 16;
            uint32_t center = name.texture_width * skala_textu_name / 2;

            uint32_t pozycja_y = 0;
            for (uint32_t y = 0; y < name.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < name.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y) * picture_width)) -
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
            TextBox os_y(end, 1, background_color);
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

            uint32_t skala_textu_os_y = 16;
            pozycja_y = 0;
            center = rotated_height * skala_textu_os_y / 2;

            for (uint32_t y = 0; y < rotated_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < rotated_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[sx + pozycja_x +
                                    ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width)] =
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
            TextBox os_x(end, 1, background_color);
            os_x.add_text(x_label, font_color);

            uint32_t skala_textu_os_x = 16;
            center = os_x.texture_width * skala_textu_os_x / 2;

            pozycja_y = 0;
            for (uint32_t y = 0; y < os_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < os_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y + picture_height -
                                       os_x.texture_height * skala_textu_os_x) *
                                      picture_width)) -
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
            uint32_t skala_textu_value_x = 8;
            uint32_t step_x = ((function_to_render->N) / segments_count);
            for (uint32_t i = 1; i < segments_count; i++) {
                TextBox value_x(7, 1, background_color);
                value_x.add_text(float_to_char(function_to_render->t[step_x * i], value, 3), font_color);
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
                                         ((sy + pozycja_y + padding_top_y + graph_height + 64) * picture_width)) -
                                        center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }
            }
            TextBox value_x(7, 1, background_color);
            value_x.add_text(float_to_char(function_to_render->t[step_x * segments_count - 1], value, 3),
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
                            texture[(sx + pozycja_x + scaled_uint_x[step_x * segments_count - 1] +
                                     ((sy + pozycja_y + padding_top_y + graph_height + 64) * picture_width)) -
                                    center] = value_x.texture[x + (y * value_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_x;
                }
                pozycja_y += skala_textu_value_x;
            }

            // values y
            uint32_t kurwa_zmienna = graph_height / segments_count;
            uint32_t skala_textu_value_y = 8;
            float step_y = (max_y - min_y) / static_cast<float>(segments_count);

            for (uint32_t i = 0; i < segments_count; i++) {
                TextBox value_y(7, 1, background_color);
                value_y.add_text(float_to_char(max_y - step_y * static_cast<float>(i), value, 3), font_color);
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
                                texture[(padding_left_x + sx + pozycja_x - center - 64) +
                                        ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                          ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                         picture_width)] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }
            }
            TextBox value_y(7, 1, background_color);
            value_y.add_text(float_to_char(max_y - step_y * static_cast<float>(segments_count), value, 3),
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
                            texture[(padding_left_x + sx + pozycja_x - center - 64) +
                                    ((padding_top_y + sy + pozycja_y + graph_height -
                                      ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                     picture_width)] = value_y.texture[x + (y * value_y.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_y;
                }
                pozycja_y += skala_textu_value_y;
            }

            // grid on x
            for (uint32_t i = 0; i < segments_count; i++) {
                draw_line(texture, picture_width, picture_height, scaled_uint_x[step_x * i],
                          picture_height - padding_bot_y, scaled_uint_x[step_x * i], 0 + padding_top_y, grid_color, 16);
            }
            draw_line(texture, picture_width, picture_height, scaled_uint_x[step_x * segments_count - 1],
                      picture_height - padding_bot_y, scaled_uint_x[step_x * segments_count - 1], 0 + padding_top_y,
                      grid_color, 16);

            // grid on y
            for (uint32_t i = 0; i < segments_count; i++) {
                draw_line(texture, picture_width, picture_height, padding_left_x, padding_top_y + (kurwa_zmienna * i),
                          picture_width - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, 16);
            }
            draw_line(texture, picture_width, picture_height, padding_left_x, padding_top_y + (graph_height),
                      picture_width - padding_right_x, padding_top_y + (graph_height), grid_color, 16);


            // os x
            if (min_y <= 0.0f && max_y >= 0.0f) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;

                draw_line(texture, picture_width, picture_height, padding_left_x, zero_y_pixel, picture_width - padding_right_x, zero_y_pixel, axis_color, 16);
            }
            // os y
            if (scaled_x[0] <= 0) {
                draw_line(texture, picture_width, picture_height, 0 + offset_x + padding_left_x,
                          picture_height - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                          16);
            }
            // wykres
            for (uint32_t i = 0; i < function_to_render->N - 1; i++) {
                draw_line(texture, picture_width, picture_height, scaled_uint_x[i], scaled_uint_y[i],
                          scaled_uint_x[i + 1], scaled_uint_y[i + 1], line_color, 8);
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
            }
            else if (name_label != nullptr) {
                uint32_t i = 0;
                while (name_label[i] != '\0' && name_label[i] != '\n' && i < 240) {
                    filepath[i] = name_label[i];
                    i++;
                }
                filepath[i] = '.';
                filepath[i + 1] = 'b';
                filepath[i + 2] = 'm';
                filepath[i + 3] = 'p';
                filepath[i + 4] = '\0';
            }
            else {
                filepath[0] = 'w';
                filepath[1] = 'y';
                filepath[2] = 'k';
                filepath[3] = 'r';
                filepath[4] = 'e';
                filepath[5] = 's';
                filepath[6] = '.';
                filepath[7] = 'b';
                filepath[8] = 'm';
                filepath[9] = 'p';
                filepath[10] = '\0';
            }

            uint32_t image_size = picture_width * picture_height * 4;
            uint32_t file_size = 54 + image_size;

            uint8_t header[54] = {0};
            header[0] = 'B';
            header[1] = 'M';
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

            if (FILE* f = fopen(filepath, "wb")) {
                fwrite(header, 1, 54, f);
                fwrite(texture, 1, image_size, f);
                fclose(f);
#ifdef __linux__
                chmod(filepath, 0666);
#endif

                std::string bmp_path(filepath);
                std::string png_path = bmp_path;
                if (png_path.size() > 4 && png_path.substr(png_path.size() - 4) == ".bmp") {
                    png_path.replace(png_path.size() - 4, 4, ".png");
                } else {
                    png_path += ".png";
                }

#ifdef __linux__
                std::string linux_cmd =
                    "if command -v magick >/dev/null 2>&1; then "
                    "  magick \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "elif command -v convert >/dev/null 2>&1; then "
                    "  convert \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "elif command -v ffmpeg >/dev/null 2>&1; then "
                    "  ffmpeg -y -i \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "fi";

                int unused_linux = std::system(linux_cmd.c_str());
#elif defined(_WIN32) || defined(__NT__)

                std::string win_cmd =
                    "powershell -Command \"Add-Type -AssemblyName System.Drawing; "
                    "if (Test-Path '" + bmp_path + "') { "
                    "  $img = [System.Drawing.Image]::FromFile('" + bmp_path + "'); "
                    "  $img.Save('" + png_path + "', [System.Drawing.Imaging.ImageFormat]::Png); "
                    "  $img.Dispose(); "
                    "  Remove-Item '" + bmp_path + "'; "
                    "}\" >nul 2>&1";

                int unused_win = std::system(win_cmd.c_str());
#endif
            }

            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }
        // a ten dla funkcji po DFT,
        Graph(const DFT* dft_to_render, const char* name_label = " ", const char* x_label = " ", const char* y_label = " ", uint32_t background_color = 0xFF000000, uint32_t line_color = 0xFF00FF00, uint32_t axis_color = 0xFFFFFFFF, uint32_t grid_color = 0xFF7D7D7D, uint32_t font_color = 0xFFFFFFFF, const char* file_path = nullptr) : bg(background_color), lc(line_color), ac(axis_color), gc(grid_color), fc(font_color) {

            //math section

            uint64_t picture_size = picture_width * picture_height;
            uint32_t K_render = (dft_to_render->K - 1) / 2;


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

            uint32_t padding_left_x = 736;
            uint32_t padding_right_x = 384;

            uint32_t padding_top_y = 256;
            uint32_t padding_bot_y = 512;

            uint32_t graph_width = picture_width - padding_left_x - padding_right_x; // 7168px
            uint32_t graph_height = picture_height - padding_top_y - padding_bot_y; // 3552px



            float scale_x = static_cast<float>(graph_width) / (dft_to_render->fk[K_render] - dft_to_render->fk[0]);
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
            TextBox name(end, 1, background_color);
            name.add_text(name_label, font_color);

            uint32_t skala_textu_name = 16;
            center = name.texture_width * skala_textu_name / 2;

            for (uint32_t y = 0; y < name.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < name.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_name; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_name; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y) * picture_width)) -
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
            TextBox os_y(end, 1, background_color);
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

            uint32_t skala_textu_os_y = 16;
            pozycja_y = 0;
            center = rotated_height * skala_textu_os_y / 2;

            for (uint32_t y = 0; y < rotated_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < rotated_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_y; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_y; sx++) {
                            texture[sx + pozycja_x +
                                    ((sy + pozycja_y + (graph_height / 2) + padding_top_y - center) * picture_width)] =
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
            TextBox os_x(end, 1, background_color);
            os_x.add_text(x_label, font_color);

            uint32_t skala_textu_os_x = 16;
            center = os_x.texture_width * skala_textu_os_x / 2;

            pozycja_y = 0;
            for (uint32_t y = 0; y < os_x.texture_height; y++) {
                uint32_t pozycja_x = 0;
                for (uint32_t x = 0; x < os_x.texture_width; x++) {
                    for (uint32_t sy = 0; sy < skala_textu_os_x; sy++) {
                        for (uint32_t sx = 0; sx < skala_textu_os_x; sx++) {
                            texture[(sx + pozycja_x + padding_left_x + (graph_width / 2) +
                                     ((sy + pozycja_y + picture_height -
                                       os_x.texture_height * skala_textu_os_x) *
                                      picture_width)) -
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
            uint32_t skala_textu_value_x = 8;
            uint32_t step_x = ((K_render) / segments_count);
            for (uint32_t i = 1; i < segments_count; i++) {
                TextBox value_x(7, 1, background_color);
                value_x.add_text(float_to_char(dft_to_render->fk[step_x * i], value, 3), font_color);
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
                                         ((sy + pozycja_y + padding_top_y + graph_height + 64) * picture_width)) -
                                        center] = value_x.texture[x + (y * value_x.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_x;
                    }
                    pozycja_y += skala_textu_value_x;
                }
            }
            TextBox value_x(7, 1, background_color);
            value_x.add_text(float_to_char(dft_to_render->fk[step_x * segments_count - 1], value, 3),
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
                            texture[(sx + pozycja_x + scaled_uint_x[step_x * segments_count - 1] +
                                     ((sy + pozycja_y + padding_top_y + graph_height + 64) * picture_width)) -
                                    center] = value_x.texture[x + (y * value_x.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_x;
                }
                pozycja_y += skala_textu_value_x;
            }

            // values y
            uint32_t kurwa_zmienna = graph_height / segments_count;

            uint32_t skala_textu_value_y = 8;
            float step_y = (max_y - min_y) / static_cast<float>(segments_count);

            for (uint32_t i = 0; i < segments_count; i++) {
                TextBox value_y(7, 1, background_color);
                value_y.add_text(float_to_char(max_y - step_y * static_cast<float>(i), value, 3), font_color);
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
                                texture[(padding_left_x + sx + pozycja_x - center - 64) +
                                        ((padding_top_y + sy + pozycja_y + (kurwa_zmienna * i) -
                                          ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                         picture_width)] = value_y.texture[x + (y * value_y.texture_width)];
                            }
                        }
                        pozycja_x += skala_textu_value_y;
                    }
                    pozycja_y += skala_textu_value_y;
                }
            }
            TextBox value_y(7, 1, background_color);
            value_y.add_text(float_to_char(max_y - step_y * static_cast<float>(segments_count), value, 3),
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
                            texture[(padding_left_x + sx + pozycja_x - center - 64) +
                                    ((padding_top_y + sy + pozycja_y + graph_height -
                                      ((value_y.texture_height * skala_textu_value_y) / 3)) *
                                     picture_width)] = value_y.texture[x + (y * value_y.texture_width)];
                        }
                    }
                    pozycja_x += skala_textu_value_y;
                }
                pozycja_y += skala_textu_value_y;
            }

            // grid on x
            for (uint32_t i = 0; i < segments_count; i++) {
                draw_line(texture, picture_width, picture_height, scaled_uint_x[step_x * i],
                          picture_height - padding_bot_y, scaled_uint_x[step_x * i], 0 + padding_top_y, grid_color, 16);
            }
            draw_line(texture, picture_width, picture_height, scaled_uint_x[step_x * segments_count - 1],
                      picture_height - padding_bot_y, scaled_uint_x[step_x * segments_count - 1], 0 + padding_top_y,
                      grid_color, 16);

            // grid on y
            for (uint32_t i = 0; i < segments_count; i++) {
                draw_line(texture, picture_width, picture_height, padding_left_x, padding_top_y + (kurwa_zmienna * i),
                          picture_width - padding_right_x, padding_top_y + (kurwa_zmienna * i), grid_color, 16);
            }
            draw_line(texture, picture_width, picture_height, padding_left_x, padding_top_y + (graph_height),
                      picture_width - padding_right_x, padding_top_y + (graph_height), grid_color, 16);

            // os x
            if (min_y <= 0.0f && max_y >= 0.0f) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                draw_line(texture, picture_width, picture_height, padding_left_x, zero_y_pixel, picture_width - padding_right_x, zero_y_pixel, axis_color, 16);
            }
            // os y
            if (scaled_x[0] <= 0) {
                draw_line(texture, picture_width, picture_height, 0 + offset_x + padding_left_x,
                picture_height - padding_bot_y, 0 + offset_x + padding_left_x, 0 + padding_top_y, axis_color,
                          16);
            }
            // wykres
            for (uint32_t i = 0; i < K_render - 1; i++) {
                uint32_t zero_y_pixel = offset_y + padding_top_y;
                draw_line(texture, picture_width, picture_height, scaled_uint_x[i], scaled_uint_y[i], scaled_uint_x[i], zero_y_pixel, line_color, 8);
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
            }
            else if (name_label != nullptr) {
                uint32_t i = 0;
                while (name_label[i] != '\0' && name_label[i] != '\n' && i < 240) {
                    filepath[i] = name_label[i];
                    i++;
                }
                filepath[i] = '.';
                filepath[i + 1] = 'b';
                filepath[i + 2] = 'm';
                filepath[i + 3] = 'p';
                filepath[i + 4] = '\0';
            }
            else {
                filepath[0] = 'w';
                filepath[1] = 'y';
                filepath[2] = 'k';
                filepath[3] = 'r';
                filepath[4] = 'e';
                filepath[5] = 's';
                filepath[6] = '.';
                filepath[7] = 'b';
                filepath[8] = 'm';
                filepath[9] = 'p';
                filepath[10] = '\0';
            }

            uint32_t image_size = picture_width * picture_height * 4;
            uint32_t file_size = 54 + image_size;

            uint8_t header[54] = {0};
            header[0] = 'B';
            header[1] = 'M';
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

            if (FILE* f = fopen(filepath, "wb")) {
                fwrite(header, 1, 54, f);
                fwrite(texture, 1, image_size, f);
                fclose(f);
#ifdef __linux__
                chmod(filepath, 0666);
#endif

                std::string bmp_path(filepath);
                std::string png_path = bmp_path;
                if (png_path.size() > 4 && png_path.substr(png_path.size() - 4) == ".bmp") {
                    png_path.replace(png_path.size() - 4, 4, ".png");
                } else {
                    png_path += ".png";
                }

#ifdef __linux__
                std::string linux_cmd =
                    "if command -v magick >/dev/null 2>&1; then "
                    "  magick \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "elif command -v convert >/dev/null 2>&1; then "
                    "  convert \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "elif command -v ffmpeg >/dev/null 2>&1; then "
                    "  ffmpeg -y -i \"" + bmp_path + "\" \"" + png_path + "\" && rm \"" + bmp_path + "\"; "
                    "fi";

                int unused_linux = std::system(linux_cmd.c_str());
#elif defined(_WIN32) || defined(__NT__)

                std::string win_cmd =
                    "powershell -Command \"Add-Type -AssemblyName System.Drawing; "
                    "if (Test-Path '" + bmp_path + "') { "
                    "  $img = [System.Drawing.Image]::FromFile('" + bmp_path + "'); "
                    "  $img.Save('" + png_path + "', [System.Drawing.Imaging.ImageFormat]::Png); "
                    "  $img.Dispose(); "
                    "  Remove-Item '" + bmp_path + "'; "
                    "}\" >nul 2>&1";

                int unused_win = std::system(win_cmd.c_str());
#endif
            }

            _mm_free(scaled_y);
            _mm_free(scaled_x);
            _mm_free(scaled_uint_x);
            _mm_free(scaled_uint_y);
        }



        ~Graph() { _mm_free(texture); };
    };

#pragma region Funkcje modulujące

    // dodac funkcje dla haminga i inne bajery z tym zwiazane

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
#pragma endregion

} // namespace ShiftDownFunctions
