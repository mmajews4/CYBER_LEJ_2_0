#include "CYBER_LEJ_2_0.h"

void setup() {

  Serial.begin(115200);
  inicjuj_lcd();
  inicjuj_interrupty();
  
}

void loop() {
//  lcd.print(wybor("TAK", "NIE", 3, 10, 3, 3, 1));
  while(1)                    // Wszystko jest w pętli aby można było wyjść z kodu za pomocą esc
  {
    wypisz_ekran_startowy();
    if(wybor("pomiar", "ranking", 1, 8, 6, 7, 0)) // ----------------------------------------------------------   R A N K I N G   ----------------
    {
      int i = 0;
    }
    else                                          // ----------------------------------------------------------    P O M I A R    ----------------
    {
      if(zawor_otwarty())             // Te if-y służą do wychodzenia do menu glownego
        if(uwaga_otworzony_zawor()) break;// Informuje że lej nie jest gotowy do picia

      if(zliczanie_czasu()) break;        // Zliczenie czasu, z możliwością wyjścia przed otwarciem zaworu

      while(!ok_flag && !esc_flag){       // Wyświetlaj wynik dopóki urzytkownik czegoś nie zrobi
      }  

      if(czy_zapisac()) break;            // 1 jeżeli nie chcemy zapisać

      zapisz();
      
    }
  }
}
