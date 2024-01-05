#ifndef CYBER_LEJ_2_0_H
#define CYBER_LEJ_2_0_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define OK 13                                 // Podmienić na interupty i flagi
#define RIGHT 14
#define DOWN 27
#define UP 26
#define LEFT 25
#define ESC 33
#define VALVE 4
#define CZAS_MIGANIA 550
#define HISTEREZA_MIGANIA 70
#define CZAS_PRZES_RANK 200

LiquidCrystal_I2C lcd(0x27, 16, 2);         // Set the LCD address to 0x27 for a 16 chars and 2 line display
//hw_timer_t *tim_cursor = NULL;

typedef struct{
  float czas;
  char* nazwa;
  int dlugosc_nazwy;
} typ_wynik;

uint8_t ok_flag = 0, right_flag = 0, down_flag = 0, left_flag = 0, up_flag = 0, esc_flag = 0; //Interrupty
float czas = 0.0;

char ekran[2][17] = {{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'},
                    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'}};

char prevEkran[2][17] = {{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'},
                         {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'}};

byte miganie[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
  
byte backspace[] = {                
  B00000,
  B00000,
  B01000,
  B11111,
  B01001,
  B00011,
  B00000,
  B00000
};

byte caps_lock[] = {                  
  B00000,
  B00000,
  B00100,
  B01110,
  B11011,
  B01010,
  B01110,
  B00000
};

byte zapis[] = {                         
  B00000,
  B00001,
  B00010,
  B00100,
  B10100,
  B01000,
  B01000,
  B00000
};

byte lej[] = {
  B11111,
  B11111,
  B01110,
  B00100,
  B00100,
  B00100,
  B10100,
  B01000
};


int wyswietl_ekran();                                                           //    -------------   F U N K C J E   --------------
void wyswietl_wybory(char*, char*, uint8_t, uint8_t);
void zamaluj_wybor(char*, char*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
int wybor(char*, char*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void wypisz_ekran_startowy();

int zawor_otwarty();
int uwaga_otworzony_zawor();
int zliczanie_czasu();
int czy_zapisac(); 
int zapisz();     // nie skończone

void IRAM_ATTR ISR_OK();                                                        //    -----------   I N T E R R U P T S   -------------
void IRAM_ATTR ISR_RIGHT();
void IRAM_ATTR ISR_DOWN();
void IRAM_ATTR ISR_LEFT();
void IRAM_ATTR ISR_UP();
void IRAM_ATTR ISR_ESC();
void inicjuj_lcd();
void inicjuj_interrupty();

#endif