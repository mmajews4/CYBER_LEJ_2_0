void wyswietl_ranking(uint8_t miejsce){       // Miejsce, tak jak w rzeczywstości zaczyna się od 1.

  for(int i = 0; i < 2; i++)              // Wypisanie miejsca
  { 
    lcd.setCursor(0, i);
    lcd.print(miejsce+i);
    if(miejsce+i < 10)
      lcd.print(" ");                     // Wypisanie przerw w odpowiednich miejscach
    lcd.print(" ");
    lcd.print(ranking[miejsce-1+i].czas); // Wypisanie 4 znaków czasu

    lcd.setCursor(7, i);
    lcd.print(" ");
  }
}

// Funkcja czeka określony czas, przesówa wczytane zmienne, a zwraca wyjsce esc lub ruch w rankingu
int przesuwanie_nazwy(uint8_t miejsce){

  uint32_t currTime, prevTime = 0;
  char przes_nazwa[2][9];
  przes_nazwa[0][8] = '\0';
  przes_nazwa[1][8] = '\0';

  for(uint8_t i = 0; i < 8; i++)
  {
    przes_nazwa[0][i] = ranking[miejsce-1].nazwa[i];
    przes_nazwa[1][i] = ranking[miejsce].nazwa[i];
  }

  while(1)
  {
    currTime = millis();
    
    if(currTime - prevTime > 1000){
      for(uint8_t i = 0; i < 2; i++){                           // Wypisanie 8 liter z nazwy a jeśli tyle nie ma to w pozostałe miejsca wstawia znak " "
        lcd.setCursor(8, i);
        lcd.print(przes_nazwa[i]);
        lcd.print("        ");                                   // Usuwa niepotrzebne znaki
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
    if(ranking[*miejsce+1].czas != 0.0)
      *miejsce = *miejsce + 1;
  }
  if(left_flag){                      // lewo - 5 do dołu
    left_flag = 0;
    if(*miejsce > 5)
      *miejsce = *miejsce - 5;
  }
  if(right_flag){                     // prawo - 5 do góry                              
    right_flag = 0;
    if(ranking[*miejsce+5].czas != 0.0)
      *miejsce = *miejsce + 5;
  }

}





