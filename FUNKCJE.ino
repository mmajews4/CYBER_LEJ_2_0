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

int wybor(char* napis_L, char* napis_P, uint8_t odl_L, uint8_t odl_P, uint8_t dlg_L, uint8_t dlg_P, uint8_t wybrano){ // napis, odleglosc, dlugosc napisu, domyślny wybor(0,1)

  uint32_t prevTime = 0, currTime;
  uint8_t zamalowane = 0, zmien = 0;

  while(!ok_flag)
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
    }

    if(!zamalowane && (currTime - prevTime > CZAS_MIGANIA + HISTEREZA_MIGANIA || zmien))
    {
      zamaluj_wybor(napis_L, napis_P, odl_L, odl_P, dlg_L, dlg_P, wybrano);
      zamalowane = 1;
      zmien = 0;
      prevTime = currTime;
    }
  }
  ok_flag = 0;
  lcd.clear();
  return wybrano;
}

void wypisz_ekran_startowy(){
  lcd.setCursor(3,0);
  lcd.print("CYBER LEJ");
  lcd.setCursor(0,0);
  lcd.write(4);
  lcd.setCursor(15,0);
  lcd.write(4);
 // for(int i = 5; i <= 7; i++){
    lcd.setCursor(14,0);
    lcd.write(6);
    //delay(2000);
  //}
}
