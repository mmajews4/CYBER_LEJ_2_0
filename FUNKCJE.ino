int wyswietl_ekran(){     // int aby sprawdzać czy na końcu jest '\0' czy coś pojebałem

  lcd.setCursor(0,0);
  lcd.print(ekran[0]);
  lcd.setCursor(0,1);
  lcd.print(ekran[1]);

  if(ekran[0][8] != '\0' || ekran[1][8] != '\0')
    return 1;

  return 0;
}

void wyswietl_wybory(char* napis_L, char* napis_P, uint8_t odl_L, uint8_t odl_P){
  lcd.setCursor(odl_L, 1);
  lcd.print(napis_L);
  lcd.setCursor(odl_P, 1);
  lcd.print(napis_P);
}

void zamaluj_wybor(char* napis_L, char* napis_P, uint8_t odl_L, uint8_t odl_P, uint8_t dlg_L, uint8_t dlg_P, uint8_t wybrano){
  if(wybrano)
  {
    lcd.setCursor(odl_L, 1);
    lcd.print(napis_L);
    
    for(int i = 0; i < dlg_P; i++)     // Zamaluj wyraz
    {
      lcd.setCursor(odl_P + i, 1);
      lcd.write(0);                      // Znak zamalowania
    }
  } else {
    lcd.setCursor(odl_P, 1);
    lcd.print(napis_P);
    
    for(int i = 0; i < dlg_L; i++)     // Zamaluj wyraz
    {
      lcd.setCursor(odl_L + i, 1);
      lcd.write(0);                      // Znak zamalowania
    }
  }
}

 // napis, odleglosc, dlugosc napisu, domyślny wybor(0,1), funkcje wyswietlania baterii w różnych menu
int wybor(char* napis_L, char* napis_P, uint8_t odl_L, uint8_t odl_P, uint8_t dlg_L, uint8_t dlg_P, uint8_t wybrano, uint8_t bateria){

  uint32_t prevTime = 0, currTime;
  uint8_t zamalowane = 0, zmien = 0;

  while(!ok_flag && !(bateria == 1 && up_flag) && !(bateria == 2 && esc_flag))
  {
    if(right_flag)              // Zmiana wyboru za pomocą przycsków
    {
      if(!wybrano)
      {
        wybrano = 1;          
        zmien = 1;              // Aby zmienić od razu po nacisniecu przycisku
        zamalowane = 0;         // Aby zamalować od razu
      }
      right_flag = 0;
      
    }
    if(left_flag)
    {
      if(wybrano)
      {
        wybrano = 0;
        zmien = 1;
        zamalowane = 0;
      }
      left_flag = 0;
    }
    currTime = millis();

    if(zamalowane && (currTime - prevTime > CZAS_MIGANIA - HISTEREZA_MIGANIA || zmien))
    {
      wyswietl_wybory(napis_L, napis_P, odl_L, odl_P);
      zamalowane = 0;
      zmien = 0;
      prevTime = currTime;

      if(bateria == 1){                                                           // Co jakis czas trzeba sprawdzać naladowanie baterii, mysle ze to dobry moment
        sprawdz_naladowanie(14, 1, 0);
        wypisz_ekran_startowy();
      } else if(bateria == 2){
        sprawdz_naladowanie(0, 1, 1);
      }
    }

    if(!zamalowane && (currTime - prevTime > CZAS_MIGANIA + HISTEREZA_MIGANIA || zmien))
    {
      zamaluj_wybor(napis_L, napis_P, odl_L, odl_P, dlg_L, dlg_P, wybrano);
      zamalowane = 1;
      zmien = 0;
      prevTime = currTime;
    }
  }

  if(bateria == 1 && up_flag) // Jeżeli jesteśmy w pierwszym trybie pokazywania baterii czyli w menu glownym to po naciśnięciu klawisza w góre idziemy do ustawień
  {
    wybrano = 2;
  }
  if(bateria == 2 && esc_flag) // Jeżeli jesteśmy w drugim trybie pokazywania baterii czyli w ustawieniach to powinnyiśmy móc z nich wyjść
  {
    wybrano = 2;
  }
  ok_flag = 0;
  up_flag = 0;
  down_flag = 0;      // Bo po wyborze zapamiętywało ruch w dół lub górę
  lcd.clear();
  return wybrano;
}

void wyswietl_ustawienia(){

  /*int currTime, prevTime = 0;

  
  while(!esc_flag){

    currTime = millis();

    if(currTime - prevTime > 2000){
      //Serial.println("Wyswietlam stan naladowania baterii");
      sprawdz_naladowanie(0, 1, 1);
      prevTime = currTime;
    }
  }
  esc_flag = 0;
  lcd.clear();*/

  uint8_t opcja = 0;

  while(!esc_flag){

    if(czytaj_z_SD && czytaj_z_flash && !wifi_on)                         // Wyswietlenie każdej możiwej kombinacji opcji i czym skutkuje naciśnięcie na nią
    {
      opcja = wybor("Wifi:OF", "SD/FLASH", 0, 8, 7, 8, opcja, 2);
      
      if(opcja == 0) wifi_on = 1; 
      else if(opcja == 1) czytaj_z_flash = 0;

    }
    else if(!czytaj_z_SD && czytaj_z_flash && !wifi_on)
    {
      opcja = wybor("Wifi:OFF", "FLASH", 0, 10, 8, 5, opcja, 2);
      
      if(opcja == 0) wifi_on = 1; 
      else if(opcja == 1) czytaj_z_SD = 1;

    }
    else if(czytaj_z_SD && !czytaj_z_flash && !wifi_on)
    {
      opcja = wybor("Wifi:OFF", "SD", 0, 11, 8, 2, opcja, 2);
      
      if(opcja == 0) wifi_on = 1; 
      else if(opcja == 1){
        czytaj_z_SD = 0;
        czytaj_z_flash = 1;
      }
    }
    else if(czytaj_z_SD && czytaj_z_flash && wifi_on)                         // Wyswietlenie każdej możiwej kombinacji opcji i czym skutkuje naciśnięcie na nią
    {
      opcja = wybor("Wifi:ON", "SD/FLASH", 0, 8, 7, 8, opcja, 2);
      
      if(opcja == 0) wifi_on = 0; 
      else if(opcja == 1) czytaj_z_flash = 0;

    }
    else if(!czytaj_z_SD && czytaj_z_flash && wifi_on)
    {
      opcja = wybor("Wifi:ON", "FLASH", 0, 10, 7, 5, opcja, 2);
      
      if(opcja == 0) wifi_on = 0; 
      else if(opcja == 1) czytaj_z_SD = 1;

    }
    else if(czytaj_z_SD && !czytaj_z_flash && wifi_on)
    {
      opcja = wybor("Wifi:ON", "SD", 0, 11, 7, 2, opcja, 2);
      
      if(opcja == 0) wifi_on = 0; 
      else if(opcja == 1){
        czytaj_z_SD = 0;
        czytaj_z_flash = 1;
      }
    }
  }
  esc_flag = 0;
  lcd.clear();

}

// Sprawdza stan baterii i go zwraca, wyświetla ikonke bateryjki i w razie potrzeby wyświetla alarm
int sprawdz_naladowanie(uint8_t pozycja_x, uint8_t czy_wyswietlic, uint8_t czy_procent){                                                      //   ZAPYTAĆ O TĄ FUNKCJĘ CZY DOBRE ZAKRESY
  
  naladowanie = map(analogRead(BATTERY), 2000, 4095, 0, 120);  // Do 120 żeby zrekompensować cahrakterystyke Li-Po która szybko spada na początku, później wolniej

  if(naladowanie < 0) naladowanie = 0;                          // zabezpieczenie przed przepełnieniem

  if(naladowanie < 10){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("LOW BATTERY!");
    lcd.setCursor(7,1);
    lcd.print(naladowanie);
    lcd.print("%  ");

    while(!esc_flag){
      Serial.println("LOW BATTERY");
    }
    lcd.clear();
    esc_flag = 0;
  }

  if(czy_wyswietlic)                        // Wyswiwetlenie bateryjki
  {
    lcd.setCursor(pozycja_x, 0);

    if(naladowanie > 70){                   // Zakres wyswiwtlania ikonki bateryjki
      lcd.write(7);
    } else if(naladowanie > 40){
      lcd.write(6);
    } else {
      lcd.write(5);
    }

    if(czy_procent)                         // Czy wypisać ile bateria ma %
    {
      lcd.print(" ");
      lcd.print(naladowanie);
      lcd.print("%  ");
    }
  }
  return naladowanie;
}

void wypisz_ekran_startowy(){
  lcd.setCursor(3,0);
  lcd.print("CYBER LEJ");
  lcd.setCursor(0,0);
  lcd.write(4);
  lcd.setCursor(15,0);
  lcd.write(4);
  //Serial.println("wypisanio ekran startowy");
}
