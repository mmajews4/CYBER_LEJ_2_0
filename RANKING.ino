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
int przesuwanie_nazwy(uint8_t miejsce_1, uint8_t miejsce_2){

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
        ile_przesunac[0] = strlen(wyniki[miejsce_1].nazwa) - DLG_PRZES_NAZWY;
        ile_przesunac[1] = strlen(wyniki[miejsce_2].nazwa) - DLG_PRZES_NAZWY;

        przes_nazwa[0] = wyniki[miejsce_1].nazwa;                // Zapamietanie wskaźnika na poczatek nazw gracza by potem go przesuwać
        przes_nazwa[1] = wyniki[miejsce_2].nazwa;

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

void ruch_menu_wynikow(char *nazwa_lejownika, uint16_t *wynik_1, uint16_t *wynik_2){
  
  uint16_t i_1, i_2;
  i_1 = *wynik_1;
  i_2 = *wynik_2;

  if(up_flag){                        // 1 do góry
    up_flag = 0;

    if(i_1 > 1) i_1--;                // Jeżeli nie jest to pierwszy wynik możesz zejść niżej

    while(strcmp(nazwa_lejownika, wyniki[i_1].nazwa) != 0 && (i_1 > 1)){
      i_1--;                          // Wyszykanie wcześniejszego wyniku danego lejownika
    }

    if(strcmp(nazwa_lejownika, wyniki[i_1].nazwa) == 0)
    {                                 // Zamiana wyników na jeden wcześniej
      *wynik_2 = *wynik_1;
      *wynik_1 = i_1;
    }
  }
  if(down_flag){                      // 1 do dołu                                // Sprawdzić co się stanie jak wyjdę poza miejsce 100
    down_flag = 0;

    if(wyniki[i_2].czas != 0) i_2++;  // jeżeli nie jest to ostatni wynik to idź dalej 

    while(strcmp(nazwa_lejownika, wyniki[i_2].nazwa) != 0 && (wyniki[i_2].czas != 0)){  
      i_2++;                          // Przejście po elementach w dół aż znajdę szukaną nazwę lub skończą mi się nazwy
    }

    if(wyniki[i_2].nazwa != 0)        // Zamiana wyników na jeden dalej
    {
      *wynik_1 = *wynik_2;
      *wynik_2 = i_2;
    }
  }

  if(left_flag){                      // zerowanie flag żeby nie ranking się nie przesuwał po wyjściu z menu lejownika
    left_flag = 0;
  }
  if(right_flag){                                             
    right_flag = 0;
  }
}

void wyswietl_wyniki(uint16_t wynik_1, uint16_t wynik_2){

  char bufor[9];

  lcd.setCursor(0, 0);

  if(wyniki[wynik_1].czas == 0){                      // Jeżeli nie ma wyniku, nie wypisanie niczego
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    return;
  } else if(wyniki[wynik_1].czas < 10){               // Wypisanie spacji w zależności od dlugości wyniku
    lcd.print("  ");
  } else if(wyniki[wynik_1].czas < 100){
    lcd.print(" ");
  }

  sprintf(bufor, "%.3f", wyniki[wynik_1].czas);       // Sformatowanie liczby z dokładnością do 3 miejsc po przecinku
  lcd.print(bufor);

  lcd.setCursor(0, 1);                                // Wypisanie drugiej liczby

  if(wyniki[wynik_2].czas == 0){                      // Jeżeli nie ma wyniku, nie wypisanie niczego
    lcd.print("       ");
    return;
  } else if(wyniki[wynik_2].czas < 10){               // Wypisanie spacji w zależności od dlugości wyniku
    lcd.print("  ");
  } else if(wyniki[wynik_2].czas < 100){
    lcd.print(" ");
  }

  sprintf(bufor, "%.3f", wyniki[wynik_2].czas);       // Sformatowanie liczby z dokładnością do 3 miejsc po przecinku
  lcd.print(bufor);  
}

void usun_wynik(uint16_t *pozycja_wyniku){

}

void menu_wynikow_lejownika(char *nazwa_lejownika, float czas){

  uint16_t wynik_1, wynik_2;
  uint16_t i = 1;

  while((czas != wyniki[i].czas) && strcmp(nazwa_lejownika, wyniki[i].nazwa)){    // Przejście do pierwszego wyniku lejownika
    i++;
  }
  wynik_1 = i;                                                                    // Zapisanie pozycji pierwszego wyniku
  i++;                                                                            // Wyjście dalej za pierwszy wynik żeby go nie wzieło jako drugi

  while(strcmp(nazwa_lejownika, wyniki[i].nazwa) && (i <= wyniki[0].czas)){       // Przejście do drugiego wyniku lejownika
    i++;
  }
  wynik_2 = i;

  while(1)
  {
    wyswietl_wyniki(wynik_1, wynik_2);                                 // Wyświetlenie czasu z dokładnością do 3 miejsc po przecinku

    if(przesuwanie_nazwy(wynik_1, wynik_2)) break;                     // wyświetlenie ESC z wyswiwtlania rankingu, jeżeli 0 to usuwanie nazwy lub ruch

    if(ok_flag){
      ok_flag = 0;
      //usun_wynik();                                                  // W nim jest także usuwanie
    }

    ruch_menu_wynikow(nazwa_lejownika, &wynik_1, &wynik_2);            // Funkcja do przemieszczania się po rankingu
  }
}



