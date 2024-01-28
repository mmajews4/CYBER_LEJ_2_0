/*
Pamięć FLASH na ESP32 jest za mała. Jest tam 4096 bit czyli 512 B
Na takiej ilości miejsca, zakładające że przekowertuje czas z floata na uint16_t z trzema miejscami po przecinku zawsze żeby wiedzić gdzie go dopisać,
oraz z nazwą praktycznie NIEAKCEPTOWALNIE krótką 8 znaków, 10 B na wpis, mogę pomieścić max 51 wpisów, za mało.

Próbowałem uruchomić kartę microSD z modułem to tej karty za pomocą przykłądu SDtest ale wyświetla się komunikat Initialization failed...
Pomyślałem że może yo być wina konwertera logiki 5V na 3V3 ale jak zamontowałem moduł na arduino nie pomogło.
Zformatowałem kartę na windowsie na FAT32, nie daiła, zformatowałem kartę spocjalnym narzędzem poleconym na forum arduino do FAT16, wykrywa że karta jest w module
ale nie może odczytać jej pojemnochci ani wpisać czy odczytać czegokolwiek.

*/