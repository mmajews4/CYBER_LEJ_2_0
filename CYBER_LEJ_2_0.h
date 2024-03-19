#ifndef CYBER_LEJ_2_0_H
#define CYBER_LEJ_2_0_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <String.h>
#include <stdlib.h>
#include "FS.h"
#include <LITTLEFS.h>
#include "SdFat.h"
#include "sdios.h"
#include "SD.h"

#define OK 13                                 // Podmienić na interupty i flagi
#define RIGHT 14
#define DOWN 27
#define UP 26
#define LEFT 25
#define ESC 33
#define VALVE 4
#define BATTERY 35
#define CZAS_MIGANIA 550
#define HISTEREZA_MIGANIA 70
#define CZAS_PRZES_RANK 200
#define DLG_PRZES_NAZWY 8                   // Długość przesuwanej nazwy

#define FORMAT_LITTLEFS_IF_FAILED false     // Potrzebne do LittleFS gdy jest uruchomione za pierwszym razem

LiquidCrystal_I2C lcd(0x27, 16, 2);         // Set the LCD address to 0x27 for a 16 chars and 2 line display
//hw_timer_t *tim_cursor = NULL;

typedef struct{
  float czas;
  char nazwa[23]; // zmienic na 21
} typ_wynik;

uint16_t ranking_index[100] = {2,6,4,5,9,10,1,7};

uint8_t miejsce = 1; // Miejsce na którym wyświetlić ranking
int16_t naladowanie;

uint8_t czytaj_z_SD = 0, czytaj_z_flash = 1, wifi_on = 0;

//char odczyt_wynikow_flash[250];

uint8_t ok_flag = 0, right_flag = 0, down_flag = 0, left_flag = 0, up_flag = 0, esc_flag = 0; //Interrupty

char ekran[2][17] = {{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'},
                    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'}};

char prevEkran[2][17] = {{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'},
                         {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'}};

char klawiatura[2][13] = {{'a','b','c','d','e','f','g','h','i','j','k','l','\0'},
                          {'m','n','o','p','r','s','t','u','w','y','z','_','\0'}};
                       
char klawiatura_m[2][13] = {{'a','b','c','d','e','f','g','h','i','j','k','l','\0'},
                            {'m','n','o','p','r','s','t','u','w','y','z','_','\0'}};

char klawiatura_D[2][13] = {{'A','B','C','D','E','F','G','H','I','J','K','L','\0'},
                            {'M','N','O','P','R','S','T','U','W','Y','Z','_','\0'}};


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

byte bateria[3][8] ={{
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111
},{
  B01110,
  B11011,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111
},{
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
}};


int wyswietl_ekran();                                                           //    -------------   F U N K C J E   ---------------
void wyswietl_wybory(char*, char*, uint8_t, uint8_t);
void zamaluj_wybor(char*, char*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
int wybor(char*, char*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void wypisz_ekran_startowy();
void wyswietl_ustawienia();
int sprawdz_naladowanie(uint8_t, uint8_t, uint8_t);

int zawor_otwarty();                                                            //    --------------   P O M I A R   ----------------
int uwaga_otworzony_zawor();
int zliczanie_czasu(typ_wynik*);
int czy_zapisac(); 
void init_klawiatura(char*);
int wpisz_nazwe(typ_wynik*);
void wstaw_do_rankingu(uint16_t);
int zapisz(typ_wynik*);

void wyswietl_ranking(uint8_t, uint8_t);                                        //    -------------   R A N K I N G   ---------------
int przesuwanie_nazwy(uint8_t, uint8_t);
void ruch_rankingu(uint8_t*);
void ruch_menu_wynikow(char*, uint16_t*, uint16_t*);
void wyswietl_wyniki(uint16_t, uint16_t);
int usun_wynik(uint16_t);
void menu_wynikow_lejownika(uint16_t);

void IRAM_ATTR ISR_OK();                                                        //    ----------   I N T E R R U P T S   ------------
void IRAM_ATTR ISR_RIGHT();
void IRAM_ATTR ISR_DOWN();
void IRAM_ATTR ISR_LEFT();
void IRAM_ATTR ISR_UP();
void IRAM_ATTR ISR_ESC();
void inicjuj_lcd();
void inicjuj_interrupty();

/*void test_LittleFS();                                                           //    --------   L I T T L E F S   F U N C   --------
void listDir(fs::FS, char*, uint8_t);
void createDir(fs::FS, char*);
void removeDir(fs::FS, char*);
void readFile(fs::FS, char*);
void writeFile(fs::FS, char*, char*);
void appendFile(fs::FS, char*, char*);
void renameFile(fs::FS, char*, char*);
void deleteFile(fs::FS, char*);
// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS, char*, char*);
void deleteFile2(fs::FS, char*);
void testFileIO(fs::FS, char*);

void inicjuj_LittleFS();                                                        //    --------------   E E P R O M   ---------------
void zapisz_we_flashu();
void czytaj_z_flashu();

void SD_test();                                                                 //    ------------------   S D  --------------------
*/

#endif