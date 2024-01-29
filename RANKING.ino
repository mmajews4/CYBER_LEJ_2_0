void wyswietl_ranking(uint8_t miejsce){       // Miejsce, tak jak w rzeczywstości zaczyna się od 1.

  for(int i = 0; i < 2; i++)              // Wypisanie miejsca
  { 
    if(ranking_index[miejsce-1+i] != 0){
      
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

    if(i_2 < wyniki[0].czas) i_2++;  // jeżeli nie jest to ostatni wynik to idź dalej 

    while(strcmp(nazwa_lejownika, wyniki[i_2].nazwa) != 0 && (i_2 <= wyniki[0].czas)){  
      i_2++;                          // Przejście po elementach w dół aż znajdę szukaną nazwę lub skończą mi się nazwy
    }

    if(i_2 <= wyniki[0].czas + 1 && *wynik_2 != i_2)        // Zamiana wyników na jeden dalej, jeżeli nie jesteśmy na końcu + 1 ruch + zabezpieczenie przed pokazaniem dwóch takich samych wyników
    {
      *wynik_1 = *wynik_2;
      *wynik_2 = i_2;
    }

   /* if(*wynik_2 == *wynik_1){
      *wynik_2 = wyniki[0].czas + 1;  // Ustawienie ostatniego wyniku na nic żeby dało radę usunąć ostatni wynik
    }*/
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

int usun_wynik(uint16_t pozycja_wyniku){

  uint16_t najlepszy_wynik = wyniki[0].czas + 1;
  char nazwa[21];
  
  lcd.clear();
  lcd.setCursor(0, 0);                                               
  lcd.print("  Usunac wynik? ");

  if(wybor("TAK", "NIE", 3, 10, 3, 3, 1, 0))
  {                      // NIE usuwaj 
    lcd.clear();  
    return 0;
  }                      // TAK usuń

  strcpy(nazwa, wyniki[pozycja_wyniku].nazwa);
  
  wyniki[pozycja_wyniku].czas = 0.0;                      // Wyzerowanie wyniku ze spisu wyników
  wyniki[pozycja_wyniku].nazwa[0] = '\0';                 // Czy wpisanie 0 do stringa da jakiś blad? Nie, jest dobrze
 
  down_flag = 1;                                          // Zrobienie ruchu w dół aby zakryć miejsce usuniętego wyniku 

  Serial.print("Nazwa: ");
  Serial.println(nazwa);
  Serial.print("Usunięta nazwa: ");
  Serial.println(wyniki[pozycja_wyniku].nazwa);

  uint16_t i = 0;                                         // Sprawdzenie czy wynik jest w rankingu
  while(ranking_index[i] != pozycja_wyniku && ranking_index[i] != 0){
    i++;
  }

  while(ranking_index[i] != 0){                           // Przesunięcie wszystkich gorszych wyników w góre
    ranking_index[i] = ranking_index[i+1];
    i++;
  }

  if(ranking_index[i] == 0) return 1;                     // Jeżeli go nie ma w rankingu, usuwanie zakończone

  // Jeżeli zawodnik ma inne wyniki, wstawienie do rankingu drugiego najlepszego

  for(int k = 1; k <= wyniki[0].czas; k++)                // Przejście po wszystkich wynikach wyszukując tego najlepszego
  {
    if(!strcmp(nazwa, wyniki[k].nazwa))                   // Jeżeli znajdzie szukaną nazwe
    { // To sprawdzi czy jej wynik jest lepszy niż najlepszy dotychczas lub czy nie jest to pierwszy znaleziony wynik
      Serial.println("Sprawdzam wynik");
      if(wyniki[k].czas < wyniki[najlepszy_wynik].czas || najlepszy_wynik == wyniki[0].czas + 1)
      {
        najlepszy_wynik = k;
        Serial.print("Najlepszy wynik: ");
        Serial.println(najlepszy_wynik);
      }
    }
  }

  wstaw_do_rankingu(najlepszy_wynik);

  for(int j = 0; j <= wyniki[0].czas; j++){
    Serial.print(wyniki[j].czas);
    Serial.print("       ");
    Serial.println(wyniki[j].nazwa);
  }

  return 1;
}

void menu_wynikow_lejownika(uint16_t miejsce){

  uint16_t wynik_1, wynik_2;
  uint16_t i = 1;
  float czas = wyniki[ranking_index[miejsce]].czas;
  char nazwa_lejownika[21];
  strcpy(nazwa_lejownika, wyniki[ranking_index[miejsce]].nazwa);

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
    sprawdz_naladowanie(0, 0, 0);                                      // Sprawdzenie naladowania baterii

    wyswietl_wyniki(wynik_1, wynik_2);                                 // Wyświetlenie czasu z dokładnością do 3 miejsc po przecinku

    if(przesuwanie_nazwy(wynik_1, wynik_2)) break;                     // wyświetlenie ESC z wyswiwtlania rankingu, jeżeli 0 to usuwanie nazwy lub ruch

    Serial.print(wynik_1);
    Serial.print("    ");
    Serial.println(wyniki[wynik_1].nazwa);
    Serial.print(wynik_2);
    Serial.print("    ");
    Serial.println(wyniki[wynik_2].nazwa);
    Serial.print("Nazwa_lejownika    ");
    Serial.println(nazwa_lejownika);
    Serial.println("    ");

    if(ok_flag){
      ok_flag = 0;
      if(usun_wynik(wynik_1)){

        if(wyniki[wynik_2].czas == 0.0){
          break;    // Jeżeli nie ma już nic do wyświetlenia to wyjdź
        } 
       /* else 
        {
          
        }*/
      }
    }

    ruch_menu_wynikow(nazwa_lejownika, &wynik_1, &wynik_2);            // Funkcja do przemieszczania się po rankingu
  }
}

/*   -------------    B U G I    -------------
- Usuwając wynik który jest pojedynczy dodaje się wynik 0.0
 
*/

