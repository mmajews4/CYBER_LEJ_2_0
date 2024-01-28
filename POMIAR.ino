uint16_t istnieje_nazwa = 0;

int zawor_otwarty(){
  return (digitalRead(VALVE));    // Softwerowa negacja zaworu, wyłączalna do testów
}

int uwaga_otworzony_zawor(){
  lcd.setCursor(1, 0); 
  lcd.print("Zawor otwarty");

  while(zawor_otwarty()){  
    if(esc_flag)                 // Wyjście z tego stanu do menu glownego 
    {
      esc_flag = 0;
      lcd.clear();
      return 1;
    }
  }
  lcd.clear();
  return 0;
}

int zliczanie_czasu(typ_wynik* temp_wynik){

  int currTime, prevTime;
  
  lcd.setCursor(3, 0); 
  lcd.print("Mozesz pic");

  while(!zawor_otwarty()){            //  -----   Czekamy na otwarcie zaworu
    if(esc_flag)
    {
      lcd.clear();
      esc_flag = 0;
      return 1;
    }
  }

  if(zawor_otwarty()){              //  -----   Gdy otworzy sie zawór
    prevTime = millis();
    lcd.clear();
                  
    while(zawor_otwarty())          //  -----   Wyswietlanie uplywajacego czasu
    {
      currTime = millis();
      temp_wynik->czas = (currTime - prevTime)/ 1000.0;
      lcd.setCursor(6, 0);                                               
      lcd.print(temp_wynik->czas);
    }

    for(int i = 0; i < 3; i++)          //  -----   Wyswietlenie wymiku (3 migniecia a potem na stałe)
    {
      currTime = prevTime = millis();   // Czekamy 500 ms
      while(currTime - prevTime < 500){
        if(esc_flag || ok_flag)         // Obsluga esc i ok
        {                               // Nie kasujemy flag bo później będzie komunikat
          return 0;
        }
        currTime = millis();
      }

      lcd.clear();
      
      currTime = prevTime = millis();   // Czekamy 500 ms
      while(currTime - prevTime < 500){ 
        if(esc_flag || ok_flag)         // Obsluga esc i ok
        {                               // Nie kasujemy flag bo później będzie komunikat
          lcd.setCursor(6, 0);
          lcd.print(temp_wynik->czas);  // Podczas wyjscia w trakcie migniecia trzeba wydrukować czas
          return 0;
        }
        currTime = millis();
      }
      lcd.setCursor(6, 0);
      lcd.print(temp_wynik->czas);
    }                  
  }
  return 0;
}

int czy_zapisac(){

  if(esc_flag)                      // Jeżeli nacisnął ESC
  {
    esc_flag = 0;
    lcd.clear();
    lcd.setCursor(0, 0);                                               
    lcd.print("Wyjsc bez zapisu");

    if(wybor("TAK", "NIE", 3, 10, 3, 3, 1))
    {                      // NIE wychodz bez zapisu    
      lcd.clear();                                      
      return 0;   // Zapisz
    } 
    else 
    {                      // TAK wyjdz bez zapisu
      lcd.clear();
      return 1;
    }
  }
  if(ok_flag)                        // Jeżeli nacisnął OK
  {
    ok_flag = 0;
    lcd.clear();
    return 0;      // Zapisz
  }
}

int czy_istnieje_taka_nazwa(typ_wynik* temp_wynik){

  Serial.println("Sprawdzanie toższamości nazw");

  istnieje_nazwa = 0;

  uint16_t i = 0;
  while(strcmp(wyniki[ranking_index[i]].nazwa, temp_wynik->nazwa) && ranking_index[i] != 0){    // Wyszukanie takiej samej nazwy
    i++;
  }
  if(ranking_index[i] == 0){

    Serial.println("Nazwy są różne");
    return 1;    // Nie znaleziono takiego samego wyniku
  }
  Serial.println("wykryto że nazwy są takie same");

  lcd.clear();
  lcd.setCursor(1, 0);                                               
  lcd.print("Nazwa istnieje!");

  if(wybor("TO_JA", "ZMIENIAM", 1, 7, 5, 8, 1))
  {                      // ZMIENIAM - wróć do klawaitury    
    lcd.clear();                                      
    return 0;            // Nie wychodź z klawiatury
  } 
  else 
  {                      // TO_JA - Zapisz tylko daj falgę że nadpisuje
    lcd.clear();
    istnieje_nazwa = i + 1;  // Przyjmuje miejsce w rankingu na którym została znaleziona nazwa
    Serial.print("Taka sama nazwa jest na miejscu: ");
    Serial.println(istnieje_nazwa - 1);
    return 1;
  }
}

void init_klawiatura(char* co_wyswietlic){
  lcd.setCursor(0, 0);                      
  lcd.print(co_wyswietlic);
  lcd.setCursor(0, 1); 
  lcd.print(klawiatura[0]);
  lcd.setCursor(13, 1);
  lcd.write(1); 
  lcd.setCursor(14, 1); 
  lcd.write(2);
  lcd.setCursor(15, 1); 
  lcd.write(3);
}

int wpisz_nazwe(typ_wynik* temp_wynik){           // zwraca 0 jeśli zapisać, w jeśli wyjść
  
  uint8_t indeks_nazwy = 0;                       
  uint8_t rzad = 0, kolumna = 0, pop_kolumna = 1; // poprzednia pozycja w kolumnie
  uint8_t specjalne = 0, pop_specjalne = 1;       // kolumna znakow specjalnych
  uint8_t pierwsza_litera = 0;                    // pamiętanie 
  temp_wynik->nazwa[0] = {'\0'};
  uint8_t dlugosc_nazwy = 0;                  // pamiętanie która litera nazwy jest obeznie wpisywana

  init_klawiatura("Twoje_Imie");

  while(1){                                       // ----------------------------------------   Sterowanie klawiaturą   -----------------------
          
    if(down_flag){                   // W dół
      down_flag = 0; 
      rzad = 1;
      lcd.setCursor(0, 1); 
      lcd.print(klawiatura[rzad]);    
    }    
    if(up_flag){                     // W góre    
      up_flag = 0;
      rzad = 0;
      lcd.setCursor(0, 1); 
      lcd.print(klawiatura[rzad]);
    }    
    if(kolumna == 11 && specjalne < 3 && right_flag){   // W prawo po spocjalnych
      right_flag = 0;
      pop_specjalne = specjalne;
      specjalne = specjalne + 1;
    }
    if(kolumna > 0 && specjalne == 0 && left_flag){     // W lewo
      left_flag = 0;
      pop_kolumna = kolumna;
      kolumna = kolumna - 1;
    }
    if(kolumna < 11 && right_flag){                     // W prawo
      right_flag = 0;
      pop_kolumna = kolumna;
      kolumna = kolumna + 1;
    }
    if(kolumna == 11 && specjalne > 0 && left_flag){    // W lewo po specjalnych
      left_flag = 0;
      pop_specjalne = specjalne;
      specjalne = specjalne - 1;
    }

    if(specjalne == 0)                                  // -----------------------------------   Wyprintowanie zmiany znaku   -------------------
    {
      lcd.setCursor(pop_kolumna, 1); 
      lcd.print(klawiatura[rzad][pop_kolumna]);
      lcd.setCursor(kolumna, 1); 
      lcd.write(0);
      lcd.setCursor(13, 1); 
      lcd.write(1);
    } else {
      lcd.setCursor(11, 1); 
      lcd.print(klawiatura[rzad][11]);
      lcd.setCursor(12 + pop_specjalne, 1); 
      if(pop_specjalne != 0)
        lcd.write(pop_specjalne);
      lcd.setCursor(12 + specjalne, 1); 
      lcd.write(0);
    }

    if(ok_flag){                                        // ----------------------------   Obsluga naciśnięcia danych przycisków   ----------------
      ok_flag = 0;

      if(specjalne == 0)                                // Wpisanie litery do nazwy
      {
        if(dlugosc_nazwy < 20){
          temp_wynik->nazwa[dlugosc_nazwy] = klawiatura[rzad][kolumna];
          temp_wynik->nazwa[dlugosc_nazwy + 1] = '\0';
          dlugosc_nazwy = dlugosc_nazwy + 1;
        }
        if(pierwsza_litera == 0 && (temp_wynik->nazwa[0] != '\0')){ // W przypadku naciśnięcia pierwszej litery, kasuje napis "Towje_Imie"
            pierwsza_litera = 1;
            lcd.setCursor(0, 0);                      
            lcd.print("               ");
        }
        lcd.setCursor(0, 0);                      
        lcd.print(temp_wynik->nazwa);        
      } 
      else if(specjalne == 1)                           // Caps lock
      {
        if(klawiatura[0][0] == 'a'){
          for(int i = 0; i < 2; i++)
            for(int j = 0; j < 12; j++)
              klawiatura[i][j] = klawiatura_D[i][j];                        
        } else {
          for(int i = 0; i < 2; i++) 
            for(int j = 0; j < 12; j++)
              klawiatura[i][j] = klawiatura_m[i][j];
        }
        lcd.setCursor(0, 1); 
        lcd.print(klawiatura[0]);
      }
      else if(specjalne == 2)                           // Backspace
      {
        if(dlugosc_nazwy > 0){
          dlugosc_nazwy = dlugosc_nazwy - 1; 
          temp_wynik->nazwa[dlugosc_nazwy] = '\0';                         
        }
        lcd.setCursor(dlugosc_nazwy, 0);  
        lcd.print(" ");       
      }
      else if(specjalne == 3)                           // Zapisz                                  
      {
        Serial.println("Próba zapisania nazwy");
        if(czy_istnieje_taka_nazwa(temp_wynik)){
          Serial.println("Wynonanie funkcji czy istniej taka nazwa, przejście do zapisu");
          return 0;            // Sprawdzenie czy już jest taka nazwa i czy ją nadpisać czy zmienić obecną nazwę
        }
        init_klawiatura(temp_wynik->nazwa); // Jako że wyczyściliśmy ekran trzeba znowy naryswoać klawiaturę
        // Wyjście bo następną rzeczą wykonywaną w funkcji main jest funkcja zapisz
      }
    }

    if(esc_flag){               // Obsluga wyjścia
      if(czy_zapisac()){        // Jeśli nie chcemy wyjsc, nie wyjdziemy z funkcji
        lcd.clear();
        return 1;
      } else {
        init_klawiatura(temp_wynik->nazwa); // Jako że wyczyściliśmy ekran trzeba znowy naryswoać klawiaturę
      }
    }
  }
}

int zapisz(typ_wynik* temp_wynik){

  uint16_t ostatni_wynik = wyniki[0].czas + 1;
  
  uint16_t wstawiany_wynik = 1;                       // Przejście do kolejnego wolnego miejsca żeby nie zostawały dziury po usunięciu wyniku
  while(wyniki[wstawiany_wynik].czas != 0.0){
    wstawiany_wynik++;
  }
  
  wyniki[wstawiany_wynik].czas = temp_wynik->czas;                  // Wpisanie wyniku w wolną komórkę tablizy z wynikami
  strcpy(wyniki[wstawiany_wynik].nazwa, temp_wynik->nazwa);
  if(wstawiany_wynik == ostatni_wynik) wyniki[0].czas = ostatni_wynik; // Jeżeli wstawiony wynik jest ostatnim wynikiem to zmiana tej zmiennej       

  wstaw_do_rankingu(wstawiany_wynik);

  return 0;
}


void wstaw_do_rankingu(uint16_t wstawiany_wynik){

  uint16_t bufor, wstaw;
  uint16_t i = 0;
  while(wyniki[wstawiany_wynik].czas > wyniki[ranking_index[i]].czas){        // Dojście wyniku na swoje miejsce w rankingu
    i++;
  }

  if(istnieje_nazwa > 0)                                          // Jeżli istnieje taka nazwa to sprawdź czy jej wynik jest lepszy niż obecny
  {
    istnieje_nazwa = istnieje_nazwa - 1;                          // Przekonwertowanie zmiennej istnieje_nazwa aby pokazywała prawidłowe miejsce znalezionego wyniku

    if(istnieje_nazwa > i)                                        // Jeżeli jest lepszy wstaw go na miejsce i przesuń resztę do momentu starego czasu 
    {
      miejsce = i + 1;                                           // +1 bo miejsca są numerowane od 1

      wstaw = wstawiany_wynik;
      while(i <= istnieje_nazwa){  // Sprawdzić czy == nie dubuluje wyniku                                // Przesunęcie reszty wyników w rankingu
        bufor = ranking_index[i];
        ranking_index[i] = wstaw;
        wstaw = bufor;
        i++;
      }
    } 
    else if(istnieje_nazwa == i)                                  // Jeżeli jest równy, porównj ich czasy
    {
      if(wyniki[wstawiany_wynik].czas < wyniki[ranking_index[istnieje_nazwa]].czas){ // Jeżeli nowy jest mniejszy to wstaw jego adres w mijecu starego, jeżeli nie, nic nie rób
        ranking_index[i] = wstawiany_wynik;
      }
      miejsce = i;
    }
    else                                                          // Jeżeli jest gorszy, nic nie rób, przejdź tylko do jego miejsca
    {
      miejsce = i + 1;                                           // +1 bo miejsca są numerowane od 1
    }
  }
  else
  {
    miejsce = i + 1;                                             // Zapisanie miejsce do na którym wyświetlić ranking

    wstaw = wstawiany_wynik;
    while(wstaw != 0){                                            // Przesunęcie reszty wyników w rankingu
      bufor = ranking_index[i];
      ranking_index[i] = wstaw;
      wstaw = bufor;
      i++;
    }
  }
}
