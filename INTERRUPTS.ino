unsigned long button_time = 0;            // Ochrona przed tzw. button debounce
unsigned long last_button_time = 0;

void IRAM_ATTR ISR_OK() {                 // Interrupt service rutine
  button_time = millis();                 // Ochrona przed tzw. button debounce
  if (button_time - last_button_time > 200)
  {
    ok_flag = 1;
    last_button_time = button_time;
  }
}
void IRAM_ATTR ISR_RIGHT() {
  button_time = millis();
  if (button_time - last_button_time > 150)
  {
    right_flag = 1;
    last_button_time = button_time;
  }
}
void IRAM_ATTR ISR_DOWN() {
  button_time = millis();
  if (button_time - last_button_time > 150)
  {
    down_flag = 1;
    last_button_time = button_time;
  }
}
void IRAM_ATTR ISR_LEFT() {
  button_time = millis();
  if (button_time - last_button_time > 150)
  {
    left_flag = 1;
    last_button_time = button_time;
  }
}
void IRAM_ATTR ISR_UP() {
  button_time = millis();
  if (button_time - last_button_time > 150)
  {
    up_flag = 1;
    last_button_time = button_time;
  }
}
void IRAM_ATTR ISR_ESC() {
  button_time = millis();
  if (button_time - last_button_time > 150)
  {
    esc_flag = 1;
    last_button_time = button_time;
  }
}

void inicjuj_lcd(){
  lcd.init();                              // initialize the LCD and turn on the blacklight
  lcd.backlight();
  lcd.createChar(0, miganie);              // Wywołanie lcd.write(0);
  lcd.createChar(1, caps_lock);
  lcd.createChar(2, backspace);
  lcd.createChar(3, zapis);
  lcd.createChar(4, lej);
  lcd.clear();
}

void inicjuj_interrupty(){
  attachInterrupt(OK, ISR_OK, RISING);        // Przyciski
  attachInterrupt(RIGHT, ISR_RIGHT, RISING);
  attachInterrupt(DOWN, ISR_DOWN, RISING);
  attachInterrupt(LEFT, ISR_LEFT, RISING);
  attachInterrupt(UP, ISR_UP, RISING);
  attachInterrupt(ESC, ISR_ESC, RISING);

/*  tim_cursor = timerBegin(0, 80, true);         // Timer interrupt
  timerAttachInterrupt(tim_cursor, &ISR_tim_cursor, true);
  timerAlarmWrite(tim_cursor, 1000000, true);
  timerAlarmEnable(tim_cursor);*/
}
