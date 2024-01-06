#include "CYBER_LEJ_2_0.h"

typ_wynik ranking[100] = {{14.53, "Mateusz_M", 9},
                          {4.37, "Kuba", 4},
                          {5.84, "Karol", 5},
                          {6.27, "Kamil", 5},
                          {6.42, "kto_S", 5},
                          {112.56, "gosciu", 6},
                          {11.05, "Jan_Pawel_Adamczewsk", 20},
                          {12.46, "kolejny_wynik", 6}};

void setup() {

  Serial.begin(115200);
  inicjuj_lcd();
  inicjuj_interrupty();
  
}

void loop() {

  while(1)                    // Wszystko jest w pętli aby można było wyjść z kodu za pomocą esc
  {
    wypisz_ekran_startowy();
    if(wybor("pomiar", "ranking", 1, 8, 6, 7, 0)) // ----------------------------------------------------------   R A N K I N G   ----------------
    {
      uint8_t miejsce = 1;

      while(1)
      {
        wyswietl_ranking(miejsce);

        if(przesuwanie_nazwy(miejsce)) break;

        if(ok_flag){
          ok_flag = 0;
          //usun_wynik();
        }

        ruch_rankingu(&miejsce);

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
      
      zapisz(&temp_wynik);
      
      
    }
  }
}
