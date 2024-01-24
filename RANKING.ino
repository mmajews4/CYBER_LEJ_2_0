void wyswietl_ranking(uint8_t miejsce){       // Miejsce, tak jak w rzeczywstości zaczyna się od 1.

  for(int i = 0; i < 2; i++)              // Wypisanie miejsca
  { 
    lcd.setCursor(0, i);
    lcd.print(miejsce+i);
    if(miejsce+i < 10)
      lcd.print(" ");                     // Wypisanie przerw w odpowiednich miejscach
    lcd.print(" ");
    lcd.print(wyniki[ranking_index[miejsce-1+i]].czas); // Wypisanie 4 znaków czasu

    lcd.setCursor(7, i);
    lcd.print(" ");
  }
}

// Funkcja czeka określony czas, przesówa wczytane zmienne, a zwraca wyjsce esc lub ruch w rankingu
int przesuwanie_nazwy(uint8_t miejsce){

  uint32_t currTime, prevTime = 0;
  char *przes_nazwa[2] = {0,0};
  int8_t ile_przesunac[2] = {0,0};                              // Zainicjowanie jej zerami aby program uznał że skończył przesuwać
  int8_t poczekaj = 1;                                          // Przed rozpoczęciem przesuwania nazwy, poczekaj chwilę żeby łatiej było ją odczytać

  while(1)
  {
    currTime = millis();
    
    if((currTime - prevTime > 400 && !poczekaj) || (currTime - prevTime > 1500 && poczekaj)){

      poczekaj = 0;                                               // Zresetowanie czekania jeżeli już poczekało

      if((ile_przesunac[0] <= 0 && ile_przesunac[1] == 1) || (ile_przesunac[1] <= 0 && ile_przesunac[0] == 1)) poczekaj = 1;  // Poczekaj też na końcu przesuwania

      if(ile_przesunac[0] <= 0 && ile_przesunac[1] <= 0)          // Jeżeli skończyliśmy prezsuwanie obu nazw to zacznij od nowa, jeżeli jest krótsze niż ekran, nie przesuwaj
      {
        ile_przesunac[0] = strlen(wyniki[ranking_index[miejsce-1]].nazwa) - DLG_PRZES_NAZWY;
        ile_przesunac[1] = strlen(wyniki[ranking_index[miejsce]].nazwa) - DLG_PRZES_NAZWY;

        przes_nazwa[0] = wyniki[ranking_index[miejsce-1]].nazwa;                // Zapamietanie wskaźnika na poczatek nazw gracza by potem go przesuwać
        przes_nazwa[1] = wyniki[ranking_index[miejsce]].nazwa;

        poczekaj = 1;                                             // Czekanie po tym jak przesunie się na początek
      }
      else
      {                                                           // Przesuwanie nazwy zliczając do dołu ilość możliwych przesunięć
        if(ile_przesunac[0] > 0){
          ile_przesunac[0] -= 1;
          przes_nazwa[0] = przes_nazwa[0] + 1;
        }
        if(ile_przesunac[1] > 0){
          ile_przesunac[1] -= 1;
          przes_nazwa[1] = przes_nazwa[1] + 1;
        }
      }

      for(uint8_t i = 0; i < 2; i++){                             // Wypisanie 8 liter z nazwy a jeśli tyle nie ma to w pozostałe miejsca wstawia znak " "
        lcd.setCursor(8, i);
        lcd.print(przes_nazwa[i]);
        lcd.print("        ");                                    // Usuwa niepotrzebne znaki za nazwą
      }
      prevTime = currTime;
    }

    if(esc_flag){
      esc_flag = 0;
      lcd.clear();
      return 1;
    }
    if(right_flag || down_flag || left_flag || up_flag || ok_flag){
      return 0;
    }
  }
}

void ruch_rankingu(uint8_t* miejsce){

  if(up_flag){                        // 1 do góry
    up_flag = 0;
    if(*miejsce > 1)
      *miejsce = *miejsce - 1;
  }
  if(down_flag){                      // 1 do dołu                                // Sprawdzić co się stanie jak wyjdę poza miejsce 100
    down_flag = 0;
    if(ranking_index[*miejsce+1] > 0)
      *miejsce = *miejsce + 1;
  }
  if(left_flag){                      // lewo - 5 do góry
    left_flag = 0;
    if(*miejsce > 5)
      *miejsce = *miejsce - 5;
  }
  if(right_flag){                     // prawo - 5 do dołu                              
    right_flag = 0;
    if(ranking_index[*miejsce+5] > 0)
      *miejsce = *miejsce + 5;
  }

}





