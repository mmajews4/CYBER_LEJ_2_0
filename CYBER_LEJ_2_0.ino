#include "CYBER_LEJ_2_0.h"

typ_wynik wyniki[255] = {{14,0},                           // Na miejscu 0 w wynikach jest zapisana ilość wyników aby można było dodać nowy, następny wynik    
                          {14.532, "Mateusz_M"},            // Nie zmieniając ich kolejności fajnie będzie też zobaczyć historię wyników   !!! Jest bug że w tabeli wyników ta nazwa wyswietla się dwa razy
                          {4.376, "Kuba"},
                          {8.844, "Karol"},
                          {6.278, "Kamil"},
                          {6.424, "a"},
                          {5.845, "Karol"},
                          {112.567, "gosciu"},
                          {7.844, "Karol"},
                          {11.053, "Jan_Pawel_Adamczewsk"},
                          {12.466, "kolejny_wynik"},
                          {9.844, "Karol"},
                          {10.844, "Karol"},
                          {11.844, "Karol"},
                          {7.424, "a"}};

void setup() {

  Serial.begin(115200);
  inicjuj_lcd();
  inicjuj_interrupty();
  
}

void loop() {

  uint8_t idz_do_rankingu = 0;

  while(1)                    // Wszystko jest w pętli aby można było wyjść z kodu za pomocą esc
  {                           // ------------------------------------------------------------------------    E K R A N   G L O W N Y   -----------
    if(!idz_do_rankingu){     // Flaga jest potrzebna aby można było przejść do rankingu po pomiarze
      wypisz_ekran_startowy();
      idz_do_rankingu = wybor("pomiar", "ranking", 1, 8, 6, 7, 0, 1);
      miejsce = 1;                                // Zmienna do trzymania wartości miejca wyświetlanego na górze LCD
    }

    if(idz_do_rankingu == 2)                      // -------------------------------------------------------   U S T A W I E N I A   -------------
    {
      idz_do_rankingu = 0;
      lcd.clear();

      lcd.setCursor(0,1);                         // Możliwa przyszła opcja
      lcd.print("Wifi:X  SD/FLASH");

      wyswietl_naladowanie();
    }
    else if(idz_do_rankingu)                      // ----------------------------------------------------------   R A N K I N G   ----------------
    {
      idz_do_rankingu = 0;                        // Flaga przejścia do rankingu

      while(1)
      {
        sprawdz_naladowanie(0, 0, 0);             // Sprawdzenie naladowania baterii

        wyswietl_ranking(miejsce);                // Wyświetlenie miejsca i czasu

        if(przesuwanie_nazwy(ranking_index[miejsce-1], ranking_index[miejsce])) break;     // wyświetlenie ESC z wyswiwtlania rankingu, jeżeli 0 to usuwanie nazwy lub ruch

        if(ok_flag){
          ok_flag = 0;
          menu_wynikow_lejownika(miejsce-1);      // W nim jest także usuwanie
        }

        ruch_rankingu(&miejsce);                  // Funkcja do przemieszczania się po rankingu

      }
    }
    else                                          // ----------------------------------------------------------    P O M I A R    ----------------
    {
      if(zawor_otwarty())                         // Te if-y służą do wychodzenia do menu glownego
        if(uwaga_otworzony_zawor()) break;        // Informuje że lej nie jest gotowy do picia

      typ_wynik temp_wynik;                       // Struct który do czasu zapisu potrzyma w sobie wyniki
      if(zliczanie_czasu(&temp_wynik)) break;     // Zliczenie czasu, z możliwością wyjścia przed otwarciem zaworu

      while(!ok_flag && !esc_flag){               // Wyświetlaj wynik dopóki urzytkownik czegoś nie zrobi  
        Serial.println("Czekam z wyswiwtlonym wynikiem");
      }  
      
      if(czy_zapisac()) break;                    // 1 jeżeli nie chcemy zapisać, wykona się tylko po nacisniecuy ESC

      if(wpisz_nazwe(&temp_wynik)) break;

      Serial.println("Zapis");
      
      if(zapisz(&temp_wynik)) break;    // Zapisanie wyniku w tablicy wyniki, ustawienie indexu rankingu oraz zapisanie na SD/FLASHu
      
      idz_do_rankingu = 1;                        // Po pomyślnym zapisaniu, wyświetl wynik w rankingu
    }
  }
}
