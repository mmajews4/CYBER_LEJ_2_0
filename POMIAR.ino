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

int zliczanie_czasu(){

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
      czas = (currTime - prevTime)/ 1000.0;
      lcd.setCursor(6, 0);                                               
      lcd.print(czas);
    }

    for(int i = 0; i < 3; i++)          //  -----   Wyswietlenie wymiku (3 migniecia a potem na stałe)
    {
      currTime = prevTime = millis();   // Czekamy 500 ms
      while(currTime - prevTime < 500){
        if(esc_flag)                    // Obsluga esc
        {                               // Nie kasujemy esc_flagi bo później będzie komunikat
          return 0;
        }
        currTime = millis();
      }

      lcd.clear();
      
      currTime = prevTime = millis();   // Czekamy 500 ms
      while(currTime - prevTime < 500){ 
        if(esc_flag)                    // Obsluga esc
        {                               // Nie kasujemy esc_flagi bo później będzie komunikat
          lcd.setCursor(6, 0);
          lcd.print(czas);              // Podczas wyjscia w trakcie migniecia trzeba wydrukować czas
          return 0;
        }
        currTime = millis();
      }
      lcd.setCursor(6, 0);
      lcd.print(czas);
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

int zapisz(){
  Serial.println("Zapisano");
}
